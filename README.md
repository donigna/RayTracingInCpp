# Ray Tracing in One Weekend - C++ Implementation

A comprehensive C++ implementation of a ray tracer based on Peter Shirley's acclaimed tutorial series. This project demonstrates fundamental ray tracing concepts through a complete, well-documented implementation that renders realistic scenes with various materials and lighting effects.

![Output](output.png?raw=true)

## 📚 Learning Through Ray Tracing

This project serves as an educational implementation of the concepts presented in **"Ray Tracing in One Weekend"** by Peter Shirley. It's designed to help students and developers understand the mathematical foundations and practical implementation of ray tracing algorithms.

### Reference Material

This implementation is based on the excellent tutorial series:
- **[_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)** by Peter Shirley
- **[_Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html)** by Peter Shirley  
- **[_Ray Tracing: The Rest of Your Life_](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)** by Peter Shirley

The tutorial series provides the theoretical foundation, while this implementation offers a complete, documented C++ codebase for hands-on learning.

## ✨ Features

### Core Ray Tracing Capabilities
- **Ray Generation**: Camera system with configurable perspective projection
- **Ray-Object Intersection**: Efficient sphere intersection using quadratic formula
- **Recursive Ray Tracing**: Multi-bounce ray tracing for realistic reflections and refractions
- **Monte Carlo Sampling**: Anti-aliasing and realistic lighting through random sampling

### Material System
- **Lambertian Materials**: Diffuse surfaces that scatter light uniformly (matte surfaces)
- **Metallic Materials**: Specular reflection with configurable surface roughness
- **Dielectric Materials**: Glass-like surfaces with realistic refraction and reflection
- **Material Scattering**: Physics-based light interaction using Snell's law and Fresnel equations

### Advanced Features
- **Defocus Blur**: Depth of field effects for realistic camera simulation
- **Gamma Correction**: Proper color space conversion for accurate display
- **High-Quality Rendering**: Configurable samples per pixel and ray depth
- **PPM Output**: Standard image format compatible with most viewers

## 🏗️ Project Structure

```
RayTracingInCpp/
├── CMakeLists.txt              # Build configuration
├── README.md                   # This documentation
├── API_DOCUMENTATION.md        # Complete API reference
├── include/                    # Header files
│   ├── rtweekend.h            # Common utilities and constants
│   ├── vec3.h                 # 3D vector mathematics
│   ├── ray.h                  # Ray representation
│   ├── color.h                # Color utilities and gamma correction
│   ├── interval.h             # Mathematical interval class
│   ├── hittable.h             # Abstract base class for hittable objects
│   ├── hittable_list.h        # Container for multiple hittable objects
│   ├── sphere.h               # Sphere geometry implementation
│   ├── material.h             # Material system (Lambertian, Metal, Dielectric)
│   └── camera.h               # Camera and rendering pipeline
├── src/
│   └── main.cpp               # Main application with scene setup
└── build/                     # Build output directory
    ├── raytracer              # Compiled executable
    └── image.ppm              # Generated ray-traced image
```

## 🚀 Quick Start

### Prerequisites

- **C++17** compatible compiler (GCC, Clang, or MSVC)
- **CMake 3.10** or later
- **Make** or **Ninja** build system

### Building the Project

1. **Clone or navigate to the project directory:**
   ```bash
   cd /path/to/RayTracingInCpp
   ```

2. **Create and enter build directory:**
   ```bash
   mkdir build
   cd build
   ```

3. **Configure and build:**
   ```bash
   cmake ..
   make
   ```

4. **Run the ray tracer:**
   ```bash
   ./raytracer > image.ppm
   ```

5. **View the result:**
   ```bash
   # Convert to PNG for better viewing
   convert image.ppm output.png
   # Or open directly with image viewers that support PPM
   ```

## 🎯 Learning Objectives

This implementation helps you understand:

### Mathematical Foundations
- **Vector Mathematics**: 3D vector operations, dot products, cross products
- **Ray-Sphere Intersection**: Quadratic formula for efficient intersection testing
- **Monte Carlo Integration**: Random sampling for anti-aliasing and realistic lighting
- **Snell's Law**: Accurate refraction calculations for dielectric materials
- **Schlick's Approximation**: Fresnel reflection probability calculations

### Computer Graphics Concepts
- **Ray Tracing Pipeline**: From camera to pixel color calculation
- **Material Models**: How different surfaces interact with light
- **Recursive Rendering**: Multi-bounce ray tracing for realistic reflections
- **Anti-aliasing**: Monte Carlo sampling to reduce visual artifacts
- **Color Spaces**: Linear to sRGB conversion with gamma correction

### Software Engineering
- **Object-Oriented Design**: Abstract base classes and polymorphism
- **Memory Management**: Smart pointers for automatic resource management
- **Modular Architecture**: Clean separation of concerns across components
- **Performance Optimization**: Efficient algorithms and data structures

## 🎨 Scene Configuration

The default scene includes:

- **Ground Sphere**: Large yellow Lambertian sphere serving as the ground
- **Random Spheres**: 400+ randomly positioned spheres with varied materials:
  - 80% Lambertian (diffuse) materials with random colors
  - 15% Metallic materials with random colors and fuzziness
  - 5% Dielectric (glass) materials
- **Three Main Spheres**:
  - Center: Glass sphere (dielectric material)
  - Left: Brown Lambertian sphere
  - Right: Metallic sphere with slight fuzziness

### Customizing the Scene

Modify `src/main.cpp` to experiment with:

```cpp
// Camera parameters
cam.aspect_ratio = 16.0 / 9.0;    // Image aspect ratio
cam.image_width = 1200;            // Resolution (higher = slower)
cam.samples_per_pixel = 100;       // Anti-aliasing quality
cam.max_depth = 50;               // Reflection depth
cam.vfov = 30;                    // Field of view (degrees)

// Camera positioning
cam.lookfrom = point3(13, 2, 3);  // Camera position
cam.lookat = point3(0, 0, 0);     // Look target
cam.vup = vec3(0, 1, 0);          // Up direction

// Defocus blur
cam.defocus_angle = 0.6;          // Blur amount
cam.focus_dist = 10.0;            // Focus distance
```

## 🔬 Technical Implementation Details

### Ray Tracing Pipeline

1. **Ray Generation**: Camera generates rays through each pixel with random sampling
2. **Intersection Testing**: Rays are tested against all objects using efficient algorithms
3. **Material Scattering**: Hit points scatter rays according to material properties
4. **Recursive Tracing**: Scattered rays continue until maximum depth or miss
5. **Color Accumulation**: Colors are accumulated and averaged across samples
6. **Gamma Correction**: Linear colors are converted to sRGB for display

### Performance Characteristics

- **Rendering Time**: Scales with `image_width² × samples_per_pixel × max_depth`
- **Memory Usage**: Minimal - uses shared pointers for efficient memory management
- **Quality vs Speed**: Higher samples and depth improve quality but increase render time

### Mathematical Accuracy

- **Ray-Sphere Intersection**: Uses optimized quadratic formula with numerical stability
- **Refraction**: Implements Snell's law with proper handling of total internal reflection
- **Reflection**: Uses law of reflection with optional surface roughness
- **Random Sampling**: Mersenne Twister generator for high-quality random numbers

## 📖 API Documentation

For detailed API documentation, see [API_DOCUMENTATION.md](API_DOCUMENTATION.md), which includes:

- Complete class and function documentation
- Usage examples and code snippets
- Performance considerations and optimization tips
- Mathematical formulas and implementation details

## 🎓 Educational Value

This implementation is particularly valuable for:

- **Computer Graphics Students**: Understanding ray tracing fundamentals
- **C++ Developers**: Learning modern C++ practices and design patterns
- **Mathematics Enthusiasts**: Seeing mathematical concepts in practical application
- **Game Developers**: Understanding rendering techniques used in modern games
- **Researchers**: Having a clean, documented reference implementation

## 🔧 Extending the Project

The modular design makes it easy to add new features:

- **New Geometric Primitives**: Implement the `hittable` interface
- **Additional Materials**: Extend the `material` base class
- **Advanced Lighting**: Add area lights, environment maps, or global illumination
- **Performance Optimizations**: Implement bounding volume hierarchies or spatial acceleration structures
- **New Output Formats**: Add support for PNG, JPEG, or other image formats

## 📄 License

This project is for educational purposes and follows the same licensing as the original "Ray Tracing in One Weekend" tutorial series. The implementation is provided as-is for learning and experimentation.

## 🤝 Contributing

This is an educational project designed for learning. Contributions that enhance the educational value are welcome:

- **Documentation Improvements**: Better explanations, examples, or tutorials
- **Code Clarity**: Cleaner implementations or better comments
- **Educational Features**: Additional examples or learning exercises
- **Performance Optimizations**: Faster algorithms or better memory usage

## 📚 Further Reading

- **Original Tutorial**: [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
- **Advanced Topics**: [_Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html)
- **Mathematical Foundations**: [_Ray Tracing: The Rest of Your Life_](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)
- **Computer Graphics**: "Real-Time Rendering" by Akenine-Möller, Haines, and Hoffman
- **Mathematics**: "3D Math Primer for Graphics and Game Development" by Fletcher Dunn

---

*This implementation demonstrates that complex computer graphics concepts can be learned through hands-on coding. The combination of mathematical theory and practical implementation provides a solid foundation for understanding modern rendering techniques.*
