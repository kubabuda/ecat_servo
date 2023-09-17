#include <stdio.h>
#include "cia402device.h"

uint16_t mockOD_Status_Word;
uint16_t mockOD_ALstatus;

cia402_axis_t cia402axis;

int main()
{
    printf("Hello World from PlatformIO!\n");
    cia402_initialize(&cia402axis, &mockOD_Status_Word, &mockOD_ALstatus);
    cia402axis.state = OPERATION_ENABLED;
    uint16_t controlword = CIA402_CONTROLWORD_ENABLE_OPERATION_COMMAND;
    cia402_state_machine(&cia402axis, controlword);
    return 0;
}
