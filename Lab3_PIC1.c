/* 
 * File:   Lab3_PIC1.c
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
#include <stdint.h>
#include <stdlib.h>
#include "SPI.h"
#include "LCD_8.h"

#define _XTAL_FREQ 8000000
#define RS PORTEbits.RE0
#define EN PORTEbits.RE2
#define D0 PORTDbits.RD0
#define D1 PORTDbits.RD1
#define D2 PORTDbits.RD2
#define D3 PORTDbits.RD3
#define D4 PORTDbits.RD4
#define D5 PORTDbits.RD5
#define D6 PORTDbits.RD6
#define D7 PORTDbits.RD7

//******************************************************************************
//Prototipos de funcion
//******************************************************************************
void setup(void);

//******************************************************************************
//Variables Globles 
//******************************************************************************
int contador;
uint8_t ADC_1;
int ADC_2;
char contador_string[4];
char ADC_1_string[4];
char ADC_2_string[4];
//******************************************************************************
//ISR
//******************************************************************************

//******************************************************************************
//Funcion principal
//******************************************************************************
void main() {
    setup();
    Lcd_Init(); //inicar el adc
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("POT1:");
    Lcd_Set_Cursor(1,7);
    Lcd_Write_String("POT2:");
    Lcd_Set_Cursor(1,13);
    Lcd_Write_String("CONT");
    while(1){
        // ESCLAVO 1// 
        PORTCbits.RC2 = 0; //slace select
        __delay_ms(1);  
        spiWrite(0); //mandamos el 0 al esclavo como instrucción del ADC
        contador = spiRead();     
        __delay_ms(1);
        spiWrite(1); //mandamos el 1 al esclavo como instruccion del cont
        ADC_1 = spiRead();    
        __delay_ms(1);
        PORTCbits.RC2 = 1; //slave deselect      
        __delay_ms(50);
        
        
      // ESCLAVO 2//  
        PORTCbits.RC1 = 0; //slace select
        __delay_ms(1);     
        spiWrite(0);
        ADC_2 = spiRead();     
        __delay_ms(1);
        PORTCbits.RC1 = 1; //slave deselect  
        __delay_ms(50);
        
        Lcd_Set_Cursor(2,1);
        int centena_ADC1 = ADC_1 / 100;
        int decena_ADC1 = (ADC_1 / 10) % 10;
        int unidad_ADC1 = ADC_1 % 10;
        sprintf(ADC_1_string, "%d%d%d", centena_ADC1, decena_ADC1, unidad_ADC1);
        Lcd_Write_String(ADC_1_string); // Muestra el dígito de las centenas
        
        Lcd_Set_Cursor(2,7);
        int centena_ADC2 = ADC_2 / 100;
        int decena_ADC2 = (ADC_2 / 10) % 10;
        int unidad_ADC2 = ADC_2 % 10;
        sprintf(ADC_2_string, "%d%d%d", centena_ADC2, decena_ADC2, unidad_ADC2);
        Lcd_Write_String(ADC_2_string); // Muestra el dígito de las centenas 
        
    if(contador >= 0 && contador < 10){
        Lcd_Set_Cursor(2,14);
        Lcd_Write_String(" ");
        Lcd_Set_Cursor(2,15);
        Lcd_Write_String(" ");
    }
    if(contador >= 10 && contador < 100){     
        Lcd_Set_Cursor(2,15);
        Lcd_Write_String(" ");
    }    
        Lcd_Set_Cursor(2,13);
        sprintf(contador_string, "%d", contador);
        Lcd_Write_String(contador_string); 
    }
}

//******************************************************************************
//Funciones  
//******************************************************************************
void setup(void){

    ANSEL = 0;
    ANSELH = 0;
    TRISC1 = 0;
    TRISC2 = 0;
    TRISB = 0;
    TRISD = 0;
    TRISA = 0;
    TRISE = 0;
    
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    //PORTD = 0;
    PORTE = 0;
    
    OSCCONbits.IRCF = 0B111;
    OSCCONbits.SCS = 1;
    PORTCbits.RC2 = 1;
    PORTCbits.RC1 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
}


