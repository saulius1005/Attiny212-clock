/*
 * GPIO.c
 *
 * Created: 2024-12-01 15:39:07
 *  Author: Saulius
 */ 
#include "Settings.h"

/**
 * @brief Initializes the GPIO pins for the microcontroller, configuring
 *        them for proper operation with external clock and RS485 communication.
 */
void GPIO_init() {

    // Set CCP register to allow configuration of IO registers
    CPU_CCP = CCP_IOREG_gc;

    // Select the external clock as the main clock source
    CLKCTRL.MCLKCTRLA = CLKCTRL_CLKSEL_EXTCLK_gc;
    
    // Wait until the external oscillator is stable
    while (CLKCTRL.MCLKSTATUS & CLKCTRL_SOSC_bm);

    // Set CCP register to allow configuration of IO registers again
    CPU_CCP = CCP_IOREG_gc;
    
    // Disable the external oscillator bypass (enable internal oscillator)
    CLKCTRL.MCLKCTRLB = 0 << CLKCTRL_PEN_bp;

    // Wait until the external oscillator is stable after the change
    while (CLKCTRL.MCLKSTATUS & CLKCTRL_SOSC_bm);

    // Set PORTA pin 5 as an output for USART TX LED
    PORTA.DIRSET = PIN5_bm;

    // Set PORTB pin 2 as output (USART0: TX)
    PORTB.DIRSET =  PIN2_bm;

    // Set PORTB pin 3 as input (Clock set, USART0: RX)
    PORTB.DIRCLR = PIN1_bm | PIN3_bm;

    // Enable pull-up resistor for TX (PORTB pin 2)
    PORTB.PIN2CTRL = PORT_PULLUPEN_bm;

    // Enable pull-up resistor for RX (PORTB pin 3)
    PORTB.PIN3CTRL = PORT_PULLUPEN_bm;
}