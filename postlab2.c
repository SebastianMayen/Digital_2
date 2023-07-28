#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdint.h>
#include <stdio.h>

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

#include "LCD_8bits.h"
#include "config_ADC.h"
#include "map.h"
#include "USART.h"

void setup(void);
int valor_ADC;
int valor_map;
int contador;
int valor_recibido;
char contador_string[4];
char ADC_string[5];

void __interrupt() ISR(void){
    
    
}

void main(void) {
setup(); 
    adc_init(0);
    Lcd_Init(); //inicar el adc
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("POT1:");
    Lcd_Set_Cursor(1,10);
    Lcd_Write_String("CONT");
    UART_RX_config (12);
    UART_TX_config (12);
    UART_write_char("1: Leer potenciometro\r+: sumar al contador\r-: restar al contador\r"); 
  while(1)
  {
        valor_ADC  = adc_read();  
        Lcd_Set_Cursor(2,1);
        valor_map = map(valor_ADC, 0, 255, 0, 500);
        int centena = valor_map / 100;
        int decena = (valor_map / 10) % 10;
        int unidad = valor_map % 10;
        Lcd_Write_Char('0' + centena); // Muestra el dígito de las centenas
        Lcd_Write_Char('.');
        Lcd_Write_Char('0' + decena); // Muestra el dígito de las decenas
        Lcd_Write_Char('0' + unidad); // Muestra el dígito de las unidades
        Lcd_Write_Char('V'); 
         
  
      UART_read_char(&valor_recibido);
      switch (valor_recibido) {
        case 43:
            contador++;
            valor_recibido = 32;
            break;

        case 45:
            contador--;
            valor_recibido = 32;
            break;

        case 49:
            sprintf(ADC_string, "El valor del voltaje es %d.%d%dV\r", centena, decena, unidad);
            UART_write_char(ADC_string);
            valor_recibido = 32;
            UART_write_char("1: Leer potenciometro\r+: sumar al contador\r-: restar al contador\r"); 
            break;

        
        }
    

    Lcd_Set_Cursor(2,10);
    sprintf(contador_string, "%d", contador);
    Lcd_Write_String(contador_string);
    
    if(contador >= 0 && contador < 10){
        Lcd_Set_Cursor(2,11);
        Lcd_Write_String(" ");
        Lcd_Set_Cursor(2,12);
        Lcd_Write_String(" ");
    }
    if(contador >= 10 && contador < 100){     
        Lcd_Set_Cursor(2,12);
        Lcd_Write_String(" ");
    }    
  }  
    return;
}

void setup(void){
    ANSEL= 0b1;             // RA0 como entrada analógica
    ANSELH = 0;
    TRISA = 0b1;            //entrada
    TRISC = 0b10000000;
    TRISD = 0;
    TRISE = 0;   
    PORTC = 0;
    TRISB = 0;   
    PORTB = 0;    
    
    //PORTD =0;
    OSCCONbits.IRCF = 0B111;
    OSCCONbits.SCS = 1;
    INTCONbits.GIE = 1;     //habilitar interrupciones globales  

}


