/**
 * @file main.cpp
 * @brief Main entry point for the Ray Tracing in One Weekend implementation
 *
 * This program demonstrates a basic ray tracer that renders two spheres with
 * different Lambertian materials. The scene consists of two spheres positioned
 * symmetrically around the origin, rendered using Monte Carlo sampling for
 * anti-aliasing and realistic lighting effects.
 *
 * @author Based on "Ray Tracing in One Weekend" by Peter Shirley
 * @version 1.0
 */

#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

/**
 * @brief Main function that sets up and renders a simple ray-traced scene
 *
 * Creates a scene with two spheres:
 * - Left sphere: Blue Lambertian material
 * - Right sphere: Red Lambertian material
 *
 * The spheres are positioned symmetrically around the origin at z = -1.0,
 * with their radius calculated to create a visually appealing composition.
 *
 * Camera settings:
 * - Aspect ratio: 16:9 (widescreen)
 * - Image width: 400 pixels
 * - Samples per pixel: 100 (for anti-aliasing)
 * - Maximum ray depth: 50 (for realistic reflections)
 * - Vertical field of view: 90 degrees
 *
 * @return int Exit status (0 for success)
 */
int main()
{
    // Create the world/scene container
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<dialectric>(1.50);
    auto material_bubble = make_shared<dialectric>(1.00 / 1.50);
    auto material_right = make_shared<metal>(color(0.8, 0.8, 0.2), 1.0);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    // Create and configure the camera
    Camera cam;

    // Set camera parameters for high-quality rendering
    cam.aspect_ratio = 16.0 / 9.0; // Widescreen aspect ratio
    cam.image_width = 400;         // Image width in pixels
    cam.samples_per_pixel = 100;   // Anti-aliasing samples per pixel
    cam.max_depth = 50;            // Maximum ray bounce depth for reflections

    // Set vertical field of view (90 degrees for wide-angle view)
    cam.vfov = 30;
    cam.lookfrom = point3(-2, 2, 1);
    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 10.0;
    cam.focus_dist = 3.4;

    // Render the scene and output to PPM format
    cam.render(world);

    return 0;
}
