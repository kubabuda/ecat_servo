#include <unity.h>
#include "cia402device.h"


uint16_t mockOD_Status_Word;
uint16_t mockOD_ALstatus;


cia402_axis_t cia402axis;


void setUp(void) {
    mockOD_ALstatus    = -1;
    mockOD_Status_Word = 0;
    cia402_initialize(&cia402axis, &mockOD_Status_Word, &mockOD_ALstatus);
}

void tearDown(void) {
    // clean stuff up here
}

//*****************************************************************************
//                             SM Initialization
//*****************************************************************************
void cia402_initialize_givenstatuswordPtr_shouldSetstatuswordPtr() {
    cia402axis.state = -1;
    // act
    cia402_initialize(&cia402axis, &mockOD_Status_Word, &mockOD_ALstatus);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.statusword == &mockOD_Status_Word, "statusword pointer should be set to call parameter");
}

void cia402_initialize_givenALstatusPtr_shouldSetALstatusPtr() {
    cia402axis.state = -1;
    // act
    cia402_initialize(&cia402axis, &mockOD_Status_Word, &mockOD_ALstatus);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.ALstatus == &mockOD_ALstatus, "ALstatus pointer should be set to call parameter");
}

void cia402_initialize_givenAxis_shouldSetStateNOT_READY_TO_SWITCH_ON() {
    cia402axis.state = -1;
    // act
    cia402_initialize(&cia402axis, &mockOD_Status_Word, &mockOD_ALstatus);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == NOT_READY_TO_SWITCH_ON, "state should be NOT_READY_TO_SWITCH_ON (0)");
}

void cia402_initialize_givenAxis_shouldSetNO_TRANSITION() {
    cia402axis.transition = -1;
    // act
    cia402_initialize(&cia402axis, &mockOD_Status_Word, &mockOD_ALstatus);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NO_TRANSITION, "transition should be NO_TRANSITION (0)");
}

void cia402_initialize_givenAxis_shouldClearFlags() {
    cia402axis.transition = -1;
    cia402axis.flags.config_allowed    = 1;
    cia402axis.flags.axis_func_enabled = 1;
    cia402axis.flags.hv_power_applied  = 1;
    cia402axis.flags.brake_applied     = 1;
    // act
    cia402_initialize(&cia402axis, &mockOD_Status_Word, &mockOD_ALstatus);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.config_allowed == 0, "config_allowed should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.axis_func_enabled == 0, "axis_func_enabled should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.hv_power_applied == 0, "hv_power_applied should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.brake_applied == 0, "brake_applied should be set to 0");
}

void cia402_initialize_givenAxis_shouldClearPrevFlags() {
    cia402axis.transition = -1;
    cia402axis.prevflags.config_allowed    = 1;
    cia402axis.prevflags.axis_func_enabled = 1;
    cia402axis.prevflags.hv_power_applied  = 1;
    cia402axis.prevflags.brake_applied     = 1;
    // act
    cia402_initialize(&cia402axis, &mockOD_Status_Word, &mockOD_ALstatus);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.prevflags.config_allowed == 0, "config_allowed should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.prevflags.axis_func_enabled == 0, "axis_func_enabled should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.prevflags.hv_power_applied == 0, "hv_power_applied should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.prevflags.brake_applied == 0, "brake_applied should be set to 0");
}
//*****************************************************************************
//                             SM Invalid state handling
//*****************************************************************************
void cia402_state_machine_givenAxisWithInvalidState_shouldSetStateToNOT_READY_TO_SWITCH_ON() {
    cia402axis.state = -1;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == NOT_READY_TO_SWITCH_ON, "state should be NOT_READY_TO_SWITCH_ON (0)");
}

void cia402_state_machine_givenAxisWithInvalidState_shouldSetStatuswordToNOT_READY_TO_SWITCH_ON() {
    cia402axis.state = -1;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == NOT_READY_TO_SWITCH_ON, "statusword should be NOT_READY_TO_SWITCH_ON (0)");
}

void cia402_state_machine_givenAxisWithInvalidState_shouldSetTransitionToNO_TRANSITION() {
    cia402axis.state = -1;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NO_TRANSITION, "transition should be NO_TRANSITION (0)");
}

void cia402_state_machine_givenAxisWithInvalidState_shouldClearFlags() {
    cia402axis.transition = -1;
    cia402axis.flags.config_allowed    = 1;
    cia402axis.flags.axis_func_enabled = 1;
    cia402axis.flags.hv_power_applied  = 1;
    cia402axis.flags.brake_applied     = 1;
    cia402axis.state = -1;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.config_allowed == 0, "config_allowed should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.axis_func_enabled == 0, "axis_func_enabled should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.hv_power_applied == 0, "hv_power_applied should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.brake_applied == 0, "brake_applied should be set to 0");
}
//*****************************************************************************
//                             NOT_READY_TO_SWITCH_ON
//*****************************************************************************
void cia402_state_machine_givenNOT_READY_TO_SWITCH_ON_andAnyCommand_shouldSetStateNOT_READY_TO_SWITCH_ON() {
    cia402axis.state = NOT_READY_TO_SWITCH_ON;
    *(cia402axis.ALstatus) = 0;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == NOT_READY_TO_SWITCH_ON, "state should be NOT_READY_TO_SWITCH_ON (0)");
}

void cia402_state_machine_givenNOT_READY_TO_SWITCH_ON_andAnyCommand_shouldSetStatuswordNOT_READY_TO_SWITCH_ON() {
    cia402axis.state = NOT_READY_TO_SWITCH_ON;
    *(cia402axis.ALstatus) = 0;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == NOT_READY_TO_SWITCH_ON, "statusword should be NOT_READY_TO_SWITCH_ON (0)");
}

void cia402_state_machine_givenNOT_READY_TO_SWITCH_ON_andAnyCommand_shouldSetTransitionNO_TRANSITION() {
    cia402axis.state = NOT_READY_TO_SWITCH_ON;
    cia402axis.transition = -1;
    *(cia402axis.ALstatus) = 0;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NO_TRANSITION, "transition should be NO_TRANSITION (0)");
}

void cia402_state_machine_givenNOT_READY_TO_SWITCH_ON_anyCommandandAL_STATUS_OP_shouldSetStateSWITCH_ON_DISABLED() {
    cia402axis.state = NOT_READY_TO_SWITCH_ON;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCH_ON_DISABLED, "state should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenNOT_READY_TO_SWITCH_ON_anyCommandandAL_STATUS_OP_shouldSetStatuswordSWITCH_ON_DISABLED() {
    cia402axis.state = NOT_READY_TO_SWITCH_ON;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == SWITCH_ON_DISABLED, "statusword should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenNOT_READY_TO_SWITCH_ON_anyCommandandAL_STATUS_OP_shouldSetTransitionNOT_READY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED() {
    cia402axis.state = NOT_READY_TO_SWITCH_ON;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NOT_READY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED, "transition should be NOT_READY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED (1)");
}

void cia402_state_machine_givenNOT_READY_TO_SWITCH_ON_anyCommandandAL_STATUS_OP_shouldSetExpectedFlagsForSWITCH_ON_DISABLED() {
    cia402axis.flags.config_allowed    = 1;
    cia402axis.flags.axis_func_enabled = 1;
    cia402axis.flags.hv_power_applied  = 1;
    cia402axis.flags.brake_applied     = 1;
    cia402axis.state = NOT_READY_TO_SWITCH_ON;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.config_allowed == 1, "config_allowed should be set to 1");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.axis_func_enabled == 0, "axis_func_enabled should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.hv_power_applied == 0, "hv_power_applied should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.brake_applied == 1, "brake_applied should be set to 1");
}
//*****************************************************************************
//                             SWITCH_ON_DISABLED
//*****************************************************************************
void cia402_state_machine_givenSWITCH_ON_DISABLED_andInvalidCommand_shouldSetStateSWITCH_ON_DISABLED() {
    cia402axis.state = SWITCH_ON_DISABLED;
    *(cia402axis.ALstatus) = 0;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCH_ON_DISABLED, "state should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenSWITCH_ON_DISABLED_andInvalidCommand_shouldSetStatuswordSWITCH_ON_DISABLED() {
    cia402axis.state = SWITCH_ON_DISABLED;
    *(cia402axis.ALstatus) = 0;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == SWITCH_ON_DISABLED, "statusword should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenSWITCH_ON_DISABLED_andInvalidCommand_shouldSetTransitionNO_TRANSITION() {
    cia402axis.state = SWITCH_ON_DISABLED;
    cia402axis.transition = -1;
    *(cia402axis.ALstatus) = 0;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NO_TRANSITION, "transition should be NO_TRANSITION (0)");
}

void cia402_state_machine_givenSWITCH_ON_DISABLED_anyCommandAndAL_STATUS_OP_shouldSetStateREADY_TO_SWITCH_ON() {
    cia402axis.state = SWITCH_ON_DISABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == READY_TO_SWITCH_ON, "state should be READY_TO_SWITCH_ON (0x21)");
}

void cia402_state_machine_givenSWITCH_ON_DISABLED_anyCommandAndAL_STATUS_OP_shouldSetStatuswordREADY_TO_SWITCH_ON() {
    cia402axis.state = SWITCH_ON_DISABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == READY_TO_SWITCH_ON, "statusword should be READY_TO_SWITCH_ON (0x21)");
}

void cia402_state_machine_givenSWITCH_ON_DISABLED_anyCommandAndAL_STATUS_OP_shouldSetTransitionSWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON() {
    cia402axis.state = SWITCH_ON_DISABLED;
    cia402axis.transition = -1;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == SWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON, "transition should be SWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON (2)");
}

void cia402_state_machine_givenSWITCH_ON_DISABLED_anyCommandAndAL_STATUS_OP_shouldSetExpectedFlagsForREADY_TO_SWITCH_ON() {
    cia402axis.flags.config_allowed    = 1;
    cia402axis.flags.axis_func_enabled = 1;
    cia402axis.flags.hv_power_applied  = 1;
    cia402axis.flags.brake_applied     = 1;
    cia402axis.state = SWITCH_ON_DISABLED;
    cia402axis.transition = -1;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.config_allowed == 1, "config_allowed should be set to 1");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.axis_func_enabled == 0, "axis_func_enabled should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.hv_power_applied == 0, "hv_power_applied should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.brake_applied == 1, "brake_applied should be set to 1");
}

void cia402_state_machine_givenSWITCH_ON_DISABLED_andSHUTDOWN_shouldSetStateREADY_TO_SWITCH_ON() {
    cia402axis.state = SWITCH_ON_DISABLED;
    *(cia402axis.ALstatus) = 0;
    uint16_t controlword = CIA402_CONTROLWORD_SHUTDOWN_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == READY_TO_SWITCH_ON, "state should be READY_TO_SWITCH_ON (0x21)");
}

void cia402_state_machine_givenSWITCH_ON_DISABLED_andSHUTDOWN_shouldSetStatuswordREADY_TO_SWITCH_ON() {
    cia402axis.state = SWITCH_ON_DISABLED;
    *(cia402axis.ALstatus) = 0;
    uint16_t controlword = CIA402_CONTROLWORD_SHUTDOWN_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == READY_TO_SWITCH_ON, "statusword should be READY_TO_SWITCH_ON (0x21)");
}

void cia402_state_machine_givenSWITCH_ON_DISABLED_andSHUTDOWN_shouldSetTransitionSWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON() {
    cia402axis.state = SWITCH_ON_DISABLED;
    cia402axis.transition = -1;
    *(cia402axis.ALstatus) = 0;
    uint16_t controlword = CIA402_CONTROLWORD_SHUTDOWN_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == SWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON, "transition should be SWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON (2)");
}

void cia402_state_machine_givenSWITCH_ON_DISABLED_andSHUTDOWN_shouldSetExpectedFlagsForREADY_TO_SWITCH_ON() {
    cia402axis.flags.config_allowed    = 1;
    cia402axis.flags.axis_func_enabled = 1;
    cia402axis.flags.hv_power_applied  = 1;
    cia402axis.flags.brake_applied     = 1;
    cia402axis.state = SWITCH_ON_DISABLED;
    cia402axis.transition = -1;
    *(cia402axis.ALstatus) = 0;
    uint16_t controlword = CIA402_CONTROLWORD_SHUTDOWN_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.config_allowed == 1, "config_allowed should be set to 1");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.axis_func_enabled == 0, "axis_func_enabled should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.hv_power_applied == 0, "hv_power_applied should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.brake_applied == 1, "brake_applied should be set to 1");
}

//*****************************************************************************
//                             READY_TO_SWITCH_ON
//*****************************************************************************
void cia402_state_machine_givenREADY_TO_SWITCH_ON_andInvalidCommand_shouldSetStateREADY_TO_SWITCH_ON() {
    cia402axis.state = READY_TO_SWITCH_ON;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == READY_TO_SWITCH_ON, "state should be READY_TO_SWITCH_ON (0x21)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andInvalidCommand_shouldSetStatuswordREADY_TO_SWITCH_ON() {
    cia402axis.state = READY_TO_SWITCH_ON;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == READY_TO_SWITCH_ON, "statusword should be READY_TO_SWITCH_ON (0x21)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andInvalidCommand_shouldSetTransitionNO_TRANSITION() {
    cia402axis.state = READY_TO_SWITCH_ON;
    cia402axis.transition = -1;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NO_TRANSITION, "transition should be NO_TRANSITION (0)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andDISABLE_VOLTAGE_shouldSetStateSWITCH_ON_DISABLED() {
    cia402axis.state = READY_TO_SWITCH_ON;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCH_ON_DISABLED, "state should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andDISABLE_VOLTAGE_shouldSetStatuswordSWITCH_ON_DISABLED() {
    cia402axis.state = READY_TO_SWITCH_ON;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == SWITCH_ON_DISABLED, "statusword should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andDISABLE_VOLTAGE_shouldSetExpectedFlagsForSWITCH_ON_DISABLED() {
    cia402axis.flags.config_allowed    = 1;
    cia402axis.flags.axis_func_enabled = 1;
    cia402axis.flags.hv_power_applied  = 1;
    cia402axis.flags.brake_applied     = 1;
    cia402axis.state = READY_TO_SWITCH_ON;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.config_allowed == 1, "config_allowed should be set to 1");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.axis_func_enabled == 0, "axis_func_enabled should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.hv_power_applied == 0, "hv_power_applied should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.brake_applied == 1, "brake_applied should be set to 1");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andDISABLE_VOLTAGE_shouldSetTransitionREADY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED() {
    cia402axis.state = READY_TO_SWITCH_ON;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == READY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED, "transition should be READY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED (7)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_shouldSetStateSWITCHED_ON() {
    cia402axis.state = READY_TO_SWITCH_ON;
    uint16_t controlword = CIA402_CONTROLWORD_SWITCH_ON_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCHED_ON, "state should be SWITCHED_ON (0x40)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_shouldSetStatuswordSWITCHED_ON() {
    cia402axis.state = READY_TO_SWITCH_ON;
    uint16_t controlword = CIA402_CONTROLWORD_SWITCH_ON_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == SWITCHED_ON, "statusword should be SWITCHED_ON (0x40)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_shouldSetTransitionREADY_TO_SWITCH_ON_TO_SWITCHED_ON() {
    cia402axis.state = READY_TO_SWITCH_ON;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_SWITCH_ON_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == READY_TO_SWITCH_ON_TO_SWITCHED_ON, "transition should be READY_TO_SWITCH_ON_TO_SWITCHED_ON (3)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_shouldSetExpectedFlagsForSWITCHED_ON() {
    cia402axis.flags.config_allowed    = 1;
    cia402axis.flags.axis_func_enabled = 1;
    cia402axis.flags.hv_power_applied  = 1;
    cia402axis.flags.brake_applied     = 1;
    cia402axis.state = READY_TO_SWITCH_ON;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_SWITCH_ON_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.config_allowed == 1, "config_allowed should be set to 1");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.axis_func_enabled == 0, "axis_func_enabled should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.hv_power_applied == 1, "hv_power_applied should be set to 1");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.brake_applied == 1, "brake_applied should be set to 1");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_ENABLE_shouldSetStateOPERATION_ENABLED() {
    cia402axis.state = READY_TO_SWITCH_ON;
    uint16_t controlword = CIA402_CONTROLWORD_SWITCH_ON_ENABLE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == OPERATION_ENABLED, "state should be OPERATION_ENABLED (0x40)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_ENABLE_shouldSetStatuswordOPERATION_ENABLED() {
    cia402axis.state = READY_TO_SWITCH_ON;
    uint16_t controlword = CIA402_CONTROLWORD_SWITCH_ON_ENABLE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == OPERATION_ENABLED, "statusword should be OPERATION_ENABLED (0x40)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_ENABLE_shouldSetTransitionREADY_TO_SWITCH_ON_TO_OPERATION_ENABLED() {
    cia402axis.state = READY_TO_SWITCH_ON;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_SWITCH_ON_ENABLE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == READY_TO_SWITCH_ON_TO_OPERATION_ENABLED, "transition should be READY_TO_SWITCH_ON_TO_OPERATION_ENABLED (34)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_ENABLE_shouldSetExpectedFlagsForOPERATION_ENABLED() {
    cia402axis.flags.config_allowed    = 1;
    cia402axis.flags.axis_func_enabled = 1;
    cia402axis.flags.hv_power_applied  = 1;
    cia402axis.flags.brake_applied     = 1;
    cia402axis.state = READY_TO_SWITCH_ON;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_SWITCH_ON_ENABLE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.config_allowed == 0, "config_allowed should be set to 1");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.axis_func_enabled == 1, "axis_func_enabled should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.hv_power_applied == 1, "hv_power_applied should be set to 1");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.brake_applied == 0, "brake_applied should be set to 1");
}

//*****************************************************************************
//                             SWITCHED_ON
//*****************************************************************************
void cia402_state_machine_givenSWITCHED_ON_andInvalidCommand_shouldSetStateSWITCHED_ON() {
    cia402axis.state = SWITCHED_ON;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCHED_ON, "state should be SWITCHED_ON (0x23)");
}

void cia402_state_machine_givenSWITCHED_ON_andInvalidCommand_shouldSetStatuswordSWITCHED_ON() {
    cia402axis.state = SWITCHED_ON;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == SWITCHED_ON, "state should be SWITCHED_ON (0x23)");
}

void cia402_state_machine_givenSWITCHED_ON_andInvalidCommand_shouldSetTransitionNO_TRANSITION() {
    cia402axis.state = SWITCHED_ON;
    cia402axis.transition = -1;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NO_TRANSITION, "transition should be NO_TRANSITION (0)");
}

void cia402_state_machine_givenSWITCHED_ON_andSHUTDOWN_shouldSetStateREADY_TO_SWITCH_ON() {
    cia402axis.state = SWITCHED_ON;
    uint16_t controlword = CIA402_CONTROLWORD_SHUTDOWN_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == READY_TO_SWITCH_ON, "state should be READY_TO_SWITCH_ON (0x23)");
}

void cia402_state_machine_givenSWITCHED_ON_andSHUTDOWN_shouldSetStatuswordREADY_TO_SWITCH_ON() {
    cia402axis.state = SWITCHED_ON;
    uint16_t controlword = CIA402_CONTROLWORD_SHUTDOWN_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == READY_TO_SWITCH_ON, "statusword should be READY_TO_SWITCH_ON (0x23)");
}

void cia402_state_machine_givenSWITCHED_ON_andSHUTDOWN_shouldSetTransitionSWITCHED_ON_TO_READY_TO_SWITCH_ON() {
    cia402axis.state = SWITCHED_ON;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_SHUTDOWN_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == SWITCHED_ON_TO_READY_TO_SWITCH_ON, "transition should be SWITCHED_ON_TO_READY_TO_SWITCH_ON (6)");
}

void cia402_state_machine_givenSWITCHED_ON_andSHUTDOWN_shouldSetExpectedFlagsForREADY_TO_SWITCH_ON() {
    cia402axis.flags.config_allowed    = 1;
    cia402axis.flags.axis_func_enabled = 1;
    cia402axis.flags.hv_power_applied  = 1;
    cia402axis.flags.brake_applied     = 1;
    cia402axis.state = SWITCHED_ON;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_SHUTDOWN_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.config_allowed == 1, "config_allowed should be set to 1");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.axis_func_enabled == 0, "axis_func_enabled should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.hv_power_applied == 0, "hv_power_applied should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.brake_applied == 1, "brake_applied should be set to 1");
}

void cia402_state_machine_givenSWITCHED_ON_andENABLE_OPERATION_shouldSetStateOPERATION_ENABLED() {
    cia402axis.state = SWITCHED_ON;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == OPERATION_ENABLED, "state should be OPERATION_ENABLED (0x23)");
}

void cia402_state_machine_givenSWITCHED_ON_andENABLE_OPERATION_shouldSetStatuswordOPERATION_ENABLED() {
    cia402axis.state = SWITCHED_ON;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == OPERATION_ENABLED, "statusword should be OPERATION_ENABLED (0x23)");
}

void cia402_state_machine_givenSWITCHED_ON_andENABLE_OPERATION_shouldSetTransitionSWITCHED_ON_TO_OPERATION_ENABLED() {
    cia402axis.state = SWITCHED_ON;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == SWITCHED_ON_TO_OPERATION_ENABLED, "transition should be SWITCHED_ON_TO_OPERATION_ENABLED (4)");
}

void cia402_state_machine_givenSWITCHED_ON_andENABLE_OPERATION_shouldSetExpectedFlagsForOPERATION_ENABLED() {
    cia402axis.flags.config_allowed    = 1;
    cia402axis.flags.axis_func_enabled = 1;
    cia402axis.flags.hv_power_applied  = 1;
    cia402axis.flags.brake_applied     = 1;
    cia402axis.state = SWITCHED_ON;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.config_allowed == 0, "config_allowed should be set to 1");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.axis_func_enabled == 1, "axis_func_enabled should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.hv_power_applied == 1, "hv_power_applied should be set to 1");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.brake_applied == 0, "brake_applied should be set to 1");
}

void cia402_state_machine_givenSWITCHED_ON_andDISABLE_VOLTAGE_shouldSetStateSWITCH_ON_DISABLED() {
    cia402axis.state = SWITCHED_ON;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCH_ON_DISABLED, "state should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenSWITCHED_ON_andDISABLE_VOLTAGE_shouldSetStatuswordSWITCH_ON_DISABLED() {
    cia402axis.state = SWITCHED_ON;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == SWITCH_ON_DISABLED, "statusword should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenSWITCHED_ON_andDISABLE_VOLTAGE_shouldSetExpectedFlagsForSWITCH_ON_DISABLED() {
    cia402axis.flags.config_allowed    = 1;
    cia402axis.flags.axis_func_enabled = 1;
    cia402axis.flags.hv_power_applied  = 1;
    cia402axis.flags.brake_applied     = 1;
    cia402axis.state = SWITCHED_ON;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.config_allowed == 1, "config_allowed should be set to 1");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.axis_func_enabled == 0, "axis_func_enabled should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.hv_power_applied == 0, "hv_power_applied should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.brake_applied == 1, "brake_applied should be set to 1");
}

void cia402_state_machine_givenSWITCHED_ON_andDISABLE_VOLTAGE_shouldSetTransitionSWITCHED_ON_TO_SWITCH_ON_DISABLED() {
    cia402axis.state = SWITCHED_ON;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == SWITCHED_ON_TO_SWITCH_ON_DISABLED, "transition should be SWITCHED_ON_TO_SWITCH_ON_DISABLED (10)");
}

//*****************************************************************************
//                             OPERATION_ENABLED
//*****************************************************************************
void cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_andCIA402_CONTROLWORD_ENABLE_OPERATION_shouldSetStateOPERATION_ENABLED_COMMAND() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == OPERATION_ENABLED, "state should be OPERATION_ENABLED (0x27)");
}

void cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_andCIA402_CONTROLWORD_ENABLE_OPERATION_shouldSetStatuswordOPERATION_ENABLED_COMMAND() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == OPERATION_ENABLED, "statusword should be OPERATION_ENABLED (0x27)");
}

void cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andCIA402_CONTROLWORD_ENABLE_OPERATION_shouldSetTransitionNO_TRANSITION_COMMAND() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NO_TRANSITION, "transition should be NO_TRANSITION (0)");
}

void cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andDISABLE_OPERATION_shouldSetStateSWITCHED_ON() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCHED_ON, "state should be SWITCHED_ON (0x23)");
}

void cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andDISABLE_OPERATION_shouldSetStatuswordSWITCHED_ON() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == SWITCHED_ON, "statusword should be SWITCHED_ON (0x23)");
}

void cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andDISABLE_OPERATION_shouldSetTransitionOPERATION_ENABLED_TO_SWITCHED_ON() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == OPERATION_ENABLED_TO_SWITCHED_ON, "transition should be OPERATION_ENABLED_TO_SWITCHED_ON (5)");
}

void cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andDISABLE_OPERATION_shouldSetExpectedFlagsForSWITCHED_ON() {
    cia402axis.flags.config_allowed    = 1;
    cia402axis.flags.axis_func_enabled = 1;
    cia402axis.flags.hv_power_applied  = 1;
    cia402axis.flags.brake_applied     = 1;
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.config_allowed == 1, "config_allowed should be set to 1");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.axis_func_enabled == 0, "axis_func_enabled should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.hv_power_applied == 1, "hv_power_applied should be set to 1");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.brake_applied == 1, "brake_applied should be set to 1");
}

void cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andSHUTDOWN_shouldSetStateREADY_TO_SWITCH_ON() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = CIA402_CONTROLWORD_SHUTDOWN_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == READY_TO_SWITCH_ON, "state should be READY_TO_SWITCH_ON (0x23)");
}

void cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andSHUTDOWN_shouldSetStatuswordREADY_TO_SWITCH_ON() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = CIA402_CONTROLWORD_SHUTDOWN_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == READY_TO_SWITCH_ON, "statusword should be READY_TO_SWITCH_ON (0x23)");
}

void cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andSHUTDOWN_shouldSetTransitionOPERATION_ENABLED_TO_READY_TO_SWITCH_ON() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_SHUTDOWN_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == OPERATION_ENABLED_TO_READY_TO_SWITCH_ON, "transition should be OPERATION_ENABLED_TO_READY_TO_SWITCH_ON (6)");
}

void cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andSHUTDOWN_shouldSetExpectedFlagsForREADY_TO_SWITCH_ON() {
    cia402axis.flags.config_allowed    = 1;
    cia402axis.flags.axis_func_enabled = 1;
    cia402axis.flags.hv_power_applied  = 1;
    cia402axis.flags.brake_applied     = 1;
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_SHUTDOWN_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.config_allowed == 1, "config_allowed should be set to 1");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.axis_func_enabled == 0, "axis_func_enabled should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.hv_power_applied == 0, "hv_power_applied should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.brake_applied == 1, "brake_applied should be set to 1");
}

void cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndDISABLE_VOLTAGE_shouldSetStateSWITCH_ON_DISABLED() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCH_ON_DISABLED, "state should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndDISABLE_VOLTAGE_shouldSetStatuswordSWITCH_ON_DISABLED() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == SWITCH_ON_DISABLED, "statusword should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndDISABLE_VOLTAGE_shouldSetExpectedFlagsForSWITCH_ON_DISABLED() {
    cia402axis.flags.config_allowed    = 1;
    cia402axis.flags.axis_func_enabled = 1;
    cia402axis.flags.hv_power_applied  = 1;
    cia402axis.flags.brake_applied     = 1;
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.config_allowed == 1, "config_allowed should be set to 1");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.axis_func_enabled == 0, "axis_func_enabled should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.hv_power_applied == 0, "hv_power_applied should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.brake_applied == 1, "brake_applied should be set to 1");
}

void cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndDISABLE_VOLTAGE_shouldSetTransitionOPERATION_ENABLED_TO_SWITCH_ON_DISABLED() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == OPERATION_ENABLED_TO_SWITCH_ON_DISABLED, "transition should be OPERATION_ENABLED_TO_SWITCH_ON_DISABLED (9)");
}

void cia402_state_machine_givenOPERATION_ENABLED_ALstatusDisconnectedAndAnyCommand_shouldSetStateSWITCH_ON_DISABLED() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = 0;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCH_ON_DISABLED, "state should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenOPERATION_ENABLED_ALstatusDisconnectedAndAnyCommand_shouldSetStatuswordSWITCH_ON_DISABLED() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = 0;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == SWITCH_ON_DISABLED, "statusword should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenOPERATION_ENABLED_ALstatusDisconnectedAndAnyCommand_shouldSetExpectedFlagsForSWITCH_ON_DISABLED() {
    cia402axis.flags.config_allowed    = 1;
    cia402axis.flags.axis_func_enabled = 1;
    cia402axis.flags.hv_power_applied  = 1;
    cia402axis.flags.brake_applied     = 1;
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = 0;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.config_allowed == 1, "config_allowed should be set to 1");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.axis_func_enabled == 0, "axis_func_enabled should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.hv_power_applied == 0, "hv_power_applied should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.brake_applied == 1, "brake_applied should be set to 1");
}

void cia402_state_machine_givenOPERATION_ENABLED_ALstatusDisconnectedAndAnyCommand_shouldSetTransitionOPERATION_ENABLED_TO_SWITCH_ON_DISABLED() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = 0;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == OPERATION_ENABLED_TO_SWITCH_ON_DISABLED, "transition should be OPERATION_ENABLED_TO_SWITCH_ON_DISABLED (9)");
}

void cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndQUICK_STOP_shouldSetStateQUICK_STOP_ACTIVE() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = CIA402_CONTROLWORD_QUICK_STOP_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == QUICK_STOP_ACTIVE, "state should be QUICK_STOP_ACTIVE (0x07)");
}

void cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndQUICK_STOP_shouldSetStatuswordQUICK_STOP_ACTIVE() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = CIA402_CONTROLWORD_QUICK_STOP_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == QUICK_STOP_ACTIVE, "statusword should be QUICK_STOP_ACTIVE (0x07)");
}

void cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndQUICK_STOP_shouldSetTransitionOPERATION_ENABLED_TO_QUICK_STOP_ACTIVE() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_QUICK_STOP_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == OPERATION_ENABLED_TO_QUICK_STOP_ACTIVE, "transition should be OPERATION_ENABLED_TO_QUICK_STOP_ACTIVE (11)");
}

void cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndQUICK_STOP_shouldSetExpectedFlagsForQUICK_STOP_ACTIVE() {
    cia402axis.flags.config_allowed    = 1;
    cia402axis.flags.axis_func_enabled = 1;
    cia402axis.flags.hv_power_applied  = 1;
    cia402axis.flags.brake_applied     = 1;
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_QUICK_STOP_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.config_allowed == 0, "config_allowed should be set to 1");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.axis_func_enabled == 1, "axis_func_enabled should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.hv_power_applied == 1, "hv_power_applied should be set to 1");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.brake_applied == 0, "brake_applied should be set to 1");
}

//*****************************************************************************
//                             QUICK_STOP_ACTIVE
//*****************************************************************************
void cia402_state_machine_givenQUICK_STOP_ACTIVE_andInvalidCommand_shouldSetStateQUICK_STOP_ACTIVE() {
    cia402axis.state = QUICK_STOP_ACTIVE;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == QUICK_STOP_ACTIVE, "state should be QUICK_STOP_ACTIVE (0x07)");
}

void cia402_state_machine_givenQUICK_STOP_ACTIVE_andInvalidCommand_shouldSetStatuswordQUICK_STOP_ACTIVE() {
    cia402axis.state = QUICK_STOP_ACTIVE;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == QUICK_STOP_ACTIVE, "statusword should be QUICK_STOP_ACTIVE (0x07)");
}

void cia402_state_machine_givenQUICK_STOP_ACTIVE_andInvalidCommand_shouldSetTransitionNO_TRANSITION() {
    cia402axis.state = QUICK_STOP_ACTIVE;
    cia402axis.transition = -1;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NO_TRANSITION, "transition should be NO_TRANSITION (0)");
}

void cia402_state_machine_givenQUICK_STOP_ACTIVE_andDISABLE_VOLTAGE_shouldSetStateSWITCH_ON_DISABLED() {
    cia402axis.state = QUICK_STOP_ACTIVE;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCH_ON_DISABLED, "state should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenQUICK_STOP_ACTIVE_andDISABLE_VOLTAGE_shouldSetStatuswordSWITCH_ON_DISABLED() {
    cia402axis.state = QUICK_STOP_ACTIVE;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == SWITCH_ON_DISABLED, "statusword should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenQUICK_STOP_ACTIVE_andDISABLE_VOLTAGE_shouldSetExpectedFlagsForSWITCH_ON_DISABLED() {
    cia402axis.flags.config_allowed    = 1;
    cia402axis.flags.axis_func_enabled = 1;
    cia402axis.flags.hv_power_applied  = 1;
    cia402axis.flags.brake_applied     = 1;
    cia402axis.state = QUICK_STOP_ACTIVE;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.config_allowed == 1, "config_allowed should be set to 1");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.axis_func_enabled == 0, "axis_func_enabled should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.hv_power_applied == 0, "hv_power_applied should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.brake_applied == 1, "brake_applied should be set to 1");
}

void cia402_state_machine_givenQUICK_STOP_ACTIVE_andDISABLE_VOLTAGE_shouldSetTransitionQUICK_STOP_ACTIVE_TO_SWITCH_ON_DISABLED() {
    cia402axis.state = QUICK_STOP_ACTIVE;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == QUICK_STOP_ACTIVE_TO_SWITCH_ON_DISABLED, "transition should be QUICK_STOP_ACTIVE_TO_SWITCH_ON_DISABLED (12)");
}

void cia402_state_machine_givenQUICK_STOP_ACTIVE_andENABLE_OPERATION_shouldSetStateQUICK_STOP_ACTIVE() {
    cia402axis.state = QUICK_STOP_ACTIVE;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == QUICK_STOP_ACTIVE, "state should be QUICK_STOP_ACTIVE (0x07): transition 16 support is not recommended");
}

void cia402_state_machine_givenQUICK_STOP_ACTIVE_andENABLE_OPERATION_shouldSetStatuswordQUICK_STOP_ACTIVE() {
    cia402axis.state = QUICK_STOP_ACTIVE;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == QUICK_STOP_ACTIVE, "statusword should be QUICK_STOP_ACTIVE (0x07): transition 16 support is not recommended");
}

void cia402_state_machine_givenQUICK_STOP_ACTIVE_andENABLE_OPERATION_shouldSetTransitionNO_TRANSITION() {
    cia402axis.state = QUICK_STOP_ACTIVE;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NO_TRANSITION, "transition should be NO_TRANSITION (0): transition 16 support is not recommended");
}

//*****************************************************************************
//                             FAULT_REACTION_ACTIVE
//*****************************************************************************
void cia402_state_machine_givenFAULT_REACTION_ACTIVE_andAnyCommand_shouldSetStateFAULT() {
    cia402axis.state = FAULT_REACTION_ACTIVE;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == FAULT, "state should be FAULT (0x08)");
}

void cia402_state_machine_givenFAULT_REACTION_ACTIVE_andAnyCommand_shouldSetStatuswordFAULT() {
    cia402axis.state = FAULT_REACTION_ACTIVE;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == FAULT, "statusword should be FAULT (0x08)");
}

void cia402_state_machine_givenFAULT_REACTION_ACTIVE_andAnyCommand_shouldSetTransitionFAULT_REACTION_ACTIVE_TO_FAULT() {
    cia402axis.state = FAULT_REACTION_ACTIVE;
    cia402axis.transition = -1;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == FAULT_REACTION_ACTIVE_TO_FAULT, "transition should be FAULT_REACTION_ACTIVE_TO_FAULT (14)");
}

void cia402_state_machine_givenFAULT_REACTION_ACTIVE_andAnyCommand_shouldSetExpectedFlagsForFAULT() {
    cia402axis.flags.config_allowed    = 1;
    cia402axis.flags.axis_func_enabled = 1;
    cia402axis.flags.hv_power_applied  = 1;
    cia402axis.flags.brake_applied     = 1;
    cia402axis.state = FAULT_REACTION_ACTIVE;
    cia402axis.transition = -1;
    uint16_t controlword = -1;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.config_allowed == 1, "config_allowed should be set to 1");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.axis_func_enabled == 0, "axis_func_enabled should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.hv_power_applied == 0, "hv_power_applied should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.brake_applied == 0, "brake_applied should be set to 0");
}

//*****************************************************************************
//                             FAULT
//*****************************************************************************
void cia402_state_machine_givenFAULT_andInvalidCommand_shouldSetStateFAULT() {
    cia402axis.state = FAULT;
    uint16_t controlword = 0;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == FAULT, "state should be FAULT (0x08)");
}

void cia402_state_machine_givenFAULT_andInvalidCommand_shouldSetStatuswordFAULT() {
    cia402axis.state = FAULT;
    uint16_t controlword = 0;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == FAULT, "statusword should be FAULT (0x08)");
}

void cia402_state_machine_givenFAULT_andInvalidCommand_shouldSetTransitionNO_TRANSITION() {
    cia402axis.state = FAULT;
    cia402axis.transition = -1;
    uint16_t controlword = 0;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NO_TRANSITION, "transition should be NO_TRANSITION (0)");
}

void cia402_state_machine_givenFAULT_andFAULT_RESET_shouldSetStateSWITCH_ON_DISABLED() {
    cia402axis.state = FAULT;
    uint16_t controlword = CIA402_CONTROLWORD_FAULT_RESET_MASK;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCH_ON_DISABLED, "state should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenFAULT_andFAULT_RESET_shouldSetStatuswordSWITCH_ON_DISABLED() {
    cia402axis.state = FAULT;
    uint16_t controlword = CIA402_CONTROLWORD_FAULT_RESET_MASK;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(*cia402axis.statusword == SWITCH_ON_DISABLED, "statusword should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenFAULT_andFAULT_RESET_shouldSetExpectedFlagsForSWITCH_ON_DISABLED() {
    cia402axis.flags.config_allowed    = 1;
    cia402axis.flags.axis_func_enabled = 1;
    cia402axis.flags.hv_power_applied  = 1;
    cia402axis.flags.brake_applied     = 1;
    cia402axis.state = FAULT;
    uint16_t controlword = CIA402_CONTROLWORD_FAULT_RESET_MASK;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.config_allowed == 1, "config_allowed should be set to 1");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.axis_func_enabled == 0, "axis_func_enabled should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.hv_power_applied == 0, "hv_power_applied should be set to 0");
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.flags.brake_applied == 1, "brake_applied should be set to 1");
}

void cia402_state_machine_givenFAULT_andFAULT_RESET_shouldSetTransitionFAULT_TO_SWITCH_ON_DISABLED() {
    cia402axis.state = FAULT;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_FAULT_RESET_MASK;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == FAULT_TO_SWITCH_ON_DISABLED, "transition should be FAULT_TO_SWITCH_ON_DISABLED (15)");
}

//*****************************************************************************
//                             Run tests
//*****************************************************************************
int main( int argc, char **argv) {
    UNITY_BEGIN();

    // initialization
    RUN_TEST(cia402_initialize_givenstatuswordPtr_shouldSetstatuswordPtr);
    RUN_TEST(cia402_initialize_givenALstatusPtr_shouldSetALstatusPtr);
    RUN_TEST(cia402_initialize_givenAxis_shouldSetStateNOT_READY_TO_SWITCH_ON);
    RUN_TEST(cia402_initialize_givenAxis_shouldSetNO_TRANSITION);
    RUN_TEST(cia402_initialize_givenAxis_shouldClearFlags);
    RUN_TEST(cia402_initialize_givenAxis_shouldClearPrevFlags);
    
    // invalid state
    RUN_TEST(cia402_state_machine_givenAxisWithInvalidState_shouldSetStateToNOT_READY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenAxisWithInvalidState_shouldSetStatuswordToNOT_READY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenAxisWithInvalidState_shouldSetTransitionToNO_TRANSITION);
    RUN_TEST(cia402_state_machine_givenAxisWithInvalidState_shouldClearFlags);
    
    // state NOT_READY_TO_SWITCH_ON
    RUN_TEST(cia402_state_machine_givenNOT_READY_TO_SWITCH_ON_andAnyCommand_shouldSetStateNOT_READY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenNOT_READY_TO_SWITCH_ON_andAnyCommand_shouldSetStatuswordNOT_READY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenNOT_READY_TO_SWITCH_ON_andAnyCommand_shouldSetTransitionNO_TRANSITION);
    
    RUN_TEST(cia402_state_machine_givenNOT_READY_TO_SWITCH_ON_anyCommandandAL_STATUS_OP_shouldSetStateSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenNOT_READY_TO_SWITCH_ON_anyCommandandAL_STATUS_OP_shouldSetStatuswordSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenNOT_READY_TO_SWITCH_ON_anyCommandandAL_STATUS_OP_shouldSetTransitionNOT_READY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenNOT_READY_TO_SWITCH_ON_anyCommandandAL_STATUS_OP_shouldSetExpectedFlagsForSWITCH_ON_DISABLED);
    
    // state SWITCH_ON_DISABLED
    RUN_TEST(cia402_state_machine_givenSWITCH_ON_DISABLED_andInvalidCommand_shouldSetStateSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenSWITCH_ON_DISABLED_andInvalidCommand_shouldSetStatuswordSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenSWITCH_ON_DISABLED_andInvalidCommand_shouldSetTransitionNO_TRANSITION);
    
    RUN_TEST(cia402_state_machine_givenSWITCH_ON_DISABLED_anyCommandAndAL_STATUS_OP_shouldSetStateREADY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenSWITCH_ON_DISABLED_anyCommandAndAL_STATUS_OP_shouldSetStatuswordREADY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenSWITCH_ON_DISABLED_anyCommandAndAL_STATUS_OP_shouldSetTransitionSWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenSWITCH_ON_DISABLED_anyCommandAndAL_STATUS_OP_shouldSetExpectedFlagsForREADY_TO_SWITCH_ON);

    RUN_TEST(cia402_state_machine_givenSWITCH_ON_DISABLED_andSHUTDOWN_shouldSetStateREADY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenSWITCH_ON_DISABLED_andSHUTDOWN_shouldSetStatuswordREADY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenSWITCH_ON_DISABLED_andSHUTDOWN_shouldSetTransitionSWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenSWITCH_ON_DISABLED_andSHUTDOWN_shouldSetExpectedFlagsForREADY_TO_SWITCH_ON);

    // state READY_TO_SWITCH_ON
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andInvalidCommand_shouldSetStateREADY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andInvalidCommand_shouldSetStatuswordREADY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andInvalidCommand_shouldSetTransitionNO_TRANSITION);
    
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andDISABLE_VOLTAGE_shouldSetStateSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andDISABLE_VOLTAGE_shouldSetStatuswordSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andDISABLE_VOLTAGE_shouldSetTransitionREADY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andDISABLE_VOLTAGE_shouldSetExpectedFlagsForSWITCH_ON_DISABLED);
    
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_shouldSetStateSWITCHED_ON);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_shouldSetStatuswordSWITCHED_ON);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_shouldSetTransitionREADY_TO_SWITCH_ON_TO_SWITCHED_ON);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_shouldSetExpectedFlagsForSWITCHED_ON);
    
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_ENABLE_shouldSetStateOPERATION_ENABLED);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_ENABLE_shouldSetStatuswordOPERATION_ENABLED);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_ENABLE_shouldSetTransitionREADY_TO_SWITCH_ON_TO_OPERATION_ENABLED);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_ENABLE_shouldSetExpectedFlagsForOPERATION_ENABLED);

    // state SWITCHED_ON
    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andInvalidCommand_shouldSetStateSWITCHED_ON);
    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andInvalidCommand_shouldSetStatuswordSWITCHED_ON);
    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andInvalidCommand_shouldSetTransitionNO_TRANSITION);
    
    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andSHUTDOWN_shouldSetStateREADY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andSHUTDOWN_shouldSetStatuswordREADY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andSHUTDOWN_shouldSetTransitionSWITCHED_ON_TO_READY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andSHUTDOWN_shouldSetExpectedFlagsForREADY_TO_SWITCH_ON);

    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andENABLE_OPERATION_shouldSetStateOPERATION_ENABLED);
    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andENABLE_OPERATION_shouldSetStatuswordOPERATION_ENABLED);
    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andENABLE_OPERATION_shouldSetTransitionSWITCHED_ON_TO_OPERATION_ENABLED);
    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andENABLE_OPERATION_shouldSetExpectedFlagsForOPERATION_ENABLED);

    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andDISABLE_VOLTAGE_shouldSetStateSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andDISABLE_VOLTAGE_shouldSetStatuswordSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andDISABLE_VOLTAGE_shouldSetTransitionSWITCHED_ON_TO_SWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andDISABLE_VOLTAGE_shouldSetExpectedFlagsForSWITCH_ON_DISABLED);
    
    // state OPERATION_ENABLED
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_andCIA402_CONTROLWORD_ENABLE_OPERATION_shouldSetStateOPERATION_ENABLED_COMMAND);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_andCIA402_CONTROLWORD_ENABLE_OPERATION_shouldSetStatuswordOPERATION_ENABLED_COMMAND);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andCIA402_CONTROLWORD_ENABLE_OPERATION_shouldSetTransitionNO_TRANSITION_COMMAND);
    
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andDISABLE_OPERATION_shouldSetStateSWITCHED_ON);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andDISABLE_OPERATION_shouldSetStatuswordSWITCHED_ON);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andDISABLE_OPERATION_shouldSetTransitionOPERATION_ENABLED_TO_SWITCHED_ON);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andDISABLE_OPERATION_shouldSetExpectedFlagsForSWITCHED_ON);
    
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andSHUTDOWN_shouldSetStateREADY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andSHUTDOWN_shouldSetStatuswordREADY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andSHUTDOWN_shouldSetTransitionOPERATION_ENABLED_TO_READY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andSHUTDOWN_shouldSetExpectedFlagsForREADY_TO_SWITCH_ON);

    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndDISABLE_VOLTAGE_shouldSetStateSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndDISABLE_VOLTAGE_shouldSetStatuswordSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndDISABLE_VOLTAGE_shouldSetTransitionOPERATION_ENABLED_TO_SWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndDISABLE_VOLTAGE_shouldSetExpectedFlagsForSWITCH_ON_DISABLED);

    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_ALstatusDisconnectedAndAnyCommand_shouldSetStateSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_ALstatusDisconnectedAndAnyCommand_shouldSetStatuswordSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_ALstatusDisconnectedAndAnyCommand_shouldSetTransitionOPERATION_ENABLED_TO_SWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_ALstatusDisconnectedAndAnyCommand_shouldSetExpectedFlagsForSWITCH_ON_DISABLED);

    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndQUICK_STOP_shouldSetStateQUICK_STOP_ACTIVE);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndQUICK_STOP_shouldSetStatuswordQUICK_STOP_ACTIVE);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndQUICK_STOP_shouldSetTransitionOPERATION_ENABLED_TO_QUICK_STOP_ACTIVE);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndQUICK_STOP_shouldSetExpectedFlagsForQUICK_STOP_ACTIVE);
    
    // state QUICK_STOP_ACTIVE
    RUN_TEST(cia402_state_machine_givenQUICK_STOP_ACTIVE_andInvalidCommand_shouldSetStateQUICK_STOP_ACTIVE);
    RUN_TEST(cia402_state_machine_givenQUICK_STOP_ACTIVE_andInvalidCommand_shouldSetStatuswordQUICK_STOP_ACTIVE);
    RUN_TEST(cia402_state_machine_givenQUICK_STOP_ACTIVE_andInvalidCommand_shouldSetTransitionNO_TRANSITION);
    
    RUN_TEST(cia402_state_machine_givenQUICK_STOP_ACTIVE_andDISABLE_VOLTAGE_shouldSetStateSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenQUICK_STOP_ACTIVE_andDISABLE_VOLTAGE_shouldSetStatuswordSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenQUICK_STOP_ACTIVE_andDISABLE_VOLTAGE_shouldSetTransitionQUICK_STOP_ACTIVE_TO_SWITCH_ON_DISABLED);    
    RUN_TEST(cia402_state_machine_givenQUICK_STOP_ACTIVE_andDISABLE_VOLTAGE_shouldSetExpectedFlagsForSWITCH_ON_DISABLED);

    RUN_TEST(cia402_state_machine_givenQUICK_STOP_ACTIVE_andENABLE_OPERATION_shouldSetStateQUICK_STOP_ACTIVE);
    RUN_TEST(cia402_state_machine_givenQUICK_STOP_ACTIVE_andENABLE_OPERATION_shouldSetStatuswordQUICK_STOP_ACTIVE);
    RUN_TEST(cia402_state_machine_givenQUICK_STOP_ACTIVE_andENABLE_OPERATION_shouldSetTransitionNO_TRANSITION);
    
    // state FAULT_REACTION_ACTIVE
    RUN_TEST(cia402_state_machine_givenFAULT_REACTION_ACTIVE_andAnyCommand_shouldSetStateFAULT);
    RUN_TEST(cia402_state_machine_givenFAULT_REACTION_ACTIVE_andAnyCommand_shouldSetStatuswordFAULT);
    RUN_TEST(cia402_state_machine_givenFAULT_REACTION_ACTIVE_andAnyCommand_shouldSetTransitionFAULT_REACTION_ACTIVE_TO_FAULT);
    RUN_TEST(cia402_state_machine_givenFAULT_REACTION_ACTIVE_andAnyCommand_shouldSetExpectedFlagsForFAULT);

    // state FAULT
    RUN_TEST(cia402_state_machine_givenFAULT_andInvalidCommand_shouldSetStateFAULT);
    RUN_TEST(cia402_state_machine_givenFAULT_andInvalidCommand_shouldSetStatuswordFAULT);
    RUN_TEST(cia402_state_machine_givenFAULT_andInvalidCommand_shouldSetTransitionNO_TRANSITION);
    
    RUN_TEST(cia402_state_machine_givenFAULT_andFAULT_RESET_shouldSetStateSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenFAULT_andFAULT_RESET_shouldSetStatuswordSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenFAULT_andFAULT_RESET_shouldSetTransitionFAULT_TO_SWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenFAULT_andFAULT_RESET_shouldSetExpectedFlagsForSWITCH_ON_DISABLED);

    UNITY_END();
}
