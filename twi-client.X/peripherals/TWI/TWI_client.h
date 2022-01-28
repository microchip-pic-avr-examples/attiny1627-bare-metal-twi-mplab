#ifndef TWI_CLIENT_H
#define	TWI_CLIENT_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdbool.h>
#include <stdint.h>
    
//If defined, internal pullup resistors will be used
#define TWI_ENABLE_PULLUPS
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> TWI_initClient(<FONT COLOR=BLUE>uint8_t</FONT> address)</B>
     * @param uint8_t address - 7-bit I2C Address to Use
     * 
     * This function initializes the TWI into Client Mode and sets up the I/O.
     */
    void TWI_initClient(uint8_t address);
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> TWI_initPins(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function initializes the I/O used by the TWI peripheral.
     */
    void TWI_initPins(void);
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> TWI_assignByteWriteHandler(<FONT COLOR=BLUE>void</FONT> (*onWrite)(<FONT COLOR=BLUE>uint8_t</FONT>))</B>
     * @param void (*onWrite) (uint8_t) - Function to call when data is written to the device
     * 
     * This function sets a user-defined function to call when data is written the device (I2C Write).
     */
    void TWI_assignByteWriteHandler(void (*onWrite)(uint8_t));
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> TWI_assignByteReadHandler(<FONT COLOR=BLUE>uint8_t</FONT> (*onRead)(<FONT COLOR=BLUE>void</FONT>))</B>
     * @param uint8_t (*onRead)(void) - Function to call when data is read from the device
     * 
     * This function sets a user-defined function to call when data is read from the device (I2C Read).
     */
    void TWI_assignByteReadHandler(uint8_t (*onRead)(void));
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> TWI_assignStopHandler(<FONT COLOR=BLUE>uint8_t</FONT> (*onStop)(<FONT COLOR=BLUE>void</FONT>))</B>
     * @param void (*onStop)(void) - Function to call when a STOP condition occurs.
     * 
     * This function sets a user-defined function to call when a STOP condition occurs.
     */    
    void TWI_assignStopHandler(void (*onStop)(void));
    
#ifdef	__cplusplus
}
#endif

#endif	/* TWI_CLIENT_H */
