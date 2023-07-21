/* 
 * File:   laboratorio.c
 * Author: Mayen
 *
 * Created on 17 de julio de 2023, 05:50 PM
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
#include "int_on_change.h"
#include "config_ADC.h"

//******************************************************************************
//Prototipos de funcion
//******************************************************************************
void setup(void);

//******************************************************************************
//Variables Globles 
//******************************************************************************
#define _XTAL_FREQ 500000

//******************************************************************************
//ISR
//******************************************************************************
void __interrupt() isr(void) {
    if (PIR1bits.ADIF) {
        PIR1bits.ADIF = 0; // Limpiar la bandera de interrupción del ADC
    }
    if (INTCONbits.RBIF) {
        if (PORTBbits.RB0 == 0) {
            PORTD++;           
        }else if (PORTBbits.RB1 == 0) {
            PORTD--;             
        }
        INTCONbits.RBIF = 0; // Limpiar la bandera de interrupción del puerto B
    }    
}
//******************************************************************************
//Funcion principal
//******************************************************************************
void main() {
setup();
ioc_init(0b11);
adc_init(0);
    while(1){
       // Realizamos una lectura del ADC y almacenamos el resultado en una variable
        PORTC = adc_read();    
        
        __delay_ms(100); // Esperar un tiempo antes de realizar otra lectura (por ejemplo, 500ms)
    }
}

//******************************************************************************
//Funciones  
//******************************************************************************
void setup(void){

    ANSELH = 0;
    ANSEL= 0b1; // RA0 como entrada analógica
  
    TRISA = 0b1;
    TRISD = 0;
    TRISC = 0;
    TRISB = 0b11111111;
    
    PORTA = 0;
    PORTD = 0;
    PORTC = 0;
    PORTB = 0;
    
    INTCONbits.GIE = 1; // Habilitar interrupciones globales

    
}


