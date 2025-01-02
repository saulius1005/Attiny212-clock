/*
 * Cosmos.c
 *
 * Created: 2024-12-01 15:37:10
 *  Author: Saulius
 */ 

#include "Settings.h"
#include "CosmosVar.h"

/**
 * @brief Checks if daylight saving time (DST) is in effect for the given date.
 * 
 * @param year The year to check.
 * @param month The month to check.
 * @param day The day of the month to check.
 * @return True if DST is in effect, otherwise false.
 */
bool is_daylight_saving_time(int year, int month, int day) {
    int march_last_sunday = 31 - ((5 * year / 4 + 4) % 7);
    int october_last_sunday = 31 - ((5 * year / 4 + 1) % 7);

    // DST is in effect from March to October
    if (month > 3 && month < 10) {
        return true;
    }
    // DST starts on the last Sunday of March
    if (month == 3 && day >= march_last_sunday) {
        return true;
    }
    // DST ends on the last Sunday of October
    if (month == 10 && day < october_last_sunday) {
        return true;
    }
    return false;
}

/**
 * @brief Calculates the Julian Day Number (JDN) for a given date and time.
 * 
 * @param year The year.
 * @param month The month.
 * @param day The day of the month.
 * @param hour The hour of the day (24-hour format).
 * @param minute The minute of the hour.
 * @param second The second of the minute.
 * @param hundreds The hundreds of the second.
 * @return The Julian Day Number.
 */
double calculate_julian_day(int year, int month, int day, int hour, int minute, int second, int hundreds) {
    if (month <= 2) {
        year -= 1;
        month += 12;
    }
    int A = year / 100;
    int B = 2 - A + A / 4;

    // Julian Day calculation
    double JD = floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1)) +
    day + B - 1524.5 + (hour + minute / 60.0 + second / 3600.0 + hundreds / 36000) / 24;
    return JD;
}

/**
 * @brief Calculates the solar mean anomaly (M) for a given Julian century.
 * 
 * @param julian_century The Julian century (number of centuries since J2000).
 * @return The solar mean anomaly in degrees.
 */
double calculate_solar_mean_anomaly(double julian_century) {
    return fmod(357.52911 + julian_century * (35999.05029 - 0.0001537 * julian_century), 360.0);
}

/**
 * @brief Calculates the true anomaly of the Sun, based on the mean anomaly.
 * 
 * @param mean_anomaly The mean anomaly of the Sun.
 * @param julian_century The Julian century (number of centuries since J2000).
 * @return The true anomaly of the Sun in degrees.
 */
double calculate_true_anomaly(double mean_anomaly, double julian_century) {
    double C = (1.914602 - julian_century * (0.004817 + 0.000014 * julian_century)) * sin(DEG_TO_RAD * mean_anomaly) +
    (0.019993 - 0.000101 * julian_century) * sin(2 * DEG_TO_RAD * mean_anomaly) +
    0.000289 * sin(3 * DEG_TO_RAD * mean_anomaly);
    return mean_anomaly + C;
}

/**
 * @brief Calculates the apparent longitude of the Sun.
 * 
 * @param true_longitude The true longitude of the Sun.
 * @param julian_century The Julian century (number of centuries since J2000).
 * @return The apparent longitude of the Sun in degrees.
 */
double calculate_solar_apparent_longitude(double true_longitude, double julian_century) {
    double omega = 125.04 - 1934.136 * julian_century;
    return true_longitude - 0.00569 - 0.00478 * sin(DEG_TO_RAD * omega);
}

/**
 * @brief Calculates the obliquity of the ecliptic.
 * 
 * @param julian_century The Julian century (number of centuries since J2000).
 * @return The obliquity of the ecliptic in degrees.
 */
double calculate_obliquity_of_ecliptic(double julian_century) {
    return 23.439292 - julian_century * (0.013004167 + 0.0000001639 * julian_century - 0.0000005036 * julian_century * julian_century);
}

/**
 * @brief Calculates the solar declination based on the apparent longitude of the Sun.
 * 
 * @param apparent_longitude The apparent longitude of the Sun.
 * @param obliquity The obliquity of the ecliptic.
 * @return The solar declination in degrees.
 */
double calculate_solar_declination(double apparent_longitude, double obliquity) {
    return asin(sin(DEG_TO_RAD * obliquity) * sin(DEG_TO_RAD * apparent_longitude)) * RAD_TO_DEG;
}

/**
 * @brief Calculates the distance between the Earth and the Sun (in astronomical units).
 * 
 * @param julian_century The Julian century (number of centuries since J2000).
 * @return The distance between the Earth and the Sun in astronomical units (AU).
 */
double calculate_solar_distance(double julian_century) {
    double M = calculate_solar_mean_anomaly(julian_century);
    double M_rad = DEG_TO_RAD * M;
    double r = 1.00014 - 0.01671 * cos(M_rad) - 0.00014 * cos(2 * M_rad);

    return r;
}

/**
 * @brief Calculates the atmospheric refraction correction based on the Sun's elevation and distance.
 * 
 * @param elevation The solar elevation in degrees.
 * @param solar_distance The distance from the Earth to the Sun in astronomical units.
 * @return The atmospheric refraction correction in degrees.
 */
double calculate_atmospheric_refraction(double elevation, double solar_distance) {
    if (elevation > -1.0) {
        double tan_elevation = tan(DEG_TO_RAD * (elevation + 10.3 / (elevation + 5.11)));
        // Adjust the refraction based on solar distance
        double refrac_correction = 0.0167 / tan_elevation;
        double distance_correction = 1.0 + 0.001 * (solar_distance - 1.0); // Correction based on distance

        return refrac_correction * distance_correction;
    }
    return 0.0;
}

/**
 * @brief Calculates the solar position (elevation and azimuth) based on the given solar parameters.
 * 
 * @note This function uses several other functions to calculate the Julian Day, solar time, 
 * and solar declination, and applies atmospheric refraction corrections.
 */
void calculate_solar_position() {
    int timezone_offset = solar_params.timezone + (is_daylight_saving_time(solar_params.year, solar_params.month, solar_params.day) ? 1 : 0);

    // Calculate Julian Day
    double JD = calculate_julian_day(solar_params.year, solar_params.month, solar_params.day, solar_params.hour - timezone_offset, solar_params.minute, solar_params.second, solar_params.hundreds);
    double JC = (JD - 2451545.0) / 36525.0;

    double M = calculate_solar_mean_anomaly(JC);
    double true_anomaly = calculate_true_anomaly(M, JC);
    double true_longitude = fmod(280.46646 + JC * (36000.76983 + JC * 0.0003032), 360.0) + (true_anomaly - M);
    double apparent_longitude = calculate_solar_apparent_longitude(true_longitude, JC);
    double obliquity = calculate_obliquity_of_ecliptic(JC);
    double declination = calculate_solar_declination(apparent_longitude, obliquity);

    double eq_time = 4.0 * (280.46646 + 36000.76983 * JC - apparent_longitude +
    (2.466 * sin(2 * DEG_TO_RAD * (280.46646 + 36000.76983 * JC))) - 
    (0.053 * sin(4 * DEG_TO_RAD * (280.46646 + 36000.76983 * JC))));
    double solar_time = (solar_params.hour * 60 + solar_params.minute + (solar_params.second + solar_params.hundreds / 10.0) / 60.0 + eq_time + 4.0 * solar_params.longitude - 60.0 * timezone_offset) / 60.0;

    double hour_angle = (solar_time - 12.0) * 15.0;

    double latitude_rad = solar_params.latitude * DEG_TO_RAD;
    double declination_rad = declination * DEG_TO_RAD;
    double hour_angle_rad = hour_angle * DEG_TO_RAD;

    double sin_elevation = sin(latitude_rad) * sin(declination_rad) +
    cos(latitude_rad) * cos(declination_rad) * cos(hour_angle_rad);

    solar_params.elevation = asin(sin_elevation) * RAD_TO_DEG;

    // Calculate solar distance
    double solar_distance = calculate_solar_distance(JC);

    // Apply atmospheric refraction correction
    solar_params.elevation += calculate_atmospheric_refraction(solar_params.elevation, solar_distance);

    // Calculate azimuth angle (normalized to [0°, 360°])
    double sin_azimuth = (cos(declination_rad) * sin(hour_angle * DEG_TO_RAD)) / cos(solar_params.elevation * DEG_TO_RAD);
    double cos_azimuth = (sin(declination_rad) - sin(latitude_rad) * sin(solar_params.elevation * DEG_TO_RAD)) / (cos(latitude_rad) * cos(solar_params.elevation * DEG_TO_RAD));

    double azimuth_rad = atan2(sin_azimuth, cos_azimuth);

    // Convert azimuth to degrees and adjust based on the hour angle
    double azimuth_temp = azimuth_rad * RAD_TO_DEG;

    if (hour_angle > 0) {
        azimuth_temp = 360.0 - azimuth_temp; // West
    }

    azimuth_temp = fmod(azimuth_temp, 360.0);
    if (azimuth_temp < 0) {
        azimuth_temp += 360.0; // Ensure positive angle
    }
    
    solar_params.azimuth = azimuth_temp;
}



