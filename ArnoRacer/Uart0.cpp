#include <string.h>
#include "ArnoRacer.h"
#define TXBFSZ 40
static char txBuf[TXBFSZ] ; //Send buffer
static char *takePtr ; //Pointer
static char *setPtr ; // Pointer
static bool txBusy ; //Busy flag

static void MakeText(void);

//Initialize
void InitUart0()
{
takePtr = setPtr = txBuf ;
txBusy = false ;
}
void SendData()
{
if ( txBusy == true )
return ;
if ( txept_u0c0 == 0 ) //On sending
return ;
MakeText();
int n = strlen( txBuf ) ;
txBusy = true ;
takePtr = txBuf ;
setPtr = &txBuf[n] ;
u0tb = *takePtr++ ;
te_u0c1 = 1; /* Transmit enable:enabled */
}
//Convert to 2-byte hex code
void itoh2( unsigned short n , char *s )
{
unsigned char b ;
for ( int m = 2 ; --m >= 0 ; ){
b = n & 0x000f ;
n >>= 4 ;
if ( b <= 9 )
*( s + m ) = '0' + b ;
else
*( s + m ) = 'A' + b - 10 ;
}
*( s + 2 ) = 0 ;
}
// Convert to 4-byte hex code
void itoh4( unsigned short n , char *s )
{
unsigned char b ;
for ( int m = 4 ; --m >= 0 ; ){
b = n & 0x000f ;
n >>= 4 ;
if ( b <= 9 )
*( s + m ) = '0' + b ;
else
*( s + m ) = 'A' + b - 10 ;
}
*( s + 4 ) = 0 ;
}
//Make text
static void MakeText()
{
int sensor[4];
GetValues(sensor);
char *t = txBuf ;
strcpy( t,"@" );
t++ ;
int n = sensor[0];
itoh4( n, t );
t += 4 ;
*t++ = ' ' ;
n = sensor[1];
itoh4( n, t );
t += 4 ;
*t++ = ' ' ;
n = sensor[2];
itoh4( n, t );
t += 4 ;
*t++ = ' ' ;
n = sensor[3];
itoh4( n, t );
t += 4 ;
*t++ = ' ' ;

char *s = txBuf ;
char c = *s++ ;
while ( s < t )
c ^= *s++ ; /*EOR*/
itoh2( c, t );
t += 2 ;
*t++ = '*' ; //terminator
*t = 0 ;
}
#pragma interrupt intS0TX(vect=17)
void intS0TX(void)
{
if ( takePtr != setPtr )
u0tb = *takePtr++ ;
else {
txBusy = false ;
te_u0c1 = 0; /* Transmit enable:disabled */
}
}
#pragma interrupt intS0RX(vect=18)
void intS0RX(void)
{
}