/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: Ambinet Light sensor
     File Name		: SENSOR_BH1750FVI.c
     Function		: SENSOR_BH1750FVI
     Create Date	: 2017/04/26
     Fix
     1.the BH1750FVI_WRITE_OPECODE() return bug.  20170509
---------------------------------------------------------------------- */
#ifndef __BH1750FVI_FUNCTION__
#define __BH1750FVI_FUNCTION__

#include <stdio.h>
#include <math.h>
#include <delay.h>
#include <i2c.h>
#include "SENSOR_BH1750FVI.h"
#include "Porting_Layer.h"

struct measurement_time RECORD_MEA_TIME;

//----------------- record change time ----------------------
void BH1750FVI_INIT(void)
{
	RECORD_MEA_TIME.BH1750_OP_Cont_H_Res_Mode_time = 69;
	RECORD_MEA_TIME.BH1750_OP_Cont_H_Res_Mode_2_time = 69;
	RECORD_MEA_TIME.BH1750_OP_Cont_L_Res_Mode_time = 24;
	RECORD_MEA_TIME.BH1750_OP_One_H_Res_Mode_time = 69;
	RECORD_MEA_TIME.BH1750_OP_One_H_Res_Mode_2_time = 69;
	RECORD_MEA_TIME.BH1750_OP_One_L_Res_Mode_time = 24;
}

CHAR8S BH1750FVI_WAKEUP(void)
{
    CHAR8S status_flag;

	status_flag = BH1750FVI_WRITE_OPECODE(BH1750FVI_SLAVE_ADDRESS,BH1750_OP_POWER_ON);
	if(status_flag <0) return -1;
	delay_us(50);

	return 0;
}


CHAR8S BH1750FVI_RESET(void)
{
 	CHAR8S status_flag;

	status_flag= BH1750FVI_WRITE_OPECODE(BH1750FVI_SLAVE_ADDRESS,BH1750_OP_POWER_ON);
	if(status_flag <0) return -1;
	delay_us(50);
	status_flag= BH1750FVI_WRITE_OPECODE(BH1750FVI_SLAVE_ADDRESS,BH1750_OP_RESET);
	if(status_flag <0) return -1;
	delay_us(50);

	BH1750FVI_INIT();

	return 0;
}

CHAR8S BH1750FVI_READ_VALUE(DOUBLE *value,CHAR8U mode)
{
	 CHAR8U  loop,loop_time;
     CHAR8U BH1750FVI_RAW_DATA[2];
     DOUBLE BH1750FVI_DATA;
     DOUBLE temp1,temp2;
	 CHAR8S status_flag;
	 INT32U  delay_time;

	if( mode == BH1750_OP_Cont_H_Res_Mode  ||  mode ==  BH1750_OP_Cont_H_Res_Mode_2  ||  mode == BH1750_OP_Cont_L_Res_Mode  || mode == BH1750_OP_One_H_Res_Mode || \
	    mode ==  BH1750_OP_One_H_Res_Mode_2  || mode ==  BH1750_OP_One_L_Res_Mode)
		{

				//-------------------------------------------------------------------
    				//set loop_time
				switch(mode)
				{
					//---------------------------------------------
					case BH1750_OP_Cont_H_Res_Mode:
					loop_time=RECORD_MEA_TIME.BH1750_OP_Cont_H_Res_Mode_time;
							break;
					//---------------------------------------------
					case BH1750_OP_Cont_H_Res_Mode_2:
					loop_time=RECORD_MEA_TIME.BH1750_OP_Cont_H_Res_Mode_2_time;
							break;
					//---------------------------------------------
					case BH1750_OP_Cont_L_Res_Mode:
					loop_time=RECORD_MEA_TIME.BH1750_OP_Cont_L_Res_Mode_time;
							break;
					//---------------------------------------------
					case BH1750_OP_One_H_Res_Mode:
					loop_time=RECORD_MEA_TIME.BH1750_OP_One_H_Res_Mode_time;
							break;
					//---------------------------------------------
					case BH1750_OP_One_H_Res_Mode_2:
					loop_time=RECORD_MEA_TIME.BH1750_OP_One_H_Res_Mode_2_time;
							break;
					//---------------------------------------------
					case BH1750_OP_One_L_Res_Mode:
					loop_time=RECORD_MEA_TIME.BH1750_OP_One_L_Res_Mode_time;
							break;
					//---------------------------------------------

				}

					// 1.8 = 120/69  ,  time base on 69(default) ->120ms
					// if loop_time =138 ,delay_time = 1.8 * 138 = 248 ms
					delay_time = (INT32U)(loop_time * (1.8));
				//-------------------------------------------------------------------


		status_flag = BH1750FVI_WRITE_OPECODE(BH1750FVI_SLAVE_ADDRESS,mode);
		if(status_flag == -1)return -1;

		for(loop=0;loop<=delay_time;loop++)delay_ms(1); //wait the BH1750FVI data ready

        status_flag = BH1750FVI_READ_OPECODE(BH1750FVI_SLAVE_ADDRESS,&BH1750FVI_RAW_DATA[0]);
		if(status_flag == -1)return -1;


               //very important -> CHAR8U to INT32U
		 temp1=(INT32U)(BH1750FVI_RAW_DATA[0]<<8);
               temp2=(INT32U)(BH1750FVI_RAW_DATA[1]);
               BH1750FVI_DATA = (DOUBLE)(temp1+temp2);

 				//calculate the value
 				//Note : 1.H-resolution mode2  -> unit 0.5 lx
 				//          2.H-resolution mode   -> unit 1    lx
 				//          3.L-resolution mode    -> unit 4    lx

				//  calculate illuminance per 1 count  formula
				// H-resolution mode     = 1/1.2 * (69/X)
				// H-resolution mode-2 = 1/1.2 * (69/X) /2
				switch(mode)
				{
					//---------------------------------------------
					case BH1750_OP_Cont_H_Res_Mode: 		//unit 1    lx
						 BH1750FVI_DATA = BH1750FVI_DATA /1.2 * (69.0/loop_time);
							break;
					//---------------------------------------------
					case BH1750_OP_Cont_H_Res_Mode_2: 		// unit 0.5 lx
						 BH1750FVI_DATA = (BH1750FVI_DATA*0.5) /1.2 * (69.0/loop_time);
							break;
					//---------------------------------------------
					case BH1750_OP_Cont_L_Res_Mode:			// unit 4    lx
						 BH1750FVI_DATA = BH1750FVI_DATA /1.2;
							break;
					//---------------------------------------------
					case BH1750_OP_One_H_Res_Mode:			//unit 1    lx
						 BH1750FVI_DATA = BH1750FVI_DATA /1.2 * (69.0/loop_time);
							break;
					//---------------------------------------------
					case BH1750_OP_One_H_Res_Mode_2:		// unit 0.5 lx
						 BH1750FVI_DATA = (BH1750FVI_DATA*0.5) /1.2 * (69.0/loop_time);
							break;
					//---------------------------------------------
					case BH1750_OP_One_L_Res_Mode:			// unit 4    lx
						 BH1750FVI_DATA = BH1750FVI_DATA /1.2;
							break;
					//---------------------------------------------

				}

              // printf("high data =0x%x ,low data=0x%x ,DATA = %lf lx \r\n",BH1750FVI_RAW_DATA[0],BH1750FVI_RAW_DATA[1],BH1750FVI_DATA);

		//return value
		*value =  BH1750FVI_DATA;
		return 0;


		}
	else
		{
		 printf("set wrong mode !!\r\n");
		 return -1;
		}

}

CHAR8S BH1750FVI_CHANGE_MEASURE_TIME(CHAR8U time,CHAR8U mode)
{

	CHAR8U change_high_byte=0x40,change_low_byte=0x60;
	CHAR8S status_flag;

	if(time <31)time =31;    	    //time min = 31;
	else if(time>254)time=254;  //time max = 254;

	if(mode == BH1750_OP_Cont_H_Res_Mode  ||  mode ==  BH1750_OP_Cont_H_Res_Mode_2 ||  mode == BH1750_OP_Cont_L_Res_Mode   ||mode == BH1750_OP_One_H_Res_Mode || \
	    mode ==  BH1750_OP_One_H_Res_Mode_2   || mode ==  BH1750_OP_One_L_Res_Mode)
		{

		 change_high_byte = change_high_byte | ((time & 0xE0)>>5);
		 change_low_byte = change_low_byte | (time & 0x1F);
	     //  printf("high = 0x%x , low = 0x%x \r\n",change_high_byte,change_low_byte);

		 // chnage measurement time
		status_flag= BH1750FVI_WRITE_OPECODE(BH1750FVI_SLAVE_ADDRESS,change_high_byte);
		 if(status_flag <0) return -1;
		 delay_us(50);
		 BH1750FVI_WRITE_OPECODE(BH1750FVI_SLAVE_ADDRESS,change_low_byte);
		 if(status_flag <0) return -1;
		 delay_us(50);
		 BH1750FVI_WRITE_OPECODE(BH1750FVI_SLAVE_ADDRESS,mode);
	   	 if(status_flag <0) return -1;


				//change measurement time
					switch(mode)
				{
					//---------------------------------------------
					case BH1750_OP_Cont_H_Res_Mode:
					RECORD_MEA_TIME.BH1750_OP_Cont_H_Res_Mode_time=time;
							break;
					//---------------------------------------------
					case BH1750_OP_Cont_H_Res_Mode_2:
					RECORD_MEA_TIME.BH1750_OP_Cont_H_Res_Mode_2_time=time;
							break;
					//---------------------------------------------
					case BH1750_OP_Cont_L_Res_Mode:
					RECORD_MEA_TIME.BH1750_OP_Cont_L_Res_Mode_time=time;
							break;
					//---------------------------------------------
					case BH1750_OP_One_H_Res_Mode:
					RECORD_MEA_TIME.BH1750_OP_One_H_Res_Mode_time=time;
							break;
					//---------------------------------------------
					case BH1750_OP_One_H_Res_Mode_2:
					RECORD_MEA_TIME.BH1750_OP_One_H_Res_Mode_2_time=time;
							break;
					//---------------------------------------------
					case BH1750_OP_One_L_Res_Mode:
					RECORD_MEA_TIME.BH1750_OP_One_L_Res_Mode_time=time;
							break;
					//---------------------------------------------
				}

		 return 0;
	       }
		else
		{
	         printf("set wrong mode !!\r\n");
		  return -1;
		}

}



CHAR8S BH1750FVI_WRITE_OPECODE(CHAR8U reg_address,CHAR8U opcode)
{
	CHAR8S ack=0,busy=0;

      i2c_stop();
      busy = i2c_start();
      if(busy)
       {
        delay_us(50);
           ack=i2c_write(reg_address | WRITE_BIT);
           if(ack == 1)
            {
                 ack=i2c_write(opcode);
                 if(ack == 1)
                 {
                     i2c_stop();
                     delay_us(10);
                     return SWI2C_STATUS_OK;
                 }
                 else
                 {
                     printf("data error\r\n");
                     goto EXIT;
                 }


            }
	    else
	    	 {
                 printf("data error\r\n");
                 goto EXIT;
		 }
       }
     else
     {
    EXIT:
         i2c_stop();
         delay_us(10);
         return SWI2C_STATUS_FAIL;
     }
}

CHAR8S BH1750FVI_READ_OPECODE(CHAR8U reg_address,CHAR8U *data)
{

    CHAR8S ack=0,busy=0;

      i2c_stop();
      busy = i2c_start();
      if(busy)
       {
        delay_us(50);
           ack=i2c_write(reg_address | READ_BIT);
            if(ack == 1)
            {

                *(data + 0) = i2c_read(MASTER_ACK);
                *(data + 1) = i2c_read(MASTER_NACK);

              i2c_stop();
              delay_us(10);
              return SWI2C_STATUS_OK;

            }
            else
            {
                printf("register error\r\n");
                goto EXIT;
            }

        }
       else
       {
        EXIT:
             i2c_stop();
             delay_us(10);
             return SWI2C_STATUS_FAIL;
       }


}
#endif		//#ifndef __BH1750FVI_FUNCTION__