# Ray Tracing in One Weekend - API Documentation

This document provides comprehensive API documentation for all public interfaces in the ray tracing implementation.

## Table of Contents

1. [Core Mathematics](#core-mathematics)
2. [Geometry System](#geometry-system)
3. [Material System](#material-system)
4. [Camera System](#camera-system)
5. [Utility Functions](#utility-functions)

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