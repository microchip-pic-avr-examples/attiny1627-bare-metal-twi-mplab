#include "TWI_host.h"

#include <avr/io.h>
#include <stdbool.h>

#define TWI_READ true
#define TWI_WRITE false

#define TWI_IS_CLOCKHELD() TWI0.MSTATUS & TWI_CLKHOLD_bm
#define TWI_IS_BUSERR() TWI0.MSTATUS & TWI_BUSERR_bm
#define TWI_IS_ARBLOST() TWI0.MSTATUS & TWI_ARBLOST_bm

#define CLIENT_NACK() TWI0.MSTATUS & TWI_RXACK_bm
#define CLIENT_ACK() !(TWI0.MSTATUS & TWI_RXACK_bm)

#define TWI_IS_BUSBUSY() ((TWI0.MSTATUS & TWI_BUSSTATE_BUSY_gc) == TWI_BUSSTATE_BUSY_gc)
//#define TWI_IS_BAD() ((TWI_IS_BUSERR()) | (TWI_IS_ARBLOST()) | (CLIENT_NACK()) | (TWI_IS_BUSBUSY()))

#define TWI_WAIT() while (!((TWI_IS_CLOCKHELD()) || (TWI_IS_BUSERR()) || (TWI_IS_ARBLOST()) || (TWI_IS_BUSBUSY())))

bool isTWIBad(void)
{
    //Checks for: NACK, ARBLOST, BUSERR, Bus Busy
    if ((((TWI0.MSTATUS) & (TWI_RXACK_bm | TWI_ARBLOST_bm | TWI_BUSERR_bm)) > 0)
            || (TWI_IS_BUSBUSY()))
    {
        return true;
    }
    return false;
}

void TWI_initHost(void)
{        
    //Setup TWI I/O
    TWI_initPins();
    
    //Standard 100kHz TWI, 4 Cycle Hold, 50ns SDA Hold Time
    TWI0.CTRLA = TWI_SDAHOLD_50NS_gc;
    
    //Enable Run in Debug
    TWI0.DBGCTRL = TWI_DBGRUN_bm;
    
    //Clear MSTATUS (write 1 to flags). BUSSTATE set to idle
    TWI0.MSTATUS = TWI_RIF_bm | TWI_WIF_bm | TWI_CLKHOLD_bm | TWI_RXACK_bm |
            TWI_ARBLOST_bm | TWI_BUSERR_bm | TWI_BUSSTATE_IDLE_gc;
    
    //Set for 100kHz from a 4MHz oscillator
    TWI0.MBAUD = 15;
    
    //[No ISRs] and Host Mode
    TWI0.MCTRLA = TWI_ENABLE_bm;
}

void TWI_initPins(void)
{
    //PB1/PB2
        
    //Output I/O
    PORTB.DIRSET = PIN1_bm | PIN2_bm;

#ifdef TWI_ENABLE_PULLUPS
    //Enable Pull-Ups
    PORTB.PIN1CTRL = PORT_PULLUPEN_bm;
    PORTB.PIN2CTRL = PORT_PULLUPEN_bm;
#endif
}

bool _startTWI(uint8_t addr, bool read)
{
    //If the Bus is Busy
    if (TWI_IS_BUSBUSY())
    {
        return false;
    }
    
    //Send Address
    TWI0.MADDR = (addr << 1) | read;
    
    //Wait...
    TWI_WAIT();
                
    if (isTWIBad())
    {
        //Stop the Bus
        TWI0.MCTRLB = TWI_MCMD_STOP_gc;
        return false;
    }
    
    //TWI Started
    return true;
}

//Internal Function: Reads len bytes from TWI, then issues a bus STOP
void _readFromTWI(uint8_t* data, uint8_t len)
{
    uint8_t bCount = 0;
    
    //Release the clock hold
    
    TWI0.MSTATUS = TWI_CLKHOLD_bm;
    
    //TWI0.MCTRLB = TWI_MCMD_RECVTRANS_gc;
    
    while (bCount < len)
    {
        //Wait...
        TWI_WAIT();
        
        //Store data
        data[bCount] = TWI0.MDATA;

        //Increment the counter
        bCount += 1;
        
        if (bCount != len)
        {
            //If not done, then ACK and read the next byte
            TWI0.MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc;
        }
    }
    
    //NACK and STOP the bus
    TWI0.MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc;
}

//Internal Function: Write len bytes to TWI. Does NOT STOP the bus. Returns true if successful
bool _writeToTWI(uint8_t* data, uint8_t len)
{
    uint8_t count = 0;
    
    while (count < len)
    {
        //Write a byte
        TWI0.MDATA = data[count];
        
        //Wait...
        TWI_WAIT();
        
        //If the client NACKed, then abort the write
        if (CLIENT_NACK())
        {
            return false;
        }
        
        //Increment the counter
        count++;
    }
    
    return true;
}

bool TWI_sendByte(uint8_t addr, uint8_t data)
{
    //Address Client Device (Write)
    if (!_startTWI(addr, TWI_WRITE))
        return false;
    
    bool success = _writeToTWI(&data, 1);
    
    //Stop the bus
    TWI0.MCTRLB = TWI_MCMD_STOP_gc;
    
    return success;
}

bool TWI_sendBytes(uint8_t addr, uint8_t* data, uint8_t len)
{
    //Address Client Device (Write)
    if (!_startTWI(addr, TWI_WRITE))
        return false;
    
    //Write the bytes to the client
    bool success = _writeToTWI(data, len);

    //Stop the bus
    TWI0.MCTRLB = TWI_MCMD_STOP_gc;
    
    
    
    return success;
}

bool TWI_readByte(uint8_t addr, uint8_t* data)
{
    //Address Client Device (Read)
    if (!_startTWI(addr, TWI_READ))
        return false;
    
    //Read byte from client
    _readFromTWI(data, 1);

    return true;
}


bool TWI_readBytes(uint8_t addr, uint8_t* data, uint8_t len)
{
    //Address Client Device (Read)
    if (!_startTWI(addr, TWI_READ))
        return false;
    
    //Read bytes from client
    _readFromTWI(data, len);
    
    return true;
}

bool TWI_sendAndReadBytes(uint8_t addr, uint8_t regAddress, uint8_t* data, uint8_t len)
{
    //Address Client Device (Write)
    if (!_startTWI(addr, TWI_WRITE))
        return false;
        
    //Write register address
    if (!_writeToTWI(&regAddress, 1))
    {
        TWI0.MCTRLB = TWI_MCMD_STOP_gc;
        return false;
    }
    
    //Restart the TWI Bus in READ mode
    TWI0.MADDR |= TWI_READ;
    TWI0.MCTRLB = TWI_MCMD_REPSTART_gc;
    
    //Wait...
    TWI_WAIT();
    
    if (isTWIBad())
    {
        //Stop the TWI Bus if an error occurred
        TWI0.MCTRLB = TWI_MCMD_STOP_gc;
        return false;
    }
    
    //Read the data from the client
    _readFromTWI(data, len);
    
    return true;
}