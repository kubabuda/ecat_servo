#ifndef __ESC_IRQ_H__
#define __ESC_IRQ_H__

#include <stdint.h>


void ESC_interrupt_enable (uint32_t mask);
void ESC_interrupt_disable (uint32_t mask);

void EXTILine1_Config (void);
void EXTILine3_Config (void);

void EXTILine1_Disable (void);
void EXTILine3_Disable (void);

// Returns SYNC0 cycle time
uint32_t ESC_enable_DC (void);

#endif  // __ESC_IRQ_H__