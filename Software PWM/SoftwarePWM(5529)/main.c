//******************************************************************************
//   Software PWM 5529
//   Austin Huang
//   Built with CCSv4 and IAR Embedded Workbench Version: 4.21
//******************************************************************************
#include <msp430.h>
#include <Math.h>

volatile unsigned int j;
int taps = 10;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
                                            // to activate previously configured port settings
    P1DIR |= BIT0;                          // Set P1.0 to output direction
    P1OUT &= ~BIT0;                         // Switch LED off


    P4DIR |=BIT7; //set Port 9.4 output ---LED
    P4OUT &= ~BIT7; //Clear P9.4

    P2DIR  &= ~BIT1;                        // Set P1.1 as input
    P2OUT |= BIT1;                          // Configure P1.1 for Pull-Up
    P2REN |= BIT1;                          // Enable Pull Up of P1.1
    P2IE |= BIT1; //enable the interrupt on Port 1.1
    P2IES &= ~BIT1; //set as falling edge
    P2IFG &= ~(BIT1); //clear interrupt flag


    TA0CTL = TASSEL_2 + MC_1 ;       // SMCLK / Upmode
    TA0CCTL1 = (CCIE);
    TA0CCTL0 = (CCIE);
    TA0CCR0 = 1000-1;                        // PWM Frequency 1 kHz
    TA0CCR1 = 500;                           // 50% Duty Cycle

    __bis_SR_register(GIE);
    while(1)
    {
        if((P5IN & BIT6))
            P4OUT &= ~BIT7; //Clear P9.4

    }
}

//Timer A interrupt vectors
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR (void)
{
    if(TA0CCR1 != 1000)
    {
       P1OUT &= ~(BIT0); //turns off red led
    }
    TA0CCTL1 &= ~BIT0; //clears flag
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
    if(TA0CCR1 != 0){
        P1OUT |= (BIT0); //turns on red led
    }
    TA0CCTL0 &= ~BIT0;  //clears flag
}

#pragma vector=PORT2_VECTOR
__interrupt void PORT2_IRS(void)
{
    P2IE &= ~BIT1;
    __delay_cycles(1000);
    P2IE |= BIT1;

    P4OUT |= BIT7; //Sets P9.4
    if(TA0CCR1 < 1000)
    {
        TA0CCR0 = 0;
        TA0CCR1 += 100;
        TA0CCR0 = 1000;
    }
    else if (TA0CCR1 >= 1000){
        TA0CCR0 = 0;
        TA0CCR1 = 0;
        TA0CCR0 = 1000;
    }
    P2IFG &= ~BIT1;
}
