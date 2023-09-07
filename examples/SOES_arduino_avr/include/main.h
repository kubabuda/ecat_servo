#ifndef __MAIN_H__
#define __MAIN_H__

#include <Arduino.h>

#if defined(ARDUINO_MEGA)

const char SpiCS_Pin = 10; // TODO set right pinout for Mega

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