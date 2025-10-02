#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "rtweekend.h"
#include <vector>

/**
 * @file hittable_list.h
 * @brief Container for multiple hittable objects
 * 
 * This file implements a hittable_list class that can contain multiple
 * hittable objects and test ray intersections against all of them.
 * It finds the closest intersection among all objects in the list.
 */

/**
 * @class hittable_list
 * @brief Container for multiple hittable objects with intersection testing
 * 
 * The hittable_list class serves as a scene container that can hold
 * multiple geometric objects. When testing for ray intersections,
 * it finds the closest intersection among all objects in the list.
 * This is essential for building complex scenes with multiple objects.
 */
class hittable_list : public hittable
{
public:
    std::vector<shared_ptr<hittable>> objects; ///< Container for hittable objects

    /**
     * @brief Default constructor - creates empty list
     */
    hittable_list() {}
    
    /**
     * @brief Constructor with initial object
     * @param object Initial hittable object to add to the list
     */
    hittable_list(shared_ptr<hittable> object) { add(object); }

    /**
     * @brief Clear all objects from the list
     */
    void clear() { objects.clear(); }

    /**
     * @brief Add a hittable object to the list
     * @param object Shared pointer to hittable object to add
     */
    void add(shared_ptr<hittable> object)
    {
        objects.push_back(object);
    }

    /**
     * @brief Test ray intersection against all objects in the list
     * @param r The ray to test for intersection
     * @param ray_t The interval along the ray to test for intersections
     * @param rec Reference to hit_record to fill with closest intersection data
     * @return True if any intersection found within interval, false otherwise
     * 
     * Tests the ray against all objects in the list and returns information
     * about the closest intersection. The algorithm maintains the closest
     * intersection found so far and updates the ray interval accordingly
     * to ensure only closer intersections are considered.
     */
    bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for (const auto &object : objects)
        {
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec))
            {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }
};

#endif