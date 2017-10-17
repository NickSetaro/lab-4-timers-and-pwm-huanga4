#include <msp430.h> 


/**
 * main.c
 * Austin Huang
 * Hardware PWM 2553
 */
volatile unsigned int f;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog

    P1DIR |= BIT0;                          // set P1.0  LED
    P1OUT &= ~BIT0;                         // clear P1.0


    P1DIR |=BIT6;                           //set Port 9.4 LED
    P1OUT &= ~BIT6;                         //clear P9.4

    P1DIR  &= ~BIT3;                        // set P1.3
    P1OUT |= BIT3;                          // configure Pull-Up
    P1REN |= BIT3;                          // enable pull-up

    TA0CCTL1 = OUTMOD_7;                    // reset/set
    TA0CTL = TASSEL_2 + MC_1 +TACLR ;       // TimerA set up, Up mode, SMCLK
    TA0CCR0 = 100-1;                        
    TA0CCR1 = 50;                           // 50% Duty Cycle


    while(1)
    {
        if(!(P1IN & BIT3)){ //button pressed
            P1OUT |= BIT6; //Sets P1.6
            if(TA0CCR1 <= 90){ // Check brightness below 90%
                TA0CCR0 = 0; // reset CCR0
                TA0CCR1 += 10; // add 10%
                TA0CCR0 = 100; // assign CCR0 value 
            }
            else if (TA0CCR1 == 100){ // Check if brightness is max
                TA0CCR0 = 0; //reset CCR0
                TA0CCR1 = 0; //reset CCR1
                TA0CCR0 = 100; //assign CCR0 value
            }
        }
        if((P1IN & BIT3)){ 
            P1OUT &= ~BIT6; //Clear P4.7
        }
        // Debounce
        for(j=100;j>0;j--)
        {
        __delay_cycles(500);
        }
    }
}
