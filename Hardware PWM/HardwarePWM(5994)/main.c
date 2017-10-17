#include <msp430.h> 


/**
 * main.c
 * Austin Huang
 * Hardware PWM 5994
 */
volatile unsigned int f;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog 
    P1DIR |= BIT0;                          // set P1.0 
    P1OUT &= ~BIT0;                         // clear P1.0
    
    P1DIR |=BIT1;                           //set Port 1.1 output LED
    P1OUT &= ~BIT1;                         //clear P1.1
    
    P5DIR  &= ~BIT6;                        // set P5.6 as input
    P5OUT |= BIT6;                          // configure pull-up
    P5REN |= BIT6;                          // enable pull-up
    
    TA0CTL = TASSEL_2 + MC_1 +TACLR ;
    TA0CCTL1 = OUTMOD_7;
    TA0CCR0 = 1000;
    TA0CCR1 = 500;
    
    P1SEL0 |= BIT0;
    P1SEL1 &= ~BIT0;

    while(1)
    {
        if(!(P5IN & BIT6)){
            P1OUT |= BIT1; //set P1.1
            if(TA0CCR1 < 1000){
                TA0CCR1 += 100;
            }
            else if (TA0CCR1 >= 1000){
                TA0CCR1 = 0;
            }
        }
        if((P5IN & BIT6)){
            P1OUT &= ~BIT1; //clear P1.1
        }
        // Debounce
        for(j=100;j>0;j--)
        {
        __delay_cycles(500);
        }
    }
}
