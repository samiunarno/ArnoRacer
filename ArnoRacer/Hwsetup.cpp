#include "ArnoRacer.h"
static void mcu_init(void);
static void port_init(void);
static void ad_init(void);
static void timer_ra_init(void);
static void timer_rd_init(void);



//
void Hwsetup()
{
	asm("FCLR I");
	mcu_init();
	port_init();
	ad_init();
	timer_ra_init();
	timer_rd_init();
	asm("FSET I");
}
//Set High-speed on-chip oscillator clock to System clock
static void mcu_init()
{
	prc0 = 1;		/* Protect off *///prc0 prcr_addr.bit.b0  CM0、CM1、CM3、OCD、FRA0、FRA1、FRA2、FRA3
	cm14 = 0;		/* Low-speed on-chip oscillator off */
	fra2 = 0x00;	/* Selects Divide-by-2 mode */
	fra00 = 1;		/* High-speed on-chip oscillator on */
	for( int n = 0; n <= 2040; n++);		/* This setting is an example of waiting time for the */
						/* oscillation stabilization. Please evaluate the time */
						/* for oscillation stabilization by a user. */
	fra01 = 1;		/* Selects High-speed on-chip oscillator */
	ocd2 = 1;		/* System clock : On-chip oscillator clock selected */
	cm1 &= 0x3F;
	cm06 = 0;		/* No division mode in system clock division */
	cm14 = 1;		/* CM16 and CM17 enable */
	prc0 = 0;		/* Protect on */
	trbmr = 0x80; /*Timer RB count source cutoff*/
	mstcr = 0x28;
}
//
static void port_init()
{
	//LED port setting
	//P0_5 LEDA
	//P0_6 LEDB
	//P0_7 LEDC
	/* Setting port registers */
	p0 = 0;				/* LED port off */
	/* Setting port direction registers */
	prc2 = 1;			/* Protect off */
	pd0 |= 0xE0;		/* LED port direction = output */
	
	//SWITCH Button & Motor Output
	p2 = 0 ;				//P2_0=Switch Input, P2_1...P2_7=Motor Output
	pu04 = 1 ;			//Pullup for Switch Input
	pd2 = 0xFE ;		//P2_0 Input, P2_1...P2_7 Output
}
//

/* A/D conversion initialize */
static void ad_init()
{
	unsigned char work;
	work = pd0 & 0xf0;
	prc2 = 1;			/* Protect off */
	pd0 &= work;		/* AN4/P0_3..An7/P0_0 port direction = input */
	adic = 0x00;		/* Enable A-D conversion interrupt level 5 */
	adcon0 = 0x00;	/* Stop A/D conversion */
	prc3 = 1;			/* Protect off */
	ocvrefan = 0;		/* On-chip reference voltage and analog input are cut off */
	prc3 = 0;			/* Protect on */
	admod = 0x22;		/* Division select : fAD divided by 2 */
							/* Clock source : f1 */
							/* A/D operating mode : one-sweep mode */
							/* A/D conversion starts by software trigger */

	for( int n = 0; n < 10; n++);	/* When the CKS2 bit is changed, wait for 3 φAD cycles */
							/* or more before starting A/D conversion */

    adinsel = 0x30;	/* Analog input pin : AN4, AN5, AN6, AN7 */
                   			/* A/D sweep pin count : 8 pins */
                   			/* A/D input group : Port P0 */

	adcon1 = 0x30;	/* Extended analog input pin not selected */
							/* 10-bit mode */
							/* A/D operation enabled */
							/* A/D open-circuit detection assist function：disabled */

	for(int n = 0; n < 10; n++);	/* When the ADSTBY bit is changed */
							/* from 0 (A/D operation stops) to */
	adic = 0x05;						/* 1 (A/D operation enabled), wait for 1 φAD cycle */
}

static void timer_ra_init()
{
	tstart_tracr = 0; /* Stop Timer RA operation */
	while(tcstf_tracr != 0);
	/* wait until tcstf_tracr becomes 0*/
	traic = 0x00; /* Disable Timer RA Interrupt */
	tstop_tracr = 1;
	/* The TRAPRE and TRA registers are initialized */
	/* Period between underflows 10ms */
	trapre = 128 - 1;
	/* Set (128 - 1) in TRAPRE register */
	tra = 195 - 1; /* Set (195 - 1) in TRA register */
	trasr = 0x00; /* TRAIO pin not used */
	traioc = 0x00; /* Set to "0" in timer mode */
	tramr = 0x10; /* Set to "000" in timer mode */
	/* Select "f8" in Count Source */
	/* Provides count source */
	traic = 0x05; //enable Timer RA Interrupt level 5
	tstart_tracr = 1; /* Start Timer RA operation */
	while(tcstf_tracr != 1);
	/* wait until tcstf_tracr becomes 1 */
}

static void timer_rd_init ()
{
	trdpsr0 = 0x08; // TRDIOB0 = P2_2
	trdpsr1 = 0x01; // TRDIOA1 = P2_4
	trdmr = 0x70; // Buffer register
	trdfcr = 0x01; //Reset Synchronous Mode
	trdcr0 = 0x23; //TRD0 cleared at compare match
	trdoer1 = 0xED; //Enable TRDIOB0,TRDIOA1
}


