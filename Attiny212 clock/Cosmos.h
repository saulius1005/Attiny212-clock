/*
 * Cosmos.h
 *
 * Created: 2024-12-01 15:37:27
 *  Author: Saulius
 */ 

#ifndef COSMOS_H_
#define COSMOS_H_

// Constants for converting between degrees and radians
#define DEG_TO_RAD 0.01745329251994329576923690768489 // pi / 180
#define RAD_TO_DEG 57.295779513082320876798154814105 // 180 / pi
#define FIXED_SHIFT 32

////////////////////////////////////////////////////////////////////////////////
// Solar Position Parameters Structure
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Structure to hold the solar position parameters and datetime information.
 * 
 * This structure stores all necessary parameters to calculate the solar position, 
 * such as geographical location (latitude, longitude, altitude) and the date/time 
 * (year, month, day, hour, minute, second, milliseconds). It also includes the 
 * calculated solar position (elevation and azimuth).
 */
typedef struct {
    double latitude;       /**< Latitude of the location (in degrees) */
    double longitude;      /**< Longitude of the location (in degrees) */
	int8_t timezone;		/**< Base timezone offset (adjust as needed for daylight savings or other time zones) */
  //  int16_t altitude;       /**< Altitude of the location (in meters) */ //moved to AVR64DD32 
    uint16_t year;        /**< Year of the date */
    uint8_t month;        /**< Month of the date (1-12) */
    uint8_t day;          /**< Day of the month (1-31) */
    uint8_t hour;         /**< Hour of the day (0-23) */
    uint8_t minute;       /**< Minute of the hour (0-59) */
    uint8_t second;       /**< Second of the minute (0-59) */
    uint8_t hundreds;	  /**< Hundreds of the second (0-9) */
    double elevation;      /**< Solar elevation angle (in degrees) */
    double azimuth;        /**< Solar azimuth angle (in degrees) */
} SolarPositionParameters;

// Declare the global solar position parameters object, which will hold the current solar position data
extern volatile SolarPositionParameters solar_params;

#endif /* COSMOS_H_ */
