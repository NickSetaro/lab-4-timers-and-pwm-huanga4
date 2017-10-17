#include <msp430.h> 


/**
 * main.c
 * Austin Huang
 * Hardware PWM 5529
 */
volatile unsigned int f;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    P1DIR |= BIT0;                          // set P1.0  LED
    P1OUT &= ~BIT0;                         // clear P1.0


    P4DIR |=BIT7;                           //set Port 4.7 LED
    P4OUT &= ~BIT7;                         //clear P4.7

    P2DIR  &= ~BIT1;                        // Set input
    P2OUT |= BIT1;                          // configure pull-up
    P2REN |= BIT1;                          // enable pull-up

    TA0CCTL1 = OUTMOD_7;                    // reset/set
    TA0CTL = TASSEL_2 + MC_1 + TACLR ;       // TimerA set up, Up mode, SMCLK
    TA0CCR0 = 100-1;                        //
    TA0CCR1 = 50;                           // 50% Duty Cycle

    while(1)
    {
        if(!(P1IN & BIT1)){ //button pressed
            P4OUT |= BIT7; //sets P4.7
            if(TA0CCR1 <= 90){ // Check brightness below 90%
                TA0CCR0 = 0; // reset CCR0
                TA0CCR1 += 10; // add 10%
                TA0CCR0 = 100; // assign CCR0 value 
            }
            else if (TA0CCR1 == 100){ // Check if brightness is max
                TA0CCR0 = 0; // reset CCR0
                TA0CCR1 = 0; // reset CCR1
                TA0CCR0 = 100; //assign CCR0 value
            }
        }
        if((P1IN & BIT1)){
            P4OUT &= ~BIT7; //Clear P4.7 
        }
        // Debounce
        for(j=100;j>0;j--)
        {
        __delay_cycles(500);
        }
    }
}
