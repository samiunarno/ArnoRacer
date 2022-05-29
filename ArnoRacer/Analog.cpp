#include "ArnoRacer.h"
unsigned short ad_data[4];

//
void InitAdc()
{
	for ( int n = 0 ; n < 4 ; n++ ){
		ad_data[n] = 0 ;
	}
}
void ad_read()
{
	adst = 1; /* Conversion start */
	for (int i = 0; i < 10; i++);
	/* When 1 (A/D conversion starts)*/
	/* is written to the ADST bit in the ADCON0 */
	/* register, an A/D conversion starts after*/
	/* the start process time has elapsed */
	/* Wait A/D conversion */
	while(adst == 1); //A/D conversion completed ?
	ad_data[0] = ad7 & 0x3FF ; //Save the A/D value
	ad_data[1] = ad6 & 0x3FF ;
	ad_data[2] = ad5 & 0x3FF ;
	ad_data[3] = ad4 & 0x3FF ;
}
//Use timer RA for AD conversion trigger (Used in pulse output mode for confirmation) 10ms
#pragma interrupt intTRAIC(vect=22)
void intTRAIC(void)
{
	adst = 1 ;		//ADC Start
}
//
//The input value is small when it is bright, and the value is large when it is dark.
#pragma interrupt/B intADIC(vect=14)
void intADIC(void)
{
	/* ---- Write A/D conversion result ---- */
	ad_data[0] = ad7 & 0x03FF ; /* AN7 */
	ad_data[1] = ad6 & 0x03FF ; /* AN6 */
	ad_data[2] = ad5 & 0x03FF ; /* AN5 */
	ad_data[3] = ad4 & 0x03FF ; /* AN4 */
}
//
int GetValue(int n)
{
	return ad_data[n] ;
}
//
void GetValues(int data[])
{
	for ( int n = 0 ; n < 4 ; n++ )
	{
		data[n] = ad_data[n] ;
	}
}

void Analog()
{
	ad_read();
	p0_7 = ~p0_7;
	if ( p0_7 == 0 ){
		//p0_1 AN6 Sensor[1]
		p0_5 = ( ad_data[1] > 0x01FF ) ? 1 : 0;
		//p0_3 AN4 Sensor[3]
		p0_6 = ( ad_data[3] > 0x01FF ) ? 1 : 0;
	}
	else {
		//p0_0 AN7 Sensor[0]
		p0_5 = ( ad_data[0] > 0x01FF ) ? 0 : 1;
		//p0_2 AN5 Sensor[2]
		p0_6 = ( ad_data[2] > 0x01FF ) ? 0 : 1;
	}
}	