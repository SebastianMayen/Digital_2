#include "interrupcion_TM0.h"


uint8_t display[3];
uint8_t bandera;

void config_TM0(void){
    bandera = 0;
     //oscilador
    OSCCONbits.IRCF = 0b111 ; //8Mhz
    OSCCONbits.SCS = 1;
    
    //tmr0
    OPTION_REGbits.T0CS = 0; //Usar Timer0 con Fosc/4
    OPTION_REGbits.PSA = 0; //Prescaler con el Timer0
    OPTION_REGbits.PS2 = 1; //Prescaler de 256
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;  
    TMR0 = 216; //VALOR INICIAL DEL TMR0
    
    //Banderas e interrupciones
    INTCONbits.T0IF = 0;    //interrupcion del tmr0
    INTCONbits.T0IE = 1;    //interrupcion del tmr0 
    INTCONbits.PEIE = 1;    // habilitar interrupciones perifericas
}
void int_TM0(int timer0){
    if (INTCONbits.T0IF){ //CHEQUEAR INTERRUPCION DEL TMR0
       PORTE = 0;
    // Chequear el valor de la variable bandera y actualizar el display correspondiente
        if (bandera == 0){//chequear la nbadera
            PORTC = display[0];// usar el prot c
            PORTE = 1;// habilita el diaplay
            bandera = 1;//cambia el valor de la bandra
            
        }
        else if (bandera == 1){ //chequea si la bandera es 1
            PORTC = display[1];// muestra el digito en potrc
            PORTE = 2;// habilita display de decenas
            bandera = 0;//cambia la bandera
        }       
        TMR0 = timer0;
        INTCONbits.T0IF = 0; ///reiniciar el tmr0
           
    }
}
