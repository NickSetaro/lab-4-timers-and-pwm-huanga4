#include <msp430.h> 
#include <Math.h>
/**
 * main.c
 * Austin Huang
 */

volatile unsigned int j;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog 
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
    P1DIR |= BIT0;                          // set P1.0 output
    P1OUT &= ~BIT0;                         // clear P1.0


    P1DIR |=BIT1; //set Port 1.1
    P1OUT &= ~BIT1; //Clear P1.1

    P5DIR  &= ~BIT6;                        // set P5.6
    P5OUT |= BIT6;                          //configure pull-up
    P5REN |= BIT6;                          //enable pull-up
    P5IE |= BIT6; //enableinterrupt
    P5IES &= ~BIT6; //set falling edge
    P5IFG &= ~(BIT6); //clear flag


    TA0CTL = TASSEL_2 + MC_1 ;   //TimerA0 set up, Up mode, SMCLK 
    TA0CCTL1 = (CCIE);
    TA0CCTL0 = (CCIE);
    TA0CCR0 = 1000-1;                        
    TA0CCR1 = 500;                           // 50% Duty Cycle

    __bis_SR_register(GIE);
    while(1)
    {
        if((P5IN & BIT6)){
            P1OUT &= ~BIT1; //clear P1.1
        }
    }
}

//Timer A interrupt vectors
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

#pragma vector=PORT5_VECTOR
__interrupt void PORT5_IRS(void)
{
    P5IE &= ~BIT6;
    __delay_cycles(1000);
    P5IE |= BIT6;

    P1OUT |= BIT1; //sets P1.1
    if(TA0CCR1 < 1000){
        TA0CCR0 = 0;  //reset CCR0
        TA0CCR1 += 100;  //assign and increment CCR0 value
        TA0CCR0 = 1000; //assign CCR0 value
    }
    else if (TA0CCR1 >= 1000){
        TA0CCR0 = 0;  //reset CCR0
        TA0CCR1 = 0;  //reset CCR1
        TA0CCR0 = 1000; //assign CCR0 value
    }
    P5IFG &= ~BIT6;
}
