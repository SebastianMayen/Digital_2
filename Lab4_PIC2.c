/* 
 * File:   Lab4_PIC2.c
 * Author: Mayen
 *
 * Created on 6 de agosto de 2023, 08:55 PM
 */
//******************************************************************************
//Palabra de congifucion
//******************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT // Bits de selecci�n del oscilador (Oscilador INTOSCIO: funci�n de E/S en el pin RA6/OSC2/CLKOUT, funci�n de E/S en el pin RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Bit de habilitaci�n del perro guardi�n (Perro guardi�n deshabilitado, se puede habilitar mediante el bit SWDTEN del registro WDTCON)
#pragma config PWRTE = OFF      // Bit de habilitaci�n del temporizador de encendido (Temporizador de encendido deshabilitado)
#pragma config MCLRE = OFF      // Bit de selecci�n de la funci�n del pin RE3/MCLR (La funci�n del pin RE3/MCLR es una entrada digital, MCLR est� conectado internamente a VDD)
#pragma config CP = OFF         // Bit de protecci�n del c�digo (Protecci�n de c�digo de memoria de programa deshabilitada)
#pragma config CPD = OFF        // Bit de protecci�n de datos (Protecci�n de c�digo de memoria de datos deshabilitada)
#pragma config BOREN = OFF      // Bits de selecci�n del restablecimiento por bajo voltaje (Restablecimiento por bajo voltaje deshabilitado)
#pragma config IESO = OFF       // Bit de selecci�n de conmutaci�n interna/externa (Modo de conmutaci�n interna/externa deshabilitado)
#pragma config FCMEN = OFF      // Bit de habilitaci�n del monitor de reloj a prueba de fallas (Monitor de reloj a prueba de fallas deshabilitado)
#pragma config LVP = OFF        // Bit de habilitaci�n de la programaci�n de voltaje bajo (El pin RB3 tiene E/S digital, se debe utilizar HV en MCLR para la programaci�n)

// CONFIG2
#pragma config BOR4V = BOR40V   // Bit de selecci�n de restablecimiento por bajo voltaje (Restablecimiento por bajo voltaje ajustado a 4.0V)
#pragma config WRT = OFF        // Bits de habilitaci�n de la escritura autom�tica en la memoria de programa Flash (Protecci�n de escritura desactivada)

//*****************************************************************************
// Definici�n e importaci�n de librer�as
//*****************************************************************************
#include <stdint.h>
#include <pic16f887.h>
#include "I2C.h"
#include "config_ADC.h"
#include <xc.h>
//*****************************************************************************
// Definici�n de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
uint8_t z;
uint8_t dato;
uint8_t valor_ADC;
//*****************************************************************************
// Definici�n de funciones para que se puedan colocar despu�s del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
//*****************************************************************************
// C�digo de Interrupci�n 
//*****************************************************************************
void __interrupt() isr(void){
   if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupci�n recepci�n/transmisi�n SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepci�n se complete
            //PORTD = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepci�n
            __delay_us(50);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = valor_ADC;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
   if(ADIF == 1){
        while (!ADIF);
        ADIF = 0;
    }
}
//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    adc_init(0);
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        valor_ADC = adc_read();
        //PORTB = valor_ADC;       
       __delay_ms(100);
    }
    return;
}
//*****************************************************************************
// Funci�n de Inicializaci�n
//*****************************************************************************
void setup(void){
    ANSEL = 0b1;
    ANSELH = 0;
    
    TRISA = 0b1;
    TRISB = 0;
    TRISD = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    
    
    
    I2C_Slave_Init(0x50);   
    
    OSCCONbits.IRCF = 0B111;
    OSCCONbits.SCS = 1;
    
  
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    
}