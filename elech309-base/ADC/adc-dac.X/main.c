 /*
 * Main file of adc-dac.X
 * Author: mosee
 *
 */

#define FCY 3685000     // ucycle frequency. Needed for __delay_ms
#include "libpic30.h"   // Contains __delay_ms definition
#include "xc.h"
#include "../lib/adc.h"
#include "../lib/mcp4821.h"


/* Timer3 ISR
 * Called automatically when T3IF is set.
 * NEVER CALL AN ISR ANYWHERE IN THE CODE
 */
void __attribute__((interrupt, no_auto_psv))_T3Interrupt(void) {
    // ISR code does the same things that the main loop did in polling
    _T3IF = 0;
    adcStart();
    _LATA4 = 1;
}


int main(void) {
    int16_t voltage;

    _TRISA4 = 0;
    // Configuration de L'ADC pour utilisation en polling sur AN0
    adcInit(ADC_MANUAL_SAMPLING);
    mcp4821Init(MCP4821_GAIN_X2);

    PR3 = 3684;                 // T=100us=(PR1+1)/3.685MHz => PR1=3684
    _T3IE = 1;
	T3CONbits.TON = 1;
    
	while(1) {
        // On attend la fin de la conversion
        if (adcConversionDone()) {
            voltage = adcRead();
            mcp4821Write(voltage);
            _LATA4 = 0;
        }
	}
}
