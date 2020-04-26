/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: Ambinet Light sensor
     File Name		: SENSOR_BH1750FVI.h
     Function		: SENSOR_BH1750FVI
     Create Date	: 2017/04/26

     HAL Layer : BH1750FVI_WRITE_OPECODE() ,
     			  BH1750FVI_READ_OPECODE() ,
     			  SWI2C  function
---------------------------------------------------------------------- */
#ifndef __BH1750FVI_HEADER__
#define __BH1750FVI_HEADER__


/*
  Measurement Mode    |    Measurement time     |    Resolution   |
 ---------------------+-------------------------+------------------
  H-resolution Mode2  |        120ms            |     0.5 lx      |
 ------------------------------------------------------------------
  H-resolution Mode   |        120ms            |     1 lx        |
 ------------------------------------------------------------------
  L-resolution Mode   |        16 ms            |     4 lx        |
 ------------------------------------------------------------------
*/

 	//calculate the value
 	//Note : 1.H-resolution mode2  -> unit 0.5 lx
 	//          2.H-resolution mode   -> unit 1    lx
 	//          3.L-resolution mode    -> unit 4    lx

	//  calculate illuminance per 1 count  formula
	// H-resolution mode     = 1/1.2 * (69/X)
	// H-resolution mode-2 = 1/1.2 * (69/X) /2


//************************************ SETTING ************************************


//SETP1: SET ADDR PIN Level
//SET BH1750 ADDR PIN ? 1:High , 0:Low
#define BH1750_PIN_ADDRESS  1

//--------------------------------- Define SLAVE ADDRESS -------------------------------------
#if BH1750_PIN_ADDRESS == 1  // ADDR pin as High
#define BH1750FVI_SLAVE_ADDRESS 0XB8
#elif  BH1750_PIN_ADDRESS == 0 // ADDR pin as Low
#define BH1750FVI_SLAVE_ADDRESS 0X46
#endif
//--------------------------------- Define SLAVE ADDRESS -------------------------------------


//-------------------define Opecode ------------------------
// NOTE: One Time Measurement : Power down after measurement.
//  so must be send a command : BH1750_OP_POWER_ON to wake up BH1705FVI.

#define BH1750_OP_POWER_DOWN			0x00   // No active state.
#define BH1750_OP_POWER_ON 				0x01   // Waiting for measurement command.
#define BH1750_OP_RESET 				0x07   // Reset Data Register value,but not acceptable in Power down mode.
#define BH1750_OP_Cont_H_Res_Mode       0x10   // Start Measurement at 1 lx resolution, 120ms to measurement.
#define BH1750_OP_Cont_H_Res_Mode_2    	0x11   // Start Measurement at 0.5 lx resolution, 120ms to measurement.
#define BH1750_OP_Cont_L_Res_Mode       0x13   // Start Measurement at 4 lx resolution, 16ms to measurement.
#define BH1750_OP_One_H_Res_Mode	    0x20   // Start Measurement at 1 lx resolution, 120ms to measurement.  **Power down after measurement.
#define BH1750_OP_One_H_Res_Mode_2		0x21   // Start Measurement at 0.5 lx resolution, 120ms to measurement. **Power down after measurement.
#define BH1750_OP_One_L_Res_Mode	    0x23   // Start Measurement at 4 lx resolution, 16ms to measurement. **Power down after measurement.
//-------------------define Opecode ------------------------

//----------------- record change time ----------------------
struct measurement_time
{
	CHAR8U BH1750_OP_Cont_H_Res_Mode_time;
	CHAR8U BH1750_OP_Cont_H_Res_Mode_2_time;
	CHAR8U BH1750_OP_Cont_L_Res_Mode_time;
	CHAR8U BH1750_OP_One_H_Res_Mode_time;
	CHAR8U BH1750_OP_One_H_Res_Mode_2_time;
	CHAR8U BH1750_OP_One_L_Res_Mode_time;
};
//----------------- record change time ----------------------

//************************************ SETTING ************************************


//----------------------------------- Function --------------------------------------
void BH1750FVI_INIT(void);
CHAR8S BH1750FVI_RESET(void);
CHAR8S BH1750FVI_WAKEUP(void);

CHAR8S BH1750FVI_WRITE_OPECODE(CHAR8U reg_address,CHAR8U opcode);
CHAR8S BH1750FVI_READ_OPECODE(CHAR8U reg_address,CHAR8U *data);

CHAR8S BH1750FVI_CHANGE_MEASURE_TIME(CHAR8U time,CHAR8U mode);
CHAR8S BH1750FVI_READ_VALUE(FLOAT *value,CHAR8U mode);
//----------------------------------- Function --------------------------------------


#endif //#ifndef __BH1750FVI_HEADER__