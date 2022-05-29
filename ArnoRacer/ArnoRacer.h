#include "sfr_r834c.h" //Definition of the R8C/34C SFR
#include "Prototype.h"

//For digital input / output state, timer state
enum { OFF,ON,UP };
//Input logical table
enum { NEG, POS };


//Digital input port
enum {
	START_PB,		// 0
	INNUM
};

enum{STOP,FORWARD,BACKWARD,TORIGHT,TOLEFT};

#define DARK		570	//03C4 => 964, 700->570
#define BRIGHT	450	//0038 => 56,  300->450