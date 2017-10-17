#Hardware PWM
##Author: Austin Huang   Co Op: Matthew Rodriguez, Seamus Plunkett
##Summary 
The program will cause an LED to change brightness increasing by 10% duty cycles by using a hardware approach.

##Notes
This program can be run on different MSP430 boards that use different pins, so the specific pin numbers will be replaced with the variable "X" (i.e. PXOUT ^= BITX;)
The first thing this program does is stop the watch dog timer to ensure that the processor will not be reset. This is done by the following statement:
```c
	WDTCTL = WDTPW + WDTHOLD or WDTCTL = WDTPW | WDTHOLD
```
The pins are initialized to determine which are inputs and outputs. 
Example:
```c
    PXDIR |= BITX;                          // Set P1.0 to output direction
    PXOUT &= ~BITX;                         // Switch LED off
    PXDIR  &= ~BITX;                        // Set P5.6 as input
    PXOUT |= BITX;                          // Configure P5.6 for Pull-Up
    PXREN |= BITX;                          // Enable Pull Up of P5.6
```
Timer is then initialized.
Example:
```c
    TA0CTL = TASSEL_2 + MC_1 +TACLR ;
    TA0CCTL1 = OUTMOD_7;
    TA0CCR0 = 1000;
    TA0CCR1 = 500;
```
The timer is hooked up to the LED.
Example:
```c
    PXSELX |= BITX;
    PXSELX &= ~BITX;
```

##How it works
When the timer reaches a specified value, an interupt will occur. A LED is toggled and set based on the 
current value of the timer. This will cause the LED to be on or off for a certain amount of time allowing for a PWM effect adjusting the brightness.

##Differences between boards
The pin assignments on each board can be different.
Certain boards with FR require high impedence on a pin to be disabled.
Certain boards use a different timer.
Certain boards are unable to use PXSEL and need the timer connected to the LED externally.

##Implementation
Use a C compiler to run the code and upload it to an MSP430. Press the button and the brightness of the LED changes.

