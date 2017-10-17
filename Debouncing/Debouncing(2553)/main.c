#include <msp430.h> 


/**
 * main.c
 * Austin Huang
 * Debouncing 2553
 */
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;// Stop watchdog

    P1SEL =0; //Select GPIO option
    P1DIR |=BIT0; //set Port 1.0 output LED

    P1DIR &=~(BIT3); //set Button
    P1REN|=BIT3;//enable pull-up/pull-down 
    P1OUT|=BIT3; //choose pull-up

    P1IE |=BIT3;//enable interrupt 
    P1IES |=BIT3;//set falling edge
    P1IFG &=~(BIT3);//clear flag

    _BIS_SR(LPM0_bits + GIE); //Low Power Mode 0
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    P1OUT ^=0x01; //Toggle
    P1IE &= ~BIT3; //disable interrupt
    
    //Debounce 1
    __delay_cycles(1);

    //Debounce 2
    TA0CTL = TASSEL_1 + MC_1 + ID_1; //Set up Timer A, Count up, divider value 2
    TA0CCTL0 = 0x10; // set up compare mode
    TA0CCR0 = 1000; // interrupt is disable; duration disabled = 1000/16kHz = 1/16 sec

    P1IFG &=~(BIT3); // Clear flag
}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{

    P1IE |= BIT3; //re-enable interrupt

}
