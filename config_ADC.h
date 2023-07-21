/* 
 * File:   config_ADC.h
 * Author: Mayen
 *
 * Created on 19 de julio de 2023, 06:35 PM
 */

#ifndef CONFIG_ADC_H
#define	CONFIG_ADC_H

#include <xc.h>

void adc_init(int channel);
int adc_read();
void adc_change_channel(int channel);
int adc_get_channel();

#endif	/* CONFIG_ADC_H */

