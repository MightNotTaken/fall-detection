#include "MS51_16K.h"
#include "uart.h"
#include "I2C_Mod.h"
#include "delay.h"

void msdelay(uint32_t);
void print(char* );

void main(void)
{
    /* Note
       MCU power on system clock is HIRC (22.1184MHz), so Fsys = 22.1184MHz
    */
    MODIFY_HIRC(HIRC_24);
    P06_QUASI_MODE;
    UART_Open(16E6, UART0_Timer3, 115200);
    ENABLE_UART0_PRINTF; /* check uart.c */

    I2C_Init();                  /* initial I2C circuit  */
    I2C_Write(0x53, "TAHIR", 5); /* I2C Master will send 10 byte 0x55,0xAA,.... to slave */

    while (1)
    {
        print("This is working\n");
        msdelay(1000);
    }
}


void msdelay(uint32_t ms) {
    Timer0_Delay(16E6, 200, ms);
}

void print(char* string) {
    while (*string) {
        UART_Send_Data(UART0, *string);
        string ++;
    }
}