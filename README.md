# 🌆 DIU Smart City Simulation – OpenGL Project

![OpenGL](https://img.shields.io/badge/OpenGL-5586A4?style=flat-square&logo=opengl&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=flat-square&logo=cplusplus&logoColor=white)

A feature-rich **3D city simulation** developed using **OpenGL and C++**. This project showcases a dynamic urban environment with real-time animations, environmental effects, and a comprehensive implementation of fundamental computer graphics algorithms.

---

## 🛠 Project Structure

- **`/src`**: Contains the primary source code (`main.cpp`) optimized for Windows/Code::Blocks.
- **`/mac`**: Contains `main.cpp` optimized for macOS (framework-based GLUT).
- **`/model`**: Contains the Code::Blocks project file (`.cbp`) and build configurations.
- **`/docs`**: Archive of documentation and previous versions (`v1.cpp`).

---

## 📐 Computer Graphics Algorithms Used

This project is built upon core graphics algorithms rather than relying solely on built-in drawing functions. Each section of the city demonstrates a specific algorithmic concept:

### 1. Bresenham's Line Algorithm
- **Usage**: Drawing structural elements with high precision.
- **Implementation**: Used for **Lamp posts**, **Traffic light poles**, and **Building outlines**.
- **Mode**: Integer-based incremental error calculation for efficient straight lines.

### 2. DDA (Digital Differential Analyzer)
- **Usage**: Rendering smooth, continuous lines.
- **Implementation**: Used for **Bridge planks**, **Crosswalk stripes**, and **Road markings**.
- **Mode**: Floating-point incremental steps for smooth visual continuity.

### 3. Midpoint Circle Algorithm
- **Usage**: Efficient circle rendering without trigonometric functions.
- **Implementation**: Used for the **Sun**, **Moon**, **Car wheels**, **Clouds**, and **Traffic light bulbs**.
- **Mode**: Symmetric 8-way plotting for optimized performance.

### 4. Reflection Transformation
- **Usage**: Creating environmental symmetry.
- **Implementation**: Applied to the **Water/River** area to create a reflection effect for the city skyline.
- **Mode**: Coordinate inversion across a specific axis.

### 5. Shear Transformation
- **Usage**: Specialized animation effects.
- **Implementation**: Used for the **Slanted Car animation** to demonstrate non-rigid body movement.
- **Mode**: Linear mapping that displaces points in a fixed direction.

---

## ▶ How to Run

### 🍏 On macOS
To run the macOS-specific version of the code:
1. Open your terminal in the project root folder.
2. Compile the code using the following command:
   ```bash
   g++ -w mac/main.cpp -o city -framework OpenGL -framework GLUT
   ```
3. Run the application:
   ```bash
   ./city
   ```

### 🪟 On Windows
1. Open **Code::Blocks**.
2. Go to `File > Open...` and select the project file: **`model/Simulation City.cbp`**.
3. Ensure that your **GLUT/FreeGLUT** libraries are correctly linked in the compiler settings.
4. Click **Build and Run** (F9).

---

## ⌨ Interactions & Controls

| Category | Key | Action |
|----------|-----|--------|
| **Environment** | `N` | Switch to **Night Mode** |
| | `D` | Switch to **Day Mode** |
| | `B` | Toggle **Rainfall** |
| | `P` | Toggle **Snowfall** |
| **Traffic** | `G` | Set Traffic Light to **Green** |
| | `Y` | Set Traffic Light to **Yellow** |
| | `R` | Set Traffic Light to **Red** |
| **Speed** | `W` | Increase Vehicle Speed |
| | `S` | Decrease Vehicle Speed |
| **Mouse** | `Left Click` | Increase Speed |
| | `Right Click` | Decrease Speed |

---

## 🎨 Customization (How to Change)

You can easily modify the city simulation by editing the variables and functions in `main.cpp`:

1.  **Changing Colors**: Find `glColor3ub(r, g, b)` calls in functions like `drawbuilding()` and change the RGB values to update the aesthetics.
2.  **Adjusting Traffic**: Modify the `busSpeed` or `CarSpeed` variables at the top of the file to change the default pace of the city.
3.  **Adding Buildings**: In the `display()` function, call `drawbuilding()` with different `glTranslated(x, y, z)` values to populate more of the city map.
4.  **Weather Intensity**: Change the `#define MAX_RAIN` or `#define MAX_SNOW` values to increase or decrease weather density.

---

## 📧 About
Developed as part of the Computer Graphics Course to demonstrate the practical application of 2D/3D algorithms in a simulated environment.
