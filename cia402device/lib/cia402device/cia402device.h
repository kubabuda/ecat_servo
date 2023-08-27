/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

#ifndef __CIA402_H__
#define __CIA402_H__

#include <stdint.h>

/*
    CiA 402 profile state machine basic implementation
    Intentionally not coupled to CANopen implementation, can be used with any CANopen or CoE (CANopen over EtherCAT) stack
 */

// CANopen state machine

#define AL_STATUS_OP                                               0x08


// CiA 402 state machine

// 0x6040 Controlword bits

#define CIA402_CONTROLWORD_SWITCH_ON                               0x0001
#define CIA402_CONTROLWORD_ENABLE_VOLTAGE                          0x0002
#define CIA402_CONTROLWORD_QUICKSTOP                               0x0004  // Optional
#define CIA402_CONTROLWORD_ENABLE_OPERATION                        0x0008
#define CIA402_CONTROLWORD_SETPOINT_ENABLE                         0x0010
// #define CIA402_CONTROLWORD_B5_RESERVED                             0x0020
// #define CIA402_CONTROLWORD_B5_RESERVED                             0x0020
#define CIA402_CONTROLWORD_FAULT_RESET                             0x0080
#define CIA402_CONTROLWORD_HALT                                    0x0100

// 0x6040 Controlword masks

#define CIA402_CONTROLWORD_MASK_SHUTDOWN                           0x0087
#define CIA402_CONTROLWORD_MASK_SWITCHON                           0x0087
#define CIA402_CONTROLWORD_MASK_SWITCHON_ENABLE                    0x008F
#define CIA402_CONTROLWORD_MASK_DISABLEVOLTAGE                     0x0082
#define CIA402_CONTROLWORD_MASK_QUICKSTOP                          0x0086
#define CIA402_CONTROLWORD_MASK_DISABLEOPERATION                   0x008F
#define CIA402_CONTROLWORD_MASK_ENABLEOPERATION                    0x008F
#define CIA402_CONTROLWORD_MASK_FAULTRESET                         0x0080

// 0x6040 Controlword commands

#define CIA402_CONTROLWORD_COMMAND_SHUTDOWN                        0x0006
#define CIA402_CONTROLWORD_COMMAND_SWITCHON                        0x0007
#define CIA402_CONTROLWORD_COMMAND_SWITCHON_ENABLE                 0x000F
#define CIA402_CONTROLWORD_COMMAND_DISABLEVOLTAGE                  0x0000
#define CIA402_CONTROLWORD_COMMAND_QUICKSTOP                       0x0002
#define CIA402_CONTROLWORD_COMMAND_DISABLEOPERATION                0x0007
#define CIA402_CONTROLWORD_COMMAND_ENABLEOPERATION                 0x000F
#define CIA402_CONTROLWORD_COMMAND_FAULTRESET                      0x0080

// 0x6041 Statusword bits

#define CIA402_STATUSWORD_READY_TO_SWITCH_ON                       0x0001
#define CIA402_STATUSWORD_SWITCHED_ON                              0x0002
#define CIA402_STATUSWORD_OPERATION_ENABLED                        0x0004
#define CIA402_STATUSWORD_FAULT                                    0x0008
#define CIA402_STATUSWORD_VOLTAGE_ENABLED                          0x0010 // Optional
#define CIA402_STATUSWORD_QUICKSTOP_ENABLED                        0x0020 // Optional
#define CIA402_STATUSWORD_SWITCH_ON_DISABLED                       0x0040
#define CIA402_STATUSWORD_CSP_DRIVE_FOLLOWS_COMMAND                0x1000
#define CIA402_STATUSWORD_CSP_FOLLOWING_ERROR                      0x2000


enum cia402_controlword_command_t {
    SHUTDOWN_CMD,
    SWITCH_ON_CMD,
    SWITCH_ON_ENABLE_CMD,
    DISABLE_VOLTAGE_CMD,
    QUICK_STOP_CMD,
    DISABLE_OPERATION_CMD,
    ENABLE_OPERATION,
    FAULT_RESET
} typedef cia402_controlword_command_t;


enum cia402_axis_state_t {
    NOT_READY_TO_SWITCH_ON = 0x00,
    SWITCH_ON_DISABLED     = 0x40,
    READY_TO_SWITCH_ON     = 0x21,
    SWITCHED_ON            = 0x23,
    OPERATION_ENABLED      = 0x27,
    QUICK_STOP_ACTIVE      = 0x07,
    FAULT_REACTION_ACTIVE  = 0x0F,
    FAULT                  = 0x08
} typedef cia402_axis_state_t;

enum cia402_axis_transition_t {
    NO_TRANSITION                                = 0,
    NOT_READY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED = 1,
    SWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON     = 2,
    READY_TO_SWITCH_ON_TO_SWITCHED_ON            = 3,
    READY_TO_SWITCH_ON_TO_OPERATION_ENABLED      = 34,
    SWITCHED_ON_TO_OPERATION_ENABLED             = 4,
    OPERATION_ENABLED_TO_SWITCHED_ON             = 5,
    SWITCHED_ON_TO_READY_TO_SWITCH_ON            = 6,
    READY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED     = 7,
    OPERATION_ENABLED_TO_READY_TO_SWITCH_ON      = 8,
    OPERATION_ENABLED_TO_SWITCH_ON_DISABLED      = 9,
    SWITCHED_ON_TO_SWITCH_ON_DISABLED            = 10,
    OPERATION_ENABLED_TO_QUICK_STOP_ACTIVE       = 11,
    QUICK_STOP_ACTIVE_TO_SWITCH_ON_DISABLED      = 12,
    DRIVE_ERROR                                  = 13, // this is not requested by master, should be set by application
    FAULT_REACTION_ACTIVE_TO_FAULT               = 14,
    FAULT_TO_SWITCH_ON_DISABLED                  = 15,
    QUICK_STOP_ACTIVE_TO_OPERATION_ENABLED       = 16, // supporting not recommended
} typedef cia402_axis_transition_t;


struct cia402_axis_t
{
    cia402_axis_state_t state;
    cia402_axis_transition_t transition;
    uint16_t * statusword;  
    uint16_t * ALstatus;
    
    void (*init_od_hook) (void);
    void (*motion_control_hook) (void);
} typedef cia402_axis_t;


void cia402_init(cia402_axis_t * axis);
void cia402_state_machine(cia402_axis_t * axis, uint16_t controlword);


#endif // #ifndef __CIA402_H__
