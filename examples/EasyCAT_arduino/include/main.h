#ifndef __MAIN_H__
#define __MAIN_H__

#include <Arduino.h>

#ifdef F407Z_ECAT_DEVKIT_HW

const char SpiCS_Pin = PA4;

const int BitOut0 = PC15;            // digital output bit 0
const int BitOut1 = PC14;            // digital output bit 1
const int BitOut2 = PC13;            // digital output bit 2
const int BitOut3 = PE6;             // digital output bit 3
const int BitOut4 = PE5;             // digital output bit 4
const int BitOut5 = PE4;             // digital output bit 5
const int BitOut6 = PE3;             // digital output bit 6
const int BitOut7 = PE2;             // digital output bit 7

#elif defined(STM32DISCO_F4)

const char SpiCS_Pin = PA4;

const int BitOut0 = PD13;             // digital output bit 0
const int BitOut1 = PD12;             // digital output bit 1
const int BitOut2 = PD14;             // digital output bit 2
const int BitOut3 = PD15;             // digital output bit 3
const int BitOut4 = PD11;             // digital output bit 4 - dummy, no LED there 
const int BitOut5 = PD11;             // digital output bit 5 - dummy, no LED there 
const int BitOut6 = PD11;             // digital output bit 6 - dummy, no LED there 
const int BitOut7 = PD11;             // digital output bit 7 - dummy, no LED there 

#elif defined(ARDUINO_NANO)

const char SpiCS_Pin = 10;

const int BitOut0 = 2;             // digital output bit 0
const int BitOut1 = 3;             // digital output bit 1
const int BitOut2 = 4;             // digital output bit 2
const int BitOut3 = 5;             // digital output bit 3
const int BitOut4 = 6;             // digital output bit 4
const int BitOut5 = 7;             // digital output bit 5
const int BitOut6 = 8;             // digital output bit 6
const int BitOut7 = 9;             // digital output bit 7

#else
#error "Selected PIO env has no hardware configuration"
#endif

void Application();

#endif // #ifndef __MAIN_H__