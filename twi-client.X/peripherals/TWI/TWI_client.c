#include "TWI_client.h"

#include <xc.h>
#include <stdbool.h>
#include <avr/interrupt.h>

#define TWI_READ true
#define TWI_WRITE false

static void (*writeHandler)(uint8_t);
static uint8_t (*readHandler)(void);
static void (*stopHandler)(void);

void TWI_initClient(uint8_t address)
{       
    //Init Function Pointers to Null
    writeHandler = 0;
    readHandler = 0;
    stopHandler = 0;
    
    //Enable Operation in Debug
    TWI0.DBGCTRL = TWI_DBGRUN_bm;
    
    //Set Client I2C Address
    TWI0.SADDR = address << 1;
    
    //Enable Data Interrupts, Address/Stop Interrupts, Enable STOP, and the TWI Peripheral
    TWI0.SCTRLA = TWI_DIEN_bm | TWI_APIEN_bm | TWI_PIEN_bm | TWI_ENABLE_bm;
}

void TWI_initPins(void)
{
    //PB1/PB0
        
    //Output I/O
    PORTB.DIRSET = PIN1_bm | PIN0_bm;

#ifdef TWI_ENABLE_PULLUPS
    //Enable Pull-Ups
    PORTB.PIN1CTRL = PORT_PULLUPEN_bm;
    PORTB.PIN0CTRL = PORT_PULLUPEN_bm;
#endif
}

void __interrupt(TWI0_TWIS_vect_num) TWI0_ISR(void)
{           
    if (TWI0.SSTATUS & TWI_DIF_bm)
    {
        //Data Interrupt Flag
        uint8_t data = 0x00;
        
        if (((TWI0.SSTATUS & TWI_DIR_bm) >> TWI_DIR_bp) == TWI_WRITE)
        {
            //Data Write (Host -> Client)
            data = TWI0.SDATA;
            
            if (writeHandler)
            {
                writeHandler(data);
            }
        }
        else
        {
            //Data Read (Host <- Client)
            
            if (readHandler)
            {
                data = readHandler();
            }
            
            TWI0.SDATA = data;
        }
        
        //ACK
        TWI0.SCTRLB = TWI_ACKACT_ACK_gc | TWI_SCMD_RESPONSE_gc;
    }
    else if (TWI0.SSTATUS & TWI_APIF_bm)
    {
        //Address Match or STOP
        
        if (TWI0.SSTATUS & TWI_AP_ADR_gc)
        {
            //Address Match
            TWI0.SCTRLB = TWI_ACKACT_ACK_gc | TWI_SCMD_RESPONSE_gc;
        }
        else
        {
            //STOP Condition
            if (stopHandler)
            {
                stopHandler();
            }
            
            TWI0.SCTRLB = TWI_ACKACT_NACK_gc | TWI_SCMD_COMPTRANS_gc;
        }
    }
    else
    {
        
        volatile uint8_t capture = TWI0.SSTATUS;
        asm("NOP");
    }
}

//Assigns the function to call when a byte is written to this device
void TWI_assignByteWriteHandler(void (*onWrite)(uint8_t))
{
    writeHandler = onWrite;
}

//Assigns the function to call when a byte is requested from this device
void TWI_assignByteReadHandler(uint8_t (*onRead)(void))
{
    readHandler = onRead;
}

//Assigns the function to call when the I2C Bus stops.
void TWI_assignStopHandler(void (*onStop)(void))
{
    stopHandler = onStop;
}
