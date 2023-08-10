/* 
 * File:   RTC_Prueba.c
 * Author: Mayen
 *
 * Created on 7 de agosto de 2023, 11:09 PM
 */

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


#define _XTAL_FREQ 8000000
#include <xc.h>
#include <stdio.h>
#include <pic16f887.h>

#include "I2C.h"                                        // Libreria del protocolo i2c
#include "LCD_8bits.h"                                        // Libreria de la pantalla lcd
#include "int_on_change.h"
#include "RTC.h"    // Libreria para el manejo del RTC DS3231
#include "map.h"

void setup ();

int8_t estado = 0;
int8_t day, mth, year, dow, hr, min, sec;
char buffer[20];
int adc;
int temperatura;

const char letra[8] = {0x0E, 0x00, 0x16, 0x19, 0x11, 0x11, 0x11, 0x00};

void __interrupt() isr(void) {
    if (INTCONbits.RBIF) {
            if (PORTBbits.RB0 == 0) {
                Lcd_Clear();
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String("                                ");
                Lcd_Set_Cursor(2,1);
                Lcd_Write_String("                                ");
                __delay_ms(50);
                estado++;           
            }
            INTCONbits.RBIF = 0; // Limpiar la bandera de interrupción del puerto B
        }
} 

void main()
{
    
    I2C_Master_Init(100000);                        // Inicializa el protocolo i2c
    setup();
    ioc_init(0b111);
    Lcd_Init(); //inicar el adc
    Lcd_Clear(); 
    while(1)
    {             
        switch(estado)
        {
            case 0:
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String("TEMP"); 
                I2C_Master_Start();
                I2C_Master_Write(0x51);
                adc = I2C_Master_Read(0);
                I2C_Master_Stop();
                temperatura = map(adc, 0, 255, 0, 100);
                Lcd_Set_Cursor(2,1);
                int centena = temperatura / 100;
                int decena = (temperatura / 10) % 10;
                int unidad = temperatura % 10;
                Lcd_Write_Char('0' + centena); // Muestra el dígito de las centenas
                Lcd_Write_Char('0' + decena); // Muestra el dígito de las decenas
                Lcd_Write_Char('0' + unidad); // Muestra el dígito de las unidades
                Lcd_Write_Char('C');               
                __delay_ms(50); 
                
                
                              
                DS3231_Get_Date((uint8_t)&day, (uint8_t)&mth, (uint8_t)&year, (uint8_t)&dow);
                DS3231_Get_Time((uint8_t)&hr, (uint8_t)&min, (uint8_t)&sec);
                
                sprintf(buffer, "%02u/%02u/20%02u", day, mth, year);
                Lcd_Set_Cursor(1,7);
                Lcd_Write_String(buffer);
                sprintf(buffer, "%02u:%02u:%02u", hr, min, sec);
                Lcd_Set_Cursor(2,7);
                Lcd_Write_String(buffer);
                __delay_ms(200);
                break;
                
            case 1:
                Lcd_Set_Cursor(1,1);
                sprintf(buffer, "Dia: %02u", day);
                Lcd_Write_String(buffer);
                
                if (PORTBbits.RB1 == 0) {
                    while(PORTBbits.RB1 == 0);
                    __delay_ms(20);
                    day++;             
                    if(day > 31){
                        day = 31;
                    }
                }
                if(PORTBbits.RB2 == 0){
                    while(PORTBbits.RB2 == 0);
                    __delay_ms(20);
                    day--;
                    if(day < 1){
                        day = 1;
                    }
                }
                break;
                
            case 2:
                Lcd_Set_Cursor(1,1);
                sprintf(buffer, "Mes: %02u", mth);
                Lcd_Write_String(buffer);
                if (PORTBbits.RB1 == 0) {
                    while(PORTBbits.RB1 == 0);
                    mth++;
                    if(mth > 12){
                        mth = 12;
                    }
                }
                if(PORTBbits.RB2 == 0){
                    while(PORTBbits.RB2 == 0);
                    mth--;
                    if(mth < 1){
                        mth = 1;
                    }
                }
                break;
                
            case 3:
                Lcd_Set_Cursor(1,1);
                Lcd_Write_Char('A');
                Lcd_Write_Char(0);
                Lcd_Write_Char('o');
                sprintf(buffer, ": %02u", year);
                Lcd_Write_String(buffer);
                if (PORTBbits.RB1 == 0) {
                    while(PORTBbits.RB1 == 0);
                    year++;
                    if(year > 99){
                        year = 99;
                    }
                }
                if (PORTBbits.RB2 == 0) {
                    while(PORTBbits.RB2 == 0);
                    year--;
                    if(year == -1){
                        year = 0;
                    }
                }
                break;
                
            case 4:
                Lcd_Set_Cursor(1,1);
                sprintf(buffer, "Dia semana: %u", dow);
                Lcd_Write_String(buffer);
                Lcd_Set_Cursor(2,1);
                sprintf(buffer, "%s    ", dw[dow]);
                Lcd_Write_String(buffer);
                if (PORTBbits.RB1 == 0) {
                    while(PORTBbits.RB1 == 0);
                    dow++;
                    if(dow > 6){
                        dow = 6;
                    }
                }
                if (PORTBbits.RB2 == 0) {
                    while(PORTBbits.RB2 == 0);
                    dow--;
                    if(dow == -1){
                        dow = 0;
                    }
                }
                break;
                
            case 5:
                Lcd_Set_Cursor(1,1);
                sprintf(buffer, "Hora: %02u", hr);
                Lcd_Write_String(buffer);
                if (PORTBbits.RB1 == 0) {
                    while(PORTBbits.RB1 == 0);
                    hr++;
                    if(hr > 23){
                        hr = 23;
                    }
                }
                if (PORTBbits.RB2 == 0) {
                    while(PORTBbits.RB2 == 0);
                    hr--;
                    if(hr == -1){
                        hr = 0;
                    }
                }
                break;
                
            case 6:
                Lcd_Set_Cursor(1,1);
                sprintf(buffer, "Minuto: %02u", min);
                Lcd_Write_String(buffer);
                if (PORTBbits.RB1 == 0) {
                    while(PORTBbits.RB1 == 0);
                    min++;
                    if(min > 59){
                        min = 59;
                    }
                }
                if (PORTBbits.RB2 == 0) {
                    while(PORTBbits.RB2 == 0);
                    min--;
                    if(min == -1){
                        min = 0;
                    }
                }
                break;
                
            case 7:
                Lcd_Set_Cursor(1,1);
                sprintf(buffer, "Segundo: %02u", sec);
                Lcd_Write_String(buffer);
                if (PORTBbits.RB1 == 0) {
                    while(PORTBbits.RB1 == 0);
                    sec++;
                    if(sec > 59){
                        sec = 59;
                    }
                }
                if (PORTBbits.RB2 == 0) {
                    while(PORTBbits.RB2 == 0);
                    sec--;
                    if(sec == -1){
                        sec = 0;
                    }
                }
                break;
                
            default:
            	day = (uint8_t)day;
            	mth = (uint8_t)mth;
            	year = (uint8_t)year;
            	dow = (uint8_t)dow;
            	hr = (uint8_t)hr;
            	min = (uint8_t)min;
            	sec = (uint8_t)sec;
                DS3231_Set_Date_Time(day,mth,year,dow,hr,min,sec);
                estado = 0;
                break;
        }
    }
}
void setup(void){
    ANSEL= 0;             // RA0 como entrada analógica
    ANSELH = 0;
    
    TRISA = 0;
    TRISB = 0b00000111; //entrada
    //TRISC = 0b10000000;
    TRISD = 0;
    TRISE = 0; 
    
    PORTA = 0;
    PORTB = 0;  
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    
    estado = 0;
    
    OSCCONbits.IRCF = 0B111;
    OSCCONbits.SCS = 1;
    INTCONbits.GIE = 1;     //habilitar interrupciones globales  

}
