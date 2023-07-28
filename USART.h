/* 
 * File:   USART.h
 * Author: Mayen
 *
 * Created on 26 de julio de 2023, 06:58 PM
 */

#ifndef USART_H
#define	USART_H

#include <xc.h>

void UART_RX_config (uint16_t baudrate);
void UART_TX_config (uint16_t baudrate);
void UART_write_char (char *palabra);
void UART_read_char (char *valor);

#endif	/* USART_H */

