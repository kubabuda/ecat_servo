#include <unity.h>
#include "cia402device.h"


uint16_t mockOD_Status_Word;
uint16_t mockOD_ALstatus;


cia402_axis_t cia402axis = {
    .ALstatus            = &mockOD_ALstatus,
    .statusword          = &mockOD_Status_Word,
    .state               = NOT_READY_TO_SWITCH_ON,
    .transition          = NO_TRANSITION,
};


void setUp(void) {
    mockOD_ALstatus       = -1;
    mockOD_Status_Word    = -1;
    cia402axis.ALstatus   = &mockOD_ALstatus;
    cia402axis.statusword = &mockOD_Status_Word;
    cia402axis.state      = NOT_READY_TO_SWITCH_ON;
    cia402axis.transition = NO_TRANSITION;
}

void tearDown(void) {
    // clean stuff up here
}

//*****************************************************************************
//                             SM Invalid state handling
//*****************************************************************************
void cia402_state_machine_givenAxisWithInvalidState_shouldResetStateToNOT_READY_TO_SWITCH_ON() {
    cia402axis.state = -1;                 // TODO fuzz
    uint16_t controlword = -1;             // TODO fuzz
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == NOT_READY_TO_SWITCH_ON, "state should be NOT_READY_TO_SWITCH_ON (0)");
}

void cia402_state_machine_givenAxisWithInvalidState_shouldResetTransitionToNO_TRANSITION() {
    cia402axis.state = -1;                 // TODO fuzz
    uint16_t controlword = -1;             // TODO fuzz
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NO_TRANSITION, "transition should be NO_TRANSITION (0)");
}
//*****************************************************************************
//                             NOT_READY_TO_SWITCH_ON
//*****************************************************************************
void cia402_state_machine_givenNOT_READY_TO_SWITCH_ON_andAnyCommand_shouldBeNOT_READY_TO_SWITCH_ON() {
    cia402axis.state = NOT_READY_TO_SWITCH_ON;
    *(cia402axis.ALstatus) = 0;            // TODO fuzz
    uint16_t controlword = -1;             // TODO fuzz
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == NOT_READY_TO_SWITCH_ON, "state should be NOT_READY_TO_SWITCH_ON (0)");
}

void cia402_state_machine_givenNOT_READY_TO_SWITCH_ON_andAnyCommand_shouldBeNO_TRANSITION() {
    cia402axis.state = NOT_READY_TO_SWITCH_ON;
    cia402axis.transition = -1;
    *(cia402axis.ALstatus) = 0;            // TODO fuzz
    uint16_t controlword = -1;             // TODO fuzz
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NO_TRANSITION, "transition should be NO_TRANSITION (0)");
}

void cia402_state_machine_givenNOT_READY_TO_SWITCH_ON_anyCommandandAL_STATUS_OP_shouldBeSWITCH_ON_DISABLED() {
    cia402axis.state = NOT_READY_TO_SWITCH_ON;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = -1;             // TODO fuzz
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCH_ON_DISABLED, "state should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenNOT_READY_TO_SWITCH_ON_anyCommandandAL_STATUS_OP_shouldBeNOT_READY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED() {
    cia402axis.state = NOT_READY_TO_SWITCH_ON;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = -1;             // TODO fuzz
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NOT_READY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED, "transition should be NOT_READY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED (1)");
}

//*****************************************************************************
//                             SWITCH_ON_DISABLED
//*****************************************************************************
void cia402_state_machine_givenSWITCH_ON_DISABLED_andInvalidCommand_shouldBeSWITCH_ON_DISABLED() {
    cia402axis.state = SWITCH_ON_DISABLED;
    *(cia402axis.ALstatus) = 0;            // TODO fuzz
    uint16_t controlword = -1;             // TODO fuzz != SHUTDOWN
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCH_ON_DISABLED, "state should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenSWITCH_ON_DISABLED_andInvalidCommand_shouldBeNO_TRANSITION() {
    cia402axis.state = SWITCH_ON_DISABLED;
    cia402axis.transition = -1;
    *(cia402axis.ALstatus) = 0;            // TODO fuzz
    uint16_t controlword = -1;             // TODO fuzz != SHUTDOWN
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NO_TRANSITION, "transition should be NO_TRANSITION (0)");
}

void cia402_state_machine_givenSWITCH_ON_DISABLED_anyCommandAndAL_STATUS_OP_shouldBeREADY_TO_SWITCH_ON() {
    cia402axis.state = SWITCH_ON_DISABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = -1;             // TODO fuzz != SHUTDOWN
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == READY_TO_SWITCH_ON, "state should be READY_TO_SWITCH_ON (0x21)");
}

void cia402_state_machine_givenSWITCH_ON_DISABLED_anyCommandAndAL_STATUS_OP_shouldBeSWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON() {
    cia402axis.state = SWITCH_ON_DISABLED;
    cia402axis.transition = -1;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = -1;             // TODO fuzz != SHUTDOWN
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == SWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON, "transition should be SWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON (2)");
}

void cia402_state_machine_givenSWITCH_ON_DISABLED_andSHUTDOWN_shouldBeREADY_TO_SWITCH_ON() {
    cia402axis.state = SWITCH_ON_DISABLED;
    *(cia402axis.ALstatus) = 0;             // TODO fuzz
    uint16_t controlword = CIA402_CONTROLWORD_SHUTDOWN_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == READY_TO_SWITCH_ON, "state should be READY_TO_SWITCH_ON (0x21)");
}

void cia402_state_machine_givenSWITCH_ON_DISABLED_andSHUTDOWN_shouldBeSWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON() {
    cia402axis.state = SWITCH_ON_DISABLED;
    cia402axis.transition = -1;
    *(cia402axis.ALstatus) = 0;             // TODO fuzz
    uint16_t controlword = CIA402_CONTROLWORD_SHUTDOWN_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == SWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON, "transition should be SWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON (2)");
}

//*****************************************************************************
//                             READY_TO_SWITCH_ON
//*****************************************************************************
void cia402_state_machine_givenREADY_TO_SWITCH_ON_andInvalidCommand_shouldBeREADY_TO_SWITCH_ON() {
    cia402axis.state = READY_TO_SWITCH_ON;
    uint16_t controlword = -1;             // TODO fuzz != DISABLE_VOLTAGE, SWITCH_ON, SWITCH_ON_ENABLE
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == READY_TO_SWITCH_ON, "state should be READY_TO_SWITCH_ON (0x21)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andInvalidCommand_shouldBeNO_TRANSITION() {
    cia402axis.state = READY_TO_SWITCH_ON;
    cia402axis.transition = -1;
    uint16_t controlword = -1;             // TODO fuzz != DISABLE_VOLTAGE, SWITCH_ON, SWITCH_ON_ENABLE
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NO_TRANSITION, "transition should be NO_TRANSITION (0)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andDISABLE_VOLTAGE_shouldBeSWITCH_ON_DISABLED() {
    cia402axis.state = READY_TO_SWITCH_ON;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCH_ON_DISABLED, "state should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andDISABLE_VOLTAGE_shouldBeREADY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED() {
    cia402axis.state = READY_TO_SWITCH_ON;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == READY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED, "transition should be READY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED (7)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_shouldBeSWITCHED_ON() {
    cia402axis.state = READY_TO_SWITCH_ON;
    uint16_t controlword = CIA402_CONTROLWORD_SWITCH_ON_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCHED_ON, "state should be SWITCHED_ON (0x40)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_shouldBeREADY_TO_SWITCH_ON_TO_SWITCHED_ON() {
    cia402axis.state = READY_TO_SWITCH_ON;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_SWITCH_ON_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == READY_TO_SWITCH_ON_TO_SWITCHED_ON, "transition should be READY_TO_SWITCH_ON_TO_SWITCHED_ON (3)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_ENABLE_shouldBeOPERATION_ENABLED() {
    cia402axis.state = READY_TO_SWITCH_ON;
    uint16_t controlword = CIA402_CONTROLWORD_SWITCH_ON_ENABLE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == OPERATION_ENABLED, "state should be OPERATION_ENABLED (0x40)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_ENABLE_shouldBeREADY_TO_SWITCH_ON_TO_OPERATION_ENABLED() {
    cia402axis.state = READY_TO_SWITCH_ON;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_SWITCH_ON_ENABLE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == READY_TO_SWITCH_ON_TO_OPERATION_ENABLED, "transition should be READY_TO_SWITCH_ON_TO_OPERATION_ENABLED (34)");
}

//*****************************************************************************
//                             SWITCHED_ON
//*****************************************************************************
void cia402_state_machine_givenSWITCHED_ON_andInvalidCommand_shouldBeSWITCHED_ON() {
    cia402axis.state = SWITCHED_ON;
    uint16_t controlword = -1;             // TODO fuzz != SHUTDOWN ENABLE_OPERATION DISABLE_VOLTAGE
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCHED_ON, "state should be SWITCHED_ON (0x23)");
}

void cia402_state_machine_givenSWITCHED_ON_andInvalidCommand_shouldBeNO_TRANSITION() {
    cia402axis.state = SWITCHED_ON;
    cia402axis.transition = -1;
    uint16_t controlword = -1;             // TODO fuzz != SHUTDOWN ENABLE_OPERATION DISABLE_VOLTAGE
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NO_TRANSITION, "transition should be NO_TRANSITION (0)");
}

void cia402_state_machine_givenSWITCHED_ON_andSHUTDOWN_shouldBeREADY_TO_SWITCH_ON() {
    cia402axis.state = SWITCHED_ON;
    uint16_t controlword = CIA402_CONTROLWORD_SHUTDOWN_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == READY_TO_SWITCH_ON, "state should be READY_TO_SWITCH_ON (0x23)");
}

void cia402_state_machine_givenSWITCHED_ON_andSHUTDOWN_shouldBeSWITCHED_ON_TO_READY_TO_SWITCH_ON() {
    cia402axis.state = SWITCHED_ON;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_SHUTDOWN_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == SWITCHED_ON_TO_READY_TO_SWITCH_ON, "transition should be SWITCHED_ON_TO_READY_TO_SWITCH_ON (6)");
}

void cia402_state_machine_givenSWITCHED_ON_andENABLE_OPERATION_shouldBeOPERATION_ENABLED() {
    cia402axis.state = SWITCHED_ON;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == OPERATION_ENABLED, "state should be OPERATION_ENABLED (0x23)");
}

void cia402_state_machine_givenSWITCHED_ON_andENABLE_OPERATION_shouldBeSWITCHED_ON_TO_OPERATION_ENABLED() {
    cia402axis.state = SWITCHED_ON;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == SWITCHED_ON_TO_OPERATION_ENABLED, "transition should be SWITCHED_ON_TO_OPERATION_ENABLED (4)");
}

void cia402_state_machine_givenSWITCHED_ON_andDISABLE_VOLTAGE_shouldBeSWITCH_ON_DISABLED() {
    cia402axis.state = SWITCHED_ON;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCH_ON_DISABLED, "state should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenSWITCHED_ON_andDISABLE_VOLTAGE_shouldBeSWITCHED_ON_TO_SWITCH_ON_DISABLED() {
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
void cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_andCIA402_CONTROLWORD_ENABLE_OPERATION_shouldBeOPERATION_ENABLED_COMMAND() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == OPERATION_ENABLED, "state should be OPERATION_ENABLED (0x27)");
}

void cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andCIA402_CONTROLWORD_ENABLE_OPERATION_shouldBeNO_TRANSITION_COMMAND() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NO_TRANSITION, "transition should be NO_TRANSITION (0)");
}

void cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andDISABLE_OPERATION_shouldBeSWITCHED_ON() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCHED_ON, "state should be SWITCHED_ON (0x23)");
}

void cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andDISABLE_OPERATION_shouldBeOPERATION_ENABLED_TO_SWITCHED_ON() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == OPERATION_ENABLED_TO_SWITCHED_ON, "transition should be OPERATION_ENABLED_TO_SWITCHED_ON (5)");
}

void cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andSHUTDOWN_shouldBeREADY_TO_SWITCH_ON() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = CIA402_CONTROLWORD_SHUTDOWN_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == READY_TO_SWITCH_ON, "state should be READY_TO_SWITCH_ON (0x23)");
}

void cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andSHUTDOWN_shouldBeOPERATION_ENABLED_TO_READY_TO_SWITCH_ON() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_SHUTDOWN_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == OPERATION_ENABLED_TO_READY_TO_SWITCH_ON, "transition should be OPERATION_ENABLED_TO_READY_TO_SWITCH_ON (6)");
}

void cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndDISABLE_VOLTAGE_shouldBeSWITCH_ON_DISABLED() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCH_ON_DISABLED, "state should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndDISABLE_VOLTAGE_shouldBeOPERATION_ENABLED_TO_SWITCH_ON_DISABLED() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == OPERATION_ENABLED_TO_SWITCH_ON_DISABLED, "transition should be OPERATION_ENABLED_TO_SWITCH_ON_DISABLED (9)");
}

void cia402_state_machine_givenOPERATION_ENABLED_ALstatusDisconnectedAndAnyCommand_shouldBeSWITCH_ON_DISABLED() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = 0;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCH_ON_DISABLED, "state should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenOPERATION_ENABLED_ALstatusDisconnectedAndAnyCommand_shouldBeOPERATION_ENABLED_TO_SWITCH_ON_DISABLED() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = 0;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == OPERATION_ENABLED_TO_SWITCH_ON_DISABLED, "transition should be OPERATION_ENABLED_TO_SWITCH_ON_DISABLED (9)");
}

void cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndQUICK_STOP_shouldBeQUICK_STOP_ACTIVE() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = CIA402_CONTROLWORD_QUICK_STOP_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == QUICK_STOP_ACTIVE, "state should be QUICK_STOP_ACTIVE (0x07)");
}

void cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndQUICK_STOP_shouldBeOPERATION_ENABLED_TO_QUICK_STOP_ACTIVE() {
    cia402axis.state = OPERATION_ENABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_QUICK_STOP_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == OPERATION_ENABLED_TO_QUICK_STOP_ACTIVE, "transition should be OPERATION_ENABLED_TO_QUICK_STOP_ACTIVE (11)");
}

//*****************************************************************************
//                             QUICK_STOP_ACTIVE
//*****************************************************************************
void cia402_state_machine_givenQUICK_STOP_ACTIVE_andInvalidCommand_shouldBeQUICK_STOP_ACTIVE() {
    cia402axis.state = QUICK_STOP_ACTIVE;
    uint16_t controlword = -1;             // TODO fuzz != DISABLE_VOLTAGE ENABLE_OPERATION
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == QUICK_STOP_ACTIVE, "state should be QUICK_STOP_ACTIVE (0x07)");
}

void cia402_state_machine_givenQUICK_STOP_ACTIVE_andInvalidCommand_shouldBeNO_TRANSITION() {
    cia402axis.state = QUICK_STOP_ACTIVE;
    cia402axis.transition = -1;
    uint16_t controlword = -1;             // TODO fuzz != DISABLE_VOLTAGE ENABLE_OPERATION
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NO_TRANSITION, "transition should be NO_TRANSITION (0)");
}

void cia402_state_machine_givenQUICK_STOP_ACTIVE_andDISABLE_VOLTAGE_shouldBeSWITCH_ON_DISABLED() {
    cia402axis.state = QUICK_STOP_ACTIVE;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCH_ON_DISABLED, "state should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenQUICK_STOP_ACTIVE_andDISABLE_VOLTAGE_shouldBeQUICK_STOP_ACTIVE_TO_SWITCH_ON_DISABLED() {
    cia402axis.state = QUICK_STOP_ACTIVE;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_DISABLE_VOLTAGE_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == QUICK_STOP_ACTIVE_TO_SWITCH_ON_DISABLED, "transition should be QUICK_STOP_ACTIVE_TO_SWITCH_ON_DISABLED (12)");
}

void cia402_state_machine_givenQUICK_STOP_ACTIVE_andENABLE_OPERATION_shouldBeQUICK_STOP_ACTIVE() {
    cia402axis.state = QUICK_STOP_ACTIVE;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == QUICK_STOP_ACTIVE, "state should be QUICK_STOP_ACTIVE (0x07): transition 16 support is not recommended");
}

void cia402_state_machine_givenQUICK_STOP_ACTIVE_andENABLE_OPERATION_shouldBeNO_TRANSITION() {
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
void cia402_state_machine_givenFAULT_REACTION_ACTIVE_andAnyCommand_shouldBeFAULT() {
    cia402axis.state = FAULT_REACTION_ACTIVE;
    uint16_t controlword = -1;             // TODO fuzz
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == FAULT, "state should be FAULT (0x08)");
}

void cia402_state_machine_givenFAULT_REACTION_ACTIVE_andAnyCommand_shouldBeFAULT_REACTION_ACTIVE_TO_FAULT() {
    cia402axis.state = FAULT_REACTION_ACTIVE;
    cia402axis.transition = -1;
    uint16_t controlword = -1;             // TODO fuzz
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == FAULT_REACTION_ACTIVE_TO_FAULT, "transition should be FAULT_REACTION_ACTIVE_TO_FAULT (14)");
}

//*****************************************************************************
//                             FAULT
//*****************************************************************************
void cia402_state_machine_givenFAULT_andInvalidCommand_shouldBeFAULT() {
    cia402axis.state = FAULT;
    uint16_t controlword = 0;              // TODO fuzz != FAULT_RESET
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == FAULT, "state should be FAULT (0x08)");
}

void cia402_state_machine_givenFAULT_andInvalidCommand_shouldBeNO_TRANSITION() {
    cia402axis.state = FAULT;
    cia402axis.transition = -1;
    uint16_t controlword = 0;              // TODO fuzz != FAULT_RESET
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NO_TRANSITION, "transition should be NO_TRANSITION (0)");
}

void cia402_state_machine_givenFAULT_andFAULT_RESET_shouldBeSWITCH_ON_DISABLED() {
    cia402axis.state = FAULT;
    uint16_t controlword = CIA402_CONTROLWORD_FAULT_RESET_MASK;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCH_ON_DISABLED, "state should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenFAULT_andFAULT_RESET_shouldBeFAULT_TO_SWITCH_ON_DISABLED() {
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

    // invalid state
    RUN_TEST(cia402_state_machine_givenAxisWithInvalidState_shouldResetStateToNOT_READY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenAxisWithInvalidState_shouldResetTransitionToNO_TRANSITION);
    // state NOT_READY_TO_SWITCH_ON
    RUN_TEST(cia402_state_machine_givenNOT_READY_TO_SWITCH_ON_andAnyCommand_shouldBeNOT_READY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenNOT_READY_TO_SWITCH_ON_andAnyCommand_shouldBeNO_TRANSITION);
    RUN_TEST(cia402_state_machine_givenNOT_READY_TO_SWITCH_ON_anyCommandandAL_STATUS_OP_shouldBeSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenNOT_READY_TO_SWITCH_ON_anyCommandandAL_STATUS_OP_shouldBeNOT_READY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED);
    // state SWITCH_ON_DISABLED
    RUN_TEST(cia402_state_machine_givenSWITCH_ON_DISABLED_andInvalidCommand_shouldBeSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenSWITCH_ON_DISABLED_andInvalidCommand_shouldBeNO_TRANSITION);
    RUN_TEST(cia402_state_machine_givenSWITCH_ON_DISABLED_anyCommandAndAL_STATUS_OP_shouldBeREADY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenSWITCH_ON_DISABLED_anyCommandAndAL_STATUS_OP_shouldBeSWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenSWITCH_ON_DISABLED_andSHUTDOWN_shouldBeREADY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenSWITCH_ON_DISABLED_andSHUTDOWN_shouldBeSWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON);
    // state READY_TO_SWITCH_ON
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andInvalidCommand_shouldBeREADY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andInvalidCommand_shouldBeNO_TRANSITION);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andDISABLE_VOLTAGE_shouldBeSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andDISABLE_VOLTAGE_shouldBeREADY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_shouldBeSWITCHED_ON);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_shouldBeREADY_TO_SWITCH_ON_TO_SWITCHED_ON);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_ENABLE_shouldBeOPERATION_ENABLED);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_ENABLE_shouldBeREADY_TO_SWITCH_ON_TO_OPERATION_ENABLED);
    // state SWITCHED_ON
    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andInvalidCommand_shouldBeSWITCHED_ON);
    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andInvalidCommand_shouldBeNO_TRANSITION);
    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andSHUTDOWN_shouldBeREADY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andSHUTDOWN_shouldBeSWITCHED_ON_TO_READY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andENABLE_OPERATION_shouldBeOPERATION_ENABLED);
    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andENABLE_OPERATION_shouldBeSWITCHED_ON_TO_OPERATION_ENABLED);
    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andDISABLE_VOLTAGE_shouldBeSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenSWITCHED_ON_andDISABLE_VOLTAGE_shouldBeSWITCHED_ON_TO_SWITCH_ON_DISABLED);
    // state OPERATION_ENABLED
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_andCIA402_CONTROLWORD_ENABLE_OPERATION_shouldBeOPERATION_ENABLED_COMMAND);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andCIA402_CONTROLWORD_ENABLE_OPERATION_shouldBeNO_TRANSITION_COMMAND);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andDISABLE_OPERATION_shouldBeSWITCHED_ON);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andDISABLE_OPERATION_shouldBeOPERATION_ENABLED_TO_SWITCHED_ON);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andSHUTDOWN_shouldBeREADY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_ENABLED_AL_STATUS_OP_andSHUTDOWN_shouldBeOPERATION_ENABLED_TO_READY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndDISABLE_VOLTAGE_shouldBeSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndDISABLE_VOLTAGE_shouldBeOPERATION_ENABLED_TO_SWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_ALstatusDisconnectedAndAnyCommand_shouldBeSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_ALstatusDisconnectedAndAnyCommand_shouldBeOPERATION_ENABLED_TO_SWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndQUICK_STOP_shouldBeQUICK_STOP_ACTIVE);
    RUN_TEST(cia402_state_machine_givenOPERATION_ENABLED_AL_STATUS_OP_AndQUICK_STOP_shouldBeOPERATION_ENABLED_TO_QUICK_STOP_ACTIVE);
    // state QUICK_STOP_ACTIVE
    RUN_TEST(cia402_state_machine_givenQUICK_STOP_ACTIVE_andInvalidCommand_shouldBeQUICK_STOP_ACTIVE);
    RUN_TEST(cia402_state_machine_givenQUICK_STOP_ACTIVE_andInvalidCommand_shouldBeNO_TRANSITION);
    RUN_TEST(cia402_state_machine_givenQUICK_STOP_ACTIVE_andDISABLE_VOLTAGE_shouldBeSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenQUICK_STOP_ACTIVE_andDISABLE_VOLTAGE_shouldBeQUICK_STOP_ACTIVE_TO_SWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenQUICK_STOP_ACTIVE_andENABLE_OPERATION_shouldBeQUICK_STOP_ACTIVE);
    RUN_TEST(cia402_state_machine_givenQUICK_STOP_ACTIVE_andENABLE_OPERATION_shouldBeNO_TRANSITION);
    // state FAULT_REACTION_ACTIVE
    RUN_TEST(cia402_state_machine_givenFAULT_REACTION_ACTIVE_andAnyCommand_shouldBeFAULT);
    RUN_TEST(cia402_state_machine_givenFAULT_REACTION_ACTIVE_andAnyCommand_shouldBeFAULT_REACTION_ACTIVE_TO_FAULT);
    // state FAULT
    RUN_TEST(cia402_state_machine_givenFAULT_andInvalidCommand_shouldBeFAULT);
    RUN_TEST(cia402_state_machine_givenFAULT_andInvalidCommand_shouldBeNO_TRANSITION);
    RUN_TEST(cia402_state_machine_givenFAULT_andFAULT_RESET_shouldBeSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenFAULT_andFAULT_RESET_shouldBeFAULT_TO_SWITCH_ON_DISABLED);

    UNITY_END();
}
