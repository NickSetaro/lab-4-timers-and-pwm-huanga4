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


    P2DIR |=BIT0; //set port
    P2OUT &= ~BIT0; //clear

    P1DIR  &= ~BIT1;                        // set P1.1 
    P1OUT |= BIT1;                          // configure pull-up
    P1REN |= BIT1;                          // enable pull-up
    P1IE |= BIT1; //enable interrupt 
    P1IES &= ~BIT1; //set falling edge
    P1IFG &= ~(BIT1); //clear flag


    TB0CTL = TBSSEL_2 + MC_1 ;       //TimerB0 set up, Up mode, SMCLK 
    TB0CCTL1 = (CCIE);
    TB0CCTL0 = (CCIE);
    TB0CCR0 = 1000-1;                        
    TB0CCR1 = 500;                           // 50% Duty Cycle

    __bis_SR_register(GIE);
    while(1)
    {
        if((P1IN & BIT1)){
            P2OUT &= ~BIT0; //clear P2.0
        }
    }
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void Timer0_B1_ISR (void)
{
    if(TB0CCR1 != 1000){
       P1OUT &= ~(BIT0); //LED off
    }
    TB0CCTL1 &= ~BIT0; //clear flag
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR (void)
{
    if(TB0CCR1 != 0){
        P1OUT |= (BIT0); //LED off
    }
    TB0CCTL0 &= ~BIT0;  //clear flag
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_IRS(void)
{
    P1IE &= ~BIT1;
    __delay_cycles(1000);
    P1IE |= BIT1;

    P2OUT |= BIT0; //sets P2.0
    if(TB0CCR1 < 1000){
        TB0CCR0 = 0; //reset CCR0
        TB0CCR1 += 100; //assign and increment CCR1 value
        TB0CCR0 = 1000; //assign CCR0 value
    }
    else if (TB0CCR1 >= 1000){
        TB0CCR0 = 0;  //reset CCR0
        TB0CCR1 = 0;  //reset CCR1
        TB0CCR0 = 1000;  //assign CCR0 value
    }
    P1IFG &= ~BIT1;
}
