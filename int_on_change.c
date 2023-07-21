#include "int_on_change.h"

void int_puertob(void){
    if (INTCONbits.RBIF) {
            if (PORTBbits.RB0 == 0) {
                PORTD++;           
            }else if (PORTBbits.RB1 == 0) {
                PORTD--;             
            }
            INTCONbits.RBIF = 0; // Limpiar la bandera de interrupción del puerto B
        }
}
void ioc_init(char pin){
    OPTION_REGbits.nRBPU = 0; // Habilitar resistencias pull-up en el puerto B
    WPUB = pin|WPUB; // Habilitar resistencia pull-up
    INTCONbits.RBIE = 1; // Habilitar interrupciones del puerto B  
    
    IOCB = pin|IOCB; 
}
