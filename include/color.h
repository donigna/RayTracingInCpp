#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

/**
 * @file color.h
 * @brief Color representation and output utilities
 * 
 * This file defines color as a type alias for vec3 and provides
 * utilities for color space conversion and output formatting.
 * Colors are represented as RGB triplets with values typically
 * in the range [0,1].
 */

/**
 * @typedef color
 * @brief Type alias for vec3 when used to represent RGB colors
 * 
 * Colors are represented as 3D vectors where:
 * - x() represents red component
 * - y() represents green component  
 * - z() represents blue component
 * 
 * Values are typically in the range [0,1] for linear color space.
 */
using color = vec3;

/**
 * @brief Convert linear color component to gamma-corrected value
 * @param linear_component Linear color component value
 * @return Gamma-corrected color component
 * 
 * Applies gamma correction using the standard sRGB gamma function
 * (approximated as square root for simplicity). This converts from
 * linear color space to the sRGB color space used by most displays.
 */
inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return std::sqrt(linear_component);

    return 0;
}

/**
 * @brief Write color to output stream in PPM format
 * @param out Output stream (typically std::cout)
 * @param pixel_color Color to write
 * 
 * Converts a linear RGB color to sRGB color space and outputs
 * it in PPM format as three integer values (0-255) separated
 * by spaces and followed by a newline.
 * 
 * The function applies gamma correction and clamps values to
 * the valid range [0, 0.999] before converting to 8-bit integers.
 */
void write_color(std::ostream &out, const color &pixel_color)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Apply gamma correction
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Clamp to valid range and convert to 8-bit integers
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
#endif