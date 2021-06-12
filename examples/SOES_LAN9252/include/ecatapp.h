#ifndef __ECAT_App_H__
#define __ECAT_App_H__

#include <stdint.h>


void ecatapp_setup();
void ecatapp_mainloop();

uint32_t ecatapp_benchmark_us();

#endif // #ifndef __ECAT_App_H__