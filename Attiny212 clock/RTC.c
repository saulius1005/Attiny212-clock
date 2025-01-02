/*
 * RTC.c
 *
 * Created: 2024-12-01 17:40:56
 *  Author: Saulius
 */ 
#include "Settings.h"

// Number of days in each month (Non-leap year)
const uint8_t daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/**
 * @brief Checks whether a given year is a leap year.
 * 
 * @param year The year to check.
 * @return uint8_t Returns 1 if the year is a leap year, 0 otherwise.
 */
uint8_t isLeapYear(uint16_t year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

/**
 * @brief Initializes the RTC (Real-Time Clock) with external clock and specified prescaler.
 */
void RTC_init() {
    while (RTC.STATUS > 0); // Wait until RTC synchronizes
    RTC.CLKSEL = RTC_CLKSEL_EXTCLK_gc; // Select external clock
    RTC.CTRLA = RTC_RTCEN_bm | RTC_PRESCALER_DIV32_gc; // Enable RTC and set prescaler to 32
    RTC.INTCTRL = 0 << RTC_CMP_bp | 1 << RTC_OVF_bp; // Enable overflow interrupt
    RTC.PER = 31250/*62466*/; // Set RTC period for 1-second overflow (20Mhz / 32 prescaler = 62500Hz, period = 625000/10 = 0.1sec)

}

/**
 * @brief Interrupt handler for RTC overflow. Updates the time and calculates solar position.
 */
ISR(RTC_CNT_vect) {
    RTC.INTFLAGS = RTC_OVF_bm; // Clear the overflow interrupt flag
    
	if(PORTB.IN & PIN1_bm){ //If time is not changat from outside
		
		//PORTA.OUTSET = PIN5_bm;// toggling PIN5 otherwise to see led illumination will be not possible
		PORTA.OUTTGL = PIN5_bm;// toggling PIN5 otherwise to see led illumination will be not possible
		    
		// Calculate solar position (can be customized to update your solar data)
		calculate_solar_position();
		printf("<%4d%02d%02d%02d%02d%02d%1d|%3.4f|%3.4f|%3.4f|%3.4f|%2d>\r\n",
			   solar_params.year, 
			   solar_params.month, 
			   solar_params.day, 
			   solar_params.hour, 
			   solar_params.minute, 
			   solar_params.second, 
			   solar_params.hundreds, 
			   solar_params.azimuth, 
			   solar_params.elevation,
			   solar_params.latitude, //55.19419 * 10 = 551.9419
			   solar_params.longitude,
			   solar_params.timezone/*,
			   solar_params.altitude*/ //moved to AVR64DD32
			   );
    
		// Increment milliseconds and handle time overflow
		solar_params.hundreds++;
		if (solar_params.hundreds >= 20) {
			solar_params.hundreds = 0;
			solar_params.second++;
        
			// Handle second overflow
			if (solar_params.second >= 60) {
				solar_params.second = 0;
				solar_params.minute++;
            
				// Handle minute overflow
				if (solar_params.minute >= 60) {
					solar_params.minute = 0;
					solar_params.hour++;
                
					// Handle hour overflow
					if (solar_params.hour >= 24) {
						solar_params.hour = 0;
						solar_params.day++;
                    
						// Handle day overflow
						uint8_t daysThisMonth = daysInMonth[solar_params.month - 1];
                    
						// Account for leap year in February
						if (solar_params.month == 2 && isLeapYear(solar_params.year)) {
							daysThisMonth = 29;
						}
                    
						// If the day exceeds the number of days in the month, reset the day and increment the month
						if (solar_params.day > daysThisMonth) {
							solar_params.day = 1;
							solar_params.month++;
                        
							// If the month exceeds 12, reset it to January and increment the year
							if (solar_params.month > 12) {
								solar_params.month = 1;
								solar_params.year++;
							}
						}
					}
				}
			}
		}
		//PORTA.OUTCLR = PIN5_bm;// toggling PIN5 otherwise to see led illumination will be not possible
	}
/*	else{
		while(!(PORTB.IN & PIN1_bm)){ // if time is changing from outside
			ClockAndDataSet(); //set new time
		}
	}*/
}
