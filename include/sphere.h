#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "rtweekend.h"

/**
 * @file sphere.h
 * @brief Sphere geometry implementation for ray tracing
 * 
 * This file implements a sphere primitive that can be intersected by rays.
 * The sphere intersection uses an optimized quadratic formula approach
 * for efficient ray-sphere intersection testing.
 */

/**
 * @class sphere
 * @brief Sphere primitive that can be intersected by rays
 * 
 * A sphere is defined by a center point and radius. The intersection
 * algorithm uses the quadratic formula to find intersection points
 * between rays and spheres efficiently.
 */
class sphere : public hittable
{
public:
    /**
     * @brief Constructor for sphere
     * @param center Center point of the sphere
     * @param radius Radius of the sphere (must be non-negative)
     * @param mat Material applied to the sphere surface
     */
    sphere(const point3 &center, double radius, shared_ptr<material> mat) 
        : center(center), radius(std::fmax(0, radius)), mat(mat)
    {
        // Material pointer is initialized via member initializer list
    }

    /**
     * @brief Test ray-sphere intersection using quadratic formula
     * @param r The ray to test for intersection
     * @param ray_t The interval along the ray to test for intersections
     * @param rec Reference to hit_record to fill with intersection data
     * @return True if intersection found within interval, false otherwise
     * 
     * Uses the quadratic formula to solve for intersection points:
     * For ray P(t) = A + t*B and sphere |P - C|² = r²:
     * 
     * (B·B)t² + 2(B·(A-C))t + (A-C)·(A-C) - r² = 0
     * 
     * Where:
     * - A = ray origin
     * - B = ray direction  
     * - C = sphere center
     * - r = sphere radius
     * 
     * The algorithm finds the closest valid intersection point within
     * the specified ray interval.
     */
    bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
        vec3 oc = center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = h * h - a * c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range
        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root))
        {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        // Fill hit record with intersection information
        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

private:
    point3 center;                    ///< Center point of the sphere
    double radius;                    ///< Radius of the sphere
    shared_ptr<material> mat;        ///< Material applied to the sphere surface
};

#endif