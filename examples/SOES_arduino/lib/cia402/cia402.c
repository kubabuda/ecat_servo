#include "cia402.h"


void cia402_init(cia402_axis_t * axis) 
{
    axis->state = NOT_READY_TO_SWITCH_ON;
    
    if (axis->init_od_hook) {
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

    switch (axis->state)
    {
    case NOT_READY_TO_SWITCH_ON: 
    {
        *(axis->statusword) |= NOT_READY_TO_SWITCH_ON;
        if (*(axis->ALstatus) == AL_STATUS_OP) {
            // transition 1
            axis->state = SWITCH_ON_DISABLED;
        }
        break;
    }
    
    case SWITCH_ON_DISABLED: 
    {
        *(axis->statusword) |= SWITCH_ON_DISABLED;
        if (is_command(controlword, SHUTDOWN_CMD) || *(axis->ALstatus) == AL_STATUS_OP) {
            // transition 2
            axis->state = READY_TO_SWITCH_ON;
        }
        break;
    }
    
    case READY_TO_SWITCH_ON: 
    {
        *(axis->statusword) |= READY_TO_SWITCH_ON;
        if (is_command(controlword, DISABLE_VOLTAGE_CMD)) {
            // transition 7
            axis->state = SWITCH_ON_DISABLED;
        }
        else if (is_command(controlword, SWITCH_ON_CMD)) {
            // transition 3
            axis->state = SWITCHED_ON;
            
            if (is_command(controlword, SWITCH_ON_ENABLE_CMD)) {
                // transitions 3 + 4 in one command
                axis->state = OPERATION_ENABLED;
                // TODO enable axis here
            }
        }
        break;
    }
    
    case SWITCHED_ON: 
    {
        *(axis->statusword) |= SWITCHED_ON;
        if (is_command(controlword, SHUTDOWN_CMD)) {
            // transition 6
            axis->state = READY_TO_SWITCH_ON;
        }
        else if (is_command(controlword, ENABLE_OPERATION)) {
            // transition 4
            axis->state = OPERATION_ENABLED;
            // TODO enable axis here
        }
        else if (is_command(controlword, DISABLE_VOLTAGE_CMD)) {
            // transition 10
            axis->state = SWITCH_ON_DISABLED;
        }
        break;
    }
    
    case OPERATION_ENABLED: 
    {
        *(axis->statusword) |= OPERATION_ENABLED;
        if (is_command(controlword, DISABLE_OPERATION_CMD)) {
            // transition 5
            axis->state = SWITCHED_ON;
        }
        else if (is_command(controlword, SHUTDOWN_CMD)) {
            // transition 8
            axis->state = READY_TO_SWITCH_ON;
        }
        else if (is_command(controlword, DISABLE_VOLTAGE_CMD)
            || *(axis->ALstatus) != AL_STATUS_OP) { // connection lost 
            // transition 9
            axis->state = SWITCH_ON_DISABLED;
        }
        else if (is_command(controlword, QUICK_STOP_CMD)) {
            // transition 11
            axis->state = QUICK_STOP_ACTIVE;
        }
        else {
            if (axis->motion_control_hook) {
                axis->motion_control_hook();
            }
        }
        break;
    }
    
    case QUICK_STOP_ACTIVE: 
    {
        *(axis->statusword) |= QUICK_STOP_ACTIVE;
        if (is_command(controlword, DISABLE_VOLTAGE_CMD)) {
            // transition 12
            axis->state = SWITCH_ON_DISABLED;
        }
        else if (is_command(controlword, ENABLE_OPERATION)) {
            // transition 16, supporting not recommended
            // axis->state = OPERATION_ENABLED;
        }        
        break;
    }
    
    case FAULT_REACTION_ACTIVE: 
    {
        *(axis->statusword) |= FAULT_REACTION_ACTIVE;
        // transition 14 is automatic
        axis->state = SWITCH_ON_DISABLED;
        break;
    }
    
    case FAULT: 
    {
        *(axis->statusword) |= FAULT;
        if (is_command(controlword, FAULT_RESET)) {
            // transition 15
            axis->state = SWITCH_ON_DISABLED;
        }
        break;
    }
    
    default:
        // this should never happen, but just in case
        axis->state = NOT_READY_TO_SWITCH_ON;
        *(axis->statusword) |= NOT_READY_TO_SWITCH_ON;
        break;
    }
}
