/*  MSP430FR5529
    Author: Cameron Korzeniowski
    Date: 10/02/18
    */

#include <msp430.h>

int Duty_Cycle = 500;
int state = 0;
void LEDSetup();
void ButtonSetup();
void TimerA0Setup();
void TimerA1Setup();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop Watchdog timer
    LEDSetup(); // Initialize our LEDS
    ButtonSetup();  // Initialize our button
    TimerA0Setup(); // Initialize Timer0
    TimerA1Setup(); // Initialize Timer1

   __enable_interrupt();

    for(;;)
    {
        if((TA0R <= Duty_Cycle) && (Duty_Cycle != 0))
        {
            P1OUT |= BIT0;
        }
        else if(TA0R > Duty_Cycle)
        {
            P1OUT &= ~BIT0;
        }
        if(!(P2IN & BIT1))
        {
            P4OUT |= BIT7;
        }
        else {P4OUT &= ~BIT7;}
    }
}
#pragma vector = PORT2_VECTOR
__interrupt void P2_ISR(void)
{
    switch(P2IV)
    {
    case P2IV_P2IFG1:
        {
            switch(state)
                {
                case 0: //going from off to on
                    TA1CTL = TASSEL_2 + MC_1 + TACLR;;
                    P2IE &= ~BIT1;
                    break;
                case 1: //going from on to off
                    TA1CTL = TASSEL_2 + MC_1 + TACLR;;
                    P2IE &= ~BIT1;
                    break;
                }
        }
    }
}
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_ISR(void)
{
    switch(state)
    {
    case 0://LED going from on to on
        if(Duty_Cycle < 1000) //PWM Incrementing conditions
        {
            Duty_Cycle += 100;
        }
        else
        {
            Duty_Cycle = 0;
        }
        P2IE |= BIT1;
        P2IES &= ~BIT1;;
        TA1CTL &= ~TASSEL_2 + TACLR;
        state = 1;
        break;
    case 1://LED going from off to off
        P2IE |= BIT1;
        P2IFG &= ~BIT1; //clear flag P1.1 button interrupt
        P2IES |= BIT1;
        TA1CTL &= ~TASSEL_2 + TACLR;
        state = 0; //go to case 0 when button is pressed
        break;
    }
}
void LEDSetup()
{
    P1DIR |= BIT0;  //sets P1.0 LED to output
    P1OUT &= ~BIT0; //turns P1.0 LED off
    P4DIR |= BIT7;  //sets P4.7 LED to output
    P4DIR |= BIT7;  //turns P4.7 LED off
}
void ButtonSetup()
{
    P2DIR &= ~BIT1;  //set P2.1 button to input
    P2REN |= BIT1;  //enable pull up resistor for P2.1
    P2OUT |= BIT1;  //active high P2.1 button
    P2IE |= BIT1;   //P2.1 button interrupt enabled
    P2IES |= BIT1;  //P2.1 interrupt triggers on positive edge
    P2IFG &= ~BIT1; //P2.1 button interrupt flag cleared in case of immediate interrupt
}
void TimerA0Setup()
{
    TA0CCR0 = 1000; //set up mode limit
    TA0CTL |= TASSEL_2 + MC_1 + TACLR; //Use SMCLK , enable up mode, clear timer registers
}
void TimerA1Setup()
{
    TA1CCTL0 = CCIE;        //interrupt enabled
    TA1CCR0 = 50000;        //overflow time =  10ms
}
