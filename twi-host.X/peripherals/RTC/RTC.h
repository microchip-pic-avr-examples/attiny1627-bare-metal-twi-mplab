#ifndef RTC_H
#define	RTC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> RTC_init(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function initializes the RTC peripheral.
     */
    void RTC_init(void);
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> PIT_setISR(<FONT COLOR=BLUE>void</FONT>(*func)(<FONT COLOR=BLUE>void</FONT>))</B>
     * 
     * This function sets a user-defined function to call when the PIT triggers an interrupt.
     */
    void PIT_setISR(void (*func)(void));
    
#ifdef	__cplusplus
}
#endif

#endif	/* RTC_H */

