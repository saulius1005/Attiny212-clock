/*
 * Attiny212 clock.c
 *
 * Created: 2024-12-01 15:35:55
 * Author : Saulius
 */ 
#include "Settings.h"

/**
 * @brief Main function that initializes all necessary peripherals and enters an infinite loop.
 * 
 * This function initializes the GPIO, USART0, and RTC peripherals. It also enables
 * global interrupts and enters an infinite loop, where the system will run until 
 * power-off or reset. The actual processing occurs in interrupts or other peripheral functions.
 */
int main(void)
{
    // Initialize GPIO for general-purpose I/O operations
    GPIO_init();
    
    // Initialize USART0 for serial communication
    USART0_init();
    
    // Initialize the RTC (Real-Time Clock) for timekeeping
    RTC_init();
    
    // Enable global interrupts to allow interrupt-driven operations
    sei();
    
    // Enter an infinite loop (the actual work is done in interrupts)
    while (1) 
    {
		while(!(PORTB.IN & PIN1_bm)){ // if time is changing from outside
			ClockAndDataSet();
		}
        // Main loop does nothing here as interrupt service routines handle the tasks
        // The RTC interrupt will update time and calculate solar position periodically
    }
}


