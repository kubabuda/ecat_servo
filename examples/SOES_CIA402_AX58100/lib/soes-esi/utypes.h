#ifndef __UTYPES_H__
#define __UTYPES_H__

#include "cc.h"

/* Object dictionary storage */

typedef struct
{
   /* Identity */

   uint32_t serial;

   /* Inputs */

   uint16_t Status_Word;
   uint32_t Position_actual;

   /* Outputs */

   uint16_t Control_Word;
   uint32_t Target_position;

   /* Parameters */

   uint8_t Error_register;
   struct
   {
      uint32_t Local_Error_Reaction;
      uint16_t SyncErrorCounterLimit;
   } ErrorSettings;
   struct
   {
      uint16_t Sync_mode;
      uint32_t CycleTime;
      uint32_t ShiftTime;
      uint16_t Sync_modes_supported;
      uint32_t Minimum_Cycle_Time;
      uint32_t Calc_and_Copy_Time;
      uint32_t Minimum_Delay_Time;
      uint16_t GetCycleTime;
      uint32_t DelayTime;
      uint32_t Sync0CycleTime;
      uint16_t SM_event_missed_counter;
      uint16_t CycleTimeTooSmallCnt;
      uint16_t Shift_too_short_counter;
      uint16_t RxPDOToggleFailed;
      uint32_t Minimum_Cycle_Distance;
      uint32_t Maximum_Cycle_Distance;
      uint32_t Minimum_SM_Sync_Distance;
      uint32_t Maximum_SM_Sync_Distance;
      uint8_t SyncError;
   } Sync_Manager_2_Parameters;
   struct
   {
      uint16_t Sync_mode;
      uint32_t CycleTime;
      uint32_t ShiftTime;
      uint16_t Sync_modes_supported;
      uint32_t Minimum_Cycle_Time;
      uint32_t Calc_and_Copy_Time;
      uint32_t Minimum_Delay_Time;
      uint16_t GetCycleTime;
      uint32_t DelayTime;
      uint32_t Sync0CycleTime;
      uint16_t SM_event_missed_counter;
      uint16_t CycleTimeTooSmallCnt;
      uint16_t Shift_too_short_counter;
      uint16_t RxPDOToggleFailed;
      uint32_t Minimum_Cycle_Distance;
      uint32_t Maximum_Cycle_Distance;
      uint32_t Minimum_SM_Sync_Distance;
      uint32_t Maximum_SM_Sync_Distance;
      uint8_t SyncError;
   } Sync_Manager_3_Parameters;
   int8_t Modes_of_operation;
   int8_t Mode_of_operation_display;
   uint32_t Supported_drive_modes;

} _Objects;

extern _Objects Obj;

#endif /* __UTYPES_H__ */
