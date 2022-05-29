#include "ArnoRacer.h"
//Timer table
// 100msec
const unsigned short timeBase[TMRNUM] = {
5, //TM00 SCI 0.5sec
10, //TM01
};
struct timer {
unsigned short stat ; //State
unsigned short rest ; //Time left
unsigned short time ; //Set time
};
static struct timer timeTbl[TMRNUM] ;
static int tick ;

void InitTimer()
{
struct timer *sp = timeTbl ;
for ( int n = 0 ; n < TMRNUM ; n++ ) {
sp->stat = OFF ;
sp->rest = 0 ;
sp->time = timeBase[n] ;
sp++ ;
}
tick = 0 ;
}
#pragma interrupt intTRAIC(vect=22)
void intTRAIC(void)
{
adst = 1 ; //ADC Start
if ( ++tick >= 10 ){ //100ms
tick = 0 ;
struct timer *sp = timeTbl ;
for ( int n = 0 ; n < TMRNUM ; n++ ) {
if ( sp->stat == ON ) {
if ( sp->rest )
sp->rest-- ;
if ( sp->rest == 0 )
sp->stat = UP ;
}
sp++ ;
}
}
}
// Start the specified timer
void OnTimer( int n )
{
timeTbl[n].rest = timeTbl[n].time ;
timeTbl[n].stat = ON ;
}
// Stop the specified timer
void OffTimer( int n )
{
timeTbl[n].stat = OFF ;
}
// Check the specified timer
int ChkTimer( int n )
{
return timeTbl[n].stat ;
}