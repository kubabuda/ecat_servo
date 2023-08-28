/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

#include "stddef.h"
#include "cia402device.h"


void cia402_init(cia402_axis_t * axis) 
{
    axis->state = NOT_READY_TO_SWITCH_ON;
    axis->transition = NO_TRANSITION;
    
    if (axis->init_od_hook != NULL) { // TODO null check
        axis->init_od_hook();
    }
}


uint8_t is_command(uint16_t controlword, cia402_controlword_t command_to_check)
{
    uint16_t mask          = 0x0000;
    uint16_t command_value = 0xFFFF;

    switch (command_to_check)
    {
    case SHUTDOWN:
        mask = CIA402_CONTROLWORD_SHUTDOWN_MASK;
        command_value = CIA402_CONTROLWORD_SHUTDOWN_COMMAND;
        break;
    case SWITCH_ON:
        mask = CIA402_CONTROLWORD_SWITCHON_MASK;
        command_value = CIA402_CONTROLWORD_SWITCHON_COMMAND;
        break;
    case SWITCH_ON_ENABLE:
        mask = CIA402_CONTROLWORD_SWITCHON_ENABLE_MASK;
        command_value = CIA402_CONTROLWORD_SWITCHON_ENABLE_COMMAND;
        break;
    case DISABLE_VOLTAGE:
        mask = CIA402_CONTROLWORD_DISABLEVOLTAGE_MASK;
        command_value = CIA402_CONTROLWORD_DISABLEVOLTAGE_COMMAND;
        break;
    case QUICK_STOP:
        mask = CIA402_CONTROLWORD_QUICKSTOP_MASK;
        command_value = CIA402_CONTROLWORD_QUICKSTOP_COMMAND;
        break;
    case DISABLE_OPERATION:
        mask = CIA402_CONTROLWORD_DISABLEOPERATION_MASK;
        command_value = CIA402_CONTROLWORD_DISABLEOPERATION_COMMAND;
        break;
    case ENABLE_OPERATION:
        mask = CIA402_CONTROLWORD_ENABLEOPERATION_MASK;
        command_value = CIA402_CONTROLWORD_ENABLEOPERATION_COMMAND;
        break;
    case FAULT_RESET:
        mask = CIA402_CONTROLWORD_FAULTRESET_MASK;
        command_value = CIA402_CONTROLWORD_FAULTRESET_COMMAND;
        break;
    default:
        break;
    }

    return (controlword & mask) == command_value;
}


void cia402_state_machine(cia402_axis_t * axis, uint16_t controlword) {
    
    *(axis->statusword) = 0x0000;
    axis->transition = NO_TRANSITION;

    switch (axis->state)
    {
    case NOT_READY_TO_SWITCH_ON: 
    {
        // automatic transition on reaching OP 
        if (*(axis->ALstatus) == AL_STATUS_OP) {
            // transition 1
            axis->state = SWITCH_ON_DISABLED;
            *(axis->statusword) |= SWITCH_ON_DISABLED;
            axis->transition = NOT_READY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED;
        } else {
            *(axis->statusword) |= NOT_READY_TO_SWITCH_ON;
        }
        break;
    }
    
    case SWITCH_ON_DISABLED: 
    {
        if (is_command(controlword, SHUTDOWN) || *(axis->ALstatus) == AL_STATUS_OP) {
            // transition 2
            axis->state = READY_TO_SWITCH_ON;
            *(axis->statusword) |= READY_TO_SWITCH_ON;
            axis->transition = SWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON;
        } else {
            *(axis->statusword) |= SWITCH_ON_DISABLED;
        }
        break;
    }
    
    case READY_TO_SWITCH_ON: 
    {
        if (is_command(controlword, DISABLE_VOLTAGE)) {
            // transition 7
            axis->state = SWITCH_ON_DISABLED;
            *(axis->statusword) |= SWITCH_ON_DISABLED;
            axis->transition = READY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED;
        } else if (is_command(controlword, SWITCH_ON)) {
            // transition 3
            axis->state = SWITCHED_ON;
            *(axis->statusword) |= SWITCHED_ON;
            axis->transition = READY_TO_SWITCH_ON_TO_SWITCHED_ON;
            
            if (is_command(controlword, SWITCH_ON_ENABLE)) {
                // transitions 3 + 4 in one command
                axis->state = OPERATION_ENABLED;
                *(axis->statusword) |= OPERATION_ENABLED;
                axis->transition = READY_TO_SWITCH_ON_TO_OPERATION_ENABLED;
            }
        } else {
            *(axis->statusword) |= READY_TO_SWITCH_ON;
        }
        break;
    }
    
    case SWITCHED_ON: 
    {
        if (is_command(controlword, SHUTDOWN)) {
            // transition 6
            axis->state = READY_TO_SWITCH_ON;
            *(axis->statusword) |= READY_TO_SWITCH_ON;
            axis->transition = SWITCHED_ON_TO_READY_TO_SWITCH_ON;
        } else if (is_command(controlword, ENABLE_OPERATION)) {
            // transition 4
            axis->state = OPERATION_ENABLED;
            *(axis->statusword) |= OPERATION_ENABLED;
            axis->transition = SWITCHED_ON_TO_OPERATION_ENABLED;
        } else if (is_command(controlword, DISABLE_VOLTAGE)) {
            // transition 10
            axis->state = SWITCH_ON_DISABLED;
            *(axis->statusword) |= SWITCH_ON_DISABLED;
            axis->transition = SWITCHED_ON_TO_SWITCH_ON_DISABLED;
        } else {
            *(axis->statusword) |= SWITCHED_ON;
        }
        break;
    }
    
    case OPERATION_ENABLED: 
    {
        if (is_command(controlword, DISABLE_OPERATION)) {
            // transition 5
            axis->state = SWITCHED_ON;
            *(axis->statusword) |= SWITCHED_ON;
            axis->transition = OPERATION_ENABLED_TO_SWITCHED_ON;
        }
        else if (is_command(controlword, SHUTDOWN)) {
            // transition 8
            axis->state = READY_TO_SWITCH_ON;
            *(axis->statusword) |= READY_TO_SWITCH_ON;
            axis->transition = OPERATION_ENABLED_TO_READY_TO_SWITCH_ON;
        }
        else if (is_command(controlword, DISABLE_VOLTAGE)
            || *(axis->ALstatus) != AL_STATUS_OP) { // connection lost 
            // transition 9
            axis->state = SWITCH_ON_DISABLED;
            *(axis->statusword) |= SWITCH_ON_DISABLED;
            axis->transition = OPERATION_ENABLED_TO_SWITCH_ON_DISABLED;
        }
        else if (is_command(controlword, QUICK_STOP)) {
            // transition 11
            axis->state = QUICK_STOP_ACTIVE;
            *(axis->statusword) |= QUICK_STOP_ACTIVE;
            axis->transition = OPERATION_ENABLED_TO_QUICK_STOP_ACTIVE;
        }
        else {
            *(axis->statusword) |= OPERATION_ENABLED;

            if (axis->motion_control_hook) {
                axis->motion_control_hook();
            }
        }
        break;
    }
    
    case QUICK_STOP_ACTIVE: 
    {
        if (is_command(controlword, DISABLE_VOLTAGE)) {
            // transition 12
            axis->state = SWITCH_ON_DISABLED;
            *(axis->statusword) |= SWITCH_ON_DISABLED;
            axis->transition = QUICK_STOP_ACTIVE_TO_SWITCH_ON_DISABLED;
        }
        else if (is_command(controlword, ENABLE_OPERATION)) {
            // transition 16, supporting not recommended
            *(axis->statusword) |= QUICK_STOP_ACTIVE;
            break;
            axis->state = OPERATION_ENABLED;
            *(axis->statusword) |= OPERATION_ENABLED;
            axis->transition = QUICK_STOP_ACTIVE_TO_OPERATION_ENABLED;
        } else {
            *(axis->statusword) |= QUICK_STOP_ACTIVE;
        }
        break;
    }
    
    case FAULT_REACTION_ACTIVE: 
    {
        // transition 14 is automatic
        axis->state = FAULT;
        *(axis->statusword) |= FAULT;
        axis->transition = FAULT_REACTION_ACTIVE_TO_FAULT;
        break;
    }
    
    case FAULT: 
    {
        if (is_command(controlword, FAULT_RESET)) {
            // transition 15
            axis->state = SWITCH_ON_DISABLED;
            *(axis->statusword) |= SWITCH_ON_DISABLED;
            axis->transition = FAULT_TO_SWITCH_ON_DISABLED;
        } else {
            *(axis->statusword) |= FAULT;
        }
        break;
    }
    
    default:
        // this should never happen, but just in case
        axis->state = NOT_READY_TO_SWITCH_ON;
        *(axis->statusword) |= NOT_READY_TO_SWITCH_ON;
        axis->transition = NO_TRANSITION;
        break;
    }
}
