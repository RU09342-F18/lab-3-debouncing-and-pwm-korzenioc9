// Author: Cameron Korzeniowski
// Date: 10/02/2018

#include <msp430.h> 
int Duty_Cycle = 500; //sets LED duty cycle to 50% to start
int debounce_state = 0;
void LEDSetup();
void ButtonSetup();
void TimerA0Setup();
void TimerA1Setup();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop Watchdog timer
    LEDSetup();
    ButtonSetup();
    TimerA0Setup();
    TimerA1Setup();
   __enable_interrupt();
    for(;;){
        if((TA0R <= Duty_Cycle) && (Duty_Cycle != 0)){
            P1OUT |= BIT6;  //LED P1.0 ON
        }
        else if(TA0R > Duty_Cycle){
            P1OUT &= ~BIT6; //LED P1.0 OFF
        }
        if(!(P1IN & BIT3)){ //button pressed
            P1OUT |= BIT0; //LED 1.6 ON
        }
        else {P1OUT &= ~BIT0;} //LED 1.6 OFF
    }
}
#pragma vector = PORT1_VECTOR
__interrupt void P1_ISR(void){
            switch(debounce_state)
                {
                case 0: //OFF -> GOING ON
                    TA1CTL = TASSEL_2 + MC_1 + TACLR; // SMCLK, continuous mode, clear timer
                    P1IE &= ~BIT3; //disable interrupts for P1.3
                    P1IFG &= ~BIT3; //Clear flag P1.3
                    break;
                case 1: //ON -> GOING OFF
                    TA1CTL = TASSEL_2 + MC_1 + TACLR; // SMCLK, continuous mode, clear timer
                    P1IE &= ~BIT3; //disable interrupts for P1.3
                    P1IFG &= ~BIT3;  //Clear flag p1.3
                    break;
                }
}
#pragma vector=TIMER1_A0_VECTOR //timer A0 interrupt routine
__interrupt void Timer1_ISR(void){
    switch(debounce_state)
    {
    case 0://turning on, on
        if(Duty_Cycle < 1000) //If Duty cycle is not at 100%
        {
            Duty_Cycle += 100; //adds 10% brightness
        }
        else
        {
            Duty_Cycle = 0; //if duty cycle is not less than 1000 (ie @ 100%), reset to 0 (off)
        }
        P1IE |= BIT3; //Enable Interrupts for P1.3
        P1IES &= ~BIT3; //low to high interrupt edge
        TA1CTL &= ~TASSEL_2 + TACLR; //Stop TimerA1, clear timer
        debounce_state = 1;//go to port 1 state 1
        break;
    case 1://turning off, off
        P1IE |= BIT3; //Enable Interrupts for P1.3
        P1IFG &= ~BIT3; //Clear interrupt flag P1.3
        P1IES |= BIT3;//high to low interrupt edge
        TA1CTL &= ~TASSEL_2 + TACLR; //Stop TimerA1, clear timer
        debounce_state = 0; //go to port 2 state 0
        break;
    }
}
void LEDSetup(){
    P1DIR |= BIT0; //Set LED P1.0 to output
    P1DIR |= BIT6; //Set LED P1.6 to output
    P1OUT &= ~BIT0;//turn LED P1.0 off
    P1OUT &= ~BIT6;//turn LED P1.6 off
}
void ButtonSetup(){
    P1DIR &= ~BIT3;//Set P1.3 to input
    P1REN |= BIT3; //Enable Pull Up Resistor for P1.3 
    P1OUT |= BIT3; //Active High P1.3
    P1IE |= BIT3;  //Enable Interrupt P1.3 
    P1IES |= BIT3; //P1.3 Interrupt positive edge trigger
    P1IFG &= ~BIT3; //Clear P1.3 Interrupt Flag
}
void TimerA0Setup(){
    TA0CCR0 = 1000;//Set period for Up mode - for PWM (1000 = 100%)
    TA0CTL |= TASSEL_2 + MC_1 + TACLR; //Use SMCLK, Up Mode, Clear Timer Registers - for PWM
}
void TimerA1Setup(){
    TA1CCTL0 = CCIE; //interrupt enabled for debouncing
    TA1CCR0 = 50000; //overflow time = 10ms for debouncing
}
