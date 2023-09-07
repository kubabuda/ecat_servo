#include "esc_coe.h"
#include "utypes.h"
#include <stddef.h>

#ifndef HW_REV
#define HW_REV "1.0"
#endif

#ifndef SW_REV
#define SW_REV "1.0"
#endif

static const char acName1000[] = "Device Type";
static const char acName1001[] = "Error register";
static const char acName1008[] = "Device Name";
static const char acName1009[] = "Hardware Version";
static const char acName100A[] = "Software Version";
static const char acName1018[] = "Identity Object";
static const char acName1018_00[] = "Max SubIndex";
static const char acName1018_01[] = "Vendor ID";
static const char acName1018_02[] = "Product Code";
static const char acName1018_03[] = "Revision Number";
static const char acName1018_04[] = "Serial Number";
static const char acName10F1[] = "ErrorSettings";
static const char acName10F1_00[] = "Max SubIndex";
static const char acName10F1_01[] = "Local Error Reaction";
static const char acName10F1_02[] = "SyncErrorCounterLimit";
static const char acName1600[] = "Control Position";
static const char acName1600_00[] = "Max SubIndex";
static const char acName1600_01[] = "Control Word";
static const char acName1600_02[] = "Target position";
static const char acName1A00[] = "Status Position";
static const char acName1A00_00[] = "Max SubIndex";
static const char acName1A00_01[] = "Status Word";
static const char acName1A00_02[] = "Position actual";
static const char acName1C00[] = "Sync Manager Communication Type";
static const char acName1C00_00[] = "Max SubIndex";
static const char acName1C00_01[] = "Communications Type SM0";
static const char acName1C00_02[] = "Communications Type SM1";
static const char acName1C00_03[] = "Communications Type SM2";
static const char acName1C00_04[] = "Communications Type SM3";
static const char acName1C12[] = "Sync Manager 2 PDO Assignment";
static const char acName1C12_00[] = "Max SubIndex";
static const char acName1C12_01[] = "PDO Mapping";
static const char acName1C13[] = "Sync Manager 3 PDO Assignment";
static const char acName1C13_00[] = "Max SubIndex";
static const char acName1C13_01[] = "PDO Mapping";
static const char acName1C32[] = "Sync Manager 2 Parameters";
static const char acName1C32_00[] = "Max SubIndex";
static const char acName1C32_01[] = "Sync mode";
static const char acName1C32_02[] = "CycleTime";
static const char acName1C32_03[] = "ShiftTime";
static const char acName1C32_04[] = "Sync modes supported";
static const char acName1C32_05[] = "Minimum Cycle Time";
static const char acName1C32_06[] = "Calc and Copy Time";
static const char acName1C32_07[] = "Minimum Delay Time";
static const char acName1C32_08[] = "GetCycleTime";
static const char acName1C32_09[] = "DelayTime";
static const char acName1C32_0A[] = "Sync0CycleTime";
static const char acName1C32_0B[] = "SM event missed counter";
static const char acName1C32_0C[] = "CycleTimeTooSmallCnt";
static const char acName1C32_0D[] = "Shift too short counter";
static const char acName1C32_0E[] = "RxPDOToggleFailed";
static const char acName1C32_0F[] = "Minimum Cycle Distance";
static const char acName1C32_10[] = "Maximum Cycle Distance";
static const char acName1C32_11[] = "Minimum SM Sync Distance";
static const char acName1C32_12[] = "Maximum SM Sync Distance";
static const char acName1C32_20[] = "SyncError";
static const char acName1C33[] = "Sync Manager 3 Parameters";
static const char acName1C33_00[] = "Max SubIndex";
static const char acName1C33_01[] = "Sync mode";
static const char acName1C33_02[] = "CycleTime";
static const char acName1C33_03[] = "ShiftTime";
static const char acName1C33_04[] = "Sync modes supported";
static const char acName1C33_05[] = "Minimum Cycle Time";
static const char acName1C33_06[] = "Calc and Copy Time";
static const char acName1C33_07[] = "Minimum Delay Time";
static const char acName1C33_08[] = "GetCycleTime";
static const char acName1C33_09[] = "DelayTime";
static const char acName1C33_0A[] = "Sync0CycleTime";
static const char acName1C33_0B[] = "SM event missed counter";
static const char acName1C33_0C[] = "CycleTimeTooSmallCnt";
static const char acName1C33_0D[] = "Shift too short counter";
static const char acName1C33_0E[] = "RxPDOToggleFailed";
static const char acName1C33_0F[] = "Minimum Cycle Distance";
static const char acName1C33_10[] = "Maximum Cycle Distance";
static const char acName1C33_11[] = "Minimum SM Sync Distance";
static const char acName1C33_12[] = "Maximum SM Sync Distance";
static const char acName1C33_20[] = "SyncError";
static const char acName6040[] = "Control Word";
static const char acName6041[] = "Status Word";
static const char acName6060[] = "Modes of operation";
static const char acName6061[] = "Mode of operation display";
static const char acName6064[] = "Position actual";
static const char acName607A[] = "Target position";
static const char acName6502[] = "Supported drive modes";

const _objd SDO1000[] =
{
  {0x0, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1000, 0x00020192, NULL},
};
const _objd SDO1001[] =
{
  {0x0, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1001, 0, &Obj.Error_register},
};
const _objd SDO1008[] =
{
  {0x0, DTYPE_VISIBLE_STRING, 72, ATYPE_RO, acName1008, 0, "cia402_id"},
};
const _objd SDO1009[] =
{
  {0x0, DTYPE_VISIBLE_STRING, 24, ATYPE_RO, acName1009, 0, HW_REV},
};
const _objd SDO100A[] =
{
  {0x0, DTYPE_VISIBLE_STRING, 24, ATYPE_RO, acName100A, 0, SW_REV},
};
const _objd SDO1018[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1018_00, 4, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_01, 0x00000B95, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_02, 0x00020192, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_03, 42, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_04, 0x00000000, &Obj.serial},
};
const _objd SDO10F1[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName10F1_00, 2, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RW, acName10F1_01, 0, &Obj.ErrorSettings.Local_Error_Reaction},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_RW, acName10F1_02, 200, &Obj.ErrorSettings.SyncErrorCounterLimit},
};
const _objd SDO1600[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1600_00, 2, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_01, 0x60400010, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_02, 0x607A0020, NULL},
};
const _objd SDO1A00[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1A00_00, 2, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_01, 0x60410010, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_02, 0x60640020, NULL},
};
const _objd SDO1C00[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_00, 4, NULL},
  {0x01, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_01, 1, NULL},
  {0x02, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_02, 2, NULL},
  {0x03, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_03, 3, NULL},
  {0x04, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_04, 4, NULL},
};
const _objd SDO1C12[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C12_00, 1, NULL},
  {0x01, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C12_01, 0x1600, NULL},
};
const _objd SDO1C13[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C13_00, 1, NULL},
  {0x01, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C13_01, 0x1A00, NULL},
};
const _objd SDO1C32[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C32_00, 32, NULL},
  {0x01, DTYPE_UNSIGNED16, 16, ATYPE_RWpre, acName1C32_01, 1, &Obj.Sync_Manager_2_Parameters.Sync_mode},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_02, 0, &Obj.Sync_Manager_2_Parameters.CycleTime},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_03, 0, &Obj.Sync_Manager_2_Parameters.ShiftTime},
  {0x04, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C32_04, 6, &Obj.Sync_Manager_2_Parameters.Sync_modes_supported},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_05, 125000, &Obj.Sync_Manager_2_Parameters.Minimum_Cycle_Time},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_06, 0, &Obj.Sync_Manager_2_Parameters.Calc_and_Copy_Time},
  {0x07, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_07, 0, &Obj.Sync_Manager_2_Parameters.Minimum_Delay_Time},
  {0x08, DTYPE_UNSIGNED16, 16, ATYPE_RW, acName1C32_08, 0, &Obj.Sync_Manager_2_Parameters.GetCycleTime},
  {0x09, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_09, 0, &Obj.Sync_Manager_2_Parameters.DelayTime},
  {0x0A, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_0A, 0, &Obj.Sync_Manager_2_Parameters.Sync0CycleTime},
  {0x0B, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C32_0B, 0, &Obj.Sync_Manager_2_Parameters.SM_event_missed_counter},
  {0x0C, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C32_0C, 0, &Obj.Sync_Manager_2_Parameters.CycleTimeTooSmallCnt},
  {0x0D, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C32_0D, 0, &Obj.Sync_Manager_2_Parameters.Shift_too_short_counter},
  {0x0E, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C32_0E, 0, &Obj.Sync_Manager_2_Parameters.RxPDOToggleFailed},
  {0x0F, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_0F, 0, &Obj.Sync_Manager_2_Parameters.Minimum_Cycle_Distance},
  {0x10, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_10, 0, &Obj.Sync_Manager_2_Parameters.Maximum_Cycle_Distance},
  {0x11, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_11, 0, &Obj.Sync_Manager_2_Parameters.Minimum_SM_Sync_Distance},
  {0x12, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_12, 0, &Obj.Sync_Manager_2_Parameters.Maximum_SM_Sync_Distance},
  {0x20, DTYPE_BOOLEAN, 1, ATYPE_RO, acName1C32_20, 0, &Obj.Sync_Manager_2_Parameters.SyncError},
};
const _objd SDO1C33[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C33_00, 32, NULL},
  {0x01, DTYPE_UNSIGNED16, 16, ATYPE_RWpre, acName1C33_01, 1, &Obj.Sync_Manager_3_Parameters.Sync_mode},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C33_02, 0, &Obj.Sync_Manager_3_Parameters.CycleTime},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C33_03, 0, &Obj.Sync_Manager_3_Parameters.ShiftTime},
  {0x04, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C33_04, 6, &Obj.Sync_Manager_3_Parameters.Sync_modes_supported},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C33_05, 125000, &Obj.Sync_Manager_3_Parameters.Minimum_Cycle_Time},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C33_06, 0, &Obj.Sync_Manager_3_Parameters.Calc_and_Copy_Time},
  {0x07, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C33_07, 0, &Obj.Sync_Manager_3_Parameters.Minimum_Delay_Time},
  {0x08, DTYPE_UNSIGNED16, 16, ATYPE_RW, acName1C33_08, 0, &Obj.Sync_Manager_3_Parameters.GetCycleTime},
  {0x09, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C33_09, 0, &Obj.Sync_Manager_3_Parameters.DelayTime},
  {0x0A, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C33_0A, 0, &Obj.Sync_Manager_3_Parameters.Sync0CycleTime},
  {0x0B, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C33_0B, 0, &Obj.Sync_Manager_3_Parameters.SM_event_missed_counter},
  {0x0C, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C33_0C, 0, &Obj.Sync_Manager_3_Parameters.CycleTimeTooSmallCnt},
  {0x0D, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C33_0D, 0, &Obj.Sync_Manager_3_Parameters.Shift_too_short_counter},
  {0x0E, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C33_0E, 0, &Obj.Sync_Manager_3_Parameters.RxPDOToggleFailed},
  {0x0F, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C33_0F, 0, &Obj.Sync_Manager_3_Parameters.Minimum_Cycle_Distance},
  {0x10, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C33_10, 0, &Obj.Sync_Manager_3_Parameters.Maximum_Cycle_Distance},
  {0x11, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C33_11, 0, &Obj.Sync_Manager_3_Parameters.Minimum_SM_Sync_Distance},
  {0x12, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C33_12, 0, &Obj.Sync_Manager_3_Parameters.Maximum_SM_Sync_Distance},
  {0x20, DTYPE_BOOLEAN, 1, ATYPE_RO, acName1C33_20, 0, &Obj.Sync_Manager_3_Parameters.SyncError},
};
const _objd SDO6040[] =
{
  {0x0, DTYPE_UNSIGNED16, 16, ATYPE_RO | ATYPE_RXPDO, acName6040, 0, &Obj.Control_Word},
};
const _objd SDO6041[] =
{
  {0x0, DTYPE_UNSIGNED16, 16, ATYPE_RO | ATYPE_TXPDO, acName6041, 0, &Obj.Status_Word},
};
const _objd SDO6060[] =
{
  {0x0, DTYPE_INTEGER8, 8, ATYPE_RW, acName6060, 8, &Obj.Modes_of_operation},
};
const _objd SDO6061[] =
{
  {0x0, DTYPE_INTEGER8, 8, ATYPE_RO, acName6061, 8, &Obj.Mode_of_operation_display},
};
const _objd SDO6064[] =
{
  {0x0, DTYPE_UNSIGNED32, 32, ATYPE_RO | ATYPE_TXPDO, acName6064, 0, &Obj.Position_actual},
};
const _objd SDO607A[] =
{
  {0x0, DTYPE_UNSIGNED32, 32, ATYPE_RO | ATYPE_RXPDO, acName607A, 0, &Obj.Target_position},
};
const _objd SDO6502[] =
{
  {0x0, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName6502, 128, &Obj.Supported_drive_modes},
};

const _objectlist SDOobjects[] =
{
  {0x1000, OTYPE_VAR, 0, 0, acName1000, SDO1000},
  {0x1001, OTYPE_VAR, 0, 0, acName1001, SDO1001},
  {0x1008, OTYPE_VAR, 0, 0, acName1008, SDO1008},
  {0x1009, OTYPE_VAR, 0, 0, acName1009, SDO1009},
  {0x100A, OTYPE_VAR, 0, 0, acName100A, SDO100A},
  {0x1018, OTYPE_RECORD, 4, 0, acName1018, SDO1018},
  {0x10F1, OTYPE_RECORD, 2, 0, acName10F1, SDO10F1},
  {0x1600, OTYPE_RECORD, 2, 0, acName1600, SDO1600},
  {0x1A00, OTYPE_RECORD, 2, 0, acName1A00, SDO1A00},
  {0x1C00, OTYPE_ARRAY, 4, 0, acName1C00, SDO1C00},
  {0x1C12, OTYPE_ARRAY, 1, 0, acName1C12, SDO1C12},
  {0x1C13, OTYPE_ARRAY, 1, 0, acName1C13, SDO1C13},
  {0x1C32, OTYPE_RECORD, 19, 0, acName1C32, SDO1C32},
  {0x1C33, OTYPE_RECORD, 19, 0, acName1C33, SDO1C33},
  {0x6040, OTYPE_VAR, 0, 0, acName6040, SDO6040},
  {0x6041, OTYPE_VAR, 0, 0, acName6041, SDO6041},
  {0x6060, OTYPE_VAR, 0, 0, acName6060, SDO6060},
  {0x6061, OTYPE_VAR, 0, 0, acName6061, SDO6061},
  {0x6064, OTYPE_VAR, 0, 0, acName6064, SDO6064},
  {0x607A, OTYPE_VAR, 0, 0, acName607A, SDO607A},
  {0x6502, OTYPE_VAR, 0, 0, acName6502, SDO6502},
  {0xffff, 0xff, 0xff, 0xff, NULL, NULL}
};
