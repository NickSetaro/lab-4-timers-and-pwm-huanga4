#include <msp430.h> 


/**
 * main.c
 * Austin Huang
 * HardwarePWM(Log based for linear appearance on eyes)
 */
#include <msp430.h>
#include <Math.h>

volatile unsigned int j;
int reps = 10;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    P1DIR |= BIT0;                          // Set P1.0 to output direction
    P1OUT &= ~BIT0;                         // Switch LED off


    P1DIR |=BIT1; //set Port 9.4 output ---LED
    P1OUT &= ~BIT1; //Clear P9.4

    P5DIR  &= ~BIT6;                        // Set P1.1 as input
    P5OUT |= BIT6;                          // Configure P1.1 for Pull-Up
    P5REN |= BIT6;                          // Enable Pull Up of P1.1

    TA0CCTL1 = OUTMOD_7;                    // Reset/Set Mode
    TA0CTL = TASSEL_2 + MC_1 +TACLR ;       // SMCLK / Up mode
    TA0CCR0 = 100-1;                        // PWM Frequency 10 kHz
    TA0CCR1 = 50;                           // 50% Duty Cycle
    P1SEL0 |= BIT0;                         // PWM output to LED P1.0
    P1SEL1 &= ~BIT0;

    while(1)
    {
        if(!(P5IN & BIT6))
        {
            P1OUT |= BIT1; //Sets bit
            if(reps > 0)
            {
                reps--;
                double logNum  = log10(reps) * 100.0;  //Equation to create increments at correct rate for log scale.
                int incrementNum = (100 - logNum);     //Difference between this and normal HardwarePWM
                TA0CCR0 = 0;
                TA0CCR1 = incrementNum;
                TA0CCR0 = 100;
            }
            else if (reps == 0){
                TA0CCR0 = 0;
                TA0CCR1 = 0;
                TA0CCR0 = 100;
                reps = 10;
            }
        }
        if((P5IN & BIT6))
            P1OUT &= ~BIT1; //Clear P9.4
        for(j=100;j>0;j--)
        {
        __delay_cycles(1000);
        }
    }
}
