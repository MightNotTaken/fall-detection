#include "MS51_16K.h"


#define I2C_CLOCK           13

void I2C_Init(void) {
    P13_OPENDRAIN_MODE;
    P14_OPENDRAIN_MODE;
    
    I2CLK = I2C_CLOCK;
    set_I2CON_I2CEN;
}

static void I2C_Wait_SI(void) {
    while (!SI);
    clr_I2CON_SI;
}

void I2C_Write(uint8_t slaveAddress, uint8_t *data, uint8_t dataSize) {
    uint8_t i;
    
    // Start condition
    set_I2CON_STA;
    I2C_Wait_SI();
    
    // Send slave address + write bit
    I2DAT = (slaveAddress << 1) | 0x00;
    I2C_Wait_SI();
    
    for (i = 0; i < dataSize; i++) {
        // Write data byte
        I2DAT = data[i];
        I2C_Wait_SI();
    }
    
    // Stop condition
    set_I2CON_STO;
    while (STO);
}

void I2C_Read(uint8_t slaveAddress, uint8_t *data, uint8_t dataSize) {
    uint8_t i;
    
    // Start condition
    set_I2CON_STA;
    I2C_Wait_SI();
    
    // Send slave address + read bit
    I2DAT = (slaveAddress << 1) | 0x01;
    I2C_Wait_SI();
    
    for (i = 0; i < dataSize - 1; i++) {
        // Read data byte with ACK
        set_I2CON_AA;
        I2C_Wait_SI();
        data[i] = I2DAT;
    }
    
    // Read last data byte with NACK
    clr_I2CON_AA;
    I2C_Wait_SI();
    data[dataSize - 1] = I2DAT;
    
    // Stop condition
    set_I2CON_STO;
    while (STO);
}