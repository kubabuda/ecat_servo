/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

 /** \file
 * \brief
 * ESC hardware layer functions.
 *
 * Function to read and write commands to the ESC. Used to read/write ESC
 * registers and memory.
 */

#include <string.h>
#include "esc.h"
#include "esc_irq.h"
#include "spi.h"
#include "rst.h"

#define MAX_READ_SIZE   128

#define ESC_CMD_READ    0x02
#define ESC_CMD_READWS  0x03
#define ESC_CMD_WRITE   0x04
#define ESC_CMD_NOP     0x00
#define ESC_TERM        0xff
#define ESC_NEXT        0x00

#define ESCREG_PDI_CONTROL         0x0140
#define ESCREG_ESC_CONFIG          0x0141
#define DC_SYNC_OUT                0x04 
#define ESCREG_CYCLIC_UNIT_CONTROL 0x0980
#define SYNC_OUT_UNIT_CONTROL_MASK 0x01
#define SYNC_OUT_ECAT_CONTROL      0x00
#define SYNC_OUT_PDI_CONTROL       0x01
#define ESCREG_SYNC0_CYCLE_TIME    0x09A0
#define ESCREG_SYNC_START_TIME     0x0990
// measured with 21MHz SPI PDI
#define SYNC_START_OFFSET     2342840 
   

static int et1100 = -1;
static uint8_t read_termination[MAX_READ_SIZE] = { 0 };

#define GPIO_ECAT_RESET    1 /* specific function to hold ESC reset on startup
                              * when emulating EEPROM
                              */

static void esc_address (uint16_t address, uint8_t command)
{
   /* Device is selected already.
    * We use 2 bytes addressing.
    */
   uint8_t data[2];

   /* address 12:5 */
   data[0] = (address >> 5);
   /* address 4:0 and cmd 2:0 */
   data[1] = ((address & 0x1F) << 3) | command;

   /* Write (and read AL interrupt register) */
   spi_bidirectionally_transfer (et1100, (uint8_t *) &ESCvar.ALevent,
                                 data, sizeof (data));
   ESCvar.ALevent = etohs (ESCvar.ALevent);
}

/** ESC read function used by the Slave stack.
 *
 * @param[in]   address     = address of ESC register to read
 * @param[out]  buf         = pointer to buffer to read in
 * @param[in]   len         = number of bytes to read
 */
void ESC_read (uint16_t address, void *buf, uint16_t len)
{
    if (len > MAX_READ_SIZE) { return; }

   /* Select device. */
   spi_select (et1100);

   /* Write address and command to device. */
   esc_address (address, ESC_CMD_READ);

   /* Here we want to read data and keep MOSI low (0x00) during
    * all bytes except the last one where we want to pull it high (0xFF).
    * Read (and write termination bytes).
    */
   spi_bidirectionally_transfer (et1100, buf, read_termination +
                                 (MAX_READ_SIZE - len), len);

   /* Un-select device. */
   spi_unselect (et1100);
}

/** ESC write function used by the Slave stack.
 *
 * @param[in]   address     = address of ESC register to write
 * @param[out]  buf         = pointer to buffer to write from
 * @param[in]   len         = number of bytes to write
 */
void ESC_write (uint16_t address, void *buf, uint16_t len)
{
   /* Select device. */
   spi_select (et1100);
   /* Write address and command to device. */
   esc_address (address, ESC_CMD_WRITE);
   /* Write data. */
   spi_write (et1100, buf, len);
   /* Un-select device. */
   spi_unselect (et1100);
}

static void task_delay(uint32_t time_us)
{
   #define DELAY_1_uS      168   // todo tweak to used clock speed
   uint32_t delay_ticks = DELAY_1_uS * time_us;

   for (int32_t i = 0; i < delay_ticks; ++i)
   {
      // do nothing
   }
}

void ESC_reset (void)
{
   volatile uint32_t timeout;

   DPRINT("esc_reset_started\n");
   
   rst_low();
   task_delay(1000);

   rst_check_start();

   while(timeout < 10000000)
   {
      /* ECAT releases resetpin, typically takes 40 us
         Reset to operational PDI is max 70 ms */
      if (is_esc_reset())
      {
         rst_high();
         break; // OK
      }
      timeout++;
      task_delay (30);
   }
   DPRINT("esc_reset_ended\n");
}

void ESC_init (const esc_cfg_t * config)
{
   rst_setup();
   rst_high();
   spi_setup();
   et1100 = 1;
   read_termination[MAX_READ_SIZE - 1] = 0xFF;

   // uint8_t device_symbol = 0;
   // while (device_symbol == 0)
   // {
   //    ESC_read(et1100, (void *)&device_symbol, sizeof(uint8_t));
   //    if ((device_symbol != 0) || (device_symbol != 0xFF)) 
   //    {
   //       DPRINT("ESC init successful");
   //    }
   // }
   // task_delay(1000); // allow ESC to load EEPROM, or if EEP_DONE can be read
   // then wait while EEP_DONE is low.
}

/** ESC interrupt enable function by the Slave stack in IRQ mode.
 *
 * @param[in]   mask     = of interrupts to enable
 */
void ESC_interrupt_enable (uint32_t mask)
{
   // PDI interrupt
   EXTILine3_Config();

   if (ESCREG_ALEVENT_DC_SYNC0 & mask)
   {
      EXTILine1_Config();
      mask &= ~ESCREG_ALEVENT_DC_SYNC0;
   }
   if (ESCREG_ALEVENT_DC_SYNC1 & mask)
   {
   //    mask &= ~ESCREG_ALEVENT_DC_SYNC1;
   }
   if (ESCREG_ALEVENT_DC_LATCH & mask)
   {
      // mask &= ~ESCREG_ALEVENT_DC_LATCH;
   }
   
   ESC_ALeventmaskwrite(ESC_ALeventmaskread() | mask);
}

/** ESC interrupt disable function by the Slave stack in IRQ mode.
 *
 * @param[in]   mask     = interrupts to disable
 */
void ESC_interrupt_disable (uint32_t mask)
{
   EXTILine3_Disable();
   if (ESCREG_ALEVENT_DC_SYNC0 & mask)
   {
      EXTILine1_Disable();
      mask &= ~ESCREG_ALEVENT_DC_SYNC0;
   }
   if (ESCREG_ALEVENT_DC_SYNC1 & mask)
   {
   //    mask &= ~ESCREG_ALEVENT_DC_SYNC1;
   }
   if (ESCREG_ALEVENT_DC_LATCH & mask)
   {
   //    mask &= ~ESCREG_ALEVENT_DC_LATCH;
   }

   ESC_ALeventmaskwrite(~mask & ESC_ALeventmaskread());
}

/** ESC enable Distributed Clocks (DC)
 *
 * @return  SYNC0 cycle time
 */
uint32_t ESC_enable_DC ()
{
   uint8_t data = 0x00;
   
   // check DC Sync Out bit: 0x140:10
   ESC_read (ESCREG_ESC_CONFIG, &data, sizeof(data));
   if (!(data & DC_SYNC_OUT)) {
      return 0; // DC sync is not enabled in ESI
   }

   // read set SYNC0 Cycle Time from 0x09A0
   uint32_t setsync0cycleTime = 0;
   ESC_read (ESCREG_SYNC0_CYCLE_TIME, &setsync0cycleTime, sizeof(uint32_t));
   setsync0cycleTime = etohl (setsync0cycleTime);

   // check Sync Unit assign 0x0980:0 ( 0 for ECAT, 1 for PDI )
   ESC_read (ESCREG_CYCLIC_UNIT_CONTROL, &data, sizeof(data));
   if (data == SYNC_OUT_PDI_CONTROL) {
      // Sync Unit assigned to PDI, configuration needs to be finished by slave
   
      // set sync start time: read system time, add offset for writing start time and activation
      ESC_read (ESCREG_LOCALTIME, (void *) &ESCvar.Time, sizeof (ESCvar.Time));
      ESCvar.Time = etohl (ESCvar.Time);
      uint32_t startTime = ESCvar.Time + SYNC_START_OFFSET;

      ESC_write (ESCREG_SYNC_START_TIME, &startTime, sizeof(startTime)); 

      // activate cyclic operation and SYNC0
      ESC_read (ESCREG_SYNC_ACT, &data, sizeof(data));
      data = data | ESCREG_SYNC_ACT_ACTIVATED | ESCREG_SYNC_SYNC0_EN;
      ESC_write (ESCREG_SYNC_ACT, &data, sizeof(data));
      data = 0x00;
      while (! (data & (ESCREG_SYNC_ACT_ACTIVATED | ESCREG_SYNC_SYNC0_EN))) {
         ESC_read (ESCREG_SYNC_ACT, &data, sizeof(data));
      }
   }

   return setsync0cycleTime;
}
