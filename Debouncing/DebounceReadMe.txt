# Debounce
##Author: Austin Huang  Co op: Matthew Rodriguez, Seamus Plunkett
##Summary
Buttons tend to have a spring constant and when pressed can cause the buttons signal to jump from high to low oscillating in a short amount of time. 
This code minimizes the effect of the oscillation by disableing the buttons ability to cause an interupt for a very short amount of time.

##Notes
The first thing this program does is stop the watch dog timerso that the processor is not reset. This is done by the statement:
```c
	WDTCTL = WDTPW + WDTHOLD or WDTCTL = WDTPW | WDTHOLD
```

Pins are initialized to determine if its an input or output and configures interupts on the pin.
```c
	P1DIR |=BIT0; //P1.0 output
    P1DIR &= ~BIT1; //set P1.1 input
    P1REN |= BIT1; //enable resistor
    P1OUT |= BIT1; //the pull-up resistor
    P1IE |= BIT1;//enable the interrupt P1.1
    P1IES |= BIT1;//set as falling edge
    P1IFG &= ~BIT1;//clear interrupt flag
```
Low power mode enabled
```c
	_BIS_SR(LPM4_bits + GIE);
```
##How it works
An interrupt occurs when the mapped button is pressed. When it is pressed it causes that interrupt to occur and executes the block of code.
This code configures a timer that causes an interrupt after 2000-1 or 1999 clock cycles divided by 2 which disables it.
When the interrupt for the timer is called the button interrupt is enabled allowing it to recive an input from the user again.

##Differences between boards
Some boards with FR require high impedence on a pin to be disabled to properly compile or work.

##Implementing
Run the code using a C compiler and put into an MSP430. Press the button and the input will be debounced. If you want to use this
on your own code you may paste the button's functionality inside of the buttons interrupt under the existing code.
