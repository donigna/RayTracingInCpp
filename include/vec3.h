#ifndef VEC3_H
#define VEC3_H

#include "rtweekend.h"

/**
 * @file vec3.h
 * @brief 3D vector mathematics and utilities
 *
 * This file implements a comprehensive 3D vector class with all essential
 * mathematical operations needed for ray tracing, including vector arithmetic,
 * geometric operations, and random vector generation.
 */

/**
 * @class vec3
 * @brief 3D vector class for representing points, directions, and colors
 *
 * The vec3 class serves multiple purposes in the ray tracer:
 * - Represents 3D points in space (when used as point3)
 * - Represents 3D directions and vectors
 * - Represents RGB colors (when used as color)
 *
 * All vector operations are optimized for ray tracing calculations.
 */
class vec3
{
public:
    double e[3]; ///< Array storing x, y, z components

    /**
     * @brief Default constructor - creates zero vector
     */
    vec3() : e{0, 0, 0} {}

    /**
     * @brief Constructor with explicit components
     * @param e0 X component
     * @param e1 Y component
     * @param e2 Z component
     */
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    /**
     * @brief Get X component
     * @return X coordinate
     */
    double x() const { return e[0]; }

    /**
     * @brief Get Y component
     * @return Y coordinate
     */
    double y() const { return e[1]; }

    /**
     * @brief Get Z component
     * @return Z coordinate
     */
    double z() const { return e[2]; }

    /**
     * @brief Unary minus operator - negates all components
     * @return Negated vector
     */
    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

    /**
     * @brief Array access operator (const)
     * @param i Component index (0=x, 1=y, 2=z)
     * @return Component value
     */
    double operator[](int i) const { return e[i]; }

    /**
     * @brief Array access operator (non-const)
     * @param i Component index (0=x, 1=y, 2=z)
     * @return Reference to component
     */
    double &operator[](int i) { return e[i]; }

    /**
     * @brief Addition assignment operator
     * @param v Vector to add
     * @return Reference to this vector
     */
    vec3 &operator+=(const vec3 &v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    /**
     * @brief Division assignment operator
     * @param t Scalar divisor
     * @return Reference to this vector
     */
    vec3 &operator*=(double t)
    {
        return *this *= 1 / t;
    }

    /**
     * @brief Calculate vector length (magnitude)
     * @return Vector length
     */
    double length() const
    {
        return std::sqrt(length_squared());
    }

    /**
     * @brief Calculate squared vector length (avoids sqrt for performance)
     * @return Squared vector length
     */
    double length_squared() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    /**
     * @brief Check if vector is near zero (for numerical stability)
     * @return True if all components are very small
     */
    bool near_zero() const
    {
        auto s = 1e-8;
        return (std::fabs(e[0]) < s) && (std::fabs(e[1] < s)) && (std::fabs(e[2]) < s);
    }

    /**
     * @brief Generate random vector with components in [0,1]
     * @return Random vector
     */
    static vec3 random()
    {
        return vec3(random_double(), random_double(), random_double());
    }

    /**
     * @brief Generate random vector with components in [min,max]
     * @param min Minimum component value
     * @param max Maximum component value
     * @return Random vector
     */
    static vec3 random(double min, double max)
    {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }
};

/**
 * @typedef point3
 * @brief Type alias for vec3 when used to represent 3D points
 *
 * This alias provides semantic clarity when vec3 is used to represent
 * spatial coordinates rather than directions or colors.
 */
using point3 = vec3;

// ============================================================================
// Vector Utility Functions
// ============================================================================

/**
 * @brief Stream output operator for vec3
 * @param out Output stream
 * @param v Vector to output
 * @return Reference to output stream
 */
inline std::ostream &operator<<(std::ostream &out, const vec3 &v)
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

/**
 * @brief Vector addition operator
 * @param u First vector
 * @param v Second vector
 * @return Sum of vectors
 */
inline vec3 operator+(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

/**
 * @brief Vector subtraction operator
 * @param u First vector
 * @param v Second vector
 * @return Difference of vectors
 */
inline vec3 operator-(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

/**
 * @brief Component-wise vector multiplication (Hadamard product)
 * @param u First vector
 * @param v Second vector
 * @return Component-wise product
 */
inline vec3 operator*(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

/**
 * @brief Scalar-vector multiplication
 * @param t Scalar multiplier
 * @param v Vector to multiply
 * @return Scaled vector
 */
inline vec3 operator*(double t, const vec3 &v)
{
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

/**
 * @brief Vector-scalar multiplication
 * @param v Vector to multiply
 * @param t Scalar multiplier
 * @return Scaled vector
 */
inline vec3 operator*(const vec3 &v, double t)
{
    return t * v;
}

/**
 * @brief Vector-scalar division
 * @param v Vector to divide
 * @param t Scalar divisor
 * @return Scaled vector
 */
inline vec3 operator/(const vec3 &v, double t)
{
    return (1 / t) * v;
}

/**
 * @brief Calculate dot product of two vectors
 * @param u First vector
 * @param v Second vector
 * @return Dot product (scalar)
 */
inline double dot(const vec3 &u, const vec3 &v)
{
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

/**
 * @brief Calculate cross product of two vectors
 * @param u First vector
 * @param v Second vector
 * @return Cross product vector
 */
inline vec3 cross(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

/**
 * @brief Calculate unit vector (normalized)
 * @param v Input vector
 * @return Unit vector in same direction
 */
inline vec3 unit_vector(const vec3 &v)
{
    return v / v.length();
}

/**
 * @brief Generate random unit vector using rejection sampling
 * @return Random unit vector uniformly distributed on unit sphere
 *
 * Uses rejection sampling to generate uniformly distributed random
 * points on the unit sphere surface.
 */
inline vec3 random_unit_vector()
{
    while (true)
    {
        auto p = vec3::random(-1, 1);
        auto lensq = p.length_squared();
        if (1e-160 < lensq && lensq <= 1)
            return p / sqrt(lensq);
    }
}

inline vec3 random_in_unit_disk()
{
    while (true)
    {
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() < 1)
            return p;
    }
}

/**
 * @brief Generate random vector on hemisphere
 * @param normal Hemisphere normal direction
 * @return Random vector on hemisphere defined by normal
 *
 * Generates a random unit vector that lies on the hemisphere
 * defined by the given normal vector.
 */
inline vec3 random_on_hemisphere(const vec3 &normal)
{
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

/**
 * @brief Calculate reflection vector
 * @param v Incident vector
 * @param n Surface normal
 * @return Reflected vector
 *
 * Calculates the reflection of vector v off a surface with normal n
 * using the law of reflection: r = v - 2(v·n)n
 */
inline vec3 reflect(const vec3 &v, const vec3 &n)
{
    return v - 2 * dot(v, n) * n;
}

/**
 * @brief Calculate refraction vector using Snell's law
 * @param uv Incident unit vector
 * @param n Surface normal
 * @param etai_over_etat Ratio of refractive indices
 * @return Refracted vector
 *
 * Calculates the refraction of a ray through a surface using Snell's law.
 * The etai_over_etat parameter is the ratio of the refractive index of the
 * incident medium to the refractive index of the transmitted medium.
 */
inline vec3 refract(const vec3 &uv, const vec3 &n, double etai_over_etat)
{
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

#endif