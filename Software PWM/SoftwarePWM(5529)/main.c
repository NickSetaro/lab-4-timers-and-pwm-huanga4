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


    P4DIR |=BIT7; //set Port 4.7 LED
    P4OUT &= ~BIT7; //clear P4.7

    P2DIR  &= ~BIT1;                        // set P2.1
    P2OUT |= BIT1;                          // configure pull-up
    P2REN |= BIT1;                          // enable pull-up
    P2IE |= BIT1; //enable interrupt
    P2IES &= ~BIT1; //set falling edge
    P2IFG &= ~(BIT1); //clear flag


    TA0CTL = TASSEL_2 + MC_1 ;       //TimerA0 set up, Up mode, SMCLK 
    TA0CCTL1 = (CCIE);
    TA0CCTL0 = (CCIE);
    TA0CCR0 = 1000-1;                        
    TA0CCR1 = 500;                           // 50% Duty Cycle

    __bis_SR_register(GIE);
    while(1)
    {
        if((P5IN & BIT6)){
            P4OUT &= ~BIT7; //clear P4.7
        }
    }
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

#pragma vector=PORT2_VECTOR
__interrupt void PORT2_IRS(void)
{
    P2IE &= ~BIT1;
    __delay_cycles(1000);
    P2IE |= BIT1;

    P4OUT |= BIT7; //sets P4.7
    if(TA0CCR1 < 1000){
        TA0CCR0 = 0; //reset CCR0
        TA0CCR1 += 100;  //assign and increment CCR1 value
        TA0CCR0 = 1000;  //assign CCR0 value;
    }
    else if (TA0CCR1 >= 1000){
        TA0CCR0 = 0;  //reset CCR0
        TA0CCR1 = 0;  //reset CCR1
        TA0CCR0 = 1000;  //assign CCR0 value;
    }
    P2IFG &= ~BIT1;
}
