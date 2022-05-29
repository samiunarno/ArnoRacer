#include "ArnoRacer.h"

#define SENSOR_1 ad_data[1]
#define SENSOR_2 ad_data[2]
#define ALPHA 100


void main(void)
{

	
	Hwsetup();
	InitAdc();
	InitMotor(70);	//100%
	InitInput();

	bool runFlag = false ;
	//Loop of main
	while(1)	/* Main processing */
	{
		Analog();
		Input();
		if ( runFlag == false )
		{
			if ( IsStartPB() == true )
			{
				InitFuzzy();
				MotorStart();
				runFlag = true ;
			}
		}
		else
		{
			if ( IsStartPB() == true )
			{
				MotorStop();
				runFlag = false ;
			}
			else
			{
				
				MotorControl();
				
			}
		}
	}
}
