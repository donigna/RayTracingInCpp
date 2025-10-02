# Ray Tracing in One Weekend

A C++ implementation of a basic ray tracer based on Peter Shirley's "Ray Tracing in One Weekend" tutorial. This project demonstrates fundamental ray tracing concepts including ray-object intersection, material scattering, and Monte Carlo sampling for realistic rendering.

## Features

- **Geometric Primitives**: Sphere intersection with optimized ray-sphere intersection algorithm
- **Material System**: 
  - Lambertian (diffuse) materials for matte surfaces
  - Metallic materials with configurable fuzziness for reflections
  - Dielectric materials for glass-like surfaces with refraction
- **Camera System**: Configurable camera with perspective projection and anti-aliasing
- **Monte Carlo Sampling**: Multi-sample anti-aliasing and realistic lighting
- **PPM Output**: Renders to standard PPM image format

## Project Structure

```
RayTracingInOneWeek/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── include/                # Header files
│   ├── rtweekend.h         # Common utilities and constants
│   ├── vec3.h             # 3D vector mathematics
│   ├── ray.h              # Ray representation
│   ├── color.h            # Color utilities and gamma correction
│   ├── interval.h         # Mathematical interval class
│   ├── hittable.h         # Abstract base class for hittable objects
│   ├── hittable_list.h    # Container for multiple hittable objects
│   ├── sphere.h           # Sphere geometry implementation
│   ├── material.h         # Material system (Lambertian, Metal, Dielectric)
│   └── camera.h           # Camera and rendering pipeline
├── src/
│   └── main.cpp           # Main application entry point
└── build/                 # Build output directory
    └── image.ppm          # Generated ray-traced image
```

## Building the Project

### Prerequisites

- C++17 compatible compiler (GCC, Clang, or MSVC)
- CMake 3.10 or later

### Build Instructions

1. Clone or navigate to the project directory:
   ```bash
   cd /path/to/RayTracingInOneWeek
   ```

2. Create build directory:
   ```bash
   mkdir build
   cd build
   ```

3. Configure and build:
   ```bash
   cmake ..
   make
   ```

4. Run the ray tracer:
   ```bash
   ./raytracer > image.ppm
   ```

## Usage

The ray tracer generates a PPM image file that can be viewed with most image viewers or converted to other formats. The default scene includes:

- A large ground sphere (yellow Lambertian material)
- A center sphere (blue Lambertian material)
- A left sphere (glass dielectric material)
- A bubble inside the left sphere (air dielectric material)
- A right sphere (metallic material with slight fuzziness)

### Configuration

You can modify the scene in `src/main.cpp` by:

- Changing sphere positions, sizes, and materials
- Adjusting camera parameters (position, field of view, samples per pixel)
- Adding new objects to the scene

### Camera Parameters

- `aspect_ratio`: Image aspect ratio (default: 16:9)
- `image_width`: Output image width in pixels (default: 400)
- `samples_per_pixel`: Anti-aliasing samples (default: 100)
- `max_depth`: Maximum ray bounce depth (default: 50)
- `vfov`: Vertical field of view in degrees (default: 90)
- `lookfrom`: Camera position
- `lookat`: Camera target point
- `vup`: Camera up vector

## Technical Details

### Ray Tracing Pipeline

1. **Ray Generation**: Camera generates rays through each pixel
2. **Intersection Testing**: Rays are tested against all objects in the scene
3. **Material Scattering**: Hit points scatter rays according to material properties
4. **Recursive Tracing**: Scattered rays continue until maximum depth or miss
5. **Color Accumulation**: Colors are accumulated and averaged across samples
6. **Gamma Correction**: Linear colors are converted to sRGB for display

### Performance Considerations

- Higher `samples_per_pixel` values improve quality but increase render time
- Higher `max_depth` values improve reflection quality but increase render time
- Larger `image_width` values increase resolution but quadratically increase render time

### Mathematical Foundations

The ray tracer implements:
- **Ray-Sphere Intersection**: Using the quadratic formula for efficient intersection testing
- **Monte Carlo Integration**: For anti-aliasing and realistic lighting
- **Snell's Law**: For accurate refraction in dielectric materials
- **Schlick's Approximation**: For realistic Fresnel reflections
- **Gamma Correction**: For proper color space conversion

## Output

The program outputs a PPM (Portable Pixmap) image to standard output. To save the image:

```bash
./raytracer > image.ppm
```

The PPM format is widely supported and can be opened in most image viewers or converted to other formats using tools like ImageMagick:

```bash
convert image.ppm image.png
```

## Learning Resources

This implementation is based on:
- "Ray Tracing in One Weekend" by Peter Shirley
- "Ray Tracing: The Next Week" by Peter Shirley
- "Ray Tracing: The Rest of Your Life" by Peter Shirley

## License

This project is for educational purposes and follows the same licensing as the original "Ray Tracing in One Weekend" tutorial.

## Contributing

This is an educational project. Feel free to experiment with:
- Adding new geometric primitives
- Implementing additional materials
- Optimizing the rendering pipeline
- Adding new features like textures or lighting models