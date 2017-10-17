#include <msp430.h> 


/**
 * main.c
 * Austin Huang
 * Hardware PWM 2311
 */
volatile unsigned int f;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog 
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode

    P1DIR |= BIT0;                          // set P1.0 LED
    P1OUT &= ~BIT0;                         // clear P1.0


    P2DIR |=BIT0;                           //set Port 2.0 LED
    P2OUT &= ~BIT0;                         //clear P2.0

    P1DIR  &= ~BIT1;                        // set P1.1
    P1OUT |= BIT1;                          // configure pull-up
    P1REN |= BIT1;                          // enable pull-up

    TB0CCTL1 = OUTMOD_7;                    // reset/set
    TB0CTL = TBSSEL_2 + MC_1 +TBCLR ;       // TimerB set up, Up mode, SMCLK
    TB0CCR0 = 100-1;                        
    TB0CCR1 = 50;                           // 50% Duty Cycle


    while(1)
    {
        if(!(P1IN & BIT1)) //button pressed
        {
            P2OUT |= BIT0; //Sets P9.4
            if(TB0CCR1 <= 90){ //Check brightness below 90%
                TB0CCR0 = 0; // reset CCR0
                TB0CCR1 += 10; // add 10
                TB0CCR0 = 100; // assign CCR0 value
            }
            else if (TB0CCR1 == 100){ // Check if brightness is max
                TB0CCR0 = 0; //reset CCR0
                TB0CCR1 = 0; //reset CCR1
                TB0CCR0 = 100; //assign CCR0 value
            }
        }
        if((P1IN & BIT1)){
            P2OUT &= ~BIT0; //Clear P9.4
        }
        // Debounce
        for(j=100;j>0;j--)
        {
        __delay_cycles(500);
        }
    }
}
