/*
 * Settings.h
 *
 * Created: 2024-12-01 15:38:31
 *  Author: Saulius
 */ 


#ifndef SETTINGS_H_
#define SETTINGS_H_

// Define the CPU clock frequency (20 MHz in this case)
#define F_CPU 20000000

// Calculate the baud rate for USART0 based on the given value
// Formula for baud rate: (F_CPU * 64) / (8 * Baud Rate)
// Adding 0.5 for rounding
#define USART0_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (8 *(float)BAUD_RATE)) + 0.5)

#include <avr/io.h>      // Include AVR I/O library for register definitions and hardware control
#include <avr/interrupt.h> // Include AVR interrupt library for ISR (Interrupt Service Routine) support
#include <stdio.h>       // Include standard I/O library for functions like printf
#include <string.h>      // Include string library for handling string functions like strlen
#include <stdlib.h> 
#include <math.h>        // Include math library for mathematical functions (e.g., sin, cos)
#include <float.h>       // Include float.h for floating point constants like FLT_MAX
#include <stdbool.h>     // Include stdbool.h for boolean type support (true/false)
#include "Cosmos.h"      // Include Cosmos.h (This is for solar calculation and related functions)

////////////////////////////////////////////////////////////////////////////////
// Function Prototypes
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Initializes the General Purpose I/O (GPIO) pins for necessary tasks.
 * 
 * This function configures the necessary GPIO pins for communication, control, 
 * or other I/O purposes. The exact configuration will depend on the application.
 */
void GPIO_init();

/**
 * @brief Initializes the Real-Time Clock (RTC).
 * 
 * This function configures and starts the RTC, setting up timekeeping operations.
 */
void RTC_init();

/**
 * @brief Initializes USART0 for serial communication.
 * 
 * This function sets up USART0 for communication with a specified baud rate, 
 * data format, and enables the transmitter and receiver.
 */
void USART0_init();

/**
 * @brief Sends a single character via USART0.
 * 
 * This function waits for the USART transmit data register to be empty and 
 * then sends the specified character to the serial port.
 * 
 * @param c The character to send.
 */
void USART0_sendChar(char c);

/**
 * @brief Custom output function for printf-style printing via USART0.
 * 
 * This function allows `printf` to output characters via USART0 by using the 
 * USART0_sendChar function.
 * 
 * @param c The character to print.
 * @param stream The stream to print to (not used in this case).
 * @return int 0 to indicate the character was successfully sent.
 */
int USART0_printChar(char c, FILE *stream);

/**
 * @brief Sends a string via USART0.
 * 
 * This function sends a string by calling the `USART0_sendChar` function for 
 * each character in the string until the entire string has been transmitted.
 * 
 * @param str The string to send.
 */
void USART0_sendString(char *str);

/**
 * @brief Reads a single character from USART0.
 * 
 * This function waits for a character to be received via USART0 and returns 
 * the received character after clearing the receive interrupt flag.
 * 
 * @return char The received character.
 */
char USART0_readChar();

void USART0_printf(const char *format, ...);

/**
 * @brief Calculates the solar position based on the current date, time, and location.
 * 
 * This function computes the solar position, including parameters such as 
 * solar elevation and azimuth based on the current date, time, and geographical 
 * coordinates. This calculation is often used for solar tracking or related applications.
 */
void calculate_solar_position();

/**
 * @brief Reads and processes a command from USART0, updating solar parameters.
 * 
 * This function listens for a command from the USART0 port, starting to read
 * when the '<' character is encountered, and stops reading when the '>' character
 * is detected. It stores the incoming characters in a buffer and passes the command
 * to `executeCommand()` for further processing once the full command has been received.
 * 
 * @note This function uses a simple state machine to capture the command string.
 *       The command string is expected to be enclosed by '<' and '>' characters.
 * 
 * @see executeCommand() for processing the command after it's fully received.
 */
void ClockAndDataSet();

#endif /* SETTINGS_H_ */
