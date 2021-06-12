#ifndef CUSTOM_PDO_NAME_H
#define CUSTOM_PDO_NAME_H

//-------------------------------------------------------------------//
//                                                                   //
//     This file has been created by the Easy Configurator tool      //
//                                                                   //
//     Easy Configurator project DS402_PDOs_EasyCatProject.prj
//                                                                   //
//-------------------------------------------------------------------//


#define CUST_BYTE_NUM_OUT	6
#define CUST_BYTE_NUM_IN	6
#define TOT_BYTE_NUM_ROUND_OUT	8
#define TOT_BYTE_NUM_ROUND_IN	8


typedef union												//---- output buffer ----
{
	uint8_t  Byte [TOT_BYTE_NUM_ROUND_OUT];
	struct
	{
		int32_t     Position_0x607A;
		uint16_t    ControlWord_0x6040;
	}Cust;
} PROCBUFFER_OUT;


typedef union												//---- input buffer ----
{
	uint8_t  Byte [TOT_BYTE_NUM_ROUND_IN];
	struct
	{
		int32_t     ActualPosition_0x6064;
		uint16_t    StatusWord_0x6041;
	}Cust;
} PROCBUFFER_IN;

#endif