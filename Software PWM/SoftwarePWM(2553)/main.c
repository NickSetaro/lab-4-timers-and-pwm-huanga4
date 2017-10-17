#include <msp430.h> 
#include <Math.h>
/**
 * main.c
 * Austin Huang
 */

volatile unsigned int j;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog

    P1DIR |= BIT0;                          // set P1.0 
    P1OUT &= ~BIT0;                         // clear P1.0


    P1DIR |=BIT6; //set Port 1.6 output ---LED
    P1OUT &= ~BIT6; //Clear P1.6

    P1DIR  &= ~BIT3;                        // set P1.3 
    P1OUT |= BIT3;                          // configure pull-up
    P1REN |= BIT3;                          // enable pull-up
    P1IE |= BIT3; //enable interrupt 
    P1IES &= ~BIT3; //set falling edge
    P1IFG &= ~(BIT3); //clear flag


    TA0CTL = TASSEL_2 + MC_1 ;      //TimerA0 set up, Up mode, SMCLK 
    TA0CCTL1 = (CCIE);              
    TA0CCTL0 = (CCIE);              
    TA0CCR0 = 1000-1;                        
    TA0CCR1 = 500;                           // 50% Duty Cycle

    __bis_SR_register(GIE);
    while(1)
    {
        if((P1IN & BIT3)){
            P1OUT &= ~BIT6; //clear P1.6
        }
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_IRS(void)
{
    P1IE &= ~BIT3; //interrupt enable off
    __delay_cycles(1000); //Debounce
    P1IE |= BIT3; //interrupt enable on
    P1OUT |= BIT6; //sets P1.6
    if(TA0CCR1 >= 1000){ // Check if brightness is max
        TA0CCR0 = 0; // reset CCR0
        TA0CCR1 = 0;// reset CCR1
        TA0CCR0 = 1000; //assign CCR0 value
    }
    else if (TA0CCR1 < 1000){ // Check brightness less than 90%
        TA0CCR0 = 0; // reset CCR0
        TA0CCR1 += 100; // add 10%
        TA0CCR0 = 1000;//  set CCR0 back to 10 kHz
    }
    P1IFG &= ~BIT3; //clear flag
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR (void)
{
    if(TA0CCR1 != 1000){
       P1OUT &= ~(BIT0); //LED off
    }
    TA0CCTL1 &= ~BIT0; //clear flag
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
    if(TA0CCR1 != 0){
        P1OUT |= (BIT0); //LED off
    }
    TA0CCTL0 &= ~BIT0;  //clear flag
}
