/*
 * CosmosVar.h
 *
 * Created: 2024-12-01 15:37:52
 *  Author: Saulius
 */ 

#ifndef COSMOSVAR_H_
#define COSMOSVAR_H_

// Declare and initialize the solar position parameters for the specified location and time
volatile SolarPositionParameters solar_params = {
	.latitude = -70.0206,        /**< Latitude of the location (in degrees) */ //4 digits after . means: +-110m
	.longitude = 162.6651,       /**< Longitude of the location (in degrees) */
	.timezone = -11,				/**< Base timezone offset (adjust as needed for daylight savings or other time zones) */
	//.altitude = 85,             /**< Altitude of the location (in meters) */ //moved to AVR64DD32
	.year = 2024,                 /**< Year of the date */
	.month = 12,                  /**< Month of the date (12 = December) */
	.day = 21,                     /**< Day of the month (2nd day) */
	.hour = 15,                   /**< Hour of the day (15:00 = 3:00 PM) */
	.minute = 8,                 /**< Minute of the hour (7 minutes past the hour) */
	.second = 30,                 /**< Second of the minute (40th second) */
	.hundreds = 0,				  /**< Hundreds (0th hundreds) */
	
	// Pre-calculated solar elevation and azimuth for the given location and time
	.elevation = 37.3,            /**< Average annual elevation for the selected coordinates (in degrees) */
	
	// Azimuth is calculated from South (180°) with an offset towards the East
	.azimuth = 171.4              /**< Azimuth direction (180° = South, 171.4° is 8.6° East of South) */
};

#endif /* COSMOSVAR_H_ */
