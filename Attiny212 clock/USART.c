/*
 * USART.c
 *
 * Created: 2024-12-01 18:27:08
 *  Author: Saulius
 */ 
#include "Settings.h"

// Setup a stream for USART0 with a custom write function (USART0_printChar).
static FILE USART_stream = FDEV_SETUP_STREAM(USART0_printChar, NULL, _FDEV_SETUP_WRITE);

/**
 * @brief Initializes USART0 for serial communication at 115200 baud rate.
 * 
 * Configures USART0 in asynchronous mode, enabling receiver and transmitter,
 * and sets the baud rate to 115200. Also configures the stream for standard output.
 */
void USART0_init() {
    // USART0.CTRLA = USART_RS485_0_bm; // Enable RS485 driver control (optional, commented out)
    
    // Set the baud rate to 115200 bps
    USART0.BAUD = (uint16_t)USART0_BAUD_RATE(/*230400*/ 2500000);

    // Enable receiver and transmitter, configure for double speed mode
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm | USART_RXMODE_CLK2X_gc;

    // Configure USART0 for asynchronous communication, 8-bit data, no parity, and 1 stop bit
    USART0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc | USART_SBMODE_1BIT_gc;

    // Set the output stream to use USART0 for printing
    stdout = &USART_stream;
}

/**
 * @brief Sends a single character via USART0.
 * 
 * This function waits until the USART0 transmit data register is empty,
 * then sends the character to the transmit register.
 * 
 * @param c The character to send.
 */
void USART0_sendChar(char c) {
    // Wait until the transmit data register is empty
    while (!(USART0.STATUS & USART_DREIF_bm));
    
    // Send the character to the USART0 transmit data register
    USART0.TXDATAL = c;
}

/**
 * @brief Custom write function to output a character to USART0.
 * 
 * This function is used for printing characters to the standard output stream
 * using the USART0 communication.
 * 
 * @param c The character to print.
 * @param stream The stream (not used in this case).
 * @return int 0 to indicate successful output.
 */
int USART0_printChar(char c, FILE *stream) {
    // Send the character via USART0
    USART0_sendChar(c);
    return 0; // Return 0 to indicate success
}

/**
 * @brief Sends a string via USART0.
 * 
 * This function iterates through each character of the string and sends it
 * one by one using the USART0_sendChar function.
 * 
 * @param str The string to send.
 */
void USART0_sendString(char *str) {
    // Iterate through each character of the string and send it via USART0
    for (size_t i = 0; i < strlen(str); i++) {
        USART0_sendChar(str[i]);
    }
}

/**
 * @brief Reads a single character from USART0.
 * 
 * This function clears the receive interrupt flag before waiting for a character
 * to be received and then returns the received character.
 * 
 * @return char The received character.
 */
char USART0_readChar() {
    // Clear the receive interrupt flag before reading
    USART0.STATUS = USART_RXCIF_bm;

    // Wait until a character is received
    while (!(USART0.STATUS & USART_RXCIF_bm));

    // Return the received character
    return USART0.RXDATAL;
}


void USART0_printf(const char *format, ...) {
	char buffer[128]; // Laikinas buferis praneðimui
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args); // Formatuojame praneðimà á buferá
	va_end(args);
	USART0_sendString(buffer); // Naudojame USART0 siuntimo funkcijà
}