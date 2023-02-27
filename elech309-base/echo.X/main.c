/*
 * Main file of echo.X
 * Author: mosee
 *
 * Projet servant a tester une connexion UART.
 * RB6 sert de patte Rx du dsPIC et RB7 sert de patte Tx.
 * L'UART du dsPIC est configure en 8N1 a 57.6 kb/s
 * Le dsPIC implemente un simple echo : il ecoute son Rx et renvoie tout ce 
 * qu'il recoit sur son Tx.
 */

#include "xc.h"


int main(void) {
    unsigned char c;
    
    /* Configuration du Peripheral Pin Select (PPS) pour connecter le signal
     * Rx de l'UART1 a RB6/RP6 et le signal Tx a RB7/RP7 */
	_U1RXR = 6;    // U1RX -> RP6
	_RP7R = 3;     // RP7 -> U1Tx

    // Configuration de l'UART1 avec un format de trame 8N1, a 57600 bits/s
    U1MODEbits.PDSEL = 0;       // 8 bits, pas de parite
    U1MODEbits.STSEL = 0;       // 1 stop bit
    /* L'UART peut fonctionner a 2 vitesses differentes. Le mode "high speed" 
     * est plus sensible au bruit et ne doit donc etre utilise que pour
     * des debits trop rapides pour le mode standard 
     * En mode standard, le deit est donne par :
     * baud rate = FCY / (16*(U1BRG+1) 
     * => U1BRG = (3.685MHz / (16*57.6kHz)) - 1  =  2.998*/
    U1MODEbits.BRGH = 0;
    U1BRG = 3;
    
    // imperativement dans cet ordre :
    U1MODEbits.UARTEN = 1;      // on active l'UART
    U1STAbits.UTXEN = 1;        // on active l'emission
    
    // Boucle principale
	while(1) {
        if (U1STAbits.URXDA) { // si UART1 a recu un octet // URXDA (UART Rx Data Available) indique qu'une donnee est disponible
            c = U1RXREG;                // on le lit // recup la donnee la plus ancienne de la memoire tampon FIFO venant du UART
            while (U1STAbits.UTXBF) {}  // on attend que le buffer d'emission soit libre // indique si le buffer de transmission est plain (car transmission de la trame met plus de temps q'une ecriture dans un registre)
            U1TXREG = c;                // et on le renvoie // ecrire dans ce registre renvoie une donnee a l'UART (par la FIFO)
        }
        
	}
}
