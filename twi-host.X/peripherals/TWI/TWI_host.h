#ifndef TWI_HOST_H
#define	TWI_HOST_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdbool.h>
#include <stdint.h>
    
//If defined, internal pullup resistors will be used
#define TWI_ENABLE_PULLUPS
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> TWI_initHost(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function initializes the TWI peripheral in Host Mode.
     */
    void TWI_initHost(void);
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> TWI_initPins(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function initializes the I/O used by the TWI peripheral.
     */
    void TWI_initPins(void);
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> TWI_sendByte(<FONT COLOR=BLUE>uint8_t</FONT> addr, <FONT COLOR=BLUE>uint8_t</FONT> data)</B>
     * @param uint8_t addr - Client Device Address
     * @param uint8_t data - Data to Send
     * 
     * This function sends a byte of data to the specified address.
     * Returns true if successful, false if it failed
     */
    bool TWI_sendByte(uint8_t addr, uint8_t data);
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> TWI_sendBytes(<FONT COLOR=BLUE>uint8_t</FONT> addr, <FONT COLOR=BLUE>uint8_t</FONT>* data, <FONT COLOR=BLUE>uint8_t</FONT> len)</B>
     * @param uint8_t addr - Client Device Address
     * @param uint8_t* data - Pointer to the data to send
     * @param uint8_t len - Number of Bytes to Send 
     * 
     * This function attempts to send len bytes of data to the client.
     * Returns true if successful, false if it failed
     */
    bool TWI_sendBytes(uint8_t addr, uint8_t* data, uint8_t len);

    /**
     * <b><FONT COLOR=BLUE>void</FONT> TWI_readByte(<FONT COLOR=BLUE>uint8_t</FONT> addr, <FONT COLOR=BLUE>uint8_t</FONT> data)</B>
     * @param uint8_t addr - Client Device Address
     * @param uint8_t* data - Where the byte received should be stored
     * 
     * This function attempts to read 1 byte of data from the client.
     * Returns true if successful, fails if unable to get data.
     */
    bool TWI_readByte(uint8_t addr, uint8_t* data);
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> TWI_readBytes(<FONT COLOR=BLUE>uint8_t</FONT> addr, <FONT COLOR=BLUE>uint8_t</FONT>* data, <FONT COLOR=BLUE>uint8_t</FONT> len)</B>
     * @param uint8_t addr - Client Device Address
     * @param uint8_t* data - Where the bytes received should be stored
     * @param uint8_t len - Number of Bytes to Send 
     * 
     * This function attempts to read len bytes of data from the client.
     * Returns true if successful, false if it failed to get data.
     */
    bool TWI_readBytes(uint8_t addr, uint8_t* data, uint8_t len);
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> TWI_sendAndReadBytes(<FONT COLOR=BLUE>uint8_t</FONT> addr, <FONT COLOR=BLUE>uint8_t</FONT> regAddress,<FONT COLOR=BLUE>uint8_t</FONT>* data, <FONT COLOR=BLUE>uint8_t</FONT> len)</B>
     * @param uint8_t addr - Client Device Address
     * @param uint8_t regAddress - Address of Register to Read From
     * @param uint8_t* data - Where the bytes received should be stored
     * @param uint8_t len - Number of Bytes to Send 
     * 
     * This function writes a single byte (regAddress) to the client, then restarts and attempts to read len of data.
     * Returns true if successful, false if it failed to get data.
     */
    bool TWI_sendAndReadBytes(uint8_t addr, uint8_t regAddress, uint8_t* data, uint8_t len);
    
#ifdef	__cplusplus
}
#endif

#endif	/* TWI_HOST_H */

