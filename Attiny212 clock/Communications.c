/*
 * Communications.c
 *
 * Created: 2024-12-14 20:06:42
 *  Author: Saulius
 */ 

#include "Settings.h"

/**
 * @brief Executes a command by parsing it and updating global solar parameters.
 * 
 * This function processes a given command string, which is expected to contain
 * solar-related parameters in a specific format. It uses `strtok` to extract
 * tokens from the command string and then updates the global `solar_params` 
 * structure with the extracted values.
 * 
 * @param command A string containing the command to be executed. The command
 *                should be formatted with pipe ('|') characters separating
 *                the different parameters (e.g., "YYYYMMDDHHMMSSX|TZ|LAT|LON").
 */
void executeCommand(char *command)
{
	// Using strtok to extract tokens
		// Split the first token into variables
		int year = 0,
			month = 0,
			day = 0,
			hour = 0,
			minute = 0,
			second = 0,
			hundreds = 0;
		char *token = strtok(command, "|");
		// Get the first token
		sscanf(token, "%4u%2u%2u%2u%2u%2u%1u", &year, &month, &day, &hour, &minute, &second, &hundreds);
		solar_params.year = year;
		solar_params.month = month;
		solar_params.day = day;
		solar_params.hour = hour;
		solar_params.minute = minute;
		solar_params.second = second,
		solar_params.hundreds = hundreds;
		// Get the second token
		token = strtok(NULL, "|");
		if (token != NULL) {
			solar_params.timezone = atoi(token); // Convert to int
		}
		// Get the third token
		token = strtok(NULL, "|");
		if (token != NULL) {
			solar_params.latitude = atof(token); // Convert to double
		}
		// Get the fourth token
		token = strtok(NULL, "|");
		if (token != NULL) {
			solar_params.longitude = atof(token); // Convert to double
		}
}

/**
 * @brief Reads and processes a command from USART0, updating solar parameters.
 * 
 * This function listens for a command from the USART0 port, starting to read
 * when the '<' character is encountered, and stops reading when the '>' character
 * is detected. It stores the incoming characters in a buffer and passes the command
 * to `executeCommand()` for further processing once the full command has been received.
 */
void ClockAndDataSet(){
	uint8_t  index = 0; // Module type: 1 for BLE receiver, 0 for standard RS485
	char command[40] = {0}; // Actually ~35
	uint8_t start = 0;
	while(1){
		char c = USART0_readChar();

		if(start == 1){
			if(c == '>'){
				start = 0;
				executeCommand(command);
				index = 0;
				break;
			}
			else
				command[index++] = c;
		}
		if(c == '<')
			start = 1;
	}
}
