// Author: Cameron Korzeniowski
// Date: 10/02/18

#include <msp430.h>
int debounce_state = 0;
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;//disable watchdog timer
    PM5CTL0 &= ~LOCKLPM5;
    P1DIR &= ~BIT1;         //set P1.1 to input
    P1REN |= BIT1;          //Resistor enabled for P1.1
    P1OUT |= BIT1;          //set resistor to pull up for P1.1
    P1IE |= BIT1;           //Enable P1.1 Interrupt
    P1IFG &= ~BIT1;         //Clear P1.1 Button Interrupt flags
    P1IES |= BIT1;          //Button P1.1 Interrupt on Positive Edge
    P1DIR |= BIT0;          //Set LED P1.0 as output
    P1OUT &= ~BIT0;         //Set LED P1.0 to be off
    TB0CCTL0 = CCIE;        //interrupt enabled
    TB0CCR0 = 50000;        //overflow period = 10ms
    __enable_interrupt();
}
#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    switch(P1IV)
    {
    case P1IV_P1IFG1:
        {
        switch(debounce_state)
            {
            case 0: //LED off, turning on
                TB0CTL = TBSSEL_2 + MC_1 + TBCLR;// SMCLK (1mhz) in continuous mode
                P1IE &= ~BIT1;                   // disable interrupts for P1.1
                break;
            case 1: //LED on, turning off
                TB0CTL = TBSSEL_2 + MC_1 + TBCLR;// SMCLK (1mhz) in continuous mode
                P1IE &= ~BIT1;                   // disable interrupts for P1.1
                break;
            }
        break;
        }
    }
}
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B0 (void)
{
    switch(debounce_state)
    {
    case 0://turning on, on
        P1OUT ^= BIT0;              //flip state of LED P1.1
        P1IE |= BIT1;               //enable P1.1 interrupts
        P1IES &= ~BIT1;              //low to high interrupt edge
        TB0CTL &= ~TBSSEL_2;        //Stop TimerA0
        TB0CTL |= TBCLR;            //Clear TimerA0
        debounce_state = 1;    //go to state 1 in port 1
        break;
    case 1://turning off, off
        P1IE |= BIT1;               //enable P1.1 interrupts
        P1IFG &= ~BIT1;             //P1.1 interrupt flags clear
        P1IES |= BIT1;              //high to low interrupt edge
        TB0CTL &= ~TBSSEL_2;        //Stop TimerA0
        TB0CTL |= TBCLR;            //Clear TimerA0
        debounce_state = 0;        //go to state 0 in port 1
        break;
    }
}
