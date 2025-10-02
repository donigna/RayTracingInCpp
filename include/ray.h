#ifndef RAY_H
#define RAY_H

#include "vec3.h"

/**
 * @file ray.h
 * @brief Ray representation for ray tracing
 * 
 * This file defines the ray class, which represents a ray in 3D space
 * with an origin point and a direction vector. Rays are fundamental
 * to ray tracing as they represent the path of light through the scene.
 */

/**
 * @class ray
 * @brief Represents a ray in 3D space
 * 
 * A ray is defined by an origin point and a direction vector.
 * The ray can be parameterized as: P(t) = origin + t * direction
 * where t is the parameter along the ray.
 */
class ray
{
public:
    /**
     * @brief Default constructor - creates ray at origin pointing in +Z
     */
    ray() {}

    /**
     * @brief Constructor with explicit origin and direction
     * @param origin Starting point of the ray
     * @param direction Direction vector (should be normalized for best results)
     */
    ray(const point3 &origin, const vec3 &direction) : orig(origin), dir(direction) {}

    /**
     * @brief Get the origin point of the ray
     * @return Reference to origin point
     */
    const point3 &origin() const { return orig; }
    
    /**
     * @brief Get the direction vector of the ray
     * @return Reference to direction vector
     */
    const vec3 &direction() const { return dir; }

    /**
     * @brief Calculate point on ray at parameter t
     * @param t Parameter along the ray (0 = origin, positive = forward)
     * @return Point on ray at parameter t
     * 
     * Uses the parametric equation: P(t) = origin + t * direction
     */
    point3 at(double t) const
    {
        return orig + t * dir;
    }

private:
    point3 orig; ///< Origin point of the ray
    vec3 dir;   ///< Direction vector of the ray
};

#endif