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


uint8_t is_command(uint16_t controlword, cia402_controlword_command_t command_to_check)
{
    uint16_t mask          = 0x0000;
    uint16_t command_value = 0xFFFF;

    switch (command_to_check)
    {
    case SHUTDOWN_CMD:
        mask = CIA402_CONTROLWORD_MASK_SHUTDOWN;
        command_value = CIA402_CONTROLWORD_COMMAND_SHUTDOWN;
        break;
    case SWITCH_ON_CMD:
        mask = CIA402_CONTROLWORD_MASK_SWITCHON;
        command_value = CIA402_CONTROLWORD_COMMAND_SWITCHON;
        break;
    case SWITCH_ON_ENABLE_CMD:
        mask = CIA402_CONTROLWORD_MASK_SWITCHON_ENABLE;
        command_value = CIA402_CONTROLWORD_COMMAND_SWITCHON_ENABLE;
        break;
    case DISABLE_VOLTAGE_CMD:
        mask = CIA402_CONTROLWORD_MASK_DISABLEVOLTAGE;
        command_value = CIA402_CONTROLWORD_COMMAND_DISABLEVOLTAGE;
        break;
    case QUICK_STOP_CMD:
        mask = CIA402_CONTROLWORD_MASK_QUICKSTOP;
        command_value = CIA402_CONTROLWORD_COMMAND_QUICKSTOP;
        break;
    case DISABLE_OPERATION_CMD:
        mask = CIA402_CONTROLWORD_MASK_DISABLEOPERATION;
        command_value = CIA402_CONTROLWORD_COMMAND_DISABLEOPERATION;
        break;
    case ENABLE_OPERATION:
        mask = CIA402_CONTROLWORD_MASK_ENABLEOPERATION;
        command_value = CIA402_CONTROLWORD_COMMAND_ENABLEOPERATION;
        break;
    case FAULT_RESET:
        mask = CIA402_CONTROLWORD_MASK_FAULTRESET;
        command_value = CIA402_CONTROLWORD_COMMAND_FAULTRESET;
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
        if (is_command(controlword, SHUTDOWN_CMD) || *(axis->ALstatus) == AL_STATUS_OP) {
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
        if (is_command(controlword, DISABLE_VOLTAGE_CMD)) {
            // transition 7
            axis->state = SWITCH_ON_DISABLED;
            *(axis->statusword) |= SWITCH_ON_DISABLED;
            axis->transition = READY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED;
        } else if (is_command(controlword, SWITCH_ON_CMD)) {
            // transition 3
            axis->state = SWITCHED_ON;
            *(axis->statusword) |= SWITCHED_ON;
            axis->transition = READY_TO_SWITCH_ON_TO_SWITCHED_ON;
            
            if (is_command(controlword, SWITCH_ON_ENABLE_CMD)) {
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
        if (is_command(controlword, SHUTDOWN_CMD)) {
            // transition 6
            axis->state = READY_TO_SWITCH_ON;
            *(axis->statusword) |= READY_TO_SWITCH_ON;
            axis->transition = SWITCHED_ON_TO_READY_TO_SWITCH_ON;
        } else if (is_command(controlword, ENABLE_OPERATION)) {
            // transition 4
            axis->state = OPERATION_ENABLED;
            *(axis->statusword) |= OPERATION_ENABLED;
            axis->transition = SWITCHED_ON_TO_OPERATION_ENABLED;
        } else if (is_command(controlword, DISABLE_VOLTAGE_CMD)) {
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
        if (is_command(controlword, DISABLE_OPERATION_CMD)) {
            // transition 5
            axis->state = SWITCHED_ON;
            *(axis->statusword) |= SWITCHED_ON;
            axis->transition = OPERATION_ENABLED_TO_SWITCHED_ON;
        }
        else if (is_command(controlword, SHUTDOWN_CMD)) {
            // transition 8
            axis->state = READY_TO_SWITCH_ON;
            *(axis->statusword) |= READY_TO_SWITCH_ON;
            axis->transition = OPERATION_ENABLED_TO_READY_TO_SWITCH_ON;
        }
        else if (is_command(controlword, DISABLE_VOLTAGE_CMD)
            || *(axis->ALstatus) != AL_STATUS_OP) { // connection lost 
            // transition 9
            axis->state = SWITCH_ON_DISABLED;
            *(axis->statusword) |= SWITCH_ON_DISABLED;
            axis->transition = OPERATION_ENABLED_TO_SWITCH_ON_DISABLED;
        }
        else if (is_command(controlword, QUICK_STOP_CMD)) {
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
        if (is_command(controlword, DISABLE_VOLTAGE_CMD)) {
            // transition 12
            axis->state = SWITCH_ON_DISABLED;
            *(axis->statusword) |= SWITCH_ON_DISABLED;
            axis->transition = QUICK_STOP_ACTIVE_TO_OPERATION_ENABLED;
        }
        else if (is_command(controlword, ENABLE_OPERATION)) {
            // transition 16, supporting not recommended
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
