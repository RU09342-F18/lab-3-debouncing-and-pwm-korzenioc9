/*Author: Cameron Korzeniowski
* Date: 10/02/18
*/
#include <msp432.h>
void main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // Disable watchdog timer
    P2DIR |= BIT2;   // P2.2 to output direction
    P1DIR &= ~BIT1;  // P1.1 to input direction
    P1REN |= BIT1;   // Enable internal resistor
    P1OUT |= BIT1;  // Set resistor to pull up mode
    P1IES |= BIT1;    // falling edge interrupt
    P1IE |= BIT1;      // Enable interrupt for P1.1
    P1IFG &= ~BIT1;     // Clear P1.1 interrupt flags
    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);   // Interrupt enable P1
    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);   // Interrupt enable TimerA0
    TA0CCTL0 = CCIE;    // Capture Compare Interrupt Enable Timer A
    __enable_irq();   // Enable global interrupt
    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;  // Disable the sleep on exit feature
    __sleep();              // Sleep Mode
    
    while(1){};    // Infinite while loop
}
// Interrupt service routine for P1
void PORT1_IRQHandler(void){
    P2OUT ^= BIT2;    // Toggle LED
    P1IE &= ~BIT1;    // Disable interrupt on P1.1
    P1IFG &= ~BIT1;   // Clear interrupt flags for P1.1
    TA0CTL = TASSEL_2 + MC_1 + TACLR;       // Timer: SMCLK, Up mode, Clear timer
    TA0CCR0 = 10000;     // Set capture compare register value for timer count (~10ms)
}
// Interrupt service routine for Timer A
void TA0_0_IRQHandler(void){
    TA0CTL = MC_0 + TACLR;    // Timer: Stop, Clear timer
    P1IE |= BIT1;           // Re-enable interrupt for P1.1
    P1IFG &= ~BIT1;        // Clear interrupt flags on P1.1
}
