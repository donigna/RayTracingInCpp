#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

/**
 * @file material.h
 * @brief Material system for ray tracing
 * 
 * This file defines the material system that determines how light
 * interacts with surfaces. Materials control ray scattering behavior,
 * including diffuse reflection, specular reflection, and refraction.
 */

/**
 * @class material
 * @brief Abstract base class for surface materials
 * 
 * Materials define how rays scatter when they hit surfaces. Each material
 * type implements different scattering behaviors to simulate various
 * surface properties like diffuse, metallic, or glass-like materials.
 */
class material
{
public:
    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     */
    virtual ~material() = default;

    /**
     * @brief Calculate how a ray scatters off this material
     * @param r_in The incident ray
     * @param rec Hit record containing intersection information
     * @param attenuation Color attenuation (how much light is absorbed)
     * @param scattered The scattered ray (output)
     * @return True if ray scatters, false if ray is absorbed
     * 
     * This method determines how an incident ray scatters when it hits
     * a surface with this material. It calculates the scattered ray
     * direction and the color attenuation due to material absorption.
     */
    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const
    {
        return false;
    }
};

/**
 * @class lambertian
 * @brief Diffuse material that scatters light uniformly
 * 
 * Lambertian materials represent matte surfaces that scatter light
 * uniformly in all directions. This creates the characteristic
 * soft, non-reflective appearance of materials like paper or chalk.
 */
class lambertian : public material
{
public:
    /**
     * @brief Constructor for Lambertian material
     * @param albedo Base color of the material (reflectance)
     */
    lambertian(const color &albedo) : albedo(albedo) {}

    /**
     * @brief Scatter ray diffusely using Lambertian reflection
     * @param r_in The incident ray
     * @param rec Hit record containing intersection information
     * @param attenuation Color attenuation (set to albedo)
     * @param scattered The scattered ray (output)
     * @return Always true (Lambertian always scatters)
     * 
     * Implements Lambertian diffuse reflection by scattering rays
     * in random directions biased toward the surface normal.
     * The scattered direction is: normal + random_unit_vector()
     */
    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered)
        const override
    {
        auto scatter_direction = rec.normal + random_unit_vector();

        // Catch degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    color albedo; ///< Base color/reflectance of the material
};

/**
 * @class metal
 * @brief Metallic material with specular reflection and fuzziness
 * 
 * Metal materials create mirror-like reflections with optional
 * surface roughness (fuzziness) that adds random variation
 * to the reflection direction.
 */
class metal : public material
{
public:
    /**
     * @brief Constructor for metal material
     * @param albedo Base color of the metal
     * @param fuzz Surface roughness (0 = perfect mirror, 1 = maximum fuzz)
     */
    metal(const color &albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    /**
     * @brief Scatter ray using metallic reflection with fuzziness
     * @param r_in The incident ray
     * @param rec Hit record containing intersection information
     * @param attenuation Color attenuation (set to albedo)
     * @param scattered The scattered ray (output)
     * @return True if ray reflects forward, false if absorbed
     * 
     * Implements metallic reflection using the law of reflection
     * with added fuzziness for surface roughness. The reflection
     * direction is: reflect(incident, normal) + fuzz * random_vector
     */
    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered)
        const override
    {
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    color albedo;  ///< Base color of the metal
    double fuzz;   ///< Surface roughness (0 = perfect mirror, 1 = maximum fuzz)
};

/**
 * @class dialectric
 * @brief Dielectric material for glass-like surfaces with refraction
 * 
 * Dielectric materials represent transparent surfaces like glass or water
 * that can both reflect and refract light. The material uses Snell's law
 * for refraction and Fresnel equations for realistic reflection/refraction
 * ratios.
 */
class dialectric : public material
{
public:
    /**
     * @brief Constructor for dielectric material
     * @param refraction_index Index of refraction (e.g., 1.5 for glass)
     */
    dialectric(double refraction_index) : refraction_index(refraction_index) {}

    /**
     * @brief Scatter ray using dielectric reflection/refraction
     * @param r_in The incident ray
     * @param rec Hit record containing intersection information
     * @param attenuation Color attenuation (set to white - no absorption)
     * @param scattered The scattered ray (output)
     * @return Always true (dielectric always scatters)
     * 
     * Implements dielectric scattering using Snell's law for refraction
     * and Fresnel equations for reflection probability. The material
     * chooses between reflection and refraction based on the Fresnel
     * reflectance and total internal reflection conditions.
     */
    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered)
        const override
    {
        attenuation = color(1.0, 1.0, 1.0);
        double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = ri * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, ri) > random_double())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, ri);

        scattered = ray(rec.p, direction);
        return true;
    }

private:
    double refraction_index; ///< Index of refraction of the material

    /**
     * @brief Calculate Fresnel reflectance using Schlick's approximation
     * @param cosine Cosine of the angle between incident ray and normal
     * @param refraction_index Index of refraction
     * @return Reflectance probability (0-1)
     * 
     * Uses Schlick's approximation to calculate the Fresnel reflectance,
     * which determines the probability of reflection vs. refraction
     * at a dielectric surface.
     */
    static double reflectance(double cosine, double refraction_index)
    {
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};

#endif