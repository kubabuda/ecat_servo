#include <unity.h>
#include "cia402device.h"

cia402_axis_t cia402axis = {
        .init_od_hook        = NULL,
        .motion_control_hook = NULL,
    }; 

void setUp(void) {
    // cia402_init(&cia402axis);
}

void tearDown(void) {
    // clean stuff up here
}

void cia402_init_givenAxis_shouldSetStateNOT_READY_TO_SWITCH_ON() {
    cia402axis.state = -1;
    // act
    cia402_init(&cia402axis);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.state == NOT_READY_TO_SWITCH_ON, "state should be NOT_READY_TO_SWITCH_ON (0)");
}

void cia402_init_givenAxis_shouldSetStateNO_TRANSITION() {
    cia402_axis_t cia402axis = {
        .init_od_hook        = NULL,
        .motion_control_hook = NULL,
    };
    cia402axis.transition = -1;
    // act
    cia402_init(&cia402axis);
    // assert
    TEST_ASSERT_TRUE_MESSAGE(cia402axis.transition == NO_TRANSITION, "transition should be NO_TRANSITION (0)");
}


int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(cia402_init_givenAxis_shouldSetStateNOT_READY_TO_SWITCH_ON);
    RUN_TEST(cia402_init_givenAxis_shouldSetStateNO_TRANSITION);

    UNITY_END();
}
