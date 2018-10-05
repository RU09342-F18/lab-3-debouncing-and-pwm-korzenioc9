/*Author: Cameron Korzeniowski
 *Date: 10/02/18
 */

#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;      // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;
    P1DIR |= BIT0;           // set P1.0 to output
    P2DIR |= BIT0;           // set P2.0 to output
    P2SEL0 |= (BIT0);      //set P2.0 as output
    P2SEL1 &= ~BIT0;
    P1DIR &= ~BIT1;      //set pin 1.3 direction to input
    P1REN |= BIT1;       //enable resistor on pin 1.3
    P1OUT |= BIT1;           //set resistor to pull up
    P1IE |= BIT1;          //enable P1.1 interrupt 
    P1IES |= BIT1;         //set interrupt edge to low
    P1IFG &= ~BIT1;        //clear P1.1 interrupt flags
    TB1CTL = TBSSEL_2 + MC_1 + ID_1;    // set Timer B1 to SMLCK, up mode, internal divider
    TB1CCR0 = 1000;         // set period
    TB1CCR1 = 500;        // set duty cycle to 50%
    TB1CCTL1 = OUTMOD_7;     // set timer to set/reset mode
    TB0CCTL0 = CCIE;        //enable CCR interrupt
    TB0CCR0 = 10000;          //set timer overflow for debounce
    TB0CTL = TBSSEL_2 + MC_0;    // Timer B0 to SMCLK, stop mode
    __bis_SR_register(GIE);     // LPM0 with interrupts enabled
    while(1); //infinite while loop
}

#pragma vector=PORT1_VECTOR  
__interrupt void Port_1(void)
{
    P1IE &= ~BIT1;       //disable P1.1 interrupt
    if(P1IES & BIT1){
            if (TB1CCR1<1000) {       //if duty cycle is less than 100%
                TB1CCR1+=100;       //increment duty cycle by 10%
            } else if (TB1CCR1 >=1000) {    //if duty cycle is 100% or more
                TB1CCR1 = 0;                //reset duty cycle
            }
    }
           P1IES ^= BIT1;    //toggle edge
           P1OUT ^= BIT0;    //toggle LED
           P1IFG &= ~BIT1;    //clear flags
           TB0CTL = TBSSEL_2 + MC_1;   // connect timer b0 to SMCLK in up mode
}

//debounce interrupt
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer_B (void) {
    TB0CTL = MC_0;          //stop timer
    TB0R = 0;               //reset timer
    P1IE |= BIT1;           //enable P1.1 interrupt
    P1IFG &= ~BIT1;        //clear P1.1 interrupt flags
}
