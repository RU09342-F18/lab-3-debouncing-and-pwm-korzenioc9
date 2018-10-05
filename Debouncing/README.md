# Software Debouncing
Mechanical switches are not perfect. Sometimes, they "bounce" or actuate more than one time per single press. This can be remedied by "Debouncing." For the purpose of this lab, we will use software to debounce a button that changes the state of an LED on an MSP430 and MSP432 board. Debouncing is accomplished by writing code that only allows the button to register one single input per press. We will use a timer to implement this on both boards.

## MSP430G2553
This code will trigger an interrupt to activate an LED on the press of a button. However, the interrupt will aso run additiona code which will disable the button's interrupt. This prevents the button from bouncing and reactuating the LED undesirably. Using an oscilloscope, we determined that button bouncing is largely over after a period of 10ms. So, our code makes the button "Wait" 10ms before it can register another input interrupt. This prevents bouncing.

## MSP432P401R
While this board uses some different syntax and port numbering, the implementation of debouncing is identical to that of the G2553. It is worth noting though that this board requires a different library than the 2553, <msp432.h>.
