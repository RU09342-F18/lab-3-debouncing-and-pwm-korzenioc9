/*Author: Cameron Korzeniowski
 *Date: 10/02/18
 */

#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;      // stop watchdog timer
    P1DIR |= BIT0;        //set P1.0 to output
    P1DIR |= BIT6;             //set P1.6 to output
    P1SEL |= (BIT6);          //set port 1 for output
    P1OUT &= ~BIT0;  //initialize LED off
    P1DIR &= ~BIT3;         //set P1.3 direction to input
    P1REN |= BIT3;       //enable resistor on P1.3
    P1OUT |= BIT3;       //set resistor to pull up
    P1IE |= BIT3;      //enable interrupt on P1.3
    P1IES |= BIT3;        //low to high interrupt edge
    P1IFG &= ~BIT3;     //clear P1.3 interrupt flags
    TA0CTL = TASSEL_2 + MC_1 + ID_1;    //UP mode, divide by 2
    TA0CCR0 = 1000;    //set period
    TA0CCR1 = 100;       //set duty cycle
    TA0CCTL1 = OUTMOD_7;     //set timer to set/reset mode
    __enable_interrupt();
    while(1); //infinite while loop
}
#pragma vector=PORT1_VECTOR  
__interrupt void Port_1(void)
{
    if(P1IES & BIT3){
            if (TA0CCR1<1000) {       //if duty cycle is less than 100%
                TA0CCR1+=100;               //increment duty cycle by 10%
            } else if (TA0CCR1 >=1000) {    //if duty cycle is 100% or more
                TA0CCR1 = 0;                //reset duty cycle
            }
    }
           P1IES ^= BIT3;       //interrupt edge
           P1OUT ^= BIT0;       //toggle LED
           P1IFG &= ~BIT3;      //clear P1.3 interrupt flags
}
