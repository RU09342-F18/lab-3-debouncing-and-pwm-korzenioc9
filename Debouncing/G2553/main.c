//Author: Cameron Korzeniowski
//Date: 10/02/18

#include <msp430.h>

int debounce_state = 0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;// disable watchdog timer
    P1DIR &= ~BIT3;         //set P1.3 to input
    P1REN |= BIT3;          //Resistor enabled for P1.3
    P1OUT |= BIT3;          //set resistor to pull up for P1.3
    P1IE |= BIT3;           //Enable P1.3 Interrupt
    P1IFG &= ~BIT3;         //Clear P1.3 Interrupt flags
    P1IES |= BIT3;          //P1.3 Interrupt on Positive Edge
    P1DIR |= BIT6;          //Set LED P1.6 as output
    P1OUT &= ~BIT6;         //Set LED P1.6 to be off
    TA0CCTL0 = CCIE;        //interrupt enable
    TA0CCR0 = 50000;        //overflow time = 10ms
    __enable_interrupt();
}
#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
        switch(debounce_state)
            {
            case 0: //LED off, turning on
                TA0CTL = TASSEL_2 + MC_1 + TACLR;// SMCLK in continuous mode
                P1IE &= ~BIT3;  // disable interrupts for P1.3 button
                P1IFG &= ~BIT3; //clear interrupt flags P1.3
                break;
            case 1: //LED on, turning off
                TA0CTL = TASSEL_2 + MC_1 + TACLR;// SMCLK in continuous mode
                P1IE &= ~BIT3;// disable interrupts for P1.3 button
                P1IFG &= ~BIT3; //clear interrupt flags P1.3
                break;
            }
}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{
    switch(debounce_state)
    {
    case 0://Turning on, on
        P1OUT ^= BIT6;       //Flip state of LED P1.6
        P1IE |= BIT3;        //Enable P1.3 interrupt
        P1IES &= ~BIT3;      //Low to high interrupt for button release
        TA0CTL &= ~TASSEL_2; //Stop TimerA0
        TA0CTL |= TACLR;     //Clear TimerA0 control
        debounce_state = 1;  //go to state 1 of port 1
        break;
    case 1://turning off, off
        P1IE |= BIT3;        //P1.3 interrupt enabled
        P1IFG &= ~BIT3;      //clear interrupt flags on P1.3
        P1IES |= BIT3;       //P1.3 interrupt high to low
        TA0CTL &= ~TASSEL_2; //Stop TimerA0
        TA0CTL |= TACLR;     //Clear TimerA0
        debounce_state = 0;  //go to state 0 of port 1
        break;
    }
}
