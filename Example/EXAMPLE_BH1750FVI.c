/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: BH1750FVI Ambinet Light sensor
     File Name		: EXAMPLE_BH1750FVI.c
     Function		: EXAMPLE_BH1750FVI
     Create Date	: 2017/04/29
---------------------------------------------------------------------- */


#include <mega32a.h>
#include <stdio.h>
#include <delay.h>
#include <math.h>
#include <i2c.h>
#include <alcd.h>
#include <datatype_Layer.h>
#include <swi2c_Layer.h>
#include <SENSOR_BH1750FVI.h>


void EXAMPLE_BH1750FVI(void);


void EXAMPLE_BH1750FVI(void)
{
	CHAR8U BH1750FVI_RAW_DATA[2];
	CHAR8S BH1750FVI_FLAG;
	DOUBLE BH1750FVI_DATA=0;
	DOUBLE temp1,temp2;
	CHAR8U lcd_char_data[8]={0}; 
			
		printf("-------------------- LIGHT SENSOR BH1750FVI --------------------\r\n");
		/*LIGHT BH1750FVI*/

		BH1750FVI_INIT();

		BH1750FVI_CHANGE_MEASURE_TIME(80,BH1750_OP_Cont_H_Res_Mode);		/*change measurement time*/

		BH1750FVI_FLAG = BH1750FVI_READ_VALUE(&BH1750FVI_DATA,BH1750_OP_Cont_H_Res_Mode);
				
		if(BH1750FVI_FLAG == 0)
		{
			/* lx  calculate */
			lcd_char_data[0]=(INT32U)(BH1750FVI_DATA/10000)%10;
			lcd_char_data[1]=(INT32U)(BH1750FVI_DATA/1000)%10;   
			lcd_char_data[2]=(INT32U)(BH1750FVI_DATA/100)%10;
			lcd_char_data[3]=(INT32U)(BH1750FVI_DATA/10)%10;  				 
			lcd_char_data[4]=(INT32U)(BH1750FVI_DATA)%10;   

			/* SHOW MODE */                    
			lcd_gotoxy(0,0);
			lcd_putsf("MODE:OT H-Res");
					
			/* SHOW lx DATA */                    
			lcd_gotoxy(0,1);
			lcd_putsf("lx:");
			/* show lx data on LCD */
			lcd_putchar(48+lcd_char_data[0]);
			lcd_putchar(48+lcd_char_data[1]);
			lcd_putchar(48+lcd_char_data[2]);
			lcd_putchar(48+lcd_char_data[3]);
			lcd_putchar(48+lcd_char_data[4]);

				
			printf("LIGHT data = %lf lx \r\n",BH1750FVI_DATA);

		}		
		else
		{
			printf("BH1750FVI error \r\n");
             	}

		printf("-------------------- LIGHT SENSOR BH1750FVI --------------------\r\n");
		
}

