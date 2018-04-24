/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Alex Zuepke <azu@sysgo.de>
 *
 * (C) Copyright 2002
 * Gary Jennejohn, DENX Software Engineering, <gj@denx.de>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */


#include <common.h>
#if defined(CONFIG_S3C2400) || defined (CONFIG_S3C2410) || defined (CONFIG_TRAB)

#include "s3c2410.h"



/* Start : add by www.100ask.net */
void (*isr_handle_array[50])(void);
S3C24X0_INTERRUPT * intregs;
    
extern void IsrUsbd(void);
extern void IsrDma2(void);

/************************* Timer ********************************/
static int intCount;

void IsrWatchdog(void);

void ClearPending(int bit) 
{
    intregs->SRCPND = bit;
    intregs->INTPND = bit;
}

void Timer_InitEx(void)
{
	intCount=0;	
	intregs->SUBSRCPND	= (1<<13);
	ClearPending(BIT_WDT_AC97/*BIT_WDT*/);
	intregs->INTMSK&=~(BIT_WDT_AC97 /*BIT_WDT*/);
	intregs->INTSUBMSK &= ~(1<<13);
}


void Timer_StartEx(void)
{
    S3C24X0_WATCHDOG * const wdtregs = S3C24X0_GetBase_WATCHDOG();
    
	wdtregs->WTCON=((get_PCLK()/1000000-1)<<8)|(0<<3)|(1<<2);	// 16us
	wdtregs->WTDAT=0xffff;
	wdtregs->WTCNT=0xffff;   

	// 1/16/(65+1),interrupt enable,reset disable,watchdog enable
	wdtregs->WTCON=((get_PCLK()/1000000-1)<<8)|(0<<3)|(1<<2)|(0<<0)|(1<<5);   
}

unsigned int Timer_StopEx(void)
{
	int count;
    S3C24X0_WATCHDOG * const wdtregs = S3C24X0_GetBase_WATCHDOG();

	wdtregs->WTCON=((get_PCLK()/1000000-1)<<8);
	intregs->INTMSK|=BIT_WDT_AC97; //BIT_WDT;
	intregs->INTSUBMSK |= (1<<13);
	
	count=(0xffff-wdtregs->WTCNT)+(intCount*0xffff);
	return ((unsigned int)count*16/1000000);
}


void  IsrWatchdog(void)
{
	intregs->SUBSRCPND	= (1<<13);
	ClearPending(BIT_WDT_AC97 /* BIT_WDT */);
	intCount++;   	
}

int g_TimerIntHappen;
void  IsrTimer4(void)
{
	ClearPending(BIT_TIMER4);
	*(volatile int *)&g_TimerIntHappen = 1;
}


void Dummy_isr(void)
{
    printf("Dummy_isr error, interrupt number: %d, INTMSK = 0x%x\n", intregs->INTOFFSET, intregs->INTMSK);
    while(1);
}

void Isr_Init(void)
{
    int i = 0;
    intregs = S3C24X0_GetBase_INTERRUPT();
    
    for (i = 0; i < sizeof(isr_handle_array) / sizeof(isr_handle_array[0]); i++ )
    {
        isr_handle_array[i] = Dummy_isr;
    }
	//printf("%ld\n",get_UCLK());
    intregs->INTMOD=0x0;	      // All=IRQ mode
    intregs->INTMSK=BIT_ALLMSK;	  // All interrupt is masked.

    //pISR_URXD0=(unsigned)Uart0_RxInt;	
    //rINTMSK=~(BIT_URXD0);   //enable UART0 RX Default value=0xffffffff

	isr_handle_array[ISR_TIMER4_OFT] = IsrTimer4;
	isr_handle_array[ISR_WDT_OFT]  = IsrWatchdog;

#ifdef CONFIG_USB_DEVICE
    isr_handle_array[ISR_USBD_OFT] = IsrUsbd;
    isr_handle_array[ISR_DMA2_OFT] = IsrDma2;
    ClearPending(BIT_DMA2);
    ClearPending(BIT_USBD);
#endif    
}


void do_irq(void)
{
	unsigned long oft = intregs->INTOFFSET;
    S3C24X0_GPIO * const gpio = S3C24X0_GetBase_GPIO();

    
	//清中断
	if( oft == 4 ) gpio->EINTPEND = 1<<7;		//EINT4-7合用IRQ4，注意EINTPEND[3:0]保留未用，向这些位写入1可能导致未知结果
	intregs->SRCPND = 1<<oft;	
	intregs->INTPND	= intregs->INTPND;	 

    /* run the isr */
    isr_handle_array[oft]();
}

#endif
