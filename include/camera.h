#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

/**
 * @file camera.h
 * @brief Camera and rendering pipeline implementation
 *
 * This file implements the camera class that handles ray generation,
 * perspective projection, anti-aliasing, and the main rendering loop.
 * The camera uses a pinhole camera model with configurable parameters.
 */

/**
 * @class Camera
 * @brief Camera system for ray tracing with perspective projection
 *
 * The Camera class implements a pinhole camera model that generates
 * rays through a virtual image plane. It supports configurable
 * field of view, camera positioning, anti-aliasing, and recursive
 * ray tracing for realistic rendering.
 */
class Camera
{
public:
    // ============================================================================
    // Image Parameters
    // ============================================================================

    double aspect_ratio = 1.0f; ///< Image aspect ratio (width/height)
    int image_width = 100;      ///< Image width in pixels
    int samples_per_pixel = 10; ///< Anti-aliasing samples per pixel
    int max_depth = 10;         ///< Maximum ray bounce depth for reflections

    // ============================================================================
    // Camera Parameters
    // ============================================================================

    double vfov = 90;                  ///< Vertical field of view in degrees
    point3 lookfrom = point3(0, 0, 0); ///< Camera position
    point3 lookat = point3(0, 0, -1);  ///< Camera target point
    vec3 vup = vec3(0, 1, 0);          ///< Camera up vector

    double defocus_angle = 0;
    double focus_dist = 10;

    /**
     * @brief Render the scene to PPM format
     * @param world The scene containing hittable objects
     *
     * Main rendering function that generates rays for each pixel,
     * performs Monte Carlo sampling for anti-aliasing, and outputs
     * the result in PPM format to standard output.
     */
    void render(const hittable &world)
    {
        initialize();

        // Output PPM header
        std::cout << "P3\n"
                  << image_width << ' ' << image_height << "\n255\n";

        // Render each pixel
        for (int j = 0; j < image_height; j++)
        {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++)
            {
                // Monte Carlo sampling for anti-aliasing
                color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; sample++)
                {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }

                // Average samples and output color
                write_color(std::cout, pixel_samples_scale * pixel_color);
            }
        }
        std::clog << "\rDone.      \n";
    }

private:
    // ============================================================================
    // Private Member Variables
    // ============================================================================

    int image_height;           ///< Calculated image height based on aspect ratio
    double pixel_samples_scale; ///< Scale factor for averaging samples (1/samples_per_pixel)
    point3 center;              ///< Camera center (pinhole position)
    point3 pixel00_loc;         ///< Location of top-left pixel center
    vec3 pixel_delta_u;         ///< Horizontal pixel-to-pixel delta vector
    vec3 pixel_delta_v;         ///< Vertical pixel-to-pixel delta vector
    vec3 u, v, w;               ///< Camera coordinate system basis vectors
    vec3 defocus_disk_u;
    vec3 defocus_disk_v;

    /**
     * @brief Initialize camera parameters and compute derived values
     *
     * Sets up the camera coordinate system, viewport dimensions,
     * and pixel spacing based on the configured camera parameters.
     * This method must be called before rendering.
     */
    void initialize()
    {
        // Calculate image height from aspect ratio
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        // Calculate sample scaling factor
        pixel_samples_scale = 1.0 / samples_per_pixel;

        // Set camera center to lookfrom position
        center = lookfrom;

        // Calculate viewport dimensions based on field of view
        // auto focal_length = (lookfrom - lookat).length();
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width) / image_height);

        // Calculate camera coordinate system
        w = unit_vector(lookfrom - lookat); // Camera forward direction
        u = unit_vector(cross(vup, w));     // Camera right direction
        v = cross(w, u);                    // Camera up direction

        // Calculate viewport vectors
        vec3 viewport_u = viewport_width * u;
        vec3 viewport_v = viewport_height * -v;

        // Calculate pixel spacing
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate top-left pixel location
        auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    /**
     * @brief Generate a ray for the given pixel coordinates
     * @param i Horizontal pixel coordinate
     * @param j Vertical pixel coordinate
     * @return Ray from camera center through the pixel
     *
     * Generates a ray from the camera center through the specified pixel,
     * with random sampling within the pixel for anti-aliasing.
     */
    ray get_ray(int i, int j) const
    {
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    /**
     * @brief Generate random offset within a pixel for anti-aliasing
     * @return Random offset vector in range [-0.5, 0.5]
     *
     * Generates a random offset within the pixel square for
     * Monte Carlo anti-aliasing sampling.
     */
    vec3 sample_square() const
    {
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    point3 defocus_disk_sample() const
    {
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    /**
     * @brief Calculate color for a ray using recursive ray tracing
     * @param r The ray to trace
     * @param depth Remaining recursion depth
     * @param world The scene containing hittable objects
     * @return Color contribution from this ray
     *
     * Recursively traces rays through the scene, handling material
     * scattering and reflections. When rays miss objects, returns
     * a gradient background color simulating sky.
     */
    color ray_color(const ray &r, int depth, const hittable &world) const
    {
        // Base case: maximum depth reached
        if (depth <= 0)
            return color(0, 0, 0);

        hit_record rec;
        if (world.hit(r, interval(0.001, infinity), rec))
        {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth - 1, world);
            return color(0, 0, 0);
        }

        // Background gradient (sky)
        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};

#endif