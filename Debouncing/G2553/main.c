/* Author: Cameron Korzeniowski
 * Date: 10/02/18
 */

#include <msp430.h> 

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;    // stop watchdog timer
    P1DIR  |= BIT0;           // set P1.0 to output direction
    P1REN  |= BIT3;         // Enables P1.3 resistor
    P1OUT  = BIT3;          // P1.3 pull up resistor
    P1IE |= BIT3;
    P1IES = 0;        // Interrupts on button press - Low to high
    P1IFG &= ~BIT3;           // Clear P1.3 interrupt flag
    TA0CCTL0 = 0x0010;      // Enables Capture/compare register for timer A0 interrupt
    TA0CCR0 = 60000;        // value for Timer A0 to count to
    TA0CTL = TASSEL_2 + MC_0;   // Timer_A SMCLK clock, timer off

        __enable_interrupt();       //enables interrupts

        __bis_SR_register(LPM0 + GIE); // Low Power Mode w/ interrupts enabled

    }

    // Interrupt Service Routine 1
    #pragma vector = PORT1_VECTOR           //interrupt toggles the LED on release of button
    __interrupt void Interrupt_1(void)      //starts timer A0 acting as delay
    {
        P1OUT ^= BIT0;                 // Toggles LED
        P1IE &= ~BIT3;                 // clear interrupt
        TA0CTL = TASSEL_2 + MC_1;      // Timer_A SMCLK clock, Up mode
    }

    // Interrupt Service Routine 2: stops Timer A0, resets interrupt
    #pragma vector=TIMER0_A0_VECTOR
    __interrupt void Interrupt_2(void)
    {
        TA0CTL = MC_0;       // Stop timer
        P1IE |= BIT3;      // Interrupt enable for P1.3
        P1IFG &= ~BIT3;      // Clear interrupt flag
    }
