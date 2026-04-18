# 🌆 DIU Smart City Simulation – OpenGL Project

![OpenGL](https://img.shields.io/badge/OpenGL-5586A4?style=flat-square&logo=opengl&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=flat-square&logo=cplusplus&logoColor=white)

A feature-rich **3D city simulation** developed using **OpenGL and C++**. This project showcases a dynamic urban environment with real-time animations, environmental effects, and a comprehensive implementation of fundamental computer graphics algorithms.

---

## 🛠 Project Structure

- **`/src`**: Contains the primary source code (`main.cpp`) optimized for Windows/Code::Blocks.
- **`/mac`**: Contains `main.cpp` optimized for macOS (framework-based GLUT).
- **`/model`**: Contains the Code::Blocks project file (`.cbp`) and build configurations.
- **`/docs`**: Project documentation and feature guides (`docs.md`).
- **`/old`**: Archive of previous stable versions (`base.cpp`).

---

## 📐 Computer Graphics Algorithms Used

This project demonstrates the manual implementation of core computer graphics algorithms, replacing standard OpenGL primitives for key scene components:

### 1. Bresenham's Line Algorithm
- **Usage**: Drawing structural elements with high precision.
- **Implementation**: Used for **Lamp posts**, **Traffic light poles**, and **Building outlines**.
- **Function**: `drawBresenhamLine(x1, y1, x2, y2)` - Integer-based incremental error calculation.

### 2. DDA (Digital Differential Analyzer)
- **Usage**: Rendering smooth lines for perspective.
- **Implementation**: Used for **Bridge planks**, **Crosswalk stripes**, and **Road markings**.
- **Function**: `drawDDALine(x1, y1, x2, y2)` - Floating-point incremental steps.

### 3. Midpoint Circle Algorithm
- **Usage**: Efficient circle rendering without trigonometric functions.
- **Implementation**: Used for the **Sun**, **Moon**, **Car wheels**, **Clouds**, and **Traffic light bulbs**.
- **Function**: `drawMidpointCircle(cx, cy, radius)` - Symmetric 8-way plotting.

### 4. Reflection Transformation
- **Usage**: Environmental symmetry in the river.
- **Implementation**: Applied to **Building reflections** in the water area (Y=150).
- **Function**: `reflectX(x, y, axisY)` - Manual coordinate reflection.

### 5. Shear Transformation
- **Usage**: Aerodynamic styling for vehicles.
- **Implementation**: Applied to the **"Special Sheared Car"** for a unique slanted look.
- **Function**: `applyShear(x, y, shx, shy)` - Manual coordinate shearing.

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
Developed by **Ovi Shekh** as part of the Computer Graphics Course to demonstrate the practical application of 2D/3D algorithms in a simulated environment.
