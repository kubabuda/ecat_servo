#include <unity.h>
#include "cia402device.h"

uint16_t mockOD_Status_Word;
uint16_t mockOD_ALstatus;

void mockObjectDictionarySetup();


cia402_axis_t cia402axis = {
    .init_od_hook        = mockObjectDictionarySetup,
    .motion_control_hook = NULL,
};

void mockObjectDictionarySetup(/*TODO this needs to eat pointer cia402_axis_t* axis*/ ) {
    cia402axis.statusword = &mockOD_Status_Word;
    cia402axis.ALstatus   = &mockOD_ALstatus;
}

void setUp(void) {
    // cia402_init(&cia402axis);
}

void tearDown(void) {
    // clean stuff up here
}

//*****************************************************************************
//                             Initialization
//*****************************************************************************
void cia402_init_givenAxis_shouldSetStateNOT_READY_TO_SWITCH_ON() {
    cia402axis.state = -1;
    // act
    cia402_init(&cia402axis);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == NOT_READY_TO_SWITCH_ON, "state should be NOT_READY_TO_SWITCH_ON (0)");
}

void cia402_init_givenAxis_shouldSetNO_TRANSITION() {
    cia402axis.transition = -1;
    // act
    cia402_init(&cia402axis);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NO_TRANSITION, "transition should be NO_TRANSITION (0)");
}
//*****************************************************************************
//                             Invalid state
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
    uint16_t controlword = -1;             // TODO fuzz != SHUTDOWN_CMD
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCH_ON_DISABLED, "state should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenSWITCH_ON_DISABLED_andInvalidCommand_shouldBeNO_TRANSITION() {
    cia402axis.state = SWITCH_ON_DISABLED;
    cia402axis.transition = -1;
    *(cia402axis.ALstatus) = 0;            // TODO fuzz
    uint16_t controlword = -1;             // TODO fuzz != SHUTDOWN_CMD
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NO_TRANSITION, "transition should be NO_TRANSITION (0)");
}

void cia402_state_machine_givenSWITCH_ON_DISABLED_anyCommandAndAL_STATUS_OP_shouldBeREADY_TO_SWITCH_ON() {
    cia402axis.state = SWITCH_ON_DISABLED;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = -1;             // TODO fuzz != SHUTDOWN_CMD
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == READY_TO_SWITCH_ON, "state should be READY_TO_SWITCH_ON (0x21)");
}

void cia402_state_machine_givenSWITCH_ON_DISABLED_anyCommandAndAL_STATUS_OP_shouldBeSWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON() {
    cia402axis.state = SWITCH_ON_DISABLED;
    cia402axis.transition = -1;
    *(cia402axis.ALstatus) = AL_STATUS_OP;
    uint16_t controlword = -1;             // TODO fuzz != SHUTDOWN_CMD
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == SWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON, "transition should be SWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON (2)");
}

void cia402_state_machine_givenSWITCH_ON_DISABLED_andSHUTDOWN_CMD_shouldBeREADY_TO_SWITCH_ON() {
    cia402axis.state = SWITCH_ON_DISABLED;
    *(cia402axis.ALstatus) = 0;             // TODO fuzz
    uint16_t controlword = CIA402_CONTROLWORD_COMMAND_SHUTDOWN;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == READY_TO_SWITCH_ON, "state should be READY_TO_SWITCH_ON (0x21)");
}

void cia402_state_machine_givenSWITCH_ON_DISABLED_andSHUTDOWN_CMD_shouldBeSWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON() {
    cia402axis.state = SWITCH_ON_DISABLED;
    cia402axis.transition = -1;
    *(cia402axis.ALstatus) = 0;             // TODO fuzz
    uint16_t controlword = CIA402_CONTROLWORD_COMMAND_SHUTDOWN;
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
    uint16_t controlword = -1;             // TODO fuzz != DISABLE_VOLTAGE_CMD, SWITCH_ON_CMD, SWITCH_ON_ENABLE_CMD
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == READY_TO_SWITCH_ON, "state should be READY_TO_SWITCH_ON (0x21)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andInvalidCommand_shouldBeNO_TRANSITION() {
    cia402axis.state = READY_TO_SWITCH_ON;
    cia402axis.transition = -1;
    uint16_t controlword = -1;             // TODO fuzz != DISABLE_VOLTAGE_CMD, SWITCH_ON_CMD, SWITCH_ON_ENABLE_CMD
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NO_TRANSITION, "transition should be NO_TRANSITION (0)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andDISABLE_VOLTAGE_CMD_shouldBeSWITCH_ON_DISABLED() {
    cia402axis.state = READY_TO_SWITCH_ON;
    uint16_t controlword = CIA402_CONTROLWORD_COMMAND_DISABLEVOLTAGE;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCH_ON_DISABLED, "state should be SWITCH_ON_DISABLED (0x40)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andDISABLE_VOLTAGE_CMD_shouldBeREADY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED() {
    cia402axis.state = READY_TO_SWITCH_ON;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_COMMAND_DISABLEVOLTAGE;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == READY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED, "transition should be READY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED (7)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_CMD_shouldBeSWITCHED_ON() {
    cia402axis.state = READY_TO_SWITCH_ON;
    uint16_t controlword = CIA402_CONTROLWORD_COMMAND_SWITCHON;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == SWITCHED_ON, "state should be SWITCHED_ON (0x40)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_CMD_shouldBeREADY_TO_SWITCH_ON_TO_SWITCHED_ON() {
    cia402axis.state = READY_TO_SWITCH_ON;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_COMMAND_SWITCHON;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == READY_TO_SWITCH_ON_TO_SWITCHED_ON, "transition should be READY_TO_SWITCH_ON_TO_SWITCHED_ON (3)");
}


void cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_ENABLE_CMD_shouldBeOPERATION_ENABLED() {
    cia402axis.state = READY_TO_SWITCH_ON;
    uint16_t controlword = CIA402_CONTROLWORD_COMMAND_SWITCHON_ENABLE;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == OPERATION_ENABLED, "state should be OPERATION_ENABLED (0x40)");
}

void cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_ENABLE_CMD_shouldBeREADY_TO_SWITCH_ON_TO_OPERATION_ENABLED() {
    cia402axis.state = READY_TO_SWITCH_ON;
    cia402axis.transition = -1;
    uint16_t controlword = CIA402_CONTROLWORD_COMMAND_SWITCHON_ENABLE;
    // act
    cia402_state_machine(&cia402axis, controlword);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == READY_TO_SWITCH_ON_TO_OPERATION_ENABLED, "transition should be READY_TO_SWITCH_ON_TO_OPERATION_ENABLED (34)");
}

//*****************************************************************************
//                             Run tests
//*****************************************************************************
int main( int argc, char **argv) {
    UNITY_BEGIN();

    // initialization
    RUN_TEST(cia402_init_givenAxis_shouldSetStateNOT_READY_TO_SWITCH_ON);
    RUN_TEST(cia402_init_givenAxis_shouldSetNO_TRANSITION);
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
    RUN_TEST(cia402_state_machine_givenSWITCH_ON_DISABLED_andSHUTDOWN_CMD_shouldBeREADY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenSWITCH_ON_DISABLED_andSHUTDOWN_CMD_shouldBeSWITCH_ON_DISABLED_TO_READY_TO_SWITCH_ON);
    // state READY_TO_SWITCH_ON
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andInvalidCommand_shouldBeREADY_TO_SWITCH_ON);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andInvalidCommand_shouldBeNO_TRANSITION);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andDISABLE_VOLTAGE_CMD_shouldBeSWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andDISABLE_VOLTAGE_CMD_shouldBeREADY_TO_SWITCH_ON_TO_SWITCH_ON_DISABLED);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_CMD_shouldBeSWITCHED_ON);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_CMD_shouldBeREADY_TO_SWITCH_ON_TO_SWITCHED_ON);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_ENABLE_CMD_shouldBeOPERATION_ENABLED);
    RUN_TEST(cia402_state_machine_givenREADY_TO_SWITCH_ON_andSWITCH_ON_ENABLE_CMD_shouldBeREADY_TO_SWITCH_ON_TO_OPERATION_ENABLED);
    // state SWITCHED_ON
    
    // state OPERATION_ENABLED
    
    // state QUICK_STOP_ACTIVE
    
    // state FAULT_REACTION_ACTIVE
    
    // state FAULT

    UNITY_END();
}
