 /*
 * Main file of adc-dac.X
 * Author: mosee
 *
 */

#include "xc.h"
#include "../lib/adc.h"
#include "../lib/mcp4821.h"



int main(void) {
    int16_t voltage;
    
    _TRISA4 = 0;
    adcInit(ADC_TIMER3_SAMPLING);
    mcp4821Init(MCP4821_GAIN_X2);

    PR3 = 368;                 // T=100us=(PR1+1)/3.685MHz => PR1=368
	T3CONbits.TON = 1;
    
	while(1) {
        // On attend la fin de la conversion
        if (adcConversionDone()) {
            _LATA4 = 1;
            voltage = adcRead();
            mcp4821Write(voltage);
            _LATA4 = 0;
        }
	}
}
