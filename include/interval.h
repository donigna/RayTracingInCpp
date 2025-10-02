#ifndef INTERVAL_H
#define INTERVAL_H

#include "rtweekend.h"

/**
 * @file interval.h
 * @brief Mathematical interval class for ray-object intersection
 * 
 * This file defines the interval class, which represents a closed
 * interval [min, max] on the real number line. It's primarily used
 * for ray-object intersection testing to determine valid intersection
 * ranges along rays.
 */

/**
 * @class interval
 * @brief Represents a closed interval [min, max] on the real number line
 * 
 * The interval class provides utilities for working with ranges of
 * real numbers, particularly useful for ray-object intersection
 * calculations where we need to determine if intersection points
 * lie within valid ranges.
 */
class interval
{
public:
    double min; ///< Lower bound of the interval
    double max; ///< Upper bound of the interval

    /**
     * @brief Default constructor - creates empty interval
     * 
     * Creates an empty interval with min = +infinity and max = -infinity.
     * This represents an invalid interval that contains no points.
     */
    interval() : min(+infinity), max(-infinity) {}

    /**
     * @brief Constructor with explicit bounds
     * @param min Lower bound of the interval
     * @param max Upper bound of the interval
     */
    interval(double min, double max) : min(min), max(max) {}

    /**
     * @brief Calculate the size (length) of the interval
     * @return Size of the interval (max - min)
     */
    double size() const
    {
        return max - min;
    }

    /**
     * @brief Check if a value is contained in the interval (inclusive)
     * @param x Value to check
     * @return True if min <= x <= max
     */
    bool contains(double x) const
    {
        return min <= x <= max;
    }

    /**
     * @brief Check if a value is surrounded by the interval (exclusive)
     * @param x Value to check
     * @return True if min < x < max
     */
    bool surrounds(double x) const
    {
        return min < x && x < max;
    }

    /**
     * @brief Clamp a value to the interval bounds
     * @param x Value to clamp
     * @return Clamped value (min if x < min, max if x > max, x otherwise)
     */
    double clamp(double x) const
    {
        if (x < min)
            return min;
        if (x > max)
            return max;
        return x;
    }

    /**
     * @brief Static empty interval (contains no points)
     */
    static const interval empty;
    
    /**
     * @brief Static universe interval (contains all points)
     */
    static const interval universe;
};

/**
 * @brief Empty interval constant - contains no points
 * 
 * An empty interval has min = +infinity and max = -infinity,
 * making it impossible for any point to be contained within it.
 */
const interval interval::empty = interval(+infinity, -infinity);

/**
 * @brief Universe interval constant - contains all points
 * 
 * The universe interval spans from -infinity to +infinity,
 * containing all possible real numbers.
 */
const interval interval::universe = interval(-infinity, +infinity);

#endif