#include <xc.h>
#include <avr/interrupt.h>

void (*PIT_ISR)(void);

void RTC_init(void)
{
    PIT_ISR = 0;
    
    //Init RTC
    
    //Run the RTC in Standby
    RTC.CTRLA = RTC_RUNSTDBY_bm;
    
    //Enable the RTC in Debug
    RTC.DBGCTRL = RTC_DBGRUN_bm;
    
    //1.024 kHz Internal Oscillator
    RTC.CLKSEL = RTC_CLKSEL_INT1K_gc;
    
    //Set for 10s period at 1.024 kHz
    RTC.PER = 10024;
    
    //Setup the PIT
    
    //Enable the PIT in Debug
    RTC.PITDBGCTRL = RTC_DBGRUN_bm;
    
    //Enable the PIT Interrupt
    RTC.PITINTCTRL = RTC_PI_bm;
    
    //PIT triggers (about) every 250ms, Enable the PIT
    RTC.PITCTRLA = RTC_PERIOD_CYC256_gc | RTC_PITEN_bm;
    
    //Enable RTC
    RTC.CTRLA |= RTC_RTCEN_bm;
}

void __interrupt(RTC_PIT_vect_num) _PIT_ISR(void)
{
    //Clear the flag
    RTC.PITINTFLAGS = RTC_PI_bm;
    
    if (PIT_ISR != 0)
    {
        PIT_ISR();
    }
}

void PIT_setISR(void (*func)(void))
{
    PIT_ISR = func;
}