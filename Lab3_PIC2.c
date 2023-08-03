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


//******************************************************************************
//Librerias
//******************************************************************************
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "SPI.h"
#include "config_ADC.h"
#include "int_on_change.h"

//******************************************************************************
//Prototipos de funcion
//******************************************************************************
void setup(void);

//******************************************************************************
//Variables Globles 
//******************************************************************************
#define _XTAL_FREQ 2000000
uint8_t valor_ADC1;
uint8_t cont;


//******************************************************************************
//ISR
//******************************************************************************
void __interrupt() isr(void){
    if(ADIF == 1){
        while (!ADIF);
        ADIF = 0;
    }
    if (INTCONbits.RBIF) {
            if (PORTBbits.RB0 == 0) {
                cont++;           
            }else if (PORTBbits.RB1 == 0) {
                cont--;             
            }
            INTCONbits.RBIF = 0; // Limpiar la bandera de interrupción del puerto B
        }
    if(SSPIF == 1){ 
        if (spiRead() == 0){
            spiWrite(valor_ADC1);
        }
        if (spiRead() == 1){
            spiWrite(cont);
        }
        SSPIF = 0;
    } 
}
//******************************************************************************
//Funcion principal
//******************************************************************************
void main() {
    setup();
    adc_init(0);
    //ioc_init(0b11);
    while(1){       
        valor_ADC1 = adc_read(); 
        __delay_ms(100);
    }
    return;
}

//******************************************************************************
//Funciones  
//******************************************************************************
void setup(void){

    ANSEL= 0b1;             // RA0 como entrada anal�gica
    ANSELH = 0;
    TRISA = 0b1;

    TRISB = 0b11;
    TRISD = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    
    OSCCONbits.IRCF = 0B111;
    OSCCONbits.SCS = 1;
    
    OPTION_REGbits.nRBPU = 0; // Habilitar resistencias pull-up en el puerto B
    WPUB = 0b11; // Habilitar resistencia pull-up
    INTCONbits.RBIE = 1; // Habilitar interrupciones del puerto B  
    
    IOCB = 0b11;
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIR1bits.SSPIF = 0;
    PIE1bits.SSPIE = 1;
    TRISAbits.TRISA5 = 1;
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
}

