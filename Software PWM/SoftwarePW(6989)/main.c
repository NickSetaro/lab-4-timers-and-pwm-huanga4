#include <msp430.h> 
#include <Math.h>
/**
 * main.c
 * Austin Huang
 */

volatile unsigned int j;
int taps = 10;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog 
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
    P1DIR |= BIT0;                          // set P1.0
    P1OUT &= ~BIT0;                         // clear P1.0


    P9DIR |=BIT7; //set port
    P9OUT &= ~BIT7; //clear

    P1DIR  &= ~BIT1;                        // set P1.1 
    P1OUT |= BIT1;                          // configure pull-up
    P1REN |= BIT1;                          // enable pull-up
    P1IE |= BIT1; //enable interrupt 
    P1IES &= ~BIT1; //set falling edge
    P1IFG &= ~(BIT1); //clear flag


    TA0CTL = TASSEL_2 + MC_1 ;        //TimerA0 set up, Up mode, SMCLK 
    TA0CCTL1 = (CCIE);
    TA0CCTL0 = (CCIE);
    TA0CCR0 = 1000-1;                        
    TA0CCR1 = 500;                           // 50% Duty Cycle

    __bis_SR_register(GIE);
    while(1)
    {
        if((P1IN & BIT1)){
            P9OUT &= ~BIT7; //clear P9.9
        }
    }
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR (void)
{
    if(TA0CCR1 != 1000){
       P1OUT &= ~(BIT0); // LED off
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

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_IRS(void)
{
    P1IE &= ~BIT1;
    __delay_cycles(100);
    P1IE |= BIT1;

    P9OUT |= BIT7; //sets P9.7
    if(TA0CCR1 < 1000){
        int incrementNum = TA0CCR1 + 100;
        TA0CCR0 = 0;
        TA0CCR1 = incrementNum;
        TA0CCR0 = 1000;
    }
    else if (TA0CCR1 >= 1000){
        TA0CCR0 = 0;
        TA0CCR1 = 0;
        TA0CCR0 = 1000;
    }
    P1IFG &= ~BIT1;
}
