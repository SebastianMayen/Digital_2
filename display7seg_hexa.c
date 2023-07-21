#include "display7seg_hexa.h"

uint8_t Primer_display;
uint8_t Segundo_display;
uint8_t display[3];
uint8_t TABLA[16] = {
    0b00111111,  //0
    0b00000110,  //1
    0b01011011,  //2
    0b01001111,  //3
    0b01100110,  //4          
    0b01101101,  //5
    0b01111101,  //6
    0b00000111,  //7
    0b01111111,  //8
    0b01101111,  //9
    0b01110111,  //A
    0b01111100,  //b
    0b00111001,  //C
    0b01011110,  //d
    0b01111001,  //E
    0b01110001   //F
};
void display7seg (uint8_t valorEntero){
    // Obtener los 4 bits más altos (parte1) usando una máscara y desplazamiento
    Primer_display = (valorEntero & 0xF0) >> 4;
    Segundo_display = valorEntero & 0x0F ;
    display[0] = TABLA[Primer_display];
    display[1] = TABLA[Segundo_display];
}
