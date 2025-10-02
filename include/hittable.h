#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"

class material;

/**
 * @file hittable.h
 * @brief Abstract base class for objects that can be hit by rays
 * 
 * This file defines the hittable interface and hit_record structure
 * that form the foundation of the ray-object intersection system.
 * Any object that can be intersected by rays must implement the
 * hittable interface.
 */

/**
 * @struct hit_record
 * @brief Contains information about a ray-object intersection
 * 
 * This structure stores all the relevant information when a ray
 * intersects with a hittable object, including the intersection
 * point, surface normal, material, and intersection parameter.
 */
class hit_record
{
public:
    point3 p;                        ///< Intersection point in 3D space
    vec3 normal;                     ///< Surface normal at intersection point
    shared_ptr<material> mat;        ///< Material of the intersected surface
    double t;                        ///< Ray parameter at intersection (distance from origin)
    bool front_face;                 ///< True if ray hits front face, false for back face

    /**
     * @brief Set the face normal based on ray direction and outward normal
     * @param r The incident ray
     * @param outward_normal The outward-pointing normal from the surface
     * 
     * Determines whether the ray hits the front or back face of the surface
     * and sets the normal accordingly. The normal is always oriented
     * opposite to the ray direction for consistent lighting calculations.
     */
    void set_face_normal(const ray &r, const vec3 &outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

/**
 * @class hittable
 * @brief Abstract base class for objects that can be intersected by rays
 * 
 * The hittable interface defines the contract that all geometric objects
 * must implement to participate in ray-object intersection testing.
 * This includes primitives like spheres, planes, and composite objects
 * like bounding volume hierarchies.
 */
class hittable
{
public:
    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     */
    virtual ~hittable() = default;

    /**
     * @brief Test if a ray intersects this object within the given interval
     * @param r The ray to test for intersection
     * @param ray_t The interval along the ray to test for intersections
     * @param rec Reference to hit_record to fill with intersection data
     * @return True if intersection found, false otherwise
     * 
     * This is the core method that all hittable objects must implement.
     * It tests whether the given ray intersects the object within the
     * specified parameter interval and fills the hit_record with
     * intersection information if a hit is found.
     */
    virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const = 0;
};

#endif