# Software PWM
In this section of the lab, we increment the brightness of an LED in steps of 10% by adjusting the duty cycle via a software loop. An if statement detects the current duty cycle of the LED, and if it is not at 100% another 10% cuty cycle is added. However, if the duty cycle is detected to be at 100%, it is reset to 0. 

## Implementation
On both the G2553 and the F5529 the code implementation is virtually identical with the exception of port numbers. Timer A0 is used to set the 100% limit of the duty cycle, while timer A1 is used to debounce the button. 
