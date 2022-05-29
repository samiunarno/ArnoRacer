#include "ArnoRacer.h"
#define PERIOD		40000
#define slowSpeed	60			//%
#define normalSpeed	70		//%
#define CONFIRM2 5
#define CONFIRM3 20
#define ZONE 	5
#define BETA 50
#define abs(n) ((n >= 0) ? n : -n )

static long maxValue;
static short counter;
static short stage ;	//Control stage
enum { S_FORWARD, S_BACKWARD, S_BACKWARD2,
	S_RETURN, S_RETURN2 };
static int oldResult ;	//Last result
static int which;

//
void InitMotor( int rate )
{
	maxValue = ((long)PERIOD*rate)/100;
	trdgra0 = trdgrc0 = PERIOD-1;
	trdgrb0 = trdgrd0 = maxValue/2;
	trdgra1 = trdgrc1 = maxValue/2;
	trdstr = 0x05;
}

void MotorDrive(int dir,int left,int right){
	if(dir == STOP){
		p2_1 = 0;
		p2_6 = 0;
		p2_3 = 0;
		p2_7 = 0;
	}
	else{
		trdgrd0 = (maxValue * left) / 100 ; // PWM1
		trdgrc1 = (maxValue * right) / 100; // PWM2	
		switch(dir){
			case FORWARD :
				p2_1 = 0 ; // Left wheel
				p2_6 = 1 ;
				p2_3 = 0 ; // Right wheel
				p2_7 = 1 ;
				break ;
			case BACKWARD :
				p2_1 = 1 ; // Left wheel
				p2_6 = 0 ;
				p2_3 = 1 ; // Right wheel
				p2_7 = 0 ;
				break ;
			case TORIGHT :
				p2_1 = 0 ; // Left Forward
				p2_6 = 1 ;
				p2_3 = 1 ; // Right Backward
				p2_7 = 0 ;
				break ;
			case TOLEFT :
				p2_1 = 1 ; // Left Backward
				p2_6 = 0 ;
				p2_3 = 0 ; // Right Forward
				p2_7 = 1 ;
				break ;
		}
	}
}
//
void MotorStart()
{
	stage = S_FORWARD ;	//Control stage
	counter = 0;
	oldResult = 0 ;	//Last result
	MotorDrive( FORWARD, slowSpeed, slowSpeed );
}
//
void MotorStop()
{
	MotorDrive( STOP, 0, 0 );
}
//
void MotorControl()
{
	int diff = 0 ;;
	int result = 0;
	int sensor[4];
	GetValues(sensor);
	switch ( stage ){
	case S_FORWARD :	//FORWARD
			//When both sides leave the course
			if ( sensor[1] < BRIGHT && sensor[2] < BRIGHT )
			{
				if ( (counter+=1) >= CONFIRM2 )	//For confirmation
				{
					MotorDrive(STOP,0,0);
					stage = S_BACKWARD;
				}
			}
			//When on the course
			else
			{
				
				counter = 0;
				diff = sensor[2] - sensor[1] ;
				result = Fuzzy( diff );
				if ( oldResult != result )
				{
					oldResult = result ;	//Last result
					//When the difference between the left and right is small
					if ( abs(result) <= ZONE )
					{
						int base = normalSpeed ;
						MotorDrive( FORWARD, base+result, base-result );	//Continue to move forward
					}
					//When the difference between left and right is large
					else
					{
						int base = slowSpeed;	//Reduce the speed
						MotorDrive( FORWARD, base+result, base-result );	//To change the direction
	
					}
				}
			}
		break ;
	case S_BACKWARD :	//BACKWARD
		MotorDrive(BACKWARD,slowSpeed,slowSpeed);
		counter = 0;
		which = 0;
		stage++;
		break ;
	
	case S_BACKWARD2 :	
		if(sensor[1]>=DARK || sensor[2]>=DARK)
		{
			if(sensor[1]>=DARK && sensor[2]>=DARK){
				if((counter+=1)>CONFIRM3){
					MotorDrive(STOP,0,0);
					stage = S_RETURN;
					break;
				}
			}
				
			else{
				counter = 0;
			}

		
		if(sensor[2]>sensor[1]+BETA)
		{
			int base = slowSpeed;
			MotorDrive(TOLEFT,base-20,base+20);
			if(which == 0)
				which = TOLEFT;
		}
		else if(sensor[2]<sensor[1]-BETA)
		{
			int base = slowSpeed;
			MotorDrive(TORIGHT,base+20,base-20);
			
			if(which == 0)
				which = TORIGHT;
		}
		}
		else{
			counter = 0;
		}
				
		break ;
	case S_RETURN:
		if ( which == TOLEFT )
		{
		int base = slowSpeed; //Slow down
		//Left fix, move only Right wheel
		MotorDrive( FORWARD, 0, base+20 );
		}
		else if ( which == TORIGHT)
		{
		int base = slowSpeed; //Slow down
		//Right fix, move only Left wheel
		MotorDrive( FORWARD, base+20,0 );
		}
		if (( counter -= 1) <= 0 )
		stage = S_RETURN2 ;
		break ;
	case S_RETURN2 : //Return to Forward
		counter = 0 ;
		oldResult = 0 ;
		stage = S_FORWARD ;
		break ;
	}
}

