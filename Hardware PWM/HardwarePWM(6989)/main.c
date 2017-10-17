#include <msp430.h> 


/**
 * main.c
 * Austin Huang
 * Hardware PWM 6989
 */
#include <msp430.h>
#include <Math.h>

volatile unsigned int f;
int press = 10;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
    P1DIR |= BIT0;                          // set P1.0 
    P1OUT &= ~BIT0;                         // clear P1.0


    P9DIR |=BIT7; //set Port 9.7 LED
    P9OUT &= ~BIT7; //clear P9.7

    P1DIR  &= ~BIT1;                        // set P1.1 
    P1OUT |= BIT1;                          // configure pull-up
    P1REN |= BIT1;                          // enable pull up 

    TA0CCTL1 = OUTMOD_7;                    // reset/set
    TA0CTL = TASSEL_2 + MC_1 +TACLR ;       // TimerA set up, Up mode, SMCLK
    TA0CCR0 = 100-1;                       
    TA0CCR1 = 50;                           // 50% Duty Cycle
    P1SEL0 |= BIT0;                         
    P1SEL1 &= ~BIT0;

    while(1)
    {
        if(!(P1IN & BIT1)){
            P9OUT |= BIT7; //Sets P9.7
            if(press > 0){
                press--;
                double logNum  = log10(press) * 100.0;
                int incrementNum = (100 - logNum);
                TA0CCR0 = 0;
                TA0CCR1 = incrementNum;
                TA0CCR0 = 100;
            }
            else if (press == 0){
                TA0CCR0 = 0;
                TA0CCR1 = 0;
                TA0CCR0 = 100;
                press = 10;
            }
        }
        if((P1IN & BIT1)){
            P9OUT &= ~BIT7; //Clear P9.7
        }
        for(j=100;j>0;j--)
        {
        __delay_cycles(500);
        }
    }
}
