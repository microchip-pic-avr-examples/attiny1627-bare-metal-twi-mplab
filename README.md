<!-- Please do not change this logo with link -->

[![MCHP](images/microchip.png)](https://www.microchip.com)

# Bare Metal TWI Driver for the ATtiny1627 Family of MCUs

This code example provides a simple bare-metal TWI driver for the ATtiny1627 family of microcontrollers. TWI (Two-Wire Interface) is a peripheral to enables the microcontroller to easily communicate on an I<sup>2</sup>C bus.

## Related Examples

- [Bare Metal TWI for the AVR&reg; DB Family of MCUs](https://github.com/microchip-pic-avr-examples/attiny1627-bare-metal-twi-mplab)
- [Bare Metal I<sup>2</sup>C driver for PIC16F15244](https://github.com/microchip-pic-avr-examples/pic16f15244-bare-metal-i2c-mplab)
- [Advanced I/O Expander using the PIC16F15244](https://github.com/microchip-pic-avr-examples/pic16f15244-family-advanced-i2c-io-expander)

## Software Used

- [MPLAB&reg; X IDE v6.0.0 or newer](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_MMTCha_attiny1627&utm_content=attiny1627-bare-metal-twi-mplab)
- [MPLAB XC8 v2.35.0 or newer](https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_MMTCha_attiny1627&utm_content=attiny1627-bare-metal-twi-mplab)

## Hardware Used

- [ATtiny1627 Curiosity Nano Evaluation Kit (DM080104)](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM080104?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_MMTCha_attiny1627&utm_content=attiny1627-bare-metal-twi-mplab)
- [Curiosity Nano Base for Click Boards&trade; (AC164162)](https://www.microchip.com/en-us/development-tool/AC164162?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_MMTCha_attiny1627&utm_content=attiny1627-bare-metal-twi-mplab)

**Important: The Curiosity Nano, MCP2221A, and I/O Expander have been configured for 3.3V operation. 5V Operation is also possible, but all demo parts must be set for 5V operation (e.g.: no mixing voltage levels). Permanent damage may occur if mixed.**

### Host Mode Testing

 - [Advanced I/O Expander Demo Board](https://github.com/microchip-pic-avr-examples/pic16f15244-family-advanced-i2c-io-expander)
  - Attaching the external pull-up resistors is recommended

### Client Mode Testing

- [MCP2221A USB-I<sup>2</sup>C Breakout Module (ADM00559)](https://www.microchip.com/developmenttools/ProductDetails/PartNO/ADM00559?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_MMTCha_attiny1627&utm_content=attiny1627-bare-metal-twi-mplab)
  - Using the external pull-up resistors are recommended

## Pin Usage

| Pin | Function
| --- | -------
| PA2 | SDA
| PA3 | SCL

## Host Mode

Host mode operation allows the microcontroller to control the I<sup>2</sup>C bus and establish communication with client devices, such as sensors and EEPROMs. *This driver only supports polling based communication in Host mode.*

**Important: This driver has not been evaluated for multi-host busses.**

### Overview of the TWI Host API

| Function Name | Description
| ------------  | -----------
| void TWI_initHost(void) | Initializes the TWI Peripheral
| void TWI_initPins(void) | Configures the I/O pins for the TWI peripheral.
| bool TWI_sendByte(uint8_t addr, uint8_t data) | Sends a single byte to a client device at the specified address.
| bool TWI_sendBytes(uint8_t addr, uint8_t* data, uint8_t len) | Sends LEN bytes to a client device at the specified address.
| bool TWI_readByte(uint8_t addr, uint8_t* data) | Reads a single byte from a client device at the specified address.
| bool TWI_readBytes(uint8_t addr, uint8_t* data, uint8_t len) | Reads LEN bytes from a client device at the specified address.
| bool TWI_sendAndReadBytes(uint8_t addr, uint8_t regAddress, uint8_t* data, uint8_t len) | Sends a single byte to a client device at the specified address, then restarts the bus and reads LEN bytes back.

**Note: For all bool functions (ex: TWI_sendByte), returning true indicates the operation was successful, while returning false indicates an error has occurred.**

If `TWI_ENABLE_PULLUPS` is defined, internal pull-up resistors on the I/O will be enabled.

*Caution: The TWI pull-ups are not very strong, and may cause marginal behavior in some cases. We recommend use external pull-up resistors, if possible.*

#### TWI Host - API Description

##### Initialization Functions

```
void TWI_initHost(void)
void TWI_initPins(void)
```

To initialize the peripheral and the I/O, two functions are provided. The first function `TWI_initHost()` initializes the peripheral settings for the TWI. The second function `TWI_initPins()` is used to setup the I/O pins for the peripheral. If different I/O pins are used (via PORTMUX or a different TWI instance), then this function will need to be modified accordingly.

##### Writing to Clients

```
bool TWI_sendByte(uint8_t addr, uint8_t data)
bool TWI_sendBytes(uint8_t addr, uint8_t* data, uint8_t len)
```

To write data from the host to the client device at ADDR, two functions are provided. Either call `TWI_sendByte(uint8_t addr, uint8_t data)` or `TWI_sendBytes(uint8_t addr, uint8_t* data, uint8_t len)`. Both functions operate similarly, however `TWI_sendByte` only sends a single byte of data, whereas `TWI_sendBytes` will try to send LEN bytes to the client.

If the client NACKs during addressing or data transmission, these functions return false.

##### Reading from Clients

```
bool TWI_readByte(uint8_t addr, uint8_t* data)
bool TWI_readBytes(uint8_t addr, uint8_t* data, uint8_t len)
```

To read data from the client, two functions are provided. The first function `TWI_readByte(uint8_t addr, uint8_t* data)` reads a single byte from the client device at ADDR, whereas the second function `bool TWI_readBytes(uint8_t addr, uint8_t* data, uint8_t len)` will attempt to read LEN bytes from the client.

If the client NACKs during addressing, these functions return false.

##### Write - Restart - Read Operations

```
bool TWI_sendAndReadBytes(uint8_t addr, uint8_t regAddress, uint8_t* data, uint8_t len)
```

To perform a write-restart-read operation in I<sup>2</sup>C, call `TWI_sendAndReadBytes(uint8_t addr, uint8_t regAddress, uint8_t* data, uint8_t len)`. This function addresses the client at ADDR in write mode, writes a single byte (regAddress), then restarts the bus. Then, the client is re-addressed in read mode and LEN bytes are read back.

If the client NACKs during addressing or transmission, then this function will return false.

### Advanced I/O Expander API (for Demo)

| Function Name | Description
| ------------  | -----------
| void advancedIO_init(void) | Initializes the Advanced I/O Expander and the TWI peripheral
| void advancedIO_setRegister(ADVANCED_IO_REGISTER reg, uint8_t value) | Sets a register inside of the Advanced I/O Expander to the value specified.
| uint8_t advancedIO_getRegister(ADVANCED_IO_REGISTER reg) | Returns the value inside of the Advanced I/O Expander.
| void advancedIO_toggleBitsInRegister(ADVANCED_IO_REGISTER reg, uint8_t mask) | Inverts the bits defined by the mask in the register specified (of the Advanced I/O Expander).
| uint8_t advancedIO_getPinState(void) | Returns the digital values on the I/O Expander pins.
| void advancedIO_setOutputsHigh(uint8_t mask) | Sets the output value of the pins defined by the mask to be '1'. **Does not set the pin as an output.**
| void advancedIO_setOutputsLow(uint8_t mask) | Sets the output value of the pins defined by the mask to be '0'. **Does not set the pin as an output.**
| void advancedIO_setPinsAsInputs(uint8_t mask) | Sets the pins defined by the mask to be outputs. **Does not set the output level of the pin.**
| void advancedIO_setPinsAsOutputs(uint8_t mask) | Sets the pins defined by the mask to be inputs.
| void advancedIO_resetToDefault(void) | Resets the Advanced I/O Expander to compile time defaults. **The !INT line must be monitored to properly use this function. Consult the Advanced I/O Expander API for more information.**
| void advancedIO_performMemoryOP(ADVANCED_IO_MEMORY_OP op) | Performs a memory operation (reset/save/load/save+load) on the Advanced I/O Expander. **The !INT line must be monitored to properly use this function. Consult the Advanced I/O Expander API for more information.**

[Advanced I/O Expander README](https://github.com/microchip-pic-avr-examples/pic16f15244-family-advanced-i2c-io-expander)

### TWI Host Demo / Test

The demo program uses the TWI, Real-Time Clock (RTC) and Sleep Controller (SLPCTRL) to blink the LEDs on the I/O Expander at 2 Hz. After initializing the I/O Expander, the microcontroller enters sleep. When the Periodic Interrupt Timer (PIT) in the RTC triggers, the microcontroller wakes up, inverts the states of the LEDs on the I/O Expander and returns to sleep.

## Client Mode

Client mode operation allows the microcontroller to act as an attached device in an application. The main microcontroller/microprocessor might handle the computations and execution of high-speed or complex operations, while the attached device may handle tasks that are simpler or require  real-time control / management without the added software complexity on the main device.

**Note: This API only supports interrupt driven operation.**

### TWI Client - API Summary

| Function Name | Description
| ------------  | -----------
| void TWI_initClient(uint8_t address) | Configures the TWI Peripheral in client mode.
| void TWI_initPins(void) | Sets up the I/O used by the TWI Peripheral.
| void TWI_assignByteWriteHandler(void (*onWrite)(uint8_t)) | Sets the function to call when a byte is received from the host. **If not defined, the received bytes are discarded.**
| void TWI_assignByteReadHandler(uint8_t (*onRead)(void)) | Sets the function to call when a byte is needed to send to the host. **If not defined, the client will send 0x00 to the host.**
| void TWI_assignStopHandler(void (*onStop)(void)) | Sets the function to call when a STOP condition occurs on the I<sup>2</sup>C bus. **If not defined, no action will occur (and the bus will STOP normally).**

**Warning: Do not use blocking code in any function handlers. This will increase response time and will lock the bus until resolved.**

If `TWI_ENABLE_PULLUPS` is defined, internal pull-up resistors on the I/O will be enabled.

*Caution: The TWI pull-ups are not very strong, and may cause marginal behavior in some cases. We recommend use external pull-up resistors, if possible.*

#### TWI Client - API Description

##### Initialization Functions

```
void TWI_initClient(uint8_t address)
void TWI_initPins(void)
```

To initialize the peripheral and the I/O, two functions are provided. The first `TWI_initClient()` initializes the peripheral settings for the TWI. The second function `TWI_initPins()` is used to setup the I/O pins for the peripheral. If different I/O pins are used (via PORTMUX or a different TWI instance), then this function will need to modified accordingly.

##### Byte Writes (Host &rarr; Client)

```
void TWI_assignByteWriteHandler(void (*onWrite)(uint8_t))
```

When the host writes a byte of data to the client, the function set by `void TWI_assignByteWriteHandler(void (*onWrite)(uint8_t))` is called. If this function was not set, the received byte is discarded. This ensures the bus will continue to operate normally. To handle a byte write, a function must have a declaration with the following format: `void myByteWriteHandler(uint8_t data)`.

##### Byte Writes (Client &rarr; Host)

```
void TWI_assignByteReadHandler(uint8_t (*onRead)(void))
```

When the host wants to read a byte of data from the client, the function set by `void TWI_assignByteReadHandler(uint8_t (*onRead)(void))` is called. If this function was not set, 0x00 is sent instead. To handle a byte read, a function must have a declaration in the following format: `uint8_t myByteReadHandler(void)`.

##### I<sup>2</sup>C Stop

```
void TWI_assignStopHandler(void (*onStop)(void))
```

When the host creates a STOP on the I<sup>2</sup>C bus, the function set by `void TWI_assignStopHandler(void (*onStop)(void))` is called. If this function was not set, no action is taken (and the bus stops normally). To trigger an event when this occurs, a function have a declaration in the following format: `void myStopHandler(void)`.

### Block Memory - API Summary

| Function Name | Description
| ------------  | -----------
| void _TWI_StoreByte(uint8_t data) | This function is called when bytes are received by the TWI peripheral. **Do not directly call this function.**
| uint8_t _TWI_RequestByte(void) | This function is called when the TWI peripheral requests bytes. **Do not directly call this function.**
| void _onTWIStop(void) | This function is called when a STOP condition is detected by the TWI peripheral. **Do not directly call this function.**
| void setupReadBuffer(volatile uint8_t* buffer, uint8_t size) | Sets the buffer to store data to.
| void setupWriteBuffer(volatile uint8_t* buffer, uint8_t size) | Sets the buffer to send data from.

If `FIRST_BYTE_ADDR` is defined, then the 1st byte received is considered the starting address.

#### Block Memory - API Description

The block memory API provides a simple way to implement bulk data read/write operations. This API is **not required** for the bare-metal TWI driver, but it can be paired with this API by adding the following lines of code to main:

```
//Attach R/W Buffers
//Note: The buffers can be separated, or they can be the same memory, if desired.
setupReadBuffer(&data[0], 16);
setupWriteBuffer(&data[8], 8);

//Assign ISRs to Block Memory Library
TWI_assignByteWriteHandler(&_TWI_StoreByte);
TWI_assignByteReadHandler(&_TWI_RequestByte);
TWI_assignStopHandler(&_onTWIStop);
```  

*Note: All 3 ISRs must be associated for proper operation*

In the example code above, the read and write buffers point to the same array, but the write buffer is offset by 8 bytes (and is specified as 8 bytes long). This is **not required** - these can be separate memory structures or they can refer to the same memory block without issue. This middleware library is designed to handle access to the memory buffers, reducing the risk of memory errors or glitches.

**Important: The memory buffers should be declared as volatile, as they are modified in an Interrupt Service Routine.**

#### Block Memory - Operation

If `FIRST_BYTE_ADDR` is defined, then this library will assume the 1st byte received is the address to start memory read/write operations. In this case, to write 1, 2, 3, 4 to the array starting at an offset, the data to send would be: \<offset>, 1, 2, 3, 4. To read data at a specific address, write \<offset>, restart the bus and begin a read operation.

If `FIRST_BYTE_ADDR` is NOT defined, then writes and reads start at 0x00.

### TWI Client Demo / Test

This demo creates a 16B RAM buffer, with 8B writable (at an offset of 8), and all 16B are readable.
