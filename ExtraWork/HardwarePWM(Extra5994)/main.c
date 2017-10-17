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
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
    P1DIR |= BIT0;                          // set P1.0
    P1OUT &= ~BIT0;                         // clear P1.0


    P1DIR |=BIT1; //set P1.1 LED
    P1OUT &= ~BIT1; //clear 1.1

    P5DIR  &= ~BIT6;                        // set P5.6
    P5OUT |= BIT6;                          // configure pull-up
    P5REN |= BIT6;                          // enable pull-up

    TA0CCTL1 = OUTMOD_7;                    // reset/set
    TA0CTL = TASSEL_2 + MC_1 +TACLR ;       // TimerA0 set up, Up mode, SMCLK
    TA0CCR0 = 100-1;                        
    TA0CCR1 = 50;                           // 50% Duty Cycle
    P1SEL0 |= BIT0;                         // PWM outputs P1.0
    P1SEL1 &= ~BIT0;

    while(1)
    {
        if(!(P5IN & BIT6)){
            P1OUT |= BIT1; //set P1.1
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
            P1OUT &= ~BIT1; //clear 1.1
        for(j=100;j>0;j--)
        {
        __delay_cycles(1000);
        }
    }
}
