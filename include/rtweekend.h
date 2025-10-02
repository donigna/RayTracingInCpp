#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <random>
#include <iostream>
#include <limits>
#include <memory>

/**
 * @file rtweekend.h
 * @brief Common utilities and constants for ray tracing
 * 
 * This header file contains common utilities, constants, and type aliases
 * used throughout the ray tracing implementation. It serves as the main
 * include file that brings together all the fundamental components.
 */

// ============================================================================
// Type Aliases
// ============================================================================

using std::make_shared; ///< Convenience alias for std::make_shared
using std::shared_ptr;  ///< Convenience alias for std::shared_ptr

// ============================================================================
// Mathematical Constants
// ============================================================================

/**
 * @brief Positive infinity constant
 * 
 * Used for representing infinite distances and invalid intervals.
 */
const double infinity = std::numeric_limits<double>::infinity();

/**
 * @brief Pi constant for angle conversions
 * 
 * Used for converting between degrees and radians.
 */
const double pi = 3.1415926535897932385;

// ============================================================================
// Utility Functions
// ============================================================================

/**
 * @brief Convert degrees to radians
 * @param degrees Angle in degrees
 * @return Equivalent angle in radians
 * 
 * Converts an angle from degrees to radians using the formula:
 * radians = degrees * π / 180
 */
inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

/**
 * @brief Generate random double in range [0, 1)
 * @return Random double value
 * 
 * Uses a static Mersenne Twister generator with uniform distribution
 * for consistent random number generation across the application.
 */
inline double random_double()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

/**
 * @brief Generate random double in specified range
 * @param min Minimum value (inclusive)
 * @param max Maximum value (exclusive)
 * @return Random double value in [min, max)
 * 
 * Generates a random double value within the specified range using
 * linear interpolation of the base random_double() function.
 */
inline double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif