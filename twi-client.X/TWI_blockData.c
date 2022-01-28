#include "TWI_blockData.h"

#include <stdint.h>
#include <stdbool.h>

static volatile bool isFirst = true;
static volatile bool wasRead = false;

static volatile uint8_t i2c_index = 0;

static volatile uint8_t* writeBuffer = 0;
static volatile uint8_t writeBufferSize = 0;

static volatile uint8_t* readBuffer = 0;
static volatile uint8_t readBufferSize = 0;

void _TWI_StoreByte(uint8_t data)
{
#ifdef FIRST_BYTE_ADDR                                                          // If set, treat the 1st byte as an index
    if (!isFirst)
    {
        if (i2c_index < writeBufferSize)
        {
            writeBuffer[i2c_index] = data;
            i2c_index++;
        }
    }
    else
    {
        isFirst = false;
        i2c_index = data;
    }
#else
    if (i2c_index < writeBufferSize)
    {
        writeBuffer[i2c_index] = data;
        i2c_index++;
    }
#endif
}

uint8_t _TWI_RequestByte(void)
{
    wasRead = true;
    uint8_t data = 0x00;
    if (i2c_index < readBufferSize)
    {
        data = readBuffer[i2c_index];
        i2c_index++;
    }
    else
    {        
        //This line is to correct for the extra byte loaded, but not sent when stopped.
        i2c_index = readBufferSize + 1;
    }
    return data;
}

void _onTWIStop(void)
{
#ifdef FIRST_BYTE_ADDR
    if ((wasRead) && (i2c_index != 0))
    {
        // If reading bytes, an extra byte is loaded but not sent when stopped.
        i2c_index--;
    }
#else
    //Reset the index
    i2c_index = 0;
#endif
    
    isFirst = true;
    wasRead = false;
}

void setupReadBuffer(volatile uint8_t* buffer, uint8_t size)
{
    readBuffer = buffer;
    readBufferSize = size;
}

void setupWriteBuffer(volatile uint8_t* buffer, uint8_t size)
{
    writeBuffer = buffer;
    writeBufferSize = size;
}
