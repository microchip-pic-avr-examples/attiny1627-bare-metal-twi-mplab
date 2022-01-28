#ifndef ADVANCED_IO_H
#define	ADVANCED_IO_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>
    
    //Available registers to access in the Advanced IO Expander
    typedef enum {
        ADV_IO_ERROR = 0x00, ADV_IO_IOCx, ADV_IO_PORTx, 
        ADV_IO_TRISx, ADV_IO_LATx, ADV_IO_IOCxP, ADV_IO_IOCxN,
        ADV_IO_WPUx = 0x08, ADV_IO_INLVLx, ADV_IO_ODCONx, ADV_IO_SLRCONx
    } ADVANCED_IO_REGISTER;
    
    typedef union {
        struct{
            unsigned SRC : 2;
            unsigned DST : 2;
            unsigned OP : 2;
            unsigned BH : 2;
        };
        
        uint8_t opCode;
    } ADVANCED_IO_MEMORY_OP;
    
#define ADV_IO_SRC_bp 0
#define ADV_IO_SRC_bm (0b11 << ADV_IO_SRC_bp)
    
#define ADV_IO_DST_bp 2
#define ADV_IO_DST_bm (0b11 << ADV_IO_DST_bp)

#define ADV_IO_OP_bp 4
#define ADV_IO_OP_bm (0b11 << ADV_IO_OP_bp)

#define ADV_IO_BH_bp 6
#define ADV_IO_BH_bm (0b11 << ADV_IO_BH_bp)

#define ADV_IO_BH_NO_CHANGE     0b00
#define ADV_IO_BH_OUTPUT_LOW    0b01
#define ADV_IO_BH_OUTPUT_HIGH   0b10
#define ADV_IO_BH_TRI_STATE     0b11

#define ADV_IO_OP_DEFAULT       0b00
#define ADV_IO_OP_SAVE          0b01
#define ADV_IO_OP_LOAD          0b10
#define ADV_IO_OP_SAVE_LOAD     0b11

//I2C Address to Use
#define ADVANCED_IO_I2C_ADDR 0x60
        
    /**
     * <b><FONT COLOR=BLUE>void</FONT> advancedIO_init(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function initializes the IO Expander Peripherals and API.
     */
    void advancedIO_init(void);
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> advancedIO_setRegister(<FONT COLOR=BLUE>ADVANCED_IO_REGISTER</FONT> reg, <FONT COLOR=BLUE>uint8_t</FONT> value)</B>
     * @param ADVANCED_IO_REGISTER reg - Register to access
     * @param uint8_t value - data to write
     * 
     * This function sets a register in the IO Expander
     */
    void advancedIO_setRegister(ADVANCED_IO_REGISTER reg, uint8_t value);
    
    /**
     * <b><FONT COLOR=BLUE>uint8_t</FONT> advancedIO_getRegister(<FONT COLOR=BLUE>ADVANCED_IO_REGISTER</FONT> reg)</B>
     * @param ADVANCED_IO_REGISTER reg - Register to access
     * 
     * This function returns the value in the specified register.
     */
    uint8_t advancedIO_getRegister(ADVANCED_IO_REGISTER reg);
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> advancedIO_toggleBitsInRegister(<FONT COLOR=BLUE>ADVANCED_IO_REGISTER</FONT> reg, <FONT COLOR=BLUE>uint8_t</FONT> mask)</B>
     * @param ADVANCED_IO_REGISTER reg - Register to access
     * @param uint8_t mask - bits to toggle 
     * 
     * This function inverts the bits that are set in the mask of the specified IO Expander Register.
     * Example: Original Value: 0xFF, Mask = 0xAA
     * Result: 0x55
     */
    void advancedIO_toggleBitsInRegister(ADVANCED_IO_REGISTER reg, uint8_t mask);
    
    /**
     * <b><FONT COLOR=BLUE>uint8_t</FONT> advancedIO_getPinState(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function returns the digital values on the IO Expander pins.
     */
    uint8_t advancedIO_getPinState(void);
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> advancedIO_setOutputsHigh(<FONT COLOR=BLUE>uint8_t</FONT> mask)</B>
     * @param uint8_t mask - Values to set
     * 
     * This function sets the masked values in the LATx register (output value) of the I/O Expander to 1.
     */
    void advancedIO_setOutputsHigh(uint8_t mask);
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> advancedIO_setOutputsLow(<FONT COLOR=BLUE>uint8_t</FONT> mask)</B>
     * @param uint8_t mask - Values to set
     * 
     * This function sets the masked values in the LATx register (output value) of the I/O Expander to 0.
     */
    void advancedIO_setOutputsLow(uint8_t mask);
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> advancedIO_setPinsAsInputs(<FONT COLOR=BLUE>uint8_t</FONT> mask)</B>
     * @param uint8_t mask - Values to set
     * 
     * This function sets the masked I/O pins as inputs to the IO Expander.
     */
    void advancedIO_setPinsAsInputs(uint8_t mask);
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> advancedIO_setPinsAsOutputs(<FONT COLOR=BLUE>uint8_t</FONT> mask)</B>
     * @param uint8_t mask - Values to set
     * 
     * This function sets the masked I/O pins as outputs to the IO Expander.
     */

    void advancedIO_setPinsAsOutputs(uint8_t mask);
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> advancedIO_resetToDefault(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function resets the IO Expander to compile time defaults.
     * Warning: !INT monitoring must be used with this function.
     * 
     * Consult the IO Expander documentation for more information.
     */
    void advancedIO_resetToDefault(void);
    
    /**
     * <b><FONT COLOR=BLUE>void</FONT> advancedIO_performMemoryOP(<FONT COLOR=BLUE>ADVANCED_IO_MEMORY_OP</FONT> op)</B>
     * @param ADVANCED_IO_MEMORY_OP op - Memory Operation to Execution
     * 
     * This function executes a memory operation on the IO Expander (Save/Load/Save+Load/Reset).
     * Warning: !INT monitoring must be used with this function.
     * 
     * Consult the IO Expander documentation for more information.
     */
    void advancedIO_performMemoryOP(ADVANCED_IO_MEMORY_OP op);
    
#ifdef	__cplusplus
}
#endif

#endif	/* ADVANCED_IO_H */

