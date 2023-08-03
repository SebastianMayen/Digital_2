/* 
 * File:   Lab3_PIC2.c
 * Author: Mayen
 *
 * Created on 31 de julio de 2023, 06:02 PM
 */

//******************************************************************************
//Palabra de congifucion
//******************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT // Bits de selección del oscilador (Oscilador INTOSCIO: función de E/S en el pin RA6/OSC2/CLKOUT, función de E/S en el pin RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Bit de habilitación del perro guardián (Perro guardián deshabilitado, se puede habilitar mediante el bit SWDTEN del registro WDTCON)
#pragma config PWRTE = OFF      // Bit de habilitación del temporizador de encendido (Temporizador de encendido deshabilitado)
#pragma config MCLRE = OFF      // Bit de selección de la función del pin RE3/MCLR (La función del pin RE3/MCLR es una entrada digital, MCLR está conectado internamente a VDD)
#pragma config CP = OFF         // Bit de protección del código (Protección de código de memoria de programa deshabilitada)
#pragma config CPD = OFF        // Bit de protección de datos (Protección de código de memoria de datos deshabilitada)
#pragma config BOREN = OFF      // Bits de selección del restablecimiento por bajo voltaje (Restablecimiento por bajo voltaje deshabilitado)
#pragma config IESO = OFF       // Bit de selección de conmutación interna/externa (Modo de conmutación interna/externa deshabilitado)
#pragma config FCMEN = OFF      // Bit de habilitación del monitor de reloj a prueba de fallas (Monitor de reloj a prueba de fallas deshabilitado)
#pragma config LVP = OFF        // Bit de habilitación de la programación de voltaje bajo (El pin RB3 tiene E/S digital, se debe utilizar HV en MCLR para la programación)

// CONFIG2
#pragma config BOR4V = BOR40V   // Bit de selección de restablecimiento por bajo voltaje (Restablecimiento por bajo voltaje ajustado a 4.0V)
#pragma config WRT = OFF        // Bits de habilitación de la escritura automática en la memoria de programa Flash (Protección de escritura desactivada)


//******************************************************************************
//Librerias
//******************************************************************************
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "SPI.h"
#include "config_ADC.h"

//******************************************************************************
//Prototipos de funcion
//******************************************************************************
void setup(void);

//******************************************************************************
//Variables Globles 
//******************************************************************************
#define _XTAL_FREQ 2000000
uint8_t valor_ADC1;

//******************************************************************************
//ISR
//******************************************************************************
void __interrupt() isr(void){
    if(ADIF == 1){
        while (!ADIF);
        ADIF = 0;
    }
    if(SSPIF == 1){       
        spiWrite(valor_ADC1);
        SSPIF = 0;
    } 
}
//******************************************************************************
//Funcion principal
//******************************************************************************
void main() {
    setup();
    adc_init(0);
    while(1){       
        valor_ADC1 = adc_read();        
    }
    return;
}

//******************************************************************************
//Funciones  
//******************************************************************************
void setup(void){

    ANSEL= 0b1;             // RA0 como entrada analógica
    ANSELH = 0;
    TRISA = 0b1;

    TRISB = 0;
    TRISD = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    
    OSCCONbits.IRCF = 0B111;
    OSCCONbits.SCS = 1;
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIR1bits.SSPIF = 0;
    PIE1bits.SSPIE = 1;
    TRISAbits.TRISA5 = 1;
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
}

