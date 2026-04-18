#include <GLUT/glut.h>
#include <iostream>
#include <math.h>
#define pi 3.142857
#include <cstdlib>
#include <vector>
#define MAX_RAIN 1000
#define MAX_SNOW 500
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
using namespace std;

// ==================== ALGORITHM IMPLEMENTATIONS ====================

// 1. BRESENHAM LINE ALGORITHM - Used for drawing straight lines with integer
// precision Uses: Lamp posts, Traffic light poles, Building outlines
void drawBresenhamLine(int x1, int y1, int x2, int y2) {
  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int sx = (x1 < x2) ? 1 : -1;
  int sy = (y1 < y2) ? 1 : -1;
  int err = dx - dy;
  int x = x1, y = y1;

  glBegin(GL_POINTS);
  while (true) {
    glVertex2i(x, y);
    if (x == x2 && y == y2)
      break;
    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x += sx;
    }
    if (e2 < dx) {
      err += dx;
      y += sy;
    }
  }
  glEnd();
}

// 2. DDA LINE ALGORITHM - Digital Differential Analyzer for smooth lines
// Uses: Bridge planks, Crosswalk lines, Road markings, Boat details
void drawDDALine(float x1, float y1, float x2, float y2) {
  float dx = x2 - x1;
  float dy = y2 - y1;
  int steps = max(abs((int)dx), abs((int)dy));

  float xInc = dx / (float)steps;
  float yInc = dy / (float)steps;
  float x = x1, y = y1;

  glBegin(GL_POINTS);
  for (int i = 0; i <= steps; i++) {
    glVertex2i((int)x, (int)y);
    x += xInc;
    y += yInc;
  }
  glEnd();
}

// 3. MIDPOINT CIRCLE ALGORITHM - Efficient circle drawing without floating
// point Uses: Sun, Moon, Clouds, Wheels, Traffic lights, Lamp bulbs
void drawMidpointCircle(int cx, int cy, int radius) {
  int x = 0;
  int y = radius;
  int d = 1 - radius;

  glBegin(GL_POINTS);
  while (x <= y) {
    glVertex2i(cx + x, cy + y);
    glVertex2i(cx - x, cy + y);
    glVertex2i(cx + x, cy - y);
    glVertex2i(cx - x, cy - y);
    glVertex2i(cx + y, cy + x);
    glVertex2i(cx - y, cy + x);
    glVertex2i(cx + y, cy - x);
    glVertex2i(cx - y, cy - x);

    x++;
    if (d < 0) {
      d += 2 * x + 1;
    } else {
      y--;
      d += 2 * (x - y) + 1;
    }
  }
  glEnd();
}

// 4. REFLECTION TRANSFORMATION - For water reflection effect
void reflectX(float &x, float &y, float axisY = 150) { y = 2 * axisY - y; }

void reflectY(float &x, float &y, float axisX = 400) { x = 2 * axisX - x; }

// 5. SHEAR TRANSFORMATION - Creates slanted/italic effect on objects
void applyShear(float &x, float &y, float shx, float shy) {
  float newX = x + shx * y;
  float newY = y + shy * x;
  x = newX;
  y = newY;
}

// ==================== ORIGINAL VARIABLES ====================

int lightState = 2;
float carPosition = 800;
float bx = -100;
float j = -100;
float k = 350;
float p = -50;
float busSpeed = 0.8;
float CarSpeed = 0.8;
float cloudSpeed = 0.3;
bool isNight = false;
bool isRaining = false;
char activeLight = 'N';

#define NUM_STARS 100
float starPositions[NUM_STARS][2];
bool starsInitialized = false;

// ==================== FORWARD DECLARATIONS ====================
void drawbuilding();

// ==================== FUNCTIONS WITH ALGORITHMS ====================

void myInit() {
  if (isNight) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  } else {
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
  }
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 700, 0, 800, -10.0, 10.0);
}

void circle(GLfloat rx, GLfloat ry, GLfloat cx, GLfloat cy) {
  drawMidpointCircle((int)cx, (int)cy, (int)((rx + ry) / 2));
}

void drawStars() {
  if (isNight) {
    if (!starsInitialized) {
      for (int i = 0; i < NUM_STARS; i++) {
        starPositions[i][0] = rand() % 700;
        starPositions[i][1] = 500 + (rand() % 300);
      }
      starsInitialized = true;
    }
    glColor3ub(255, 255, 255);
    glBegin(GL_POINTS);
    for (int i = 0; i < NUM_STARS; i++) {
      glVertex2f(starPositions[i][0], starPositions[i][1]);
    }
    glEnd();
  }
}

void updatePosition(int value) {
  int crosswalkPosition = 300;
  int stopPoint = crosswalkPosition - 5;

  if (lightState == 0 && j + 90 >= stopPoint && j + 90 <= crosswalkPosition) {
    // Stop at red light
  } else {
    j += busSpeed;
  }

  if (j > 800) {
    j = -100;
  }

  if (p <= 800) {
    p += cloudSpeed;
  } else {
    p = -50;
  }

  glutPostRedisplay();
  glutTimerFunc(20, updatePosition, 0);
}

void updatePosition2(int value) {
  int crosswalkPosition = 438;
  int stopPoint = crosswalkPosition - 5;

  if (lightState == 0 && carPosition >= stopPoint &&
      carPosition <= crosswalkPosition) {
    // Stop at red light
  } else {
    carPosition -= busSpeed;
  }

  if (carPosition < -100) {
    carPosition = 800;
  }

  glutPostRedisplay();
  glutTimerFunc(20, updatePosition2, 0);
}

void updatePosition3(int value) {
  int crosswalkPosition = 300;
  int stopPoint = crosswalkPosition - 30;

  if (lightState == 0 && bx + 90 >= stopPoint && bx + 90 <= crosswalkPosition) {
    // Stop at red light
  } else {
    bx += CarSpeed;
  }

  if (bx > 800) {
    bx = -100;
  }

  glutPostRedisplay();
  glutTimerFunc(20, updatePosition3, 0);
}

// ==================== WATER REFLECTION USING REFLECTION ALGORITHM
// ====================
void drawWaterReflection() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(0.3f, 0.5f, 0.7f, 0.5f);

  glBegin(GL_QUADS);
  glVertex2f(0, 0);
  glVertex2f(800, 0);
  glVertex2f(800, 150);
  glVertex2f(0, 150);
  glEnd();

  // Draw reflected buildings using reflection
  glPushMatrix();
  glScalef(1.0, -0.5, 1.0);
  glTranslatef(0, -300, 0);
  glColor4f(0.6f, 0.6f, 0.6f, 0.3f);
  // Building reflection would be drawn here
  glPopMatrix();

  glDisable(GL_BLEND);
}

// ==================== BRIDGE USING DDA ALGORITHM ====================
void drawBridgeWithDDA() {
  // Bridge planks using DDA lines
  glColor3ub(139, 69, 19);
  for (int i = 0; i <= 145; i += 15) {
    float leftX = 370 + (i / 3.5);
    float rightX = 460 - (i / 3.5);
    drawDDALine(leftX, i, rightX, i); // DDA used for bridge planks
  }

  // Bridge outline using Bresenham
  glColor3ub(205, 133, 63);
  drawBresenhamLine(370, 0, 400, 150);   // Bresenham for left edge
  drawBresenhamLine(460, 0, 430, 150);   // Bresenham for right edge
  drawBresenhamLine(370, 0, 460, 0);     // Bresenham for bottom edge
  drawBresenhamLine(400, 150, 430, 150); // Bresenham for top edge
}

// ==================== LAMP POST USING BRESENHAM ALGORITHM ====================
void drawLampPostWithBresenham() {
  float x = 450;
  float w = 250;
  float y = 50;
  float z = 650;

  glColor3ub(20, 20, 20);

  // Lamp post 1 - Using Bresenham for vertical poles
  drawBresenhamLine(z - 10, 150, z + 10, 150); // Base
  drawBresenhamLine(z - 3, 170, z + 3, 270);   // Pole
  
  // Lamp post 2
  drawBresenhamLine(x - 10, 150, x + 10, 150);
  drawBresenhamLine(x - 3, 170, x + 3, 270);

  // Lamp post 3
  drawBresenhamLine(w - 10, 150, w + 10, 150);
  drawBresenhamLine(w - 3, 170, w + 3, 270);

  // Lamp post 4
  drawBresenhamLine(y - 10, 150, y + 10, 150);
  drawBresenhamLine(y - 3, 170, y + 3, 270);

  // Lamp bulbs using Midpoint Circle
  if (isNight) {
    glColor3ub(255, 255, 200);
  } else {
    glColor3ub(150, 150, 100);
  }
  drawMidpointCircle(z, 275, 5);
  drawMidpointCircle(x, 275, 5);
  drawMidpointCircle(w, 275, 5);
  drawMidpointCircle(y, 275, 5);
}

// ==================== SHEARED CAR DEMONSTRATION ====================
void drawShearedCar() {
  glPushMatrix();
  glTranslatef(600, 180, 0); // Position on road

  // Original car shape (will be sheared)
  float carBody[4][2] = {{-30, 0}, {30, 0}, {30, 20}, {-30, 20}};
  float carRoof[4][2] = {{-20, 20}, {20, 20}, {15, 35}, {-15, 35}};
  float wheel1[4][2] = {{-20, -5}, {-10, -5}, {-10, 0}, {-20, 0}};
  float wheel2[4][2] = {{10, -5}, {20, -5}, {20, 0}, {10, 0}};

  // Apply shear transformation to create slanted car effect
  float shx = 0.3; // Shear in X direction
  float shy = 0.0; // No shear in Y

  for (int i = 0; i < 4; i++) {
    applyShear(carBody[i][0], carBody[i][1], shx, shy);
    applyShear(carRoof[i][0], carRoof[i][1], shx, shy);
    applyShear(wheel1[i][0], wheel1[i][1], shx, shy);
    applyShear(wheel2[i][0], wheel2[i][1], shx, shy);
  }

  // Draw sheared car body
  glColor3ub(0, 255, 255); // Cyan color for sheared car
  glBegin(GL_POLYGON);
  for (int i = 0; i < 4; i++) {
    glVertex2f(carBody[i][0], carBody[i][1]);
  }
  glEnd();

  // Draw sheared car roof
  glColor3ub(100, 100, 100);
  glBegin(GL_POLYGON);
  for (int i = 0; i < 4; i++) {
    glVertex2f(carRoof[i][0], carRoof[i][1]);
  }
  glEnd();

  // Draw wheels with Midpoint Circle
  glColor3ub(0, 0, 0);
  drawMidpointCircle(-15, -2, 8);
  drawMidpointCircle(15, -2, 8);

  glPopMatrix();
}

// ==================== ROAD MARKINGS USING DDA ====================
void drawRoadMarkingsWithDDA() {
  // Dashed road center line using DDA
  glColor3f(1.0, 1.0, 1.0);
  for (int i = 0; i < 800; i += 100) {
    drawDDALine(i, 225, i + 50, 225); // DDA for road markings
  }

  // Crosswalk stripes using DDA
  for (int i = 0; i < 15; i++) {
    int stripeY = 160 + i * 15;
    if (stripeY + 5 > 300)
      break;

    float leftX = 300 + i * 2;
    float rightX = 350 - i * 2;

    drawDDALine(leftX, stripeY, rightX, stripeY);
    drawDDALine(leftX - 1, stripeY + 5, rightX + 1, stripeY + 5);
  }
}

// ==================== TRAFFIC LIGHT WITH BRESENHAM ====================
void drawTrafficLightWithBresenham() {
  // Traffic light pole using Bresenham
  glColor3ub(25, 25, 25);
  drawBresenhamLine(349, 150, 351, 300); // First pole
  drawBresenhamLine(334, 300, 336, 400); // Second pole

  // Traffic Light Box
  glBegin(GL_QUADS);
  glColor3ub(0, 0, 0);
  glVertex2f(342, 300);
  glVertex2f(358, 300);
  glVertex2f(358, 350);
  glVertex2f(342, 350);
  glEnd();

  glBegin(GL_QUADS);
  glColor3ub(0, 0, 0);
  glVertex2f(325, 400);
  glVertex2f(345, 400);
  glVertex2f(345, 450);
  glVertex2f(325, 450);
  glEnd();

  // Lights using Midpoint Circle
  if (activeLight == 'R') {
    glColor3ub(255, 0, 0);
    drawMidpointCircle(350, 340, 7);
    drawMidpointCircle(335, 440, 7);
  } else {
    glColor3ub(50, 50, 50);
    drawMidpointCircle(350, 340, 7);
    drawMidpointCircle(335, 440, 7);
  }

  if (activeLight == 'Y') {
    glColor3ub(255, 255, 0);
    drawMidpointCircle(350, 325, 7);
    drawMidpointCircle(335, 425, 7);
  } else {
    glColor3ub(50, 50, 50);
    drawMidpointCircle(350, 325, 7);
    drawMidpointCircle(335, 425, 7);
  }

  if (activeLight == 'G') {
    glColor3ub(0, 255, 0);
    drawMidpointCircle(350, 310, 7);
    drawMidpointCircle(335, 410, 7);
  } else {
    glColor3ub(50, 50, 50);
    drawMidpointCircle(350, 310, 7);
    drawMidpointCircle(335, 410, 7);
  }
}

void drawSunAndClouds() {
  glBegin(GL_QUADS);
  glColor3ub(isNight ? 0 : 0, isNight ? 0 : 119, isNight ? 60 : 190);
  glVertex2f(0, 0);
  glVertex2f(800, 0);
  glVertex2f(800, 150);
  glVertex2f(0, 150);
  glEnd();

  drawWaterReflection();

  glBegin(GL_QUADS);
  glColor3f(0.2, 0.2, 0.2);
  glVertex2f(0, 150);
  glVertex2f(800, 150);
  glVertex2f(800, 300);
  glVertex2f(0, 300);
  glEnd();

  // Road markings using DDA
  drawRoadMarkingsWithDDA();

  // Bridge using DDA and Bresenham
  drawBridgeWithDDA();

  if (isNight) {
    // Moon using Midpoint Circle
    glColor3ub(255, 255, 255);
    drawMidpointCircle(350, 705, 36);
    glColor3ub(0, 0, 0);
    drawMidpointCircle(360, 710, 30);

    // Dark clouds using Midpoint Circle
    glColor3ub(50, 50, 50);
    drawMidpointCircle(400, 665, 20);
    drawMidpointCircle(410, 675, 20);
    drawMidpointCircle(410, 655, 20);
    drawMidpointCircle(420, 680, 20);
    drawMidpointCircle(p + 200, 745, 20);
    drawMidpointCircle(p + 210, 755, 20);
    drawMidpointCircle(p + 210, 735, 20);
    drawMidpointCircle(p + 220, 750, 20);
  } else {
    // Sun using Midpoint Circle
    glColor3ub(253, 183, 77);
    drawMidpointCircle(400, 705, 36);

    // Light clouds using Midpoint Circle
    glColor3ub(232, 241, 255);
    drawMidpointCircle(400, 665, 20);
    drawMidpointCircle(410, 675, 20);
    drawMidpointCircle(410, 655, 20);
    drawMidpointCircle(420, 680, 20);
    drawMidpointCircle(p + 200, 745, 20);
    drawMidpointCircle(p + 210, 755, 20);
    drawMidpointCircle(p + 210, 735, 20);
    drawMidpointCircle(p + 220, 750, 20);
  }
}

void drawbuilding() {
  glPushMatrix();
  glTranslated(30.0, 250.0, 0.0);
  glScalef(0.9, 0.9, 1.0);

  // Building outlines using Bresenham
  glColor3ub(100, 100, 100);
  drawBresenhamLine(50, 90, 110, 90);
  drawBresenhamLine(110, 90, 110, 475);
  drawBresenhamLine(110, 475, 50, 475);
  drawBresenhamLine(50, 475, 50, 90);

  glBegin(GL_QUADS);
  glColor3ub(204, 204, 204);
  glVertex2f(50, 90);
  glVertex2f(110, 90);
  glVertex2f(110, 475);
  glVertex2f(50, 475);
  glEnd();

  glBegin(GL_QUADS);
  glColor3ub(242, 242, 242);
  glVertex2f(52.5, 90);
  glVertex2f(106, 90);
  glVertex2f(106, 460);
  glVertex2f(52.5, 460);
  glEnd();

  glBegin(GL_QUADS);
  glColor3ub(204, 204, 204);
  glVertex2f(110, 90);
  glVertex2f(140, 90);
  glVertex2f(140, 420);
  glVertex2f(110, 420);
  glEnd();

  glBegin(GL_QUADS);
  glColor3ub(242, 242, 242);
  glVertex2f(110, 90);
  glVertex2f(137.5, 90);
  glVertex2f(137.5, 410);
  glVertex2f(110, 410);
  glEnd();

  // Windows using Midpoint Circle
  glColor3ub(177, 124, 119);
  drawMidpointCircle(125, 385, 8);
  drawMidpointCircle(145, 385, 8);
  drawMidpointCircle(165, 385, 8);

  glColor3ub(85, 119, 119);
  drawMidpointCircle(100, 330, 8);
  drawMidpointCircle(120, 330, 8);

  glPopMatrix();

  glPushMatrix();
  glTranslated(10.0, 250.0, 0.0);
  glScalef(0.9, 0.9, 1.0);

  // Trees using Midpoint Circle
  drawMidpointCircle(0, 150, 22);
  drawMidpointCircle(10, 170, 22);
  drawMidpointCircle(13, 140, 22);
  drawMidpointCircle(22, 150, 25);
  drawMidpointCircle(30, 150, 22);
  drawMidpointCircle(0, 250, 40);

  glPopMatrix();

  glPushMatrix();
  glTranslated(50.0, 220.0, 0.0);
  glScalef(0.9, 0.9, 1.0);

  glBegin(GL_QUADS);
  glColor3ub(204, 204, 204);
  glVertex2f(200, 120);
  glVertex2f(287, 120);
  glVertex2f(287, 395);
  glVertex2f(200, 395);
  glEnd();

  // Windows with Midpoint Circle
  glColor3ub(128, 197, 215);
  drawMidpointCircle(230, 140, 6);
  drawMidpointCircle(250, 140, 6);
  drawMidpointCircle(270, 140, 6);

  glPopMatrix();

  glPushMatrix();
  glTranslated(103.0, 250.0, 0.0);
  glScalef(0.9, 0.9, 1.0);

  glBegin(GL_QUADS);
  glColor3ub(204, 204, 204);
  glVertex2f(575, 90);
  glVertex2f(635, 90);
  glVertex2f(635, 475);
  glVertex2f(575, 475);
  glEnd();

  glPopMatrix();
}

void drawBus() {
  int roadBase = 150;
  int busOffset = roadBase - 20;

  glBegin(GL_QUADS);
  glColor3ub(255, 81, 76);
  glVertex2f(j + 10, busOffset + 80);
  glVertex2f(j + 90, busOffset + 80);
  glVertex2f(j + 90, busOffset + 105);
  glVertex2f(j + 10, busOffset + 105);
  glEnd();

  glBegin(GL_QUADS);
  glColor3ub(255, 81, 76);
  glVertex2f(j + 10, busOffset + 55);
  glVertex2f(j + 92, busOffset + 55);
  glVertex2f(j + 92, busOffset + 80);
  glVertex2f(j + 10, busOffset + 80);
  glEnd();

  glBegin(GL_QUADS);
  glColor3ub(0, 51, 0);
  glVertex2f(j + 11, busOffset + 81);
  glVertex2f(j + 89, busOffset + 81);
  glVertex2f(j + 89, busOffset + 102);
  glVertex2f(j + 11, busOffset + 102);
  glEnd();

  // Windows
  glColor3ub(230, 255, 255);
  glBegin(GL_QUADS);
  glVertex2f(j + 12, busOffset + 85);
  glVertex2f(j + 20, busOffset + 85);
  glVertex2f(j + 20, busOffset + 100);
  glVertex2f(j + 12, busOffset + 100);
  glEnd();

  glBegin(GL_QUADS);
  glVertex2f(j + 22, busOffset + 85);
  glVertex2f(j + 30, busOffset + 85);
  glVertex2f(j + 30, busOffset + 100);
  glVertex2f(j + 22, busOffset + 100);
  glEnd();

  // Wheels using Midpoint Circle
  glColor3ub(0, 0, 0);
  drawMidpointCircle(j + 25, busOffset + 55, 10);
  drawMidpointCircle(j + 78, busOffset + 55, 10);

  glColor3ub(255, 255, 255);
  drawMidpointCircle(j + 25, busOffset + 55, 6);
  drawMidpointCircle(j + 78, busOffset + 55, 6);
}

void drawCar() {
  int roadBase = 200;
  int carOffset = roadBase - 5;

  glBegin(GL_QUADS);
  glColor3ub(255, 81, 76);
  glVertex2f(carPosition - 10, carOffset + 80);
  glVertex2f(carPosition - 90, carOffset + 80);
  glVertex2f(carPosition - 90, carOffset + 105);
  glVertex2f(carPosition - 10, carOffset + 105);
  glEnd();

  glBegin(GL_QUADS);
  glColor3ub(255, 81, 76);
  glVertex2f(carPosition - 10, carOffset + 55);
  glVertex2f(carPosition - 92, carOffset + 55);
  glVertex2f(carPosition - 92, carOffset + 80);
  glVertex2f(carPosition - 10, carOffset + 80);
  glEnd();

  glBegin(GL_QUADS);
  glColor3ub(0, 51, 0);
  glVertex2f(carPosition - 11, carOffset + 81);
  glVertex2f(carPosition - 89, carOffset + 81);
  glVertex2f(carPosition - 89, carOffset + 102);
  glVertex2f(carPosition - 11, carOffset + 102);
  glEnd();

  // Wheels using Midpoint Circle
  glColor3ub(0, 0, 0);
  drawMidpointCircle(carPosition - 25, carOffset + 55, 10);
  drawMidpointCircle(carPosition - 78, carOffset + 55, 10);

  glColor3ub(255, 255, 255);
  drawMidpointCircle(carPosition - 25, carOffset + 55, 6);
  drawMidpointCircle(carPosition - 78, carOffset + 55, 6);
}

void drawCar2() {
  int roadBase = 150;
  int busOffset = roadBase - 40;
  glPushMatrix();
  glTranslatef(bx + 0, 100, 0);
  glScalef(0.5, 0.5, 1.0);
  glTranslatef(-450, -100, 0);

  glColor3ub(255, 0, 0);
  glBegin(GL_POLYGON);
  glVertex2d(bx + 410, busOffset + 100);
  glVertex2d(bx + 490, busOffset + 100);
  glVertex2d(bx + 485, busOffset + 130);
  glVertex2d(bx + 410, busOffset + 130);
  glEnd();

  // Wheels using Midpoint Circle
  glColor3ub(0, 0, 0);
  drawMidpointCircle(bx + 435, busOffset + 100, 14);
  drawMidpointCircle(bx + 465, busOffset + 100, 14);

  glColor3ub(245, 245, 245);
  drawMidpointCircle(bx + 435, busOffset + 100, 10);
  drawMidpointCircle(bx + 465, busOffset + 100, 10);

  glPopMatrix();
}

void drawTrafficLight() {
  drawTrafficLightWithBresenham(); // Using Bresenham algorithm
}

void drawTrafficLight2() {
  // Already included in drawTrafficLightWithBresenham()
}

float boatX = 0.0;

// ==================== BOAT WITH DDA FOR DETAILS ====================
void drawBoatWithDDA() {
  glPushMatrix();
  glTranslatef(boatX + 0.0, 30.0, 0.0);
  glScalef(6.0, 6.0, 1.0);

  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_POLYGON);
  glVertex2f(boatX + 3.5f, 5.5f);
  glVertex2f(boatX + 3.5f, 8.5f);
  glVertex2f(boatX + 19.5f, 8.5f);
  glVertex2f(boatX + 19.5f, 5.5f);
  glEnd();

  glColor3f(0.0f, 0.0f, 1.0f);
  glBegin(GL_POLYGON);
  glVertex2f(boatX + 1.0f, 5.5f);
  glVertex2f(boatX + 4.0f, 1.0f);
  glVertex2f(boatX + 19.0f, 1.0f);
  glVertex2f(boatX + 21.5f, 5.5f);
  glEnd();

  // Boat details using DDA
  glColor3f(0.7f, 0.4f, 0.2f);
  drawDDALine(boatX + 4.0f, 5.5f, boatX + 4.0f, 8.0f);
  drawDDALine(boatX + 5.0f, 5.5f, boatX + 5.0f, 8.0f);

  glPopMatrix();
}

void drawBoat() {
  drawBoatWithDDA(); // Using DDA for boat details
}

void drawLampPost() {
  drawLampPostWithBresenham(); // Using Bresenham for lamp posts
}

void drawTree() {
  glColor3ub(75, 35, 5);
  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(550, 150, 0);
  glVertex3f(560, 150, 0);
  glVertex3f(560, 190, 0);
  glVertex3f(550, 190, 0);
  glEnd();

  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(380, 150, 0);
  glVertex3f(390, 150, 0);
  glVertex3f(390, 190, 0);
  glVertex3f(380, 190, 0);
  glEnd();

  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(140, 150, 0);
  glVertex3f(150, 150, 0);
  glVertex3f(150, 190, 0);
  glVertex3f(140, 190, 0);
  glEnd();

  // Tree leaves using Midpoint Circle
  glColor3ub(0, 102, 0);
  drawMidpointCircle(555, 210, 30);
  drawMidpointCircle(385, 210, 30);
  drawMidpointCircle(145, 210, 30);
}

float humanY = 160;
float stepSize = 0.2;
int stepFrame = 0;

void drawHuman(float x, float y, int step) {
  glPushMatrix();
  glTranslatef(x, y, 0);
  glScalef(2.0, 2.0, 1.0f);

  glColor3f(0.0, 0.0, 1.0);
  glBegin(GL_POLYGON);
  glVertex2f(-3, 0);
  glVertex2f(3, 0);
  glVertex2f(3, 10);
  glVertex2f(-3, 10);
  glEnd();

  // Head using Midpoint Circle
  glColor3f(1.0, 0.8, 0.6);
  drawMidpointCircle(0, 12, 2);

  glColor3f(1.0f, 0.2f, 0.2f);
  if (step % 20 < 10) {
    glBegin(GL_POLYGON);
    glVertex2f(-2, 0);
    glVertex2f(-3, 0);
    glVertex2f(-3, -7);
    glVertex2f(-2, -7);
    glEnd();
  } else {
    glBegin(GL_POLYGON);
    glVertex2f(2, 0);
    glVertex2f(3, 0);
    glVertex2f(3, -7);
    glVertex2f(2, -7);
    glEnd();
  }

  glPopMatrix();
}

void updateHuman(int value) {
  int crosswalkPosition = 160;
  int stopPoint = crosswalkPosition - 5;

  if ((lightState == 2 || lightState == 1) && humanY >= stopPoint &&
      humanY <= crosswalkPosition) {
    // Stop at red light
  } else {
    humanY += stepSize;
    stepFrame++;
  }

  if (humanY > 300) {
    humanY = 160;
  }

  glutPostRedisplay();
  glutTimerFunc(16, updateHuman, 0);
}

void updateBoat(int value) {
  boatX += 0.5;
  if (boatX > 150) {
    boatX = -50;
  }
  glutPostRedisplay();
  glutTimerFunc(20, updateBoat, 0);
}

struct Raindrop {
  float x, y;
  float speed;
} rain[MAX_RAIN];

void initRain() {
  for (int i = 0; i < MAX_RAIN; i++) {
    rain[i].x = (rand() % SCREEN_WIDTH);
    rain[i].y = (rand() % SCREEN_HEIGHT);
    rain[i].speed = (rand() % 5) + 2;
  }
}

void drawRain() {
  if (!isRaining)
    return;

  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINES);
  for (int i = 0; i < MAX_RAIN; i++) {
    glVertex2f(rain[i].x, rain[i].y);
    glVertex2f(rain[i].x, rain[i].y - 10);
  }
  glEnd();
}

void updateRain(int value) {
  if (isRaining) {
    for (int i = 0; i < MAX_RAIN; i++) {
      rain[i].y -= rain[i].speed;
      if (rain[i].y < 0) {
        rain[i].x = rand() % SCREEN_WIDTH;
        rain[i].y = SCREEN_HEIGHT;
      }
    }
  }
  glutPostRedisplay();
  glutTimerFunc(30, updateRain, 0);
}

struct Snow {
  float x, y;
  float speed;
  float drift;
} snow[MAX_SNOW];

bool isSnowing = false;

void initSnow() {
  for (int i = 0; i < MAX_SNOW; i++) {
    snow[i].x = rand() % SCREEN_WIDTH;
    snow[i].y = rand() % SCREEN_HEIGHT + SCREEN_HEIGHT / 2;
    snow[i].speed = (rand() % 2) + 1;
    snow[i].drift = (rand() % 3) - 1;
  }
}

void updateSnow(int value) {
  if (isSnowing) {
    for (int i = 0; i < MAX_SNOW; i++) {
      snow[i].y -= snow[i].speed;
      snow[i].x += snow[i].drift;
      if (snow[i].y < 0) {
        snow[i].x = rand() % SCREEN_WIDTH;
        snow[i].y = SCREEN_HEIGHT;
        snow[i].drift = (rand() % 3) - 1;
      }
    }
  }
  glutPostRedisplay();
  glutTimerFunc(50, updateSnow, 0);
}

void drawSnow() {
  if (isSnowing) {
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(3);
    glBegin(GL_POINTS);
    for (int i = 0; i < MAX_SNOW; i++) {
      glVertex2f(snow[i].x, snow[i].y);
    }
    glEnd();
  }
}

float birdX = -50;
float birdY = 600;
float wingAngle = 0;
bool wingUp = true;

void drawBird() {
  if (!isNight) {
    glColor3ub(0, 0, 0);
    glPushMatrix();
    glTranslatef(birdX, birdY + 80, 0);

    glBegin(GL_POLYGON);
    glVertex2f(-5, 0);
    glVertex2f(5, 2.5);
    glVertex2f(7.5, 0);
    glVertex2f(5, -2.5);
    glVertex2f(-5, 0);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(5, 1);
    glVertex2f(7.5, 1.5);
    glVertex2f(7.5, -1.5);
    glVertex2f(5, -1);
    glEnd();

    glColor3ub(255, 165, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(7.5, 0.5);
    glVertex2f(9, 0);
    glVertex2f(7.5, -0.5);
    glEnd();

    glPopMatrix();
  }
}

void updateBird(int value) {
  if (!isNight) {
    birdX += 5;
    if (birdX > 850)
      birdX = -50;

    if (wingUp)
      wingAngle += 5;
    else
      wingAngle -= 5;

    if (wingAngle >= 20)
      wingUp = false;
    if (wingAngle <= -20)
      wingUp = true;
  }
  glutPostRedisplay();
  glutTimerFunc(50, updateBird, 0);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  drawStars();
  drawSunAndClouds();
  drawBus();
  drawCar();
  drawCar2();

  drawTrafficLight();
  drawTrafficLight2();
  drawBoat();
  drawLampPost();
  drawTree();
  drawHuman(325, humanY, stepFrame);

  drawbuilding();
  drawBird();

  // Display sheared car as special effect
  drawShearedCar(); // Using Shear algorithm

  drawRain();
  drawSnow();

  glutSwapBuffers();
  glFlush();
}

void handleKeyboard(unsigned char key, int x, int y) {
  if (key == 'n' || key == 'N') {
    isNight = true;
    myInit();
    glutPostRedisplay();
  } else if (key == 'd' || key == 'D') {
    isNight = false;
    myInit();
    glutPostRedisplay();
  }

  if (key == 'p' || key == 'P') {
    isSnowing = !isSnowing;
    if (isSnowing) {
      isRaining = false;
      initSnow();
      glutTimerFunc(50, updateSnow, 0);
    }
  } else if (key == 'b' || key == 'B') {
    isRaining = !isRaining;
    if (isRaining) {
      isSnowing = false;
      initRain();
      glutTimerFunc(30, updateRain, 0);
    }
  }

  if (key == 'w') {
    busSpeed += 0.2;
    CarSpeed += 0.2;
  } else if (key == 's') {
    if (busSpeed > 0.2)
      busSpeed -= 0.2;
    if (CarSpeed > 0.2)
      CarSpeed -= 0.2;
  }

  if (key == 'r' || key == 'R') {
    activeLight = 'R';
    lightState = 0;
  } else if (key == 'y' || key == 'Y') {
    activeLight = 'Y';
    lightState = 1;
  } else if (key == 'g' || key == 'G') {
    activeLight = 'G';
    lightState = 2;
  }
}

void handleMouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    busSpeed += 0.2;
    CarSpeed += 0.2;
  } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    if (busSpeed > 0.2)
      busSpeed -= 0.2;
    if (CarSpeed > 0.2)
      CarSpeed -= 0.2;
  }
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

  int windowWidth = 1450;
  int windowHeight = 750;
  glutInitWindowSize(windowWidth, windowHeight);

  int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
  int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);

  int posX = (screenWidth - windowWidth) / 2;
  int posY = (screenHeight - windowHeight) / 2;

  glutInitWindowPosition(posX, posY);
  glutCreateWindow(
      "DIU SmartCity - Bresenham, DDA, Midpoint, Reflection & Shear");

  myInit();

  glutDisplayFunc(display);

  glutTimerFunc(20, updatePosition, 0);
  glutTimerFunc(20, updatePosition2, 0);
  glutTimerFunc(20, updatePosition3, 0);
  glutKeyboardFunc(handleKeyboard);
  glutMouseFunc(handleMouse);
  glutTimerFunc(30, updateRain, 0);
  glutTimerFunc(50, updateSnow, 0);
  glutTimerFunc(16, updateHuman, 0);
  glutTimerFunc(50, updateBird, 0);
  glutTimerFunc(20, updateBoat, 0);

  glutMainLoop();
}
