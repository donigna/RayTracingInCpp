# Ray Tracing in One Weekend - Complete API Documentation

This document provides comprehensive API documentation for all public interfaces in the ray tracing implementation. It serves as both a reference guide and an educational resource for understanding ray tracing concepts through code.

## 📚 Educational Context

This API documentation is designed to complement the learning experience from [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) by Peter Shirley. Each section includes not only technical specifications but also explanations of the underlying mathematical and physical concepts.

## Table of Contents

1. [Core Mathematics](#core-mathematics)
2. [Geometry System](#geometry-system)
3. [Material System](#material-system)
4. [Camera System](#camera-system)
5. [Utility Functions](#utility-functions)
6. [Mathematical Foundations](#mathematical-foundations)
7. [Performance Considerations](#performance-considerations)
8. [Common Usage Patterns](#common-usage-patterns)

---

## Core Mathematics

### vec3 Class

The `vec3` class represents 3D vectors, points, and colors in the ray tracer.

#### Type Aliases
```cpp
using point3 = vec3;  // For representing 3D points
using color = vec3;   // For representing RGB colors
```

#### Constructors
```cpp
vec3();                                    // Default: (0, 0, 0)
vec3(double e0, double e1, double e2);    // Explicit components
```

#### Accessors
```cpp
double x() const;  // Get X component
double y() const;  // Get Y component  
double z() const;  // Get Z component
double operator[](int i) const;      // Array access (const)
double& operator[](int i);           // Array access (non-const)
```

#### Operators
```cpp
vec3 operator-() const;                    // Unary minus
vec3& operator+=(const vec3& v);          // Addition assignment
vec3& operator*=(double t);                // Division assignment
vec3 operator+(const vec3& u, const vec3& v);  // Vector addition
vec3 operator-(const vec3& u, const vec3& v);  // Vector subtraction
vec3 operator*(const vec3& u, const vec3& v);  // Component-wise multiplication
vec3 operator*(double t, const vec3& v);       // Scalar multiplication
vec3 operator*(const vec3& v, double t);        // Scalar multiplication
vec3 operator/(const vec3& v, double t);       // Scalar division
```

#### Mathematical Operations
```cpp
double length() const;                    // Vector magnitude
double length_squared() const;            // Squared magnitude (faster)
bool near_zero() const;                   // Check if near zero vector
vec3 random();                            // Random vector [0,1)
vec3 random(double min, double max);     // Random vector [min,max)
```

#### Utility Functions
```cpp
double dot(const vec3& u, const vec3& v);           // Dot product
vec3 cross(const vec3& u, const vec3& v);           // Cross product
vec3 unit_vector(const vec3& v);                     // Normalize vector
vec3 random_unit_vector();                           // Random unit vector
vec3 random_on_hemisphere(const vec3& normal);      // Random hemisphere vector
vec3 reflect(const vec3& v, const vec3& n);         // Reflection vector
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat);  // Refraction
```

### ray Class

Represents a ray in 3D space with origin and direction.

#### Constructors
```cpp
ray();                                           // Default constructor
ray(const point3& origin, const vec3& direction); // Explicit constructor
```

#### Accessors
```cpp
const point3& origin() const;     // Get ray origin
const vec3& direction() const;    // Get ray direction
```

#### Operations
```cpp
point3 at(double t) const;  // Get point at parameter t: origin + t * direction
```

### interval Class

Represents a closed interval [min, max] on the real number line.

#### Constructors
```cpp
interval();                           // Empty interval
interval(double min, double max);     // Explicit bounds
```

#### Static Constants
```cpp
static const interval empty;      // Empty interval
static const interval universe;    // Universe interval
```

#### Operations
```cpp
double size() const;                    // Interval length
bool contains(double x) const;          // Check if x ∈ [min, max]
bool surrounds(double x) const;         // Check if x ∈ (min, max)
double clamp(double x) const;           // Clamp x to interval bounds
```

---

## Geometry System

### hittable Interface

Abstract base class for objects that can be intersected by rays.

#### Virtual Methods
```cpp
virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
```

### hit_record Structure

Contains information about ray-object intersections.

#### Members
```cpp
point3 p;                        // Intersection point
vec3 normal;                     // Surface normal
shared_ptr<material> mat;       // Material at intersection
double t;                        // Ray parameter at intersection
bool front_face;                 // True if hitting front face
```

#### Methods
```cpp
void set_face_normal(const ray& r, const vec3& outward_normal);
```

### sphere Class

Sphere primitive implementing the hittable interface.

#### Constructor
```cpp
sphere(const point3& center, double radius, shared_ptr<material> mat);
```

#### Parameters
- `center`: Center point of the sphere
- `radius`: Sphere radius (must be non-negative)
- `mat`: Material applied to the sphere surface

### hittable_list Class

Container for multiple hittable objects.

#### Constructors
```cpp
hittable_list();                                    // Empty list
hittable_list(shared_ptr<hittable> object);        // With initial object
```

#### Methods
```cpp
void clear();                                       // Clear all objects
void add(shared_ptr<hittable> object);             // Add object to list
bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
```

#### Members
```cpp
std::vector<shared_ptr<hittable>> objects;  // Container for objects
```

---

## Material System

### material Interface

Abstract base class for surface materials.

#### Virtual Methods
```cpp
virtual bool scatter(const ray& r_in, const hit_record& rec, 
                     color& attenuation, ray& scattered) const;
```

#### Parameters
- `r_in`: Incident ray
- `rec`: Hit record with intersection information
- `attenuation`: Color attenuation (output)
- `scattered`: Scattered ray (output)

#### Returns
- `true`: Ray scatters
- `false`: Ray is absorbed

### lambertian Class

Diffuse material that scatters light uniformly.

#### Constructor
```cpp
lambertian(const color& albedo);
```

#### Parameters
- `albedo`: Base color/reflectance of the material

#### Behavior
- Scatters rays in random directions biased toward surface normal
- Always scatters (never absorbs)
- Creates matte, non-reflective appearance

### metal Class

Metallic material with specular reflection and surface roughness.

#### Constructor
```cpp
metal(const color& albedo, double fuzz);
```

#### Parameters
- `albedo`: Base color of the metal
- `fuzz`: Surface roughness (0 = perfect mirror, 1 = maximum fuzz)

#### Behavior
- Reflects rays using law of reflection
- Adds random variation based on fuzz parameter
- May absorb rays that reflect backward

### dialectric Class

Dielectric material for glass-like surfaces with refraction.

#### Constructor
```cpp
dialectric(double refraction_index);
```

#### Parameters
- `refraction_index`: Index of refraction (e.g., 1.5 for glass)

#### Behavior
- Uses Snell's law for refraction
- Uses Fresnel equations for reflection probability
- Handles total internal reflection
- No color absorption (white attenuation)

#### Static Methods
```cpp
static double reflectance(double cosine, double refraction_index);
```

---

## Camera System

### Camera Class

Camera system for ray generation and rendering.

#### Public Members

##### Image Parameters
```cpp
double aspect_ratio = 1.0f;     // Image aspect ratio (width/height)
int image_width = 100;          // Image width in pixels
int samples_per_pixel = 10;      // Anti-aliasing samples per pixel
int max_depth = 10;             // Maximum ray bounce depth
```

##### Camera Parameters
```cpp
double vfov = 90;                           // Vertical field of view (degrees)
point3 lookfrom = point3(0, 0, 0);         // Camera position
point3 lookat = point3(0, 0, -1);          // Camera target point
vec3 vup = vec3(0, 1, 0);                  // Camera up vector
```

#### Public Methods
```cpp
void render(const hittable& world);
```

#### Parameters
- `world`: Scene containing hittable objects

#### Behavior
- Generates rays for each pixel
- Performs Monte Carlo sampling for anti-aliasing
- Outputs PPM format to standard output
- Shows progress during rendering

---

## Utility Functions

### Color Utilities

#### Color Conversion
```cpp
double linear_to_gamma(double linear_component);
```

Converts linear color component to gamma-corrected value using square root approximation.

#### Color Output
```cpp
void write_color(std::ostream& out, const color& pixel_color);
```

Writes color to output stream in PPM format (0-255 integers).

### Random Number Generation
```cpp
double random_double();                    // Random double [0, 1)
double random_double(double min, double max);  // Random double [min, max)
```

### Mathematical Utilities
```cpp
double degrees_to_radians(double degrees);  // Convert degrees to radians
```

### Constants
```cpp
const double infinity;  // Positive infinity
const double pi;        // Pi constant
```

---

## Usage Examples

### Creating a Simple Scene

```cpp
#include "rtweekend.h"
#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"

int main() {
    // Create scene
    hittable_list world;
    
    // Create materials
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<dialectric>(1.5);
    auto material_right = make_shared<metal>(color(0.8, 0.8, 0.2), 0.0);
    
    // Add spheres to scene
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, material_ground));
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, material_left));
    world.add(make_shared<sphere>(point3(1, 0, -1), 0.5, material_right));
    
    // Configure camera
    Camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.vfov = 90;
    cam.lookfrom = point3(-2, 2, 1);
    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);
    
    // Render scene
    cam.render(world);
    
    return 0;
}
```

### Vector Operations

```cpp
vec3 a(1, 2, 3);
vec3 b(4, 5, 6);

vec3 sum = a + b;                    // Vector addition
double dot_product = dot(a, b);      // Dot product
vec3 cross_product = cross(a, b);   // Cross product
vec3 normalized = unit_vector(a);   // Normalize vector
double length = a.length();         // Vector length
```

### Ray Operations

```cpp
point3 origin(0, 0, 0);
vec3 direction(1, 0, 0);
ray r(origin, direction);

point3 point_at_t = r.at(2.5);      // Point at t=2.5
```

### Material Creation

```cpp
// Diffuse material
auto diffuse = make_shared<lambertian>(color(0.7, 0.3, 0.3));

// Metallic material with slight fuzziness
auto metal_mat = make_shared<metal>(color(0.8, 0.8, 0.9), 0.1);

// Glass material
auto glass = make_shared<dialectric>(1.5);
```

---

## Performance Considerations

### Rendering Parameters

- **samples_per_pixel**: Higher values improve quality but increase render time linearly
- **max_depth**: Higher values improve reflection quality but increase render time exponentially
- **image_width**: Larger images increase render time quadratically

### Optimization Tips

1. Use `length_squared()` instead of `length()` when comparing distances
2. Minimize `max_depth` for faster rendering during development
3. Use lower `samples_per_pixel` for preview renders
4. Consider bounding volume hierarchies for complex scenes

### Memory Management

- All objects use `shared_ptr` for automatic memory management
- Materials are shared between objects to reduce memory usage
- The ray tracer is designed to be memory-efficient for educational purposes

---

## Error Handling

The ray tracer includes several built-in safeguards:

- Sphere radius is clamped to non-negative values
- Metal fuzz parameter is clamped to [0, 1]
- Ray intersection intervals prevent self-intersection
- Degenerate scatter directions are handled gracefully

For production use, additional error checking and validation would be recommended.

---

## Mathematical Foundations

### Ray-Sphere Intersection Mathematics

The sphere intersection algorithm is based on the quadratic formula. For a ray **P(t) = A + t·B** and a sphere **|P - C|² = r²**:

**Quadratic Equation:**
```
(B·B)t² + 2(B·(A-C))t + (A-C)·(A-C) - r² = 0
```

**Where:**
- **A** = ray origin (point3)
- **B** = ray direction (vec3)
- **C** = sphere center (point3)
- **r** = sphere radius (double)

**Optimized Implementation:**
```cpp
vec3 oc = center - r.origin();
auto a = r.direction().length_squared();
auto h = dot(r.direction(), oc);
auto c = oc.length_squared() - radius * radius;
auto discriminant = h * h - a * c;
```

### Snell's Law for Refraction

Dielectric materials use Snell's law for refraction:

**Snell's Law:**
```
η₁ sin(θ₁) = η₂ sin(θ₂)
```

**Implementation:**
```cpp
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}
```

### Schlick's Approximation for Fresnel Reflectance

Used in dielectric materials to determine reflection vs. refraction probability:

**Schlick's Formula:**
```
R(θ) = R₀ + (1 - R₀)(1 - cos(θ))⁵
```

**Where:**
```
R₀ = ((1 - η)/(1 + η))²
```

---

## Performance Considerations

### Rendering Time Complexity

**Primary Factors:**
- **Image Resolution**: O(width × height)
- **Samples per Pixel**: O(samples_per_pixel)
- **Ray Depth**: O(max_depth)
- **Scene Complexity**: O(objects × intersections)

**Total Complexity**: O(width × height × samples_per_pixel × max_depth × objects)

### Optimization Strategies

1. **Early Ray Termination**: Stop tracing when attenuation becomes negligible
2. **Bounding Volume Hierarchies**: Reduce intersection tests for complex scenes
3. **Importance Sampling**: Focus samples on important light contributions
4. **Parallel Processing**: Multi-threaded ray tracing for faster rendering

### Memory Usage Patterns

- **Shared Pointers**: Efficient material sharing between objects
- **Stack-Based Recursion**: Ray tracing depth limited by stack size
- **Minimal Object State**: Only essential data stored per object

---

## Common Usage Patterns

### Creating a Basic Scene

```cpp
#include "rtweekend.h"
#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"

int main() {
    // Create scene container
    hittable_list world;
    
    // Create materials
    auto ground_material = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto center_material = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto left_material = make_shared<dielectric>(1.5);
    auto right_material = make_shared<metal>(color(0.8, 0.8, 0.2), 0.0);
    
    // Add objects to scene
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, ground_material));
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, center_material));
    world.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, left_material));
    world.add(make_shared<sphere>(point3(1, 0, -1), 0.5, right_material));
    
    // Configure camera
    Camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.vfov = 90;
    cam.lookfrom = point3(-2, 2, 1);
    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);
    
    // Render scene
    cam.render(world);
    
    return 0;
}
```

### Material Creation Patterns

```cpp
// Diffuse materials (matte surfaces)
auto diffuse_red = make_shared<lambertian>(color(0.7, 0.3, 0.3));
auto diffuse_blue = make_shared<lambertian>(color(0.3, 0.3, 0.7));

// Metallic materials (reflective surfaces)
auto perfect_mirror = make_shared<metal>(color(0.8, 0.8, 0.9), 0.0);
auto rough_metal = make_shared<metal>(color(0.7, 0.6, 0.5), 0.3);

// Dielectric materials (transparent surfaces)
auto glass = make_shared<dielectric>(1.5);        // Glass
auto diamond = make_shared<dielectric>(2.4);      // Diamond
auto water = make_shared<dielectric>(1.33);      // Water
```

### Vector Operations Examples

```cpp
// Basic vector operations
vec3 a(1, 2, 3);
vec3 b(4, 5, 6);

vec3 sum = a + b;                    // Vector addition
vec3 diff = a - b;                   // Vector subtraction
vec3 scaled = 2.0 * a;               // Scalar multiplication
double dot_product = dot(a, b);       // Dot product
vec3 cross_product = cross(a, b);    // Cross product
vec3 normalized = unit_vector(a);     // Normalize vector
double length = a.length();          // Vector length
double length_sq = a.length_squared(); // Squared length (faster)

// Random vector generation
vec3 random_vec = vec3::random();                    // [0,1) components
vec3 random_range = vec3::random(-1, 1);            // [-1,1) components
vec3 random_unit = random_unit_vector();             // Unit sphere surface
vec3 random_hemisphere = random_on_hemisphere(normal); // Hemisphere
```

### Ray Operations Examples

```cpp
// Ray creation and manipulation
point3 origin(0, 0, 0);
vec3 direction(1, 0, 0);
ray r(origin, direction);

// Get points along the ray
point3 point_at_t1 = r.at(1.0);      // Point at t=1.0
point3 point_at_t2 = r.at(2.5);      // Point at t=2.5

// Ray properties
point3 ray_origin = r.origin();       // Get origin
vec3 ray_direction = r.direction();   // Get direction
```

### Advanced Camera Configuration

```cpp
Camera cam;

// High-quality rendering settings
cam.aspect_ratio = 16.0 / 9.0;       // Widescreen
cam.image_width = 1920;              // Full HD width
cam.samples_per_pixel = 500;         // High-quality anti-aliasing
cam.max_depth = 100;                 // Deep reflections

// Camera positioning
cam.vfov = 20;                       // Narrow field of view
cam.lookfrom = point3(13, 2, 3);    // Camera position
cam.lookat = point3(0, 0, 0);       // Look target
cam.vup = vec3(0, 1, 0);            // Up direction

// Depth of field effects
cam.defocus_angle = 0.6;            // Blur amount
cam.focus_dist = 10.0;              // Focus distance
```

---

## Learning Exercises

### Beginner Exercises

1. **Modify Colors**: Change the colors of spheres in the scene
2. **Adjust Camera**: Experiment with different camera positions and angles
3. **Add Spheres**: Create additional spheres with different materials
4. **Change Resolution**: Render at different image sizes

### Intermediate Exercises

1. **Material Properties**: Experiment with different fuzziness values for metals
2. **Refraction Indices**: Try different refraction indices for dielectric materials
3. **Scene Composition**: Create artistic scenes with specific color schemes
4. **Performance Testing**: Measure render times with different quality settings

### Advanced Exercises

1. **New Materials**: Implement emissive or anisotropic materials
2. **Geometric Primitives**: Add planes, boxes, or other shapes
3. **Lighting Models**: Implement area lights or environment lighting
4. **Optimization**: Add spatial acceleration structures

---

## Troubleshooting

### Common Issues

**Compilation Errors:**
- Ensure C++17 support is enabled
- Check that all header files are in the include directory
- Verify CMake configuration

**Rendering Issues:**
- **Black Images**: Check ray direction and camera positioning
- **No Reflections**: Increase max_depth parameter
- **Aliasing**: Increase samples_per_pixel
- **Slow Rendering**: Reduce image size or samples for testing

**Mathematical Issues:**
- **NaN Values**: Check for division by zero in vector operations
- **Infinite Loops**: Ensure ray termination conditions are met
- **Wrong Colors**: Verify gamma correction and color clamping

### Debugging Tips

1. **Print Ray Information**: Add debug output to trace ray paths
2. **Visualize Normals**: Color objects by their surface normals
3. **Check Intersections**: Verify ray-object intersection calculations
4. **Profile Performance**: Measure time spent in different parts of the code

---

*This documentation provides a comprehensive guide to understanding and using the ray tracing implementation. For the theoretical background, refer to the original tutorial series by Peter Shirley.*