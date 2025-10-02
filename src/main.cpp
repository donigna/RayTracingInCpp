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

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8)
                {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    // Create and configure the camera
    Camera cam;

    // Set camera parameters for high-quality rendering
    cam.aspect_ratio = 16.0 / 9.0; // Widescreen aspect ratio
    cam.image_width = 1200;        // Image width in pixels
    cam.samples_per_pixel = 100;   // Anti-aliasing samples per pixel
    cam.max_depth = 50;            // Maximum ray bounce depth for reflections

    // Set vertical field of view (90 degrees for wide-angle view)
    cam.vfov = 30;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    // Render the scene and output to PPM format
    cam.render(world);

    return 0;
}
