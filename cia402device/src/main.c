#include <stdio.h>
#include "cia402device.h"

uint16_t mockOD_Status_Word;
uint16_t mockOD_ALstatus;

cia402_axis_t cia402axis = {
    .ALstatus            = &mockOD_ALstatus,
    .statusword          = &mockOD_Status_Word,
    .state               = NOT_READY_TO_SWITCH_ON,
    .transition          = NO_TRANSITION,
};

int main()
{
    printf("Hello World from PlatformIO!\n");
    cia402axis.state = OPERATION_ENABLED;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    cia402_state_machine(&cia402axis, controlword);
    return 0;
}
