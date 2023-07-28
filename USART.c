#include "USART.h"
#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>

char palabra;
char valor;

void UART_RX_config (uint16_t baudrate){
    SPBRG = baudrate;
    SYNC = 0;
    SPEN = 1;
    CREN = 1;
    RCIF = 0;
    RCIE = 1;
}
void UART_TX_config (uint16_t baudrate){
    SPBRG = baudrate;
    TXEN = 1;
}
void UART_write_char (char *palabra){
    // Esperar hasta que el buffer de transmisión (TXREG) esté vacío
    while(*palabra !='\0'){
        while(TXIF != 1);
        TXREG= *palabra;
        *palabra++;
    }
    /*while(PIR1bits.RCIF==0){ // interrupcion de eusart
            ;
        }*/
}
void UART_read_char (char *valor){
    if(RCIF == 1){
        *valor = RCREG;
        RCIF = 0;   
    }
}