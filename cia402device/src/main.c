#include <stdio.h>
#include "cia402device.h"

uint16_t mockOD_Status_Word;
uint16_t mockOD_ALstatus;
cia402_axis_t cia402axis = {
    .init_od_hook        = NULL,
    .motion_control_hook = NULL,
    .statusword          = &mockOD_Status_Word,
    .ALstatus            = &mockOD_ALstatus,
};

int main()
{
    printf("Hello World from PlatformIO!\n");
    cia402axis.state = OPERATION_ENABLED;
    uint16_t controlword = CIA402_CONTROLWORD_COMMAND_ENABLEOPERATION;
    cia402_state_machine(&cia402axis, controlword);
    return 0;
}
