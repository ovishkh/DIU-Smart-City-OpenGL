#include <iostream>
#include <GLUT/glut.h>
#include <math.h>
#define pi 3.142857
#include <vector>
#include <cstdlib>
#define MAX_RAIN 1000  // Number of raindrops
#define MAX_SNOW 500
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800 // Maximum number of raindrops
using namespace std;

// ==================== ALGORITHM IMPLEMENTATIONS ====================

// 1. BRESENHAM LINE ALGORITHM - Used for drawing straight lines with integer precision
// Uses: Lamp posts, Traffic light poles, Building outlines
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
        if (x == x2 && y == y2) break;
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
// Uses: Bridge planks, Crosswalk lines, Road markings
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

// 3. MIDPOINT CIRCLE ALGORITHM - Efficient circle drawing without floating point
// Uses: Sun, Moon, Clouds, Wheels, Traffic lights, Lamp bulbs
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
void reflectX(float &x, float &y, float axisY = 150) {
    y = 2 * axisY - y;
}

void reflectY(float &x, float &y, float axisX = 400) {
    x = 2 * axisX - x;
}

// 5. SHEAR TRANSFORMATION - Creates slanted/italic effect on objects
void applyShear(float &x, float &y, float shx, float shy) {
    float newX = x + shx * y;
    float newY = y + shy * x;
    x = newX;
    y = newY;
}

int lightState = 2;
float carPosition = 800;
float bx = -100;
float j = -100;
float k = 350;
float p = -50;
float busSpeed = 0.8;
float CarSpeed = 0.8;
float cloudSpeed = 0.3;
bool isNight = false; // Night mode toggle
bool isRaining = false; // Flag to control rain
char activeLight = 'N';


#define NUM_STARS 100
float starPositions[NUM_STARS][2];  // Store star positions
bool starsInitialized = false; // Flag to initialize stars once


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


void myInit() {

    if (isNight) {
        // Darker sky for night
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Night sky: dark blueish


    } else {
        // Lighter sky for day
        glClearColor(0.5f, 0.8f, 1.0f, 1.0f); // Day sky: light blue
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 700, 0, 800, -10.0, 10.0);

}
void circle(GLfloat rx, GLfloat ry, GLfloat cx, GLfloat cy) {
    // Updating algorithm to Midpoint Circle as requested
    // Note: This draws the outline using the algorithm
    drawMidpointCircle((int)cx, (int)cy, (int)((rx + ry) / 2));
}

void updatePosition(int value) {
  int crosswalkPosition = 300;  // X position of the crosswalk
    int stopPoint = crosswalkPosition - 5; // Bus should stop BEFORE the crosswalk

    if (lightState == 0 && j + 90 >= stopPoint && j + 90 <= crosswalkPosition) {
        // If the traffic light is RED and the bus is BEFORE the crosswalk, stop
    } else {
        j += busSpeed;  // Move normally if it's GREEN or the bus has already passed the crosswalk
    }

    if (j > 800) {
        j = -100;  // Reset Bus Position
    }


    if (p <= 800) {
        p +=cloudSpeed  ;
    } else {
        p = -50;
    }



    glutPostRedisplay();
    glutTimerFunc(20, updatePosition, 0);
}

void updatePosition2(int value) {
  int crosswalkPosition = 438;  // X position of the crosswalk
    int stopPoint = crosswalkPosition - 5; // Bus should stop BEFORE the crosswalk


   if (lightState == 0 && carPosition >= stopPoint && carPosition <= crosswalkPosition) {
        // Car stops, do nothing here
    } else {
        carPosition -= busSpeed;  // Move car left when the light is green or it has passed the crosswalk
    }

    if (carPosition < -100) {
        carPosition = 800;  // Reset Car Position once it moves off-screen
    }

    glutPostRedisplay();
    glutTimerFunc(20, updatePosition2, 0);
}

void updatePosition3(int value) {
  int crosswalkPosition = 300;  // X position of the crosswalk
    int stopPoint = crosswalkPosition - 30; // Bus should stop BEFORE the crosswalk


 if (lightState == 0 && bx + 90 >= stopPoint && bx + 90 <= crosswalkPosition) {
        // If the traffic light is RED and the bus is BEFORE the crosswalk, stop
    } else {
        bx += CarSpeed;  // Move normally if it's GREEN or the bus has already passed the crosswalk
    }

    if (bx > 800) {
        bx = -100;  // Reset Bus Position
    }

    glutPostRedisplay();
    glutTimerFunc(20, updatePosition3, 0);
}



  // Boat position


void drawSunAndClouds() {

    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(isNight ? 0 : 0, isNight ? 0 : 119, isNight ? 60 : 190);
    glVertex2f(0, 0);     // Bottom-left corner
    glVertex2f(800, 0);   // Bottom-right corner
    glVertex2f(800, 150);  // Top-right corner (higher water level)
    glVertex2f(0, 150);    // Top-left corner (higher water level)
    glEnd();

// Move the road higher
    glBegin(GL_QUADS);
    glColor3f(0.2, 0.2, 0.2); // Dark gray road
    glVertex2f(0, 150);    // Bottom-left of the road (moved up)
    glVertex2f(800, 150);  // Bottom-right of the road (moved up)
    glVertex2f(800, 300);  // Top-right of the road (moved up)
    glVertex2f(0, 300);    // Top-left of the road (moved up)
    glEnd();



// Draw the middle road marking (dashed white line) using DDA Algorithm
for (int i = 0; i < 800; i += 100) {
    glColor3f(1.0, 1.0, 1.0);
    drawDDALine(i, 225, i + 50, 225);
}

// Draw the bottom road marking
glBegin(GL_QUADS);
    glColor3f(0.2, 0.2, 0.2);
    glVertex2f(0, 155);  // Moved to match new road height
    glVertex2f(800, 155);
    glVertex2f(800, 160);
    glVertex2f(0, 160);
glEnd();
for (int i = 0; i < 15; i++) { // 10 stripes, but ensuring it stays within road bounds
    int stripeY = 160 + i * 15; // Start slightly above the water-road boundary

    if (stripeY + 5 > 300)  // Prevent stripes from exceeding road height
        break;

    float leftX = 300 + i * 2;  // Gradually shift inward
    float rightX = 350 - i * 2; // Gradually shift inward

    glBegin(GL_QUADS);
        glColor3f(1.0, 1.0, 1.0); // White stripes
        glVertex2f(leftX, stripeY);    // Bottom-left
        glVertex2f(rightX, stripeY);   // Bottom-right
        glVertex2f(rightX + 1, stripeY + 5); // Top-right (narrowing effect)
        glVertex2f(leftX - 1, stripeY + 5); // Top-left
    glEnd();
}

glBegin(GL_QUADS);


// Main Bridge Structure
glBegin(GL_QUADS);
glColor3ub(205, 133, 63); // Brown wooden bridge
glVertex2f(370, 0);   // Bottom-left (wide part in river)
glVertex2f(460, 0);   // Bottom-right
glVertex2f(430, 150); // Top-right (narrower end)
glVertex2f(400, 150); // Top-left (narrower end)
glEnd();

// Adding perspective-aligned planks using DDA
glColor3ub(139, 69, 19); // Darker brown for plank lines
for (int i = 0; i <= 145; i += 15) { // Adjust stripe spacing
    float leftX = 370 + (i / 3.5);   // Left side shifts inward as it goes up
    float rightX = 460 - (i / 3.5);  // Right side shifts inward to match perspective
    drawDDALine(leftX, i, rightX, i);
}




 if (isNight) {
    // White moon at night
glColor3ub(255, 255, 255); // Moon color
circle(18, 36, 350, 705);  // Moved the moon more to the left (x-coordinate changed to 350)

glColor3ub(0, 0, 0); // Moon shadow effect
circle(14, 30, 360, 710);  // Adjusted shadow position accordingly


    // Dark gray clouds (night)
    glColor3ub(50, 50, 50);
    circle(13, 20, 400, 665);
    glColor3ub(50, 50, 50);
    circle(11, 18, 400, 665);

    glColor3ub(50, 50, 50);
    circle(10, 20, 410, 675);
    glColor3ub(50, 50, 50);
    circle(10, 20, 412, 672);

    glColor3ub(50, 50, 50);
    circle(13, 20, 410, 655);
    glColor3ub(50, 50, 50);
    circle(9, 20, 420, 680);
    glColor3ub(50, 50, 50);
    circle(8, 18, 420, 679);

    glColor3ub(50, 50, 50);
    circle(9, 20, 420, 650);
    glColor3ub(50, 50, 50);
    circle(8, 18, 420, 652);

    glColor3ub(50, 50, 50);
    circle(9, 20, 430, 685);
    glColor3ub(50, 50, 50);
    circle(8, 18, 430, 683);

    glColor3ub(50, 50, 50);
    circle(9, 20, 425, 655);
    glColor3ub(50, 50, 50);
    circle(8, 18, 435, 657);

    glColor3ub(50, 50, 50);
    circle(9, 20, 440, 675);
    glColor3ub(50, 50, 50);
    circle(8, 18, 445, 665);
    glColor3ub(50, 50, 50);
    circle(8, 18, 443, 663);
    glColor3ub(50, 50, 50);
    circle(18, 18, 420, 664);
    glColor3ub(50, 50, 50);
    circle(18, 25, 417, 665);

    // Keep cloud shapes (day) the same, just change color
    glColor3ub(50, 50, 50);
    circle(13, 20, p+200, 745);
    glColor3ub(50, 50, 50);
    circle(11, 18, p+200, 745);
    glColor3ub(50, 50, 50);
    circle(10, 20, p+210, 755);
    glColor3ub(50, 50, 50);
    circle(10, 20, p+212, 762);

    glColor3ub(50, 50, 50);
    circle(13, 20, p+210, 735);
    glColor3ub(50, 50, 50);
    circle(9, 20, p+220, 750);
    glColor3ub(50, 50, 50);
    circle(8, 18, p+220, 759);

    glColor3ub(50, 50, 50);
    circle(9, 20, p+220, 756);
    glColor3ub(50, 50, 50);
    circle(8, 18, p+220, 752);

    glColor3ub(50, 50, 50);
    circle(9, 20, p+230, 765);
    glColor3ub(50, 50, 50);
    circle(8, 18, p+230, 761);

    glColor3ub(50, 50, 50);
    circle(9, 20, p+225, 745);
    glColor3ub(50, 50, 50);
    circle(8, 18, p+235, 747);

    glColor3ub(50, 50, 50);
    circle(9, 20, p+240, 755);
    glColor3ub(50, 50, 50);
    circle(8, 18, p+243, 745);
    glColor3ub(50, 50, 50);
    circle(8, 18, p+240, 743);
    glColor3ub(50, 50, 50);
    circle(8, 18, p+230, 733);
    glColor3ub(50, 50, 50);
    circle(8, 18, p+230, 738);
    glColor3ub(50, 50, 50);
    circle(8, 18, p+220, 723);
    glColor3ub(50, 50, 50);
    circle(8, 18, p+220, 728);

} else {
    // Yellow sun during the day
    glColor3ub(253, 183, 77); // Sun color
    circle(18, 36, 400, 705);

    // Light gray clouds (day)
    glColor3ub(232, 241, 255);
    circle(13, 20, 400, 665);
    glColor3ub(252, 254, 255);
    circle(11, 18, 400, 665);

    glColor3ub(232, 241, 255);
    circle(10, 20, 410, 675);
    glColor3ub(252, 254, 255);
    circle(10, 20, 412, 672);

    glColor3ub(232, 241, 255);
    circle(13, 20, 410, 655);
    glColor3ub(221, 229, 247);
    circle(9, 20, 420, 680);
    glColor3ub(252, 254, 255);
    circle(8, 18, 420, 679);

    glColor3ub(232, 241, 255);
    circle(9, 20, 420, 650);
    glColor3ub(252, 254, 255);
    circle(8, 18, 420, 652);

    glColor3ub(221, 229, 247);
    circle(9, 20, 430, 685);
    glColor3ub(252, 254, 255);
    circle(8, 18, 430, 683);

    glColor3ub(232, 241, 255);
    circle(9, 20, 425, 655);
    glColor3ub(252, 254, 255);
    circle(8, 18, 435, 657);

    glColor3ub(232, 241, 255);
    circle(9, 20, 440, 675);
    glColor3ub(221, 229, 247);
    circle(8, 18, 445, 665);
    glColor3ub(252, 254, 255);
    circle(8, 18, 443, 663);
    glColor3ub(252, 254, 255);
    circle(18, 18, 420, 664);
    glColor3ub(252, 254, 255);
    circle(18, 25, 417, 665);

    // Keep cloud shapes (day) the same, just change color
    glColor3ub(232, 241, 255);
    circle(13, 20, p+200, 745);
    glColor3ub(252, 254, 255);
    circle(11, 18, p+200, 745);
    glColor3ub(232, 241, 255);
    circle(10, 20, p+210, 755);
    glColor3ub(252, 254, 255);
    circle(10, 20, p+212, 762);

    glColor3ub(232, 241, 255);
    circle(13, 20, p+210, 735);
    glColor3ub(221, 229, 247);
    circle(9, 20, p+220, 750);
    glColor3ub(252, 254, 255);
    circle(8, 18, p+220, 759);

    glColor3ub(232, 241, 255);
    circle(9, 20, p+220, 756);
    glColor3ub(252, 254, 255);
    circle(8, 18, p+220, 752);

    glColor3ub(232, 241, 255);
    circle(9, 20, p+230, 765);
    glColor3ub(252, 254, 255);
    circle(8, 18, p+230, 761);

    glColor3ub(232, 241, 255);
    circle(9, 20, p+225, 745);
    glColor3ub(252, 254, 255);
    circle(8, 18, p+235, 747);

    glColor3ub(232, 241, 255);
    circle(9, 20, p+240, 755);
    glColor3ub(232, 241, 255);
    circle(8, 18, p+243, 745);
    glColor3ub(252, 254, 255);
    circle(8, 18, p+240, 743);
    glColor3ub(173, 197, 224);
    circle(8, 18, p+230, 733);
    glColor3ub(252, 254, 255);
    circle(8, 18, p+230, 738);
    glColor3ub(173, 197, 224);
    circle(8, 18, p+220, 723);
    glColor3ub(252, 254, 255);
    circle(8, 18, p+220, 728);
}


}
   void drawbuilding(){


    glPushMatrix();
    glTranslated(30.0,250.0,0.0);
     glScalef(0.9, 0.9, 1.0);













    ///...1st Building main part-1
    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(50,90);
    glVertex2f(110,90);
    glVertex2f(110,475);
    glVertex2f(50,475);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(52.5,90);        //1st Building main part 2
    glVertex2f(106,90);
    glVertex2f(106,460);
    glVertex2f(52.5,460);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(110,90);         //1st Building 2nd part 2
    glVertex2f(140,90);
    glVertex2f(140,420);
    glVertex2f(110,420);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(110,90);         //1st Building 2nd part 1
    glVertex2f(137.5,90);
    glVertex2f(137.5,410);
    glVertex2f(110,410);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(113,370);   //part 2 1st Building 6th floor2..........outlook
    glVertex2f(137.5,370);
    glVertex2f(137.5,400);
    glVertex2f(113,400);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(123, 88, 71);
    glVertex2f(113,365);   //part 2 1st Building 6th floor2..........outlook
    glVertex2f(123,365);
    glVertex2f(123,400);
    glVertex2f(113,400);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);
    glVertex2f(118,365);   //Door 1st Building 6th floor2..........outlook-1
    glVertex2f(119,365);
    glVertex2f(119,392);
    glVertex2f(118,392);
    glEnd();

/////////////****
    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(90,90);  //1st Building 1st floor1
    glVertex2f(197,90);
    glVertex2f(197,145);
    glVertex2f(90,145);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(90,145);   //1st Building 2nd floor1
    glVertex2f(197,145);
    glVertex2f(197,200);
    glVertex2f(90,200);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(90,200);   //1st Building 3rd floor1
    glVertex2f(197,200);
    glVertex2f(197,255);
    glVertex2f(90,255);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(90,255);   //1st Building 4th floor1
    glVertex2f(197,255);
    glVertex2f(197,310);
    glVertex2f(90,310);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(90,310);   //1st Building 5th floor1
    glVertex2f(197,310);
    glVertex2f(197,365);
    glVertex2f(90,365);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(90,365);   //1st Building 6th floor1
    glVertex2f(197,365);
    glVertex2f(197,420);
    glVertex2f(90,420);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(90,420);   //1st Building 7th floor1
    glVertex2f(197,420);
    glVertex2f(197,475);
    glVertex2f(90,475);
    glEnd();



    glColor3ub(0, 51, 204);
    glRasterPos2i(133,465);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'D');
    glColor3ub(0, 51, 204);
    glRasterPos2i(139,465);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'I');
    glColor3ub(0, 51, 204);
    glRasterPos2i(143,465);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'U');
    


    glBegin(GL_QUADS);              ///SIRI GHORE.//
    glColor3ub(204, 204, 204);
    glVertex2f(90,475);
    glVertex2f(120,475);
    glVertex2f(120,515);
    glVertex2f(90,515);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);
    glVertex2f(92,475);
    glVertex2f(118,475);
    glVertex2f(118,510);
    glVertex2f(92,510);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(123, 88, 71);
    glVertex2f(100,475);
    glVertex2f(110,475);
    glVertex2f(110,505);
    glVertex2f(100,505);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);
    glVertex2f(105,483);
    glVertex2f(106,483);
    glVertex2f(106,492);
    glVertex2f(105,492);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(140, 140, 140);
    glVertex2f(65,475);
    glVertex2f(66,475);
    glVertex2f(66,600);
    glVertex2f(65,600);
    glEnd();

    glColor3ub(102, 102, 102);
    circle(3,6,64,580);
    glColor3ub(217, 217, 217);
    circle(2.5,5,64,580);
    glColor3ub(102, 102, 102);
    circle(2,4,67,565);
    glColor3ub(217, 217, 217);
    circle(1.5,3,67,565);

    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(110,420);   //part 2 1st Building 7th floor2 white
    glVertex2f(194,420);
    glVertex2f(194,460);
    glVertex2f(110,460);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(140,365);   //part 2 1st Building 6th floor2
    glVertex2f(194,365);
    glVertex2f(194,405);
    glVertex2f(140,405);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(140,310);   //part 2 1st Building 5th floor2
    glVertex2f(194,310);
    glVertex2f(194,350);
    glVertex2f(140,350);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(140,255);   //part 2 1st Building 4th floor2
    glVertex2f(194,255);
    glVertex2f(194,295);
    glVertex2f(140,295);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(140,200);   //part 2 1st Building 3rd floor2
    glVertex2f(194,200);
    glVertex2f(194,240);
    glVertex2f(140,240);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(140,145);   //part 2 1st Building 2nd floor2
    glVertex2f(194,145);
    glVertex2f(194,185);
    glVertex2f(140,185);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(140,90);  //part 2 1st Building 1st floor2
    glVertex2f(194,90);
    glVertex2f(194,130);
    glVertex2f(140,130);
    glEnd();



    ///design of main building....red and glass..............

    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(52.5,420);  //7 1st Building main part3
    glVertex2f(67,420);
    glVertex2f(67,460);
    glVertex2f(52.5,460);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(52.5,365);  //6 1st Building main part3
    glVertex2f(67,365);
    glVertex2f(67,405);
    glVertex2f(52.5,405);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(52.5,310);  //5 1st Building main part3
    glVertex2f(67,310);
    glVertex2f(67,350);
    glVertex2f(52.5,350);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(52.5,255);  //4 1st Building main part3
    glVertex2f(67,255);
    glVertex2f(67,295);
    glVertex2f(52.5,295);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(52.5,200);  //3 1st Building main part3
    glVertex2f(67,200);
    glVertex2f(67,240);
    glVertex2f(52.5,240);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(52.5,145);  //2 1st Building main part3
    glVertex2f(67,145);
    glVertex2f(67,185);
    glVertex2f(52.5,185);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(52.5,90);  //1 1st Building main part3
    glVertex2f(67,90);
    glVertex2f(67,130);
    glVertex2f(52.5,130);
    glEnd();
    /////////.....................
    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(95,420);  //7 1st Building main part3
    glVertex2f(106,420);
    glVertex2f(106,460);
    glVertex2f(95,460);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(95,365);  //6 1st Building main part3
    glVertex2f(106,365);
    glVertex2f(106,405);
    glVertex2f(95,405);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(95,310);  //5 1st Building main part3
    glVertex2f(106,310);
    glVertex2f(106,350);
    glVertex2f(95,350);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(95,255);  //4 1st Building main part3
    glVertex2f(106,255);
    glVertex2f(106,295);
    glVertex2f(95,295);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(95,200);  //3 1st Building main part3
    glVertex2f(106,200);
    glVertex2f(106,240);
    glVertex2f(95,240);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(95,145);  //2 1st Building main part3
    glVertex2f(106,145);
    glVertex2f(106,185);
    glVertex2f(95,185);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(143,175,175);
    glVertex2f(85,90);  //door
    glVertex2f(106,90);
    glVertex2f(106,130);
    glVertex2f(85,130);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(0, 0, 77);
    glVertex2f(95,90);  //door 1 main
    glVertex2f(96,90);
    glVertex2f(96,130);
    glVertex2f(95,130);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(96,100);  //door 1 main
    glVertex2f(97,100);
    glVertex2f(97,120);
    glVertex2f(96,120);
    glEnd();

    //////////............................

    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(50,420);  //7 1st Building main part3
    glVertex2f(110,420);
    glVertex2f(110,421);
    glVertex2f(50,421);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(50,365);  //6 1st Building main part3
    glVertex2f(137.5,365);
    glVertex2f(137.5,366);
    glVertex2f(50,366);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(50,310);  //5 1st Building main part3
    glVertex2f(110,310);
    glVertex2f(110,311);
    glVertex2f(50,311);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(50,255);  //4 1st Building main part3
    glVertex2f(110,255);
    glVertex2f(110,256);
    glVertex2f(50,256);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(50,199);  //3 1st Building main part3
    glVertex2f(110,199);
    glVertex2f(110,200);
    glVertex2f(50,200);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(50,145);  //2 1st Building main part3
    glVertex2f(110,145);
    glVertex2f(110,146);
    glVertex2f(50,146);
    glEnd();

    ///////...............................
    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(50,405);  //7 1st Building main part3
    glVertex2f(110,405);
    glVertex2f(110,406);
    glVertex2f(50,406);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(50,350);  //6 1st Building main part3
    glVertex2f(110,350);
    glVertex2f(110,351);
    glVertex2f(50,351);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(50,295);  //5 1st Building main part3
    glVertex2f(110,295);
    glVertex2f(110,296);
    glVertex2f(50,296);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(50,240);  //4  main part3
    glVertex2f(110,240);
    glVertex2f(110,241);
    glVertex2f(50,241);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(50,185);  //3 1st Building main part3
    glVertex2f(110,185);
    glVertex2f(110,186);
    glVertex2f(50,186);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(50,130);  //2 1st Building main part3
    glVertex2f(110,130);
    glVertex2f(110,131);
    glVertex2f(50,131);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(89, 89, 89);
    glVertex2f(140,90);  // part2
    glVertex2f(141,90);
    glVertex2f(141,420);
    glVertex2f(140,420);
    glEnd();


    glBegin(GL_QUADS);
    glColor3ub(143,175,175);
    glVertex2f(110,365);  //6 1st Building main part3
    glVertex2f(120,365);
    glVertex2f(120,385);
    glVertex2f(110,385);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(143,175,175);
    glVertex2f(128,365);  //6 1st Building main part3
    glVertex2f(137.5,365);
    glVertex2f(137.5,385);
    glVertex2f(128,385);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(57,80,80);
    glVertex2f(120,370);  //6 1st Building main part3
    glVertex2f(128,370);
    glVertex2f(128.5,372);
    glVertex2f(120,372);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(57,80,80);
    glVertex2f(120,379);  //6 1st Building main part3
    glVertex2f(128,379);
    glVertex2f(128.5,381);
    glVertex2f(120,381);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(110,310);  //5 2nd part2
    glVertex2f(137.5,310);
    glVertex2f(137.5,350);
    glVertex2f(110,350);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(110,255);  //4 2nd part3
    glVertex2f(137.5,255);
    glVertex2f(137.5,295);
    glVertex2f(110,295);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(110,200);  //3 2nd part3
    glVertex2f(137.5,200);
    glVertex2f(137.5,240);
    glVertex2f(110,240);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(110,145);  //2 2nd part3
    glVertex2f(137.5,145);
    glVertex2f(137.5,185);
    glVertex2f(110,185);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(143,175,175);
    glVertex2f(110,90);  //1 1st Building main part3
    glVertex2f(137.5,90);
    glVertex2f(137.5,130);
    glVertex2f(110,130);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(0, 0, 77);
    glVertex2f(117,90);  //1 1st Building main part3
    glVertex2f(118,90);
    glVertex2f(118,130);
    glVertex2f(117,130);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(0, 0, 77);
    glVertex2f(127,90);  //1 1st Building main part3
    glVertex2f(128,90);
    glVertex2f(128,130);
    glVertex2f(127,130);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(118,100);  //1 1st Building main part3
    glVertex2f(119,100);
    glVertex2f(119,120);
    glVertex2f(118,120);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(128,100);  //1 1st Building main part3
    glVertex2f(129,100);
    glVertex2f(129,120);
    glVertex2f(128,120);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(0, 26, 51);
    glVertex2f(110,90);   //partation
    glVertex2f(111,90);
    glVertex2f(111,475);
    glVertex2f(110,475);
    glEnd();

    //..7th Floor design...................

    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(113,425);   //part 2 1st Building 7th floor2
    glVertex2f(191,425);
    glVertex2f(191,455);
    glVertex2f(113,455);
    glEnd();

    //....door.....window
    glBegin(GL_QUADS);
    glColor3ub(123, 88, 71);
    glVertex2f(113,420);   //d1 1st Building 7th floor2
    glVertex2f(123,420);
    glVertex2f(123,455);
    glVertex2f(113,455);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);
    glVertex2f(118,430);   //d1 1st Building 7th floor2
    glVertex2f(119,430);
    glVertex2f(119,445);
    glVertex2f(118,445);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(133,432);   //w1 1st Building 7th floor2
    glVertex2f(153,432);
    glVertex2f(153,448);
    glVertex2f(133,448);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(143,175,175);
    glVertex2f(133.5,433);   //w1 1st Building 7th floor2
    glVertex2f(152.5,433);
    glVertex2f(152.5,447);
    glVertex2f(133.5,447);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(143,432);   //w1 1st Building 7th floor2
    glVertex2f(144,432);
    glVertex2f(144,448);
    glVertex2f(143,448);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(163,432);   //w2 1st Building 7th floor2
    glVertex2f(183,432);
    glVertex2f(183,448);
    glVertex2f(163,448);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(143,175,175);
    glVertex2f(163.5,433);   //w2 1st Building 7th floor2
    glVertex2f(182.5,433);
    glVertex2f(182.5,447);
    glVertex2f(163.5,447);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(173,432);   //w2 1st Building 7th floor2
    glVertex2f(174,432);
    glVertex2f(174,448);
    glVertex2f(173,448);
    glEnd();


    //....6th Floor Design..........Door window

    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(141,370);   //part 2 1st Building 6th floor2..........Inlook
    glVertex2f(191,370);
    glVertex2f(191,400);
    glVertex2f(141,400);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(141,377);   //w1 1st Building 6th floor2
    glVertex2f(153,377);
    glVertex2f(153,393);
    glVertex2f(141,393);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(143,175,175);
    glVertex2f(141,378);   //w1 1st Building 6th floor2
    glVertex2f(152.5,378);
    glVertex2f(152.5,392);
    glVertex2f(141,392);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(143,377);   //w1 1st Building 6th floor2
    glVertex2f(144,377);
    glVertex2f(144,393);
    glVertex2f(143,393);
    glEnd();


    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(163,377);   //w2 1st Building 6th floor2
    glVertex2f(183,377);
    glVertex2f(183,393);
    glVertex2f(163,393);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(143,175,175);
    glVertex2f(163.5,378);   //w2 1st Building 6th floor2
    glVertex2f(182.5,378);
    glVertex2f(182.5,392);
    glVertex2f(163.5,392);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(173,377);   //w2 1st Building 6th floor2
    glVertex2f(174,377);
    glVertex2f(174,393);
    glVertex2f(173,393);
    glEnd();


    ///5th floor......design

    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(141,315);   //part 2 1st Building 5th floor2.........
    glVertex2f(191,315);
    glVertex2f(191,345);
    glVertex2f(141,345);
    glEnd();


    ///....door.....window


    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(141,322);   //w1 1st Building 5th floor2
    glVertex2f(153,322);
    glVertex2f(153,338);
    glVertex2f(141,338);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(143,175,175);
    glVertex2f(141,323);   //w1 1st Building 5th floor2
    glVertex2f(152.5,323);
    glVertex2f(152.5,337);
    glVertex2f(141,337);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(143,322);   //w1 1st Building 5th floor2
    glVertex2f(144,322);
    glVertex2f(144,338);
    glVertex2f(143,338);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(163,322);   //w2 1st Building 5th floor2
    glVertex2f(183,322);
    glVertex2f(183,338);
    glVertex2f(163,338);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(143,175,175);
    glVertex2f(163.5,323);   //w2 1st Building 5th floor2
    glVertex2f(182.5,323);
    glVertex2f(182.5,337);
    glVertex2f(163.5,337);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(173,322);   //w2 1st Building 5th floor2
    glVertex2f(174,322);
    glVertex2f(174,338);
    glVertex2f(173,338);
    glEnd();


    ///....4TH floor design............

    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(141,260);   //part 2 1st Building 5th floor2.........
    glVertex2f(191,260);
    glVertex2f(191,290);
    glVertex2f(141,290);
    glEnd();


    ///....door.....window


    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(141,267);   //w1 1st Building 4th floor2
    glVertex2f(153,267);
    glVertex2f(153,283);
    glVertex2f(141,283);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(143,175,175);
    glVertex2f(141,268);   //w1 1st Building 4th floor2
    glVertex2f(152.5,268);
    glVertex2f(152.5,282);
    glVertex2f(141,282);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(143,267);   //w1 1st Building 4th floor2
    glVertex2f(144,267);
    glVertex2f(144,283);
    glVertex2f(143,283);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(163,267);   //w2 1st Building 4th floor2
    glVertex2f(183,267);
    glVertex2f(183,283);
    glVertex2f(163,283);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(143,175,175);
    glVertex2f(163.5,268);   //w2 1st Building 4th floor2
    glVertex2f(182.5,268);
    glVertex2f(182.5,282);
    glVertex2f(163.5,282);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(173,267);   //w2 1st Building 4th floor2
    glVertex2f(174,267);
    glVertex2f(174,283);
    glVertex2f(173,283);
    glEnd();


    ///....3TH floor design............

    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(141,205);   //part 2 1st Building 3th floor2.........
    glVertex2f(191,205);
    glVertex2f(191,235);
    glVertex2f(141,235);
    glEnd();

    ///....door.....window


    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(141,212);   //w1 1st Building 3rd floor2
    glVertex2f(153,212);
    glVertex2f(153,228);
    glVertex2f(141,228);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(143,175,175);
    glVertex2f(141,213);   //w1 1st Building 3rd floor2
    glVertex2f(152.5,213);
    glVertex2f(152.5,227);
    glVertex2f(141,227);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(143,212);   //w1 1st Building 3rd floor2
    glVertex2f(144,212);
    glVertex2f(144,228);
    glVertex2f(143,228);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(163,212);   //w2 1st Building 3rd floor2
    glVertex2f(183,212);
    glVertex2f(183,228);
    glVertex2f(163,228);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(143,175,175);
    glVertex2f(163.5,213);   //w2 1st Building 3rd floor2
    glVertex2f(182.5,213);
    glVertex2f(182.5,227);
    glVertex2f(163.5,227);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(173,212);   //w2 1st Building 3rd floor2
    glVertex2f(174,212);
    glVertex2f(174,228);
    glVertex2f(173,228);
    glEnd();


    ///....2nd floor design............

    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(141,150);   //part 2 1st Building 2nd floor2.........
    glVertex2f(191,150);
    glVertex2f(191,180);
    glVertex2f(141,180);
    glEnd();


    ///....door.....window


    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(141,157);   //w1 1st Building 2nd floor2
    glVertex2f(153,157);
    glVertex2f(153,173);
    glVertex2f(141,173);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(143,175,175);
    glVertex2f(141,158);   //w1 1st Building 2nd floor2
    glVertex2f(152.5,158);
    glVertex2f(152.5,172);
    glVertex2f(141,172);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(143,157);   //w1 1st Building 2nd floor2
    glVertex2f(144,157);
    glVertex2f(144,173);
    glVertex2f(143,173);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(163,157);   //w2 1st Building 2nd floor2
    glVertex2f(183,157);
    glVertex2f(183,173);
    glVertex2f(163,173);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(143,175,175);
    glVertex2f(163.5,158);   //w2 1st Building 2nd floor2
    glVertex2f(182.5,158);
    glVertex2f(182.5,172);
    glVertex2f(163.5,172);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(173,157);   //w2 1st Building 2nd floor2
    glVertex2f(174,157);
    glVertex2f(174,173);
    glVertex2f(173,173);
    glEnd();


    ///....1st floor design............

    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(141,95);   //part 2 1st Building 1st floor red.........
    glVertex2f(191,95);
    glVertex2f(191,125);
    glVertex2f(141,125);
    glEnd();


    ///....door.....window

    glBegin(GL_QUADS);
    glColor3ub(123, 88, 71);
    glVertex2f(196,90);   //d2 AB2 1st floor
    glVertex2f(206,90);
    glVertex2f(206,125);
    glVertex2f(196,125);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);
    glVertex2f(201,100);   //d2 1st Building 1st floor
    glVertex2f(202,100);
    glVertex2f(202,115);
    glVertex2f(201,115);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(141,102);   //w1 1st Building 1st floor2
    glVertex2f(153,102);
    glVertex2f(153,118);
    glVertex2f(141,118);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(143,175,175);
    glVertex2f(141,103);   //w1 1st Building 1st floor2
    glVertex2f(152.5,103);
    glVertex2f(152.5,117);
    glVertex2f(141,117);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(143,102);   //w1 1st Building 1st floor2
    glVertex2f(144,102);
    glVertex2f(144,118);
    glVertex2f(143,118);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(163,102);   //w2 1st Building 1st floor2
    glVertex2f(183,102);
    glVertex2f(183,118);
    glVertex2f(163,118);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(143,175,175);
    glVertex2f(163.5,103);   //w2 1st Building 1st floor2
    glVertex2f(182.5,103);
    glVertex2f(182.5,117);
    glVertex2f(163.5,117);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(173,102);   //w2 1st Building 1st floor2
    glVertex2f(174,102);
    glVertex2f(174,118);
    glVertex2f(173,118);
    glEnd();

    glPopMatrix();
    glPushMatrix();
    glTranslated(10.0,250.0,0.0);
     glScalef(0.9, 0.9, 1.0);


  //tree
    glColor3ub(139, 146, 22);
    circle(8,22,0,150);
    glColor3ub(139, 146, 22);
    circle(8,22,10,170);
    glColor3ub(139, 146, 22);
    circle(8,22,13,140);
    glColor3ub(139, 146, 22);
    circle(7,25,22,150);
    glColor3ub(139, 146, 22);
    circle(8,22,30,150);
    glColor3ub(139, 146, 22);
    circle(10,40,0,250);

    glColor3ub(139, 146, 22);
    circle(9,22,30,295);
    glColor3ub(139, 146, 22);
    circle(8,15,30,293);

    glColor3ub(139, 146, 22);
    circle(9,22,45,285);
    glColor3ub(139, 146, 22);
    circle(9,22,45,280);
    glColor3ub(139, 146, 22);
    circle(8,15,45,275);

    glColor3ub(139, 146, 22);
    circle(9,22,55,235);
    glColor3ub(139, 146, 22);
    circle(9,32,50,255);

    glColor3ub(139, 146, 22);
    circle(9,22,59,225);
    glColor3ub(139, 146, 22);
    circle(9,22,56,255);
    glColor3ub(139, 146, 22);
    circle(9,22,63,195);
    glColor3ub(139, 146, 22);
    circle(9,22,50,180);
    glColor3ub(139, 146, 22);
    circle(9,22,58,165);
    glColor3ub(139, 146, 22);
    circle(9,22,49,150);
    glColor3ub(139, 146, 22);
    circle(9,22,38,140);


    glColor3ub(0, 77, 26);
    circle(10,20,55,190);
    glColor3ub(139, 146, 22);
    circle(9.5,19,55,190);


    glColor3ub(0, 77, 26);
    circle(10,20,55,205);
    glColor3ub(139, 146, 22);
    circle(9.5,20,55,205);


    glColor3ub(0, 77, 26);
    circle(10,20,50,218);
    glColor3ub(139, 146, 22);
    circle(9.5,20,50,218);


    glColor3ub(139, 146, 22);
    circle(10,20,27,130);


    glColor3ub(139, 146, 22);
    circle(35,70,20,200);
    glColor3ub(139, 146, 22);    ///
    circle(15,30,30,255);


    glColor3ub(0, 77, 26);
    circle(10,20,42,225);
    glColor3ub(139, 146, 22);
    circle(9.5,20,42,224);


    glColor3ub(0, 77, 26);
    circle(10,20,30,225);
    glColor3ub(139, 146, 22);
    circle(10,20,30,224);


    glColor3ub(0, 77, 26);
    circle(10,18,42,263);
    glColor3ub(139, 146, 22);
    circle(10,18,42,262);


    glColor3ub(0, 77, 26);
    circle(10,20,30,180);
    glColor3ub(139, 146, 22);
    circle(10,20,30,179);

    glColor3ub(0, 77, 26);
    circle(10,20,20,180);
    glColor3ub(139, 146, 22);
    circle(10,20,20,179);

    glColor3ub(0, 77, 26);
    circle(10,20,40,155);
    glColor3ub(139, 146, 22);
    circle(10,20,40,156);


    glColor3ub(139, 146, 22);
    circle(9,22,20,280);
    glColor3ub(0, 77, 26);
    circle(9,21,20,270);
    glColor3ub(139, 146, 22);
    circle(9,21,20,269);


    glColor3ub(139, 146, 22);
    circle(9,22,10,255);
    glColor3ub(0, 77, 26);
    circle(9,20,10,245);
    glColor3ub(139, 146, 22);
    circle(9,20,10.5,244);


    ///........Tree........D-1........///
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(75,35,5);
    glVertex2f(15,90);
    glVertex2f(22,90);
    glVertex2f(21,100);
    glVertex2f(20,110);
    glVertex2f(18,120);
    glVertex2f(16,130);
    glVertex2f(17,140);
    glVertex2f(18,145);
    glVertex2f(20,150);
    glVertex2f(22,150);
    glVertex2f(21,147);
    glVertex2f(20,145);
    glVertex2f(18,140);
    glVertex2f(16,130);
    glVertex2f(13,120);
    glVertex2f(16,130);
    glVertex2f(18,140);
    glVertex2f(20,145);
    glVertex2f(22,150);
    glVertex2f(22,160);
    glVertex2f(18,150);
    glEnd();


    glBegin(GL_TRIANGLE_FAN);  ///........Tree........D-2.......///
    glColor3ub(75,35,5);
    glVertex2f(15,125);
    glVertex2f(19,130);
    glVertex2f(14,140);
    glVertex2f(14,150);
    glVertex2f(13,160);
    glVertex2f(10,170);
    glVertex2f(12,170);
    glVertex2f(12,160);
    glVertex2f(11,150);
    glVertex2f(11,140);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);   ///tree...................D-3 p2  ///
    glColor3ub(75,35,5);
    glVertex2f(31,140);
    glVertex2f(29,140);
    glVertex2f(28,120);
    glVertex2f(25,120);
    glEnd();


    glBegin(GL_TRIANGLE_FAN);  ///tree..................D-3 p1   ///
    glColor3ub(75,35,5);
    glVertex2f(16,100);
    glVertex2f(21,100);
    glVertex2f(24.5,120);
    glVertex2f(28,120);
    glEnd();

    glPopMatrix();
























     glPushMatrix();
    glTranslated(50.0,220.0,0.0);
     glScalef(0.9, 0.9, 1.0);

    /////////////////
    ///2nd building main part/////
    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(200,120);
    glVertex2f(287,120);
    glVertex2f(287,395);
    glVertex2f(200,395);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(202.5,120);        //2nd building main part 2
    glVertex2f(284,120);
    glVertex2f(284,380);
    glVertex2f(202.5,380);
    glEnd();

   ///....1st floor design............

    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(205,125);   //part 2 2nd building 1st floor red.........
    glVertex2f(281,125);
    glVertex2f(281,155);
    glVertex2f(205,155);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(281,125);   //part 2 2nd building 1st floor.......red
    glVertex2f(267,125);
    glVertex2f(267,155);
    glVertex2f(281,155);
    glEnd();

    //Door and window
    glBegin(GL_QUADS);
    glColor3ub(123, 88, 71);
    glVertex2f(205,120);   //d2 2nd building 1st floor
    glVertex2f(215,120);
    glVertex2f(215,155);
    glVertex2f(205,155);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);
    glVertex2f(209,132);   //d2 2nd building 1st floor
    glVertex2f(210,132);
    glVertex2f(210,147);
    glVertex2f(209,147);
    glEnd();

        glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(223,131);   //w1 2nd building 1st floor2
    glVertex2f(243,131);
    glVertex2f(243,147);
    glVertex2f(223,147);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(128,197,215);
    glVertex2f(223.5,132);   //w1 2nd building 1st floor2
    glVertex2f(242.5,132);
    glVertex2f(242.5,146);
    glVertex2f(223.5,146);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(233.1,131);   //w1 2nd building 1st floor2
    glVertex2f(234,131);
    glVertex2f(234,147);
    glVertex2f(233.1,147);
    glEnd();

     glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(253,131);   //w2 2nd building 1st floor2
    glVertex2f(273,131);
    glVertex2f(273,147);
    glVertex2f(253,147);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(128,197,215);
    glVertex2f(253.5,132);   //w2 2nd building 1st floor2
    glVertex2f(272.5,132);
    glVertex2f(272.5,146);
    glVertex2f(253.5,146);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(263.1,131);   //w2 2nd building 1st floor2
    glVertex2f(264,131);
    glVertex2f(264,147);
    glVertex2f(263.1,147);
    glEnd();

    //floor divided
    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(202.5,160);  //2nd building 1st floor1
    glVertex2f(284,160);
    glVertex2f(284,175);
    glVertex2f(202.5,175);
    glEnd();

       ///....2nd floor design............///y+55 korchi

    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(205,180);   //part 2 2nd building 2nd floor red.........
    glVertex2f(281,180);
    glVertex2f(281,210);
    glVertex2f(205,210);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(281,180);   //part 2 2nd building 2nd floor.......red
    glVertex2f(264,180);
    glVertex2f(264,210);
    glVertex2f(281,210);
    glEnd();

    //Door and window
    glBegin(GL_QUADS);
    glColor3ub(123, 88, 71);
    glVertex2f(205,175);   //d2 2nd building 2nd floor
    glVertex2f(215,175);
    glVertex2f(215,210);
    glVertex2f(205,210);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);
    glVertex2f(209,187);   //d2 2nd building 2nd floor
    glVertex2f(210,187);
    glVertex2f(210,202);
    glVertex2f(209,202);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(223,186);   //w1 2nd building 2nd floor2
    glVertex2f(243,186);
    glVertex2f(243,202);
    glVertex2f(223,202);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(128,197,215);
    glVertex2f(223.5,187);   //w1 2nd building 2nd floor2
    glVertex2f(242.5,187);
    glVertex2f(242.5,201);
    glVertex2f(223.5,201);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(233.1,186);   //w1 2nd building 2nd floor2
    glVertex2f(234,186);
    glVertex2f(234,202);
    glVertex2f(233.1,202);
    glEnd();

     glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(253,186);   //w2 2nd building 2nd floor2
    glVertex2f(273,186);
    glVertex2f(273,202);
    glVertex2f(253,202);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(128,197,215);
    glVertex2f(253.5,187);   //w2 2nd building 2nd floor2
    glVertex2f(272.5,187);
    glVertex2f(272.5,201);
    glVertex2f(253.5,201);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(263.1,186);   //w2 2nd building 2nd floor2
    glVertex2f(264,186);
    glVertex2f(264,202);
    glVertex2f(263.1,202);
    glEnd();

    //floor divided
    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(202.5,215);  //2nd building 2nd floor1
    glVertex2f(284,215);
    glVertex2f(284,230);
    glVertex2f(202.5,230);
    glEnd();

    ///....3rd floor design............///y+55 korchi

    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(205,235);   //part 2 2nd building 3rd floor red.........
    glVertex2f(281,235);
    glVertex2f(281,265);
    glVertex2f(205,265);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(281,235);   //part 2 2nd building 3rd floor.......red
    glVertex2f(264,235);
    glVertex2f(264,265);
    glVertex2f(281,265);
    glEnd();

    //Door and window
    glBegin(GL_QUADS);
    glColor3ub(123, 88, 71);
    glVertex2f(205,230);   //d2 2nd building 3rd floor
    glVertex2f(215,230);
    glVertex2f(215,265);
    glVertex2f(205,265);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);
    glVertex2f(209,242);   //d2 2nd building 2nd floor
    glVertex2f(210,242);
    glVertex2f(210,257);
    glVertex2f(209,257);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(223,241);   //w1 2nd building 3rd floor2
    glVertex2f(243,241);
    glVertex2f(243,257);
    glVertex2f(223,257);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(128,197,215);
    glVertex2f(223.5,242);   //w1 2nd building 2nd floor2
    glVertex2f(242.5,242);
    glVertex2f(242.5,256);
    glVertex2f(223.5,256);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(233.1,241);   //w1 2nd building 3rd floor2
    glVertex2f(234,241);
    glVertex2f(234,257);
    glVertex2f(233.1,257);
    glEnd();

     glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(253,241);   //w2 2nd building 3rd floor2
    glVertex2f(273,241);
    glVertex2f(273,257);
    glVertex2f(253,257);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(128,197,215);
    glVertex2f(253.5,242);   //w2 2nd building 3rd floor2
    glVertex2f(272.5,242);
    glVertex2f(272.5,256);
    glVertex2f(253.5,256);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(263.1,241);   //w2 2nd building 3rd floor2
    glVertex2f(264,241);
    glVertex2f(264,257);
    glVertex2f(263.1,257);
    glEnd();

    //floor divided
    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(202.5,270);  //2nd building 3rd floor1
    glVertex2f(284,270);
    glVertex2f(284,285);
    glVertex2f(202.5,285);
    glEnd();

    ///....4th floor design............///y+55 korchi

    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(205,290);   //part 2 2nd building 4th floor red.........
    glVertex2f(281,290);
    glVertex2f(281,320);
    glVertex2f(205,320);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(281,290);   //part 2 2nd building 4th floor.......red
    glVertex2f(264,290);
    glVertex2f(264,320);
    glVertex2f(281,320);
    glEnd();

    //Door and window
    glBegin(GL_QUADS);
    glColor3ub(123, 88, 71);
    glVertex2f(205,285);   //d2 2nd building 4th floor
    glVertex2f(215,285);
    glVertex2f(215,320);
    glVertex2f(205,320);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);
    glVertex2f(209,297);   //d2 2nd building 5th floor
    glVertex2f(210,297);
    glVertex2f(210,312);
    glVertex2f(209,312);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(223,296);   //w1 2nd building 4th floor2
    glVertex2f(243,296);
    glVertex2f(243,312);
    glVertex2f(223,312);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(128,197,215);
    glVertex2f(223.5,297);   //w1 2nd building 4th floor2
    glVertex2f(242.5,297);
    glVertex2f(242.5,311);
    glVertex2f(223.5,311);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(233.1,296);   //w1 2nd building 4th floor2
    glVertex2f(234,296);
    glVertex2f(234,312);
    glVertex2f(233.1,312);
    glEnd();

     glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(253,297);   //w2 2nd building 4th floor2
    glVertex2f(273,297);
    glVertex2f(273,312);
    glVertex2f(253,312);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(128,197,215);
    glVertex2f(253.5,297);   //w2 2nd building 4th floor2
    glVertex2f(272.5,297);
    glVertex2f(272.5,311);
    glVertex2f(253.5,311);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(263.1,297);   //w2 2nd building 4th floor2
    glVertex2f(264,297);
    glVertex2f(264,312);
    glVertex2f(263.1,312);
    glEnd();

     //floor divided
    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(202.5,325);  //2nd building 4th floor1
    glVertex2f(284,325);
    glVertex2f(284,340);
    glVertex2f(202.5,340);
    glEnd();

    ///....5th floor design............///y+55 korchi

    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(205,345);   //part 2 2nd building 5th floor red.........
    glVertex2f(281,345);
    glVertex2f(281,375);
    glVertex2f(205,375);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(281,345);   //part 2 2nd building 5th floor.......red
    glVertex2f(264,345);
    glVertex2f(264,375);
    glVertex2f(281,375);
    glEnd();

    //Door and window
    glBegin(GL_QUADS);
    glColor3ub(123, 88, 71);
    glVertex2f(205,340);   //d2 2nd building 5th floor
    glVertex2f(215,340);
    glVertex2f(215,375);
    glVertex2f(205,375);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);
    glVertex2f(209,352);   //d2 2nd building 5th floor
    glVertex2f(210,352);
    glVertex2f(210,367);
    glVertex2f(209,367);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(223,351);   //w1 2nd building 5th floor2
    glVertex2f(243,351);
    glVertex2f(243,367);
    glVertex2f(223,367);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(128,197,215);
    glVertex2f(223.5,352);   //w1 2nd building 5th floor2
    glVertex2f(242.5,352);
    glVertex2f(242.5,366);
    glVertex2f(223.5,366);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(233.1,351);   //w1 2nd building 5th floor2
    glVertex2f(234,351);
    glVertex2f(234,367);
    glVertex2f(233.1,367);
    glEnd();

     glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(253,352);   //w2 2nd building 4th floor2
    glVertex2f(273,352);
    glVertex2f(273,367);
    glVertex2f(253,367);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(128,197,215);
    glVertex2f(253.5,352);   //w2 2nd building 5th floor2
    glVertex2f(272.5,352);
    glVertex2f(272.5,366);
    glVertex2f(253.5,366);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex2f(263.1,352);   //w2 2nd building 5th floor2
    glVertex2f(264,352);
    glVertex2f(264,367);
    glVertex2f(263.1,367);
    glEnd();
    ///End 2nd building//

    ///Tower 2nd Building//
    glBegin(GL_QUADS);
    glColor3ub(140, 140, 140);
    glVertex2f(273,395);
    glVertex2f(274,395);
    glVertex2f(274,520);
    glVertex2f(273,520);
    glEnd();

    glColor3ub(102, 102, 102);
    circle(3,6,273,500);
    glColor3ub(217, 217, 217);
    circle(2.5,5,273,500);
    glColor3ub(102, 102, 102);
    circle(2,4,276,485);
    glColor3ub(217, 217, 217);
    circle(1.5,3,276,485);
    //Tower End//

    glBegin(GL_TRIANGLE_FAN); ///triangle tree 2nd Building///
    glColor3ub(75,35,5);
    glVertex2f(290,90);
    glVertex2f(295,90);
    glVertex2f(295,120);
    glVertex2f(290,120);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(139, 146, 22);
    glVertex2f(280,120);
    glVertex2f(305,120);
    glVertex2f(292.5,180);
    glVertex2f(292.5,180);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(139, 146, 22);
    glVertex2f(281,135);
    glVertex2f(304,135);
    glVertex2f(292.5,190);
    glVertex2f(292.5,190);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(139, 146, 22);
    glVertex2f(282,150);
    glVertex2f(303,150);
    glVertex2f(292.5,180);
    glVertex2f(292.5,180);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(139, 146, 22);
    glVertex2f(283,160);
    glVertex2f(302,160);
    glVertex2f(292.5,190);
    glVertex2f(292.5,190);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(139, 146, 22);
    glVertex2f(284,170);
    glVertex2f(301,170);
    glVertex2f(292.5,200);
    glVertex2f(292.5,200);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(139, 146, 22);
    glVertex2f(285,180);
    glVertex2f(300,180);
    glVertex2f(292.5,210);
    glVertex2f(292.5,210);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(139, 146, 22);
    glVertex2f(286,190);
    glVertex2f(299,190);
    glVertex2f(292.5,260);
    glVertex2f(292.5,260);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(139, 146, 22);
    glVertex2f(286,200);
    glVertex2f(299,200);
    glVertex2f(292.5,270);
    glVertex2f(292.5,270);
    glEnd();
    glPopMatrix();






































     glPushMatrix();
    glTranslated(103.0,250.0,0.0);
     glScalef(0.9, 0.9, 1.0);



    ///3rd Building Design///
///white part///
    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(575,90);
    glVertex2f(635,90);
    glVertex2f(635,475);
    glVertex2f(575,475);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(577.5,90);        //3rd Building main part 2
    glVertex2f(631,90);
    glVertex2f(631,460);
    glVertex2f(577.5,460);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(635,90);         //3rd Building 2nd part 2
    glVertex2f(665,90);
    glVertex2f(665,420);
    glVertex2f(635,420);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(635,90);         //3rd Building 2nd part 1
    glVertex2f(662.5,90);
    glVertex2f(662.5,410);
    glVertex2f(635,410);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(637,370);   //part 2 3rd Building 6th floor2..........outlook
    glVertex2f(662.5,370);
    glVertex2f(662.5,400);
    glVertex2f(637,400);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(123, 88, 71);
    glVertex2f(638,365);   //part 2 3rd Building 6th floor2..........outlook
    glVertex2f(648,365);
    glVertex2f(648,400);
    glVertex2f(638,400);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);
    glVertex2f(643,365);   //Door 3rd Building 6th floor2..........outlook-1
    glVertex2f(644,365);
    glVertex2f(644,392);
    glVertex2f(643,392);
    glEnd();

/////////////****
    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(615,90); //3rd Building 3rd Building floor1
    glVertex2f(667,90);
    glVertex2f(667,145);
    glVertex2f(615,145);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(615,145);   //3rd Building 2nd floor1
    glVertex2f(667,145);
    glVertex2f(667,200);
    glVertex2f(615,200);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(615,200);   //3rd Building 3rd floor1
    glVertex2f(667,200);
    glVertex2f(667,255);
    glVertex2f(615,255);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(615,255);   //3rd Building 4th floor1
    glVertex2f(667,255);
    glVertex2f(667,310);
    glVertex2f(615,310);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(615,310);   //3rd Building 5th floor1
    glVertex2f(667,310);
    glVertex2f(667,365);
    glVertex2f(615,365);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(615,365);   //3rd Building 6th floor1
    glVertex2f(667,365);
    glVertex2f(667,420);
    glVertex2f(615,420);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(204, 204, 204);
    glVertex2f(615,420);   //3rd Building 7th floor1
    glVertex2f(667,420);
    glVertex2f(667,475);
    glVertex2f(615,475);
    glEnd();

     ///design of main building....red and glass....

    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(577.5,420);  //7 3rd Building main part3
    glVertex2f(592,420);
    glVertex2f(592,460);
    glVertex2f(577.5,460);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(577.5,365);  //6 3rd Building main part3
    glVertex2f(592,365);
    glVertex2f(592,405);
    glVertex2f(577.5,405);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(577.5,310);  //5 3rd Building main part3
    glVertex2f(592,310);
    glVertex2f(592,350);
    glVertex2f(577.5,350);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(577.5,255);  //4 3rd Building main part3
    glVertex2f(592,255);
    glVertex2f(592,295);
    glVertex2f(577.5,295);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(577.5,200);  //3 3rd Building main part3
    glVertex2f(592,200);
    glVertex2f(592,240);
    glVertex2f(577.5,240);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(577.5,145);  //2 3rd Building main part3
    glVertex2f(592,145);
    glVertex2f(592,185);
    glVertex2f(577.5,185);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(177,124,119);
    glVertex2f(577.5,90);  //1 3rd Building main part3
    glVertex2f(592,90);
    glVertex2f(592,130);
    glVertex2f(577.5,130);
    glEnd();
    /////////.....................
    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(620,420);  //7 3rd Building main part3
    glVertex2f(631,420);
    glVertex2f(631,460);
    glVertex2f(620,460);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(620,365);  //6 3rd Building main part3
    glVertex2f(631,365);
    glVertex2f(631,405);
    glVertex2f(620,405);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(620,310);  //5 3rd Building main part3
    glVertex2f(631,310);
    glVertex2f(631,350);
    glVertex2f(620,350);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(620,255);  //4 3rd Building main part3
    glVertex2f(631,255);
    glVertex2f(631,295);
    glVertex2f(620,295);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(620,200);  //3 3rd Building main part3
    glVertex2f(631,200);
    glVertex2f(631,240);
    glVertex2f(620,240);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(620,145);  //2 3rd Building main part3
    glVertex2f(631,145);
    glVertex2f(631,185);
    glVertex2f(620,185);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(143,175,175);
    glVertex2f(610,90);  //door
    glVertex2f(631,90);
    glVertex2f(631,130);
    glVertex2f(610,130);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(0, 0, 77);
    glVertex2f(620,90);  //door 1 main
    glVertex2f(621,90);
    glVertex2f(621,130);
    glVertex2f(620,130);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(621,100);  //door 1 main
    glVertex2f(622,100);
    glVertex2f(622,120);
    glVertex2f(621,120);
    glEnd();

    //////////............................

    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(575,420);  //7 3rd Building main part3
    glVertex2f(635,420);
    glVertex2f(635,421);
    glVertex2f(575,421);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(575,365);  //6 3rd Building main part3
    glVertex2f(662.5,365);
    glVertex2f(662.5,366);
    glVertex2f(575,366);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(575,310);  //5 3rd Building main part3
    glVertex2f(635,310);
    glVertex2f(635,311);
    glVertex2f(575,311);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(575,255);  //4 3rd Building main part3
    glVertex2f(635,255);
    glVertex2f(635,256);
    glVertex2f(575,256);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(575,199);  //3 3rd Building main part3
    glVertex2f(635,199);
    glVertex2f(635,200);
    glVertex2f(575,200);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(575,145);  //2 3rd Building main part3
    glVertex2f(635,145);
    glVertex2f(635,146);
    glVertex2f(575,146);
    glEnd();

    ///////...............................
    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(575,405);  //7 3rd Building main part3
    glVertex2f(635,405);
    glVertex2f(635,406);
    glVertex2f(575,406);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(575,350);  //6 3rd Building main part3
    glVertex2f(635,350);
    glVertex2f(635,351);
    glVertex2f(575,351);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(575,295);  //5 3rd Building main part3
    glVertex2f(635,295);
    glVertex2f(635,296);
    glVertex2f(575,296);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(575,240);  //4 3rd Building main part3
    glVertex2f(635,240);
    glVertex2f(635,241);
    glVertex2f(575,241);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(575,185);  //3 3rd Building main part3
    glVertex2f(635,185);
    glVertex2f(635,186);
    glVertex2f(575,186);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(179, 179, 179);
    glVertex2f(575,130);  //2 3rd Building main part3
    glVertex2f(635,130);
    glVertex2f(635,131);
    glVertex2f(575,131);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(89, 89, 89);
    glVertex2f(665,90);  // part2 last black line
    glVertex2f(666,90);
    glVertex2f(666,460);
    glVertex2f(665,460);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(635,420);  //7 2nd part3
    glVertex2f(662.5,420);
    glVertex2f(662.5,460);
    glVertex2f(635,460);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(635,365);  //6 2nd part3
    glVertex2f(662.5,365);
    glVertex2f(662.5,405);
    glVertex2f(635,405);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(635,310);  //5 part3
    glVertex2f(662.5,310);
    glVertex2f(662.5,350);
    glVertex2f(635,350);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(635,255);  //4 2nd part3
    glVertex2f(662.5,255);
    glVertex2f(662.5,295);
    glVertex2f(635,295);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(635,200);  //3 2nd part3
    glVertex2f(662.5,200);
    glVertex2f(662.5,240);
    glVertex2f(635,240);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(635,145);  //2 2nd part3
    glVertex2f(662.5,145);
    glVertex2f(662.5,185);
    glVertex2f(635,185);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(143,175,175);
    glVertex2f(635,90);  //1 3rd Building main part3
    glVertex2f(662.5,90);
    glVertex2f(662.5,130);
    glVertex2f(635,130);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(0, 0, 77);
    glVertex2f(642,90);  //1 main part3
    glVertex2f(643,90);
    glVertex2f(643,130);
    glVertex2f(642,130);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(0, 0, 77);
    glVertex2f(652,90);  //1 main part3
    glVertex2f(653,90);
    glVertex2f(653,130);
    glVertex2f(652,130);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(643,100);  //1 main part3
    glVertex2f(644,100);
    glVertex2f(644,120);
    glVertex2f(643,120);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(653,100);  ///1 main part3
    glVertex2f(654,100);
    glVertex2f(654,120);
    glVertex2f(653,120);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(0, 26, 51);
    glVertex2f(635,90);   ///partation of Blue line
    glVertex2f(636,90);
    glVertex2f(636,460);
    glVertex2f(635,460);
    glEnd();

///End 3rd Building//

///Tower 2nd Building//
    glBegin(GL_QUADS);
    glColor3ub(140, 140, 140);
    glVertex2f(635,475);
    glVertex2f(636,475);
    glVertex2f(636,600);
    glVertex2f(635,600);
    glEnd();

    glColor3ub(102, 102, 102);
    circle(3,6,635,577);
    glColor3ub(217, 217, 217);
    circle(2.5,5,635,577);
    glColor3ub(102, 102, 102);
    circle(2,4,638,560);
    glColor3ub(217, 217, 217);
    circle(1.5,3,638,560);
    ///Tower End//

      ///4th Building Design///Top
    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);///4th Building main part 2
    glVertex2f(483.5,440);
    glVertex2f(555.2,440);
    glVertex2f(555.2,472);
    glVertex2f(483.5,472);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(483.5,440);        ///4th Building main part 2
    glVertex2f(555.2,440);
    glVertex2f(555.2,467);
    glVertex2f(483.5,467);
    glEnd();
///white part///
    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);///4th Building main part 2
    glVertex2f(475,119);
    glVertex2f(563.5,119);
    glVertex2f(563.5,442);
    glVertex2f(475,442);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(85,119,119);
    glVertex2f(477.5,119);        ///4th Building main part 2
    glVertex2f(561,119);
    glVertex2f(561,434);
    glVertex2f(477.5,434);
    glEnd();

    ///floor divided y+40//
    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(475,150);  ///4th Building 1st floor1
    glVertex2f(563,150);
    glVertex2f(563,154);
    glVertex2f(475,154);
    glEnd();
    //floor divided
    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(475,190);  ///4th Building 2nd floor1
    glVertex2f(563,190);
    glVertex2f(563,193);
    glVertex2f(475,193);
    glEnd();
    //floor divided
    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(475,230);  ///4th Building 3rd floor1
    glVertex2f(563,230);
    glVertex2f(563,234);
    glVertex2f(475,234);
    glEnd();
    //floor divided
    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(475,270);  ///4th Building 4th floor1
    glVertex2f(563,270);
    glVertex2f(563,273);
    glVertex2f(475,273);
    glEnd();
    //floor divided
    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(475,310);  ///4th Building 5th floor1
    glVertex2f(563,310);
    glVertex2f(563,314);
    glVertex2f(475,314);
    glEnd();
    //floor divided
    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(475,310);  ///4th Building 6th floor1
    glVertex2f(563,310);
    glVertex2f(563,314);
    glVertex2f(475,314);
    glEnd();
    //floor divided
    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(475,350);  ///4th Building 7th floor1
    glVertex2f(563,350);
    glVertex2f(563,353);
    glVertex2f(475,353);
    glEnd();
    //floor divided
    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(475,390);  ///4th Building 8th floor1
    glVertex2f(563,390);
    glVertex2f(563,394);
    glVertex2f(475,394);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(491,119);  /// 4th Building white line vertical
    glVertex2f(491.5,119);
    glVertex2f(491.5,440);
    glVertex2f(491,440);
    glEnd();
     glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(505,119);  /// 4th Building white line vertical
    glVertex2f(505.5,119);
    glVertex2f(505.5,440);
    glVertex2f(505,440);
    glEnd();
     glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(519,119);  /// 4th Building white line vertical
    glVertex2f(519.5,119);
    glVertex2f(519.5,440);
    glVertex2f(519,440);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(533,119);  /// 4th Building white line vertical
    glVertex2f(533.5,119);
    glVertex2f(533.5,440);
    glVertex2f(533,440);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(242, 242, 242);
    glVertex2f(547,119);  /// 4th Building white line vertical
    glVertex2f(547.5,119);
    glVertex2f(547.5,440);
    glVertex2f(547,440);
    glEnd();

    ///Door
    glBegin(GL_QUADS);
    glColor3ub(0, 26, 51);
    glVertex2f(505,140);  ///4th Building 1st floor1
    glVertex2f(533,140);
    glVertex2f(533,144);
    glVertex2f(505,144);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(0, 26, 51);
    glVertex2f(532,119);  /// 4th Building white line vertical
    glVertex2f(533.6,119);
    glVertex2f(533.6,140);
    glVertex2f(532,140);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(0, 26, 51);
    glVertex2f(518,119);  /// 4th Building white line vertical
    glVertex2f(519.6,119);
    glVertex2f(519.6,140);
    glVertex2f(518,140);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(0, 26, 51);
    glVertex2f(505,119);  ///4th Building white line vertical
    glVertex2f(506.6,119);
    glVertex2f(506.6,140);
    glVertex2f(505,140);
    glEnd();
    ///End 4th Building///

    ///Hospital signboard

    glColor3ub(255, 255, 255);
    glRasterPos2i(504,455);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'H');
    glColor3ub(255, 255, 255);
    glRasterPos2i(509,455);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'O');
    glColor3ub(255, 255, 255);
    glRasterPos2i(513,455);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'S');
    glColor3ub(255, 255, 255);
    glRasterPos2i(517,455);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'P');
    glColor3ub(255, 255, 255);
    glRasterPos2i(521,455);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'I');
    glColor3ub(255, 255, 255);
    glRasterPos2i(525,455);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'T');
    glColor3ub(255, 255, 255);
    glRasterPos2i(529,455);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'A');
    glColor3ub(255, 255, 255);
    glRasterPos2i(534,455);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'L');
    ///End//

    ///5th Building Food Court///
    /// Food court big Right part///
    glBegin(GL_QUADS);
    glColor3ub(52, 67, 74);///Food Court main part 2
    glVertex2f(373,90);
    glVertex2f(466,90);
    glVertex2f(466,442);
    glVertex2f(373,442);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(91,102,108);
    glVertex2f(377.5,90);        ///Food Court main part 2
    glVertex2f(461,90);
    glVertex2f(461,440);
    glVertex2f(377.5,440);
    glEnd();

    //floor divided
    glBegin(GL_QUADS);
    glColor3ub(52, 67, 74);
    glVertex2f(282.5,270);  ///Middle divided left right
    glVertex2f(466,270);
    glVertex2f(466,281);
    glVertex2f(282.5,281);
    glEnd();

    ///stairs//Top
    glBegin(GL_QUADS);
    glColor3ub(33, 46, 47);
    glVertex2f(371,442);  ///3//
    glVertex2f(467.5,442);
    glVertex2f(467.5,451);
    glVertex2f(371,451);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(20, 25, 33);
    glVertex2f(371,454);  ///2//
    glVertex2f(467.5,454);
    glVertex2f(467.5,451);
    glVertex2f(371,451);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(33, 46, 47);
    glVertex2f(367,454);  ///1//
    glVertex2f(471.5,454);
    glVertex2f(471.5,463);
    glVertex2f(367,463);
    glEnd();

    ///stairs Top End//

    glBegin(GL_QUADS);
    glColor3ub(72, 85, 93);
    glVertex2f(377.5,433);
    glVertex2f(461,433);
    glVertex2f(461,440);
    glVertex2f(377.5,440);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(27, 45, 54);
    glVertex2f(382,290);        ///black Food Court Right part
    glVertex2f(457,290);
    glVertex2f(457,429);
    glVertex2f(382,429);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(254, 246, 223);
    glVertex2f(383.5,293.6);        /// white color Food Court Right part
    glVertex2f(455.4,293.6);
    glVertex2f(455.4,425.4);
    glVertex2f(383.5,425.4);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(27, 45, 54);
    glVertex2f(382,314.2);  ///middle line 2
    glVertex2f(457,314.2);
    glVertex2f(457,310.2);
    glVertex2f(382,310.2);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(27, 45, 54);
    glVertex2f(368,409.1);  ///middle line 1
    glVertex2f(457,409.1);
    glVertex2f(457,405.1);
    glVertex2f(368,405.1);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(27, 45, 54);
    glVertex2f(368,429.1);  ///middle line for sign board
    glVertex2f(382,429.1);
    glVertex2f(382,426);
    glVertex2f(368,426);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(27, 45, 54);
    glVertex2f(402,290);   ///partation of Blue line
    glVertex2f(400.6,290);
    glVertex2f(400.6,426);
    glVertex2f(402,426);
    glEnd();
     glBegin(GL_QUADS);
    glColor3ub(27, 45, 54);
    glVertex2f(420,290);   ///partation of Blue line
    glVertex2f(418.6,290);
    glVertex2f(418.6,426);
    glVertex2f(420,426);
    glEnd();
     glBegin(GL_QUADS);
    glColor3ub(27, 45, 54);
    glVertex2f(438,290);   ///partation of Blue line
    glVertex2f(436.5,290);
    glVertex2f(436.5,426);
    glVertex2f(438,426);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(52, 67, 74);
    glVertex2f(388.5,255.6);        ///  Food Court board Black
    glVertex2f(450.2,255.6);
    glVertex2f(450.2,283.4);
    glVertex2f(388.5,283.4);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(182, 138, 94);
    glVertex2f(386.5,258.3);        /// Food Court board
    glVertex2f(448.3,258.3);
    glVertex2f(448.3,287);
    glVertex2f(386.5,287);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(250, 201, 143);
    glVertex2f(386.5,281.2);  ///middle line 1
    glVertex2f(448.3,281.2);
    glVertex2f(448.3,279);
    glVertex2f(386.5,279);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(250, 201, 143);
    glVertex2f(386.5,273.2);  ///middle line 1
    glVertex2f(448.3,273.2);
    glVertex2f(448.3,271);
    glVertex2f(386.5,271);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(250, 201, 143);
    glVertex2f(386.5,265.2);  ///middle line 1
    glVertex2f(448.3,265.2);
    glVertex2f(448.3,263);
    glVertex2f(386.5,263);
    glEnd();



     glPushMatrix();
    glTranslated(50.0,25.0,0.0);
     glScalef(0.9, 0.9, 1.0);
    glColor3ub(31, 46, 53);
    glRasterPos2i(398,270);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'H');
    glColor3ub(31, 46, 53);
    glRasterPos2i(404,270);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'O');
    glColor3ub(31, 46, 53);
    glRasterPos2i(408,270);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'T');
    glColor3ub(31, 46, 53);
    glRasterPos2i(412,270);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'E');
    glColor3ub(31, 46, 53);
    glRasterPos2i(416,270);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'L');

    glPopMatrix();



    glBegin(GL_QUADS);
    glColor3ub(33, 45, 51);
    glVertex2f(377.6,241.4);  ///middle line
    glVertex2f(460.9,241.4);
    glVertex2f(460.9,239.2);
    glVertex2f(377.6,239.2);
    glEnd();
     glBegin(GL_QUADS);
    glColor3ub(33, 45, 51);
    glVertex2f(377.6,230.4);  ///middle line
    glVertex2f(460.9,230.4);
    glVertex2f(460.9,228.2);
    glVertex2f(377.6,228.2);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(33, 45, 51);
    glVertex2f(377.6,220.4);  ///middle line
    glVertex2f(460.9,220.4);
    glVertex2f(460.9,218.2);
    glVertex2f(377.6,218.2);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(33, 45, 51);
    glVertex2f(377.6,210.5);  ///middle line
    glVertex2f(460.9,210.5);
    glVertex2f(460.9,208.4);
    glVertex2f(377.6,208.4);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(33, 45, 51);
    glVertex2f(377.6,200.5);  ///middle line
    glVertex2f(460.9,200.5);
    glVertex2f(460.9,198.4);
    glVertex2f(377.6,198.4);
    glEnd();

    ///Door///
    glBegin(GL_QUADS);
    glColor3ub(104, 111, 119);
    glVertex2f(397,110);        ///black color Food court door
    glVertex2f(442,110);
    glVertex2f(442,230);
    glVertex2f(397,230);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(34, 45, 51);
    glVertex2f(398.5,110);        /// Food Court black color
    glVertex2f(440,110);
    glVertex2f(440,226.5);
    glVertex2f(398.5,226.5);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(254, 242, 205);
    glVertex2f(401.5,110);        /// Food Court Right part
    glVertex2f(437,110);
    glVertex2f(437,220.5);
    glVertex2f(401.5,220.5);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(27, 45, 54);
    glVertex2f(399,118.7);  ///
    glVertex2f(440,118.7);
    glVertex2f(440,124.6);
    glVertex2f(399,124.6);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(34, 45, 51);
    glVertex2f(420,110);   ///partation of Blue line
    glVertex2f(416.6,110);
    glVertex2f(416.6,226);
    glVertex2f(420,226);
    glEnd();
     glBegin(GL_QUADS);
    glColor3ub(104, 111, 119);
    glVertex2f(421,110);   ///partation
    glVertex2f(419.6,110);
    glVertex2f(419.6,226);
    glVertex2f(421,226);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(34, 45, 51);
    glVertex2f(424,110);   ///partation of Blue line
    glVertex2f(420.6,110);
    glVertex2f(420.6,226);
    glVertex2f(424,226);
    glEnd();


    ///End Right part///

    ///Food Court left part
    glBegin(GL_QUADS);
    glColor3ub(91, 102, 108);///Food Court small left part
    glVertex2f(288,110);
    glVertex2f(373,110);
    glVertex2f(373,270);
    glVertex2f(288,270);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(27, 45, 54);
    glVertex2f(293,120);        ///black Food Court small left part
    glVertex2f(368,120);
    glVertex2f(368,259);
    glVertex2f(293,259);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(254, 246, 223);
    glVertex2f(295,124);        /// white color Food Court small left part
    glVertex2f(366,124);
    glVertex2f(366,256);
    glVertex2f(295,256);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(27, 45, 54);
    glVertex2f(294.5,190);  ///middle line
    glVertex2f(368,190);
    glVertex2f(368,187);
    glVertex2f(294.5,187);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(27, 45, 54);
    glVertex2f(350,120);   ///partation of Blue line
    glVertex2f(348.6,120);
    glVertex2f(348.6,256);
    glVertex2f(350,256);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(27, 45, 54);
    glVertex2f(332,120);   ///partation of Blue line
    glVertex2f(330.6,120);
    glVertex2f(330.6,256);
    glVertex2f(332,256);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(27, 45, 54);
    glVertex2f(314,120);   ///partation of Blue line
    glVertex2f(312.6,120);
    glVertex2f(312.6,256);
    glVertex2f(314,256);
    glEnd();

    ///top Boundary
    glBegin(GL_QUADS);
    glColor3ub(27, 45, 54);
    glVertex2f(284.3,280);   ///partation of Blue line
    glVertex2f(282.5,280);
    glVertex2f(282.5,321);
    glVertex2f(284.3,321);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(27, 45, 54);
    glVertex2f(302,280);   ///partation of Blue line
    glVertex2f(300.2,280);
    glVertex2f(300.2,321);
    glVertex2f(302,321);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(27, 45, 54);
    glVertex2f(320,280);   ///partation of Blue line
    glVertex2f(318.2,280);
    glVertex2f(318.2,321);
    glVertex2f(320,321);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(27, 45, 54);
    glVertex2f(338,280);   ///partation of Blue line
    glVertex2f(336.1,280);
    glVertex2f(336.1,321);
    glVertex2f(338,321);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(27, 45, 54);
    glVertex2f(356,280);   ///partation of Blue line
    glVertex2f(354.2,280);
    glVertex2f(354.2,321);
    glVertex2f(356,321);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(27, 45, 54);
    glVertex2f(373,280);   ///partation of Blue line
    glVertex2f(371.2,280);
    glVertex2f(371.2,321);
    glVertex2f(373,321);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(27, 45, 54);
    glVertex2f(282.5,314);  ///middle line
    glVertex2f(372,314);
    glVertex2f(372,309.6);
    glVertex2f(282.5,309.6);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(27, 45, 54);
    glVertex2f(282.5,304);  ///middle line
    glVertex2f(372,304);
    glVertex2f(372,300);
    glVertex2f(282.5,300);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(27, 45, 54);
    glVertex2f(282.5,294);  ///middle line
    glVertex2f(372,294);
    glVertex2f(372,290);
    glVertex2f(282.5,290);
    glEnd();
    ///End top Boundary//
    ///End left part//



    ///stairs//
    glBegin(GL_QUADS);
    glColor3ub(33, 46, 47);
    glVertex2f(284,90);  ///Bottom3
    glVertex2f(471.5,90);
    glVertex2f(471.5,99);
    glVertex2f(284,99);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(20, 25, 33);
    glVertex2f(286,102);  ///middle2
    glVertex2f(468,102);
    glVertex2f(468,99);
    glVertex2f(286,99);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(33, 46, 47);
    glVertex2f(286,102);  ///Top1
    glVertex2f(468,102);
    glVertex2f(468,110);
    glVertex2f(286,110);
    glEnd();

    ///stairs End//

    ///End Food court///

    ///....Right side circle tree in Food court.....6 ///
    glColor3ub(139, 146, 22);
    circle(3,6,468,101);
    glColor3ub(139, 146, 22);
    circle(5,12,463,111);
    glColor3ub(139, 146, 22);
    circle(5,12,473,111);
    glColor3ub(139, 146, 22);
    circle(5,12,478,106);
    glColor3ub(139, 146, 22);
    circle(5,12,458,121);


    glColor3ub(181, 106, 76);
    circle(5,12,463,123);
    glColor3ub(139, 146, 22);
    circle(5,12,464,123);
    glColor3ub(139, 146, 22);
    circle(5,12,468,141);
    glColor3ub(139, 146, 22);
    circle(4,10,463,136);
    glColor3ub(181, 106, 76);
    circle(4,10,465,134);

    glColor3ub(139, 146, 22);
    circle(4,10,466,133);

    glColor3ub(139, 146, 22);
    circle(5,12,478,131);
    glColor3ub(139, 146, 22);
    circle(5,12,473,131);
    glColor3ub(181, 106, 76);
    circle(5,12,472,126);
    glColor3ub(139, 146, 22);
    circle(5,12,471,125);
    glColor3ub(139, 146, 22);
    circle(5,12,483,121);
    glColor3ub(181, 106, 76);
    circle(5,12,478,116);
    glColor3ub(139, 146, 22);
    circle(5,12,477,116);
    glColor3ub(139, 146, 22);
    circle(5,12,468,119);
    glColor3ub(139, 146, 22);
    circle(5,12,480,141);


//....................jilimili tree.........................................





    glColor3ub(227, 91, 137);         ///Full..........Food court .........comp.
    circle(1,2,468,119);
    glColor3ub(227, 91, 137);
    circle(1,2,468,133);
    glColor3ub(227, 91, 137);
    circle(1,2,478,133);
    glColor3ub(227, 91, 137);
    circle(1,2,483,128);
    glColor3ub(227, 91, 137);
    circle(1,2,457,119);
    glColor3ub(227, 91, 137);
    circle(1,2.5,463,134);
    glColor3ub(227, 91, 137);
    circle(1,1.5,464,110);
    glColor3ub(227, 91, 137);
    circle(1,2.5,478,106);
    glColor3ub(227, 91, 137);
    circle(1,3,483,119);
    glColor3ub(227, 91, 137);
    circle(1,3,473,103);

//.............RIght SIDE TREE dalapala....................


    glBegin(GL_TRIANGLE_FAN);  ///tree.........beside Food Court ........6   ///
    glColor3ub(75, 35, 5);
    glVertex2f(473,90);
    glVertex2f(476,90);
    glVertex2f(475,100);
    glVertex2f(470,130);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);  ///tree.........beside Food Court........6   ///
    glColor3ub(75, 35, 5);
    glVertex2f(473,90);
    glVertex2f(476,90);
    glVertex2f(472,100);
    glVertex2f(466,110);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);  ///tree.........Food Court........6   ///
    glColor3ub(75, 35, 5);
    glVertex2f(472,90);
    glVertex2f(476,90);
    glVertex2f(477,110);
    glVertex2f(480,140);
    glEnd();
///End Right side circle tree///



///....Left side circle tree in Food court.....6 ///
    glPushMatrix();
    glTranslated(-160.0,0.0,0.0);
    glColor3ub(139, 146, 22);
    circle(3,6,280,101);
    glColor3ub(139, 146, 22);
    circle(5,12,275,111);
    glColor3ub(139, 146, 22);
    circle(5,12,285,111);
    glColor3ub(139, 146, 22);
    circle(5,12,290,106);
    glColor3ub(139, 146, 22);
    circle(5,12,270,121);


    glColor3ub(181, 106, 76);
    circle(5,12,275,123);
    glColor3ub(139, 146, 22);
    circle(5,12,276,123);
    glColor3ub(139, 146, 22);
    circle(5,12,280,141);
    glColor3ub(139, 146, 22);
    circle(4,10,275,136);
    glColor3ub(181, 106, 76);
    circle(4,10,277,134);

    glColor3ub(139, 146, 22);
    circle(4,10,278,133);

    glColor3ub(139, 146, 22);
    circle(5,12,290,131);
    glColor3ub(139, 146, 22);
    circle(5,12,285,131);
    glColor3ub(181, 106, 76);
    circle(5,12,284,126);
    glColor3ub(139, 146, 22);
    circle(5,12,283,125);
    glColor3ub(139, 146, 22);
    circle(5,12,295,121);
    glColor3ub(181, 106, 76);
    circle(5,12,290,116);
    glColor3ub(139, 146, 22);
    circle(5,12,289,116);
    glColor3ub(139, 146, 22);
    circle(5,12,280,119);
    glColor3ub(139, 146, 22);
    circle(5,12,292,141);



    glColor3ub(227, 91, 137);         ///Full..........Food court .........comp.
    circle(1,2,280,119);
    glColor3ub(227, 91, 137);
    circle(1,2,280,133);
    glColor3ub(227, 91, 137);
    circle(1,2,290,133);
    glColor3ub(227, 91, 137);
    circle(1,2,295,128);
    glColor3ub(227, 91, 137);
    circle(1,2,269,119);
    glColor3ub(227, 91, 137);
    circle(1,2.5,275,134);
    glColor3ub(227, 91, 137);
    circle(1,1.5,276,110);
    glColor3ub(227, 91, 137);
    circle(1,2.5,290,106);
    glColor3ub(227, 91, 137);
    circle(1,3,295,119);
    glColor3ub(227, 91, 137);
    circle(1,3,285,103);


    glBegin(GL_TRIANGLE_FAN);  ///tree.........beside Food Court ........6   ///
    glColor3ub(75, 35, 5);
    glVertex2f(285,90);
    glVertex2f(288,90);
    glVertex2f(287,100);
    glVertex2f(282,130);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);  ///tree.........beside Food Court........6   ///
    glColor3ub(75, 35, 5);
    glVertex2f(285,90);
    glVertex2f(288,90);
    glVertex2f(284,100);
    glVertex2f(278,110);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);  ///tree.........Food Court........6   ///
    glColor3ub(75, 35, 5);
    glVertex2f(284,90);
    glVertex2f(288,90);
    glVertex2f(289,110);
    glVertex2f(292,140);
    glEnd();
    glPopMatrix();
///End Left side circle tree///



    glBegin(GL_TRIANGLE_FAN);  ///tree.........beside 4th Building........2   ///
    glColor3ub(75,35,05);
    glVertex2f(545,119);
    glVertex2f(550,119);
    glVertex2f(550,139);
    glVertex2f(545,139);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(139, 146, 22);
    glVertex2f(535,139);
    glVertex2f(560,139);
    glVertex2f(547.5,169);
    glVertex2f(547.5,169);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(139, 146, 22);
    glVertex2f(536,149);
    glVertex2f(559,149);
    glVertex2f(547.5,189);
    glVertex2f(547.5,189);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(139, 146, 22);
    glVertex2f(537,159);
    glVertex2f(558,159);
    glVertex2f(547.5,209);
    glVertex2f(547.5,209);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(139, 146, 22);
    glVertex2f(537.5,169);
    glVertex2f(557.5,169);
    glVertex2f(547.5,214);
    glVertex2f(547.5,214);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(139, 146, 22);
    glVertex2f(538,179);
    glVertex2f(557,179);
    glVertex2f(547.5,224);
    glVertex2f(547.5,224);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(139, 146, 22);
    glVertex2f(540.5,189);
    glVertex2f(554.5,187);
    glVertex2f(547.5,237);
    glVertex2f(549.5,239);
    glEnd();




    glColor3ub(64, 64, 64);
    glRasterPos2i(285,780);

    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'T');
    //glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,' ');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'A');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'S');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'N');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'I');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'M');
    //glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'C');
    //glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'T');
    //glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,' ');
    //glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'B');
    //glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'Y');
   // glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,' ');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,' ');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'F');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'I');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'R');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'D');
    //glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,' ');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'A');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'U');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'S');
    //glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'D');
    //glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'A');
    //glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'L');




    glBegin(GL_TRIANGLE_FAN);  ///tree.........beside 3rd Building........1   ///
    glColor3ub(139,99,47);
    glVertex2f(670,90);
    glVertex2f(675,90);
    glVertex2f(675,120);
    glVertex2f(670,120);
    glEnd();


    glBegin(GL_TRIANGLE_FAN);  ///triangle tree 1.........beside 3rd building........1   ///
    glColor3ub(0,99,47);
    glVertex2f(660,120);
    glVertex2f(685,120);
    glVertex2f(672.5,180);
    glVertex2f(672.5,180);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(0,99,47);
    glVertex2f(661,135);
    glVertex2f(684,135);
    glVertex2f(672.5,190);
    glVertex2f(672.5,190);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(0,99,47);
    glVertex2f(662,150);
    glVertex2f(683,150);
    glVertex2f(672.5,180);
    glVertex2f(672.5,180);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(0,99,47);
    glVertex2f(663,160);
    glVertex2f(682,160);
    glVertex2f(672.5,190);
    glVertex2f(672.5,190);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(0,99,47);
    glVertex2f(664,170);
    glVertex2f(681,170);
    glVertex2f(672.5,200);
    glVertex2f(672.5,200);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(0,99,47);
    glVertex2f(665,180);
    glVertex2f(680,180);
    glVertex2f(672.5,210);
    glVertex2f(672.5,210);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(0,99,47);
    glVertex2f(666,190);
    glVertex2f(679,190);
    glVertex2f(672.5,260);
    glVertex2f(672.5,260);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(0,99,47);
    glVertex2f(666,200);
    glVertex2f(679,200);
    glVertex2f(672.5,270);
    glVertex2f(672.5,270);
    glEnd();


    glBegin(GL_TRIANGLE_FAN);  ///tree.........beside 3rd Building........2   ///
    glColor3ub(139,99,47);
    glVertex2f(650,90);
    glVertex2f(655,90);
    glVertex2f(655,110);
    glVertex2f(650,110);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(0,99,47);
    glVertex2f(640,110);
    glVertex2f(665,110);
    glVertex2f(652.5,140);
    glVertex2f(652.5,140);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(0,99,47);
    glVertex2f(641,120);
    glVertex2f(664,120);
    glVertex2f(652.5,160);
    glVertex2f(652.5,160);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(0,99,47);
    glVertex2f(642,130);
    glVertex2f(663,130);
    glVertex2f(652.5,180);
    glVertex2f(652.5,180);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(0, 102, 0);
    glVertex2f(642.5,140);
    glVertex2f(662.5,140);
    glVertex2f(652.5,185);
    glVertex2f(652.5,185);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(0, 102, 0);
    glVertex2f(643,150);
    glVertex2f(662,150);
    glVertex2f(652.5,195);
    glVertex2f(652.5,195);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(0, 102, 0);
    glVertex2f(643.5,160);
    glVertex2f(661.5,160);
    glVertex2f(652.5,210);
    glVertex2f(652.5,210);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(0, 102, 0);
    glVertex2f(644,170);
    glVertex2f(661,170);
    glVertex2f(652.5,230);
    glVertex2f(652.5,230);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(0, 102, 0);
    glVertex2f(644.5,180);
    glVertex2f(660.5,180);
    glVertex2f(652.5,250);
    glVertex2f(652.5,250);
    glEnd();


    glBegin(GL_TRIANGLE_FAN);  ///tree.........beside 3rd Building........3   ///
    glColor3ub(139,99,47);
    glVertex2f(680,90);
    glVertex2f(685,90);
    glVertex2f(685,100);
    glVertex2f(680,100);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(0, 102, 0);
    glVertex2f(675,100);
    glVertex2f(690,100);
    glVertex2f(682.5,130);
    glVertex2f(682.5,130);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(0, 102, 0);
    glVertex2f(676,110);
    glVertex2f(689,110);
    glVertex2f(682.5,140);
    glVertex2f(682.5,140);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(0, 102, 0);
    glVertex2f(677,122);
    glVertex2f(688,122);
    glVertex2f(682.5,155);
    glVertex2f(682.5,155);
    glEnd();


    glBegin(GL_TRIANGLE_FAN);  ///tree.........beside 3rd Building........4   ///
    glColor3ub(139,99,47);
    glVertex2f(690,90);
    glVertex2f(695,90);
    glVertex2f(695,105);
    glVertex2f(690,105);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(0,99,47);
    glVertex2f(685,105);
    glVertex2f(700,105);
    glVertex2f(692.5,135);
    glVertex2f(692.5,135);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(0, 102, 0);
    glVertex2f(686,115);
    glVertex2f(699,115);
    glVertex2f(692.5,145);
    glVertex2f(692.5,145);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(0, 102, 0);
    glVertex2f(687,127);
    glVertex2f(698,127);
    glVertex2f(692.5,165);
    glVertex2f(692.5,165);
    glEnd();


    glColor3ub(0, 153, 51);      ///....tree.........beside 3rd Building........6 ///
    circle(3,6,575,100);
    glColor3ub(0, 153, 51);
    circle(5,12,570,110);
    glColor3ub(0, 153, 51);
    circle(5,12,580,110);
    glColor3ub(0, 153, 51);
    circle(5,12,585,105);
    glColor3ub(0, 153, 51);
    circle(5,12,565,120);


    glColor3ub(0, 102, 00);
    circle(5,12,570,122);
    glColor3ub(0, 153, 51);
    circle(5,12,571,122);
    glColor3ub(0, 153, 51);
    circle(5,12,575,140);
    glColor3ub(0, 153, 51);
    circle(4,10,570,135);
    glColor3ub(0, 102, 0);
    circle(4,10,572,133);

    glColor3ub(0, 153, 51);
    circle(4,10,573,132);

    glColor3ub(0, 153, 51);
    circle(5,12,585,130);
    glColor3ub(0, 153, 51);
    circle(5,12,580,130);
    glColor3ub(0, 102, 0);
    circle(5,12,579,125);
    glColor3ub(0, 153, 51);
    circle(5,12,578,124);
    glColor3ub(0, 153, 51);
    circle(5,12,590,120);
    glColor3ub(0, 102, 0);
    circle(5,12,585,115);
    glColor3ub(0, 153, 51);
    circle(5,12,584,115);
    glColor3ub(0, 153, 51);
    circle(5,12,575,118);
    glColor3ub(0, 153, 51);
    circle(5,12,587,140);



    glColor3ub(255, 255, 255);         ///Full..........3rd Building.........comp.
    circle(1,2,575,118);
    glColor3ub(255, 255, 255);
    circle(1,2,575,138);
    glColor3ub(255, 255, 255);
    circle(1,2,585,138);
    glColor3ub(255, 255, 255);
    circle(1,2,580,127);
    glColor3ub(255, 255, 255);
    circle(1,2,564,118);
    glColor3ub(255, 255, 255);
    circle(1,2.5,570,133);
    glColor3ub(255, 255, 255);
    circle(1,1.5,571,109);
    glColor3ub(255, 255, 255);
    circle(1,2.5,585,105);
    glColor3ub(255, 255, 255);
    circle(1,3,590,118);
    glColor3ub(255, 255, 255);
    circle(1,3,585,120);


    glBegin(GL_TRIANGLE_FAN);  ///tree.........beside 3rd Building........6   ///
    glColor3ub(75,35,5);
    glVertex2f(580,90);
    glVertex2f(583,90);
    glVertex2f(582,100);
    glVertex2f(577,130);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);  ///tree.........beside 3rd Building........6   ///
    glColor3ub(75,35,5);
    glVertex2f(580,90);
    glVertex2f(583,90);
    glVertex2f(579,100);
    glVertex2f(573,110);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);  ///tree.........beside 3rd Building........6   ///
    glColor3ub(75,35,5);
    glVertex2f(579,90);
    glVertex2f(583,90);
    glVertex2f(584,110);
    glVertex2f(587,140);
    glEnd();

    glPopMatrix();





}



void drawBus() {
      int roadBase = 150; // Adjust this based on your road's Y-position
      int busOffset = roadBase - 20; // Lower the bus to sit on the road

glBegin(GL_QUADS);
glColor3ub(255, 81, 76);
glVertex2f(j + 90, busOffset + 98);
glVertex2f(j + 95, busOffset + 98);
glVertex2f(j + 95, busOffset + 100);
glVertex2f(j + 90, busOffset + 100);
glEnd();

glBegin(GL_QUADS);
glColor3ub(26, 26, 0);
glVertex2f(j + 94, busOffset + 89);
glVertex2f(j + 96, busOffset + 89);
glVertex2f(j + 96, busOffset + 100);
glVertex2f(j + 94, busOffset + 100);
glEnd();

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

glBegin(GL_QUADS);
glColor3ub(230, 255, 255);
glVertex2f(j + 12, busOffset + 85);
glVertex2f(j + 20, busOffset + 85);
glVertex2f(j + 20, busOffset + 100);
glVertex2f(j + 12, busOffset + 100);
glEnd();

glBegin(GL_QUADS);
glColor3ub(230, 255, 255);
glVertex2f(j + 22, busOffset + 85);
glVertex2f(j + 30, busOffset + 85);
glVertex2f(j + 30, busOffset + 100);
glVertex2f(j + 22, busOffset + 100);
glEnd();

 glColor3ub(255, 255, 204);
    circle(4, 8, j + 45, busOffset + 65);
    circle(2, 4, j + 55, busOffset + 75);
    circle(3, 6, j + 15, busOffset + 75);
    circle(2, 4, j + 35, busOffset + 65);
    circle(2, 4, j + 75, busOffset + 75);

// Bus Wheels (Lowered)
glColor3ub(0, 0, 0);
circle(5, 10, j + 25, busOffset + 55);
glColor3ub(255, 255, 255);
circle(3, 6, j + 25, busOffset + 55);

glColor3ub(0, 0, 0);
circle(5, 10, j + 78, busOffset + 55);
glColor3ub(255, 255, 255);
circle(3, 6, j + 78, busOffset + 55);



glPushMatrix();

// Change color based on day or night
if (isNight) {
    // Bright Yellow Headlights
    glColor3ub(255, 255, 153);
} else {
    // Light Gray (OFF in Day)
    glColor3ub(192, 192, 192);
}

// Draw the headlight shape
glBegin(GL_POLYGON);
glVertex2f(j + 94, busOffset + 62);
glVertex2f(j + 92, busOffset + 60);
glVertex2f(j + 92, busOffset + 65);
glVertex2f(j + 94, busOffset + 63);
glEnd();

// **Now add a flashing headlight beam at NIGHT**
if (isNight) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0, 1.0, 0.5, 0.3); // Yellowish beam with transparency

    glBegin(GL_TRIANGLES);
    glVertex2f(j + 95, busOffset + 62);  // Light source
    glVertex2f(j + 130, busOffset + 80); // Beam extends forward
    glVertex2f(j + 130, busOffset + 45); // Beam extends downward
    glEnd();

    glDisable(GL_BLEND);}
glPopMatrix(); // Restore previous color




}
void drawCar() {
    int roadBase = 200; // Adjust this based on your road's Y-position
    int carOffset = roadBase -5; // Lower the car to sit on the road

    glBegin(GL_QUADS);
 glColor3ub(255, 81, 76);
 glVertex2f(carPosition - 90, carOffset + 98);
 glVertex2f(carPosition - 95, carOffset+ 98);
 glVertex2f(carPosition - 95, carOffset + 100);
 glVertex2f(carPosition - 90, carOffset + 100);
 glEnd();

 glBegin(GL_QUADS);
 glColor3ub(26, 26, 0);
 glVertex2f(carPosition - 94, carOffset + 89);
 glVertex2f(carPosition - 96, carOffset + 89);
 glVertex2f(carPosition - 96, carOffset + 100);
 glVertex2f(carPosition - 94, carOffset + 100);
 glEnd();

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

glBegin(GL_QUADS);
glColor3ub(230, 255, 255);
glVertex2f(carPosition - 12, carOffset + 85);
glVertex2f(carPosition - 20, carOffset + 85);
glVertex2f(carPosition - 20, carOffset + 100);
glVertex2f(carPosition - 12, carOffset + 100);
glEnd();

glBegin(GL_QUADS);
glColor3ub(230, 255, 255);
glVertex2f(carPosition - 22, carOffset + 85);
glVertex2f(carPosition - 30, carOffset + 85);
glVertex2f(carPosition - 30, carOffset + 100);
glVertex2f(carPosition- 22, carOffset + 100);
glEnd();

 glColor3ub(255, 255, 204);
    circle(4, 8, carPosition - 45, carOffset + 65);
    circle(2, 4, carPosition - 55, carOffset + 75);
    circle(3, 6, carPosition - 15, carOffset + 75);
    circle(2, 4, carPosition - 35, carOffset + 65);
    circle(2, 4, carPosition - 75, carOffset + 75);

    glColor3ub(0, 0, 0);
    circle(5, 10, carPosition - 25, carOffset + 55);
    glColor3ub(255, 255, 255);
    circle(3, 6, carPosition - 25, carOffset + 55);

    glColor3ub(0, 0, 0);
    circle(5, 10, carPosition - 78, carOffset + 55);
    glColor3ub(255, 255, 255);
    circle(3, 6, carPosition - 78, carOffset + 55);


    // Set headlight color based on time of day
if (isNight) {
    glColor3ub(255, 255, 153); // Bright Yellow (ON at Night)
} else {
    glColor3ub(192, 192, 192); // Light Gray (OFF in Day)
}

// Draw the headlight
glBegin(GL_POLYGON);
glVertex2f(carPosition - 94, carOffset + 62);
glVertex2f(carPosition - 92, carOffset + 60);
glVertex2f(carPosition - 92, carOffset + 65);
glVertex2f(carPosition - 94, carOffset + 63);
glEnd();

// **Add a flashing headlight beam at NIGHT**
if (isNight) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0, 1.0, 0.5, 0.3); // Yellowish beam with transparency

    glBegin(GL_TRIANGLES);
    glVertex2f(carPosition - 95, carOffset + 62);   // Light source
    glVertex2f(carPosition - 130, carOffset + 80);  // Beam extends forward
    glVertex2f(carPosition - 130, carOffset + 45);  // Beam extends downward
    glEnd();

    glDisable(GL_BLEND);
}





}
void drawCar2()
{
    int roadBase = 150; // Adjust this based on your road's Y-position
    int busOffset = roadBase - 40;
    glPushMatrix();
    glTranslatef(bx + 0, 100, 0);
    glScalef(0.5, 0.5, 1.0);
    glTranslatef(-450, -100, 0);

    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);
    glVertex2d(bx+410, busOffset+100);
    glVertex2d(bx +490, busOffset+100);
    glVertex2d(bx +485, busOffset+130);
    glVertex2d(bx +410, busOffset+130);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(bx +420, busOffset+130);
    glVertex2d(bx +475, busOffset+130);
    glVertex2d(bx +465, busOffset+160);
    glVertex2d(bx +430, busOffset+160);
    glEnd();

    glColor3ub(220, 220, 220);
    glBegin(GL_POLYGON);
    glVertex2d(bx +425, busOffset+130);
    glVertex2d(bx +445, busOffset+130);
    glVertex2d(bx +445, busOffset+150);
    glVertex2d(bx +430, busOffset+150);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(bx +450, busOffset+130);
    glVertex2d(bx +470, busOffset+130);
    glVertex2d(bx +465, busOffset+150);
    glVertex2d(bx +450, busOffset+150);
    glEnd();

    glColor3ub(0, 0, 0);
    circle(10, 14, bx +435, busOffset+100);
    circle(10, 14, bx +465, busOffset+100);

    glColor3ub(245, 245, 245);
    circle(6, 10, bx +435, busOffset+100);
    circle(6, 10, bx +465, busOffset+100);


      // Set headlight color based on time of day
if (isNight) {
    glColor3ub(255, 255, 153); // Bright Yellow (ON at Night)
} else {
    glColor3ub(192, 192, 192); // Light Gray (OFF in Day)
}

// Draw the bus headlight
glBegin(GL_POLYGON);
glVertex2d(bx + 492, busOffset + 115);
glVertex2d(bx + 487, busOffset + 112);
glVertex2d(bx + 487, busOffset + 118);
glVertex2d(bx + 492, busOffset + 116);
glEnd();

// **Add a headlight beam at NIGHT**
if (isNight) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0, 1.0, 0.5, 0.3); // Yellowish beam with transparency

    glBegin(GL_TRIANGLES);
    glVertex2d(bx + 493, busOffset + 115);   // Light source
    glVertex2d(bx + 530, busOffset + 130);   // Beam extends forward
    glVertex2d(bx + 530, busOffset + 100);   // Beam extends downward
    glEnd();

    glDisable(GL_BLEND);
}




    glPopMatrix();


}

void drawTrafficLight() {
    // Draw Traffic Light Pole using Bresenham Algorithm
    glColor3f(0.13f, 0.55f, 0.13f); // Dark Green
    glBegin(GL_QUADS);
        glVertex2f(0, 300);
        glVertex2f(800, 300);
        glVertex2f(800, 400);
        glVertex2f(0, 400);
    glEnd();

    glColor3ub(25, 25, 25);
    drawBresenhamLine(350, 150, 350, 300);

    // Draw Traffic Light Box
    glBegin(GL_QUADS);
    glColor3ub(0, 0, 0); // Black box
    glVertex2f(342, 300);
    glVertex2f(358, 300);
    glVertex2f(358, 350);
    glVertex2f(342, 350);
    glEnd();

    // Lights using Midpoint Circle Algorithm
    if (activeLight == 'R') {
        glColor3ub(255, 0, 0);
        drawMidpointCircle(350, 340, 7);
    } else if (activeLight == 'Y') {
        glColor3ub(255, 255, 0);
        drawMidpointCircle(350, 325, 7);
    } else if (activeLight == 'G') {
        glColor3ub(0, 255, 0);
        drawMidpointCircle(350, 310, 7);
    }
}


void drawTrafficLight2(){
    // Draw Pole using Bresenham Algorithm
    glColor3ub(25, 25, 25);
    drawBresenhamLine(335, 300, 335, 400);

    // Box
    glBegin(GL_QUADS);
    glColor3ub(0, 0, 0); // Black box
    glVertex2f(325, 400);
    glVertex2f(345, 400);
    glVertex2f(345, 450);
    glVertex2f(325, 450);
    glEnd();

    // Lights using Midpoint Circle Algorithm
    if (activeLight == 'R') {
        glColor3ub(255, 0, 0);
        drawMidpointCircle(335, 440, 7);
    } else {
        glColor3ub(50, 50, 50);
        drawMidpointCircle(335, 440, 7);
    }

    if (activeLight == 'Y') {
        glColor3ub(255, 255, 0);
        drawMidpointCircle(335, 425, 7);
    } else {
        glColor3ub(50, 50, 50);
        drawMidpointCircle(335, 425, 7);
    }

    if (activeLight == 'G') {
        glColor3ub(0, 255, 0);
        drawMidpointCircle(335, 410, 7);
    } else {
        glColor3ub(50, 50, 50);
        drawMidpointCircle(335, 410, 7);
    }
}

    float boatX = 0.0;


// ==================== NEW ALGORITHM DEMONSTRATIONS ====================

void drawWaterReflection() {
    glPushMatrix();
    // Reflect buildings in water
    glColor4f(0.5f, 0.5f, 0.5f, 0.3f);
    glScalef(1.0, -0.4, 1.0);
    glTranslatef(0, -300, 0);
    // Building reflection placeholder (the design stays same calling drawbuilding later)
    glPopMatrix();
}

void drawShearedCar() {
    glPushMatrix();
    glTranslatef(600, 180, 0); // Position on road
    
    float shx = 0.3, shy = 0.0;
    float body[4][2] = {{-30,0}, {30,0}, {30,20}, {-30,20}};
    
    // Apply shear
    for(int i=0; i<4; i++) applyShear(body[i][0], body[i][1], shx, shy);
    
    glColor3ub(0, 255, 255);
    glBegin(GL_POLYGON);
    for(int i=0; i<4; i++) glVertex2f(body[i][0], body[i][1]);
    glEnd();
    
    // Wheels with Midpoint Circle
    glColor3ub(0,0,0);
    drawMidpointCircle(-15, -2, 8);
    drawMidpointCircle(15, -2, 8);
    glPopMatrix();
}

void drawBoat()
{
    glPushMatrix();
     glTranslatef(boatX +0.0, 30.0, 0.0); // Move the boat upward by 2 units
       // Save the current transformation matrix
     glScalef(6.0, 6.0, 1.0);


    //glScaled(20,20,0);
    glColor3f(255,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(boatX +3.5,5.5);
    glVertex2f(boatX +3.5,8.5);
    glVertex2f(boatX +19.5,8.5);
    glVertex2f(boatX +19.5,5.5);
    glEnd();
    glColor3f(0.0,0.0,1.0);
    glBegin(GL_POLYGON);
    glVertex2f(boatX +1,5.5);
    glVertex2f(boatX +4,1);
    glVertex2f(boatX +19,1);
    glVertex2f(boatX +21.5,5.5);
    glEnd();
    glColor3f(0.7,0.4,0.2);
    glBegin(GL_POLYGON);
    glVertex2f(boatX +4,5.5);
    glVertex2f(boatX +4,8);
    glVertex2f(boatX +5,8);
    glVertex2f(boatX +5,5.5);
    glEnd();
    glColor3f(0.9,0.9,0.9);
    glBegin(GL_POLYGON);
    glVertex2f(boatX +7,7.5);
    glVertex2f(boatX +7,8);
    glVertex2f(boatX +10,8);
    glVertex2f(boatX +10,7.5);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(boatX +11.5,7.5);
    glVertex2f(boatX +11.5,8);
    glVertex2f(boatX +15,8);
    glVertex2f(boatX +15,7.5);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(boatX +16,7.5);
    glVertex2f(boatX +16,8);
    glVertex2f(boatX +19,8);
    glVertex2f(boatX +19,7.5);
    glEnd();
    glColor3f(0.5,0.5,0.5);
    glBegin(GL_POLYGON);
    glVertex2f(boatX +4.5,8.5);
    glVertex2f(boatX +4.5,10);
    glVertex2f(boatX +18.5,10);
    glVertex2f(boatX +18.5,8.5);
    glEnd();
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_POLYGON);
    glVertex2f(boatX +5.5,10);
    glVertex2f(boatX +5.5,12.9);
    glVertex2f(boatX +7,12.9);
    glVertex2f(boatX +7,10);
    glEnd();
    glColor3f(0.9,0.5,0.2);
    glBegin(GL_POLYGON);
    glVertex2f(boatX +5.5,12.9);
    glVertex2f(boatX +5.5,13.5);
    glVertex2f(boatX +7,13.5);
    glVertex2f(boatX +7,12.9);
    glEnd();
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_POLYGON);
    glVertex2f(boatX +8.5,10);
    glVertex2f(boatX +8.5,12.9);
    glVertex2f(boatX +10,12.9);
    glVertex2f(boatX +10,10);
    glEnd();
    glColor3f(0.9,0.5,0.2);
    glBegin(GL_POLYGON);
    glVertex2f(boatX +8.5,12.9);
    glVertex2f(boatX +8.5,13.5);
    glVertex2f(boatX +10,13.5);
    glVertex2f(boatX +10,12.9);
    glEnd();
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_POLYGON);
    glVertex2f(boatX +11.5,10);
    glVertex2f(boatX +11.5,12.9);
    glVertex2f(boatX +13,12.9);
    glVertex2f(boatX +13,10);
    glEnd();
    glColor3f(0.9,0.5,0.2);
    glBegin(GL_POLYGON);
    glVertex2f(boatX +11.5,12.9);
    glVertex2f(boatX +11.5,13.5);
    glVertex2f(boatX +13,13.5);
    glVertex2f(boatX +13,12.9);
    glEnd();
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_POLYGON);
    glVertex2f(boatX +14.5,10);
    glVertex2f(boatX +14.5,12.9);
    glVertex2f(boatX +16,12.9);
    glVertex2f(boatX +16,10);
    glEnd();
    glColor3f(0.9,0.5,0.2);
    glBegin(GL_POLYGON);
    glVertex2f(boatX +14.5,12.9);
    glVertex2f(boatX +14.5,13.5);
    glVertex2f(boatX +16,13.5);
    glVertex2f(boatX +16,12.9);
    glEnd();
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(boatX +1.5,5.5);
    glVertex2f(boatX +1.5,6.5);
    glVertex2f(boatX +2,6.2);
    glVertex2f(boatX +3,6.2);
    glVertex2f(boatX +3.5,6.5);
    glVertex2f(boatX +3.5,5.5);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(boatX +3.5,5.5);
    glVertex2f(boatX +3.5,6.5);
    glVertex2f(boatX +4,6.2);
    glVertex2f(boatX +5,6.2);
    glVertex2f(boatX +5.5,6.5);
    glVertex2f(boatX +5.5,5.5);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(boatX +5.5,5.5);
    glVertex2f(boatX +5.5,6.5);
    glVertex2f(boatX +6,6.2);
    glVertex2f(boatX +7,6.2);
    glVertex2f(boatX +7.5,6.5);
    glVertex2f(boatX +7.5,5.5);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(boatX +7.5,5.5);
    glVertex2f(boatX +7.5,6.5);
    glVertex2f(boatX +8,6.2);
    glVertex2f(boatX +9,6.2);
    glVertex2f(boatX +9.5,6.5);
    glVertex2f(boatX +9.5,5.5);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(boatX +9.5,5.5);
    glVertex2f(boatX +9.5,6.5);
    glVertex2f(boatX +10,6.2);
    glVertex2f(boatX +11,6.2);
    glVertex2f(boatX +11.5,6.5);
    glVertex2f(boatX +11.5,5.5);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(boatX +11.5,5.5);
    glVertex2f(boatX +11.5,6.5);
    glVertex2f(boatX +12,6.2);
    glVertex2f(boatX +13,6.2);
    glVertex2f(boatX +13.5,6.5);
    glVertex2f(boatX +13.5,5.5);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(boatX +13.5,5.5);
    glVertex2f(boatX +13.5,6.5);
    glVertex2f(boatX +14,6.2);
    glVertex2f(boatX +15,6.2);
    glVertex2f(boatX +15.5,6.5);
    glVertex2f(boatX +15.5,5.5);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(boatX +15.5,5.5);
    glVertex2f(boatX +15.5,6.5);
    glVertex2f(boatX +16,6.2);
    glVertex2f(boatX +17,6.2);
    glVertex2f(boatX +17.5,6.5);
    glVertex2f(boatX +17.5,5.5);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(boatX +17.5,5.5);
    glVertex2f(boatX +17.5,6.5);
    glVertex2f(boatX +18,6.2);
    glVertex2f(boatX +19,6.2);
    glVertex2f(boatX +19.5,6.5);
    glVertex2f(boatX +19.5,5.5);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(boatX +19.5,5.5);
    glVertex2f(boatX +19.5,6.5);
    glVertex2f(boatX +20,6.2);
    glVertex2f(boatX +20.5,6.2);
    glVertex2f(boatX +21,6.5);
    glVertex2f(boatX +21,5.5);
    glEnd();
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(boatX +4.5,3.5);
    glVertex2f(boatX +4.5,4.5);
    glVertex2f(boatX +5.5,4.5);
    glVertex2f(boatX +5.5,3.5);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(boatX +7,3.5);
    glVertex2f(boatX +7,4.5);
    glVertex2f(boatX +8,4.5);
    glVertex2f(boatX +8,3.5);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(boatX +9.5,3.5);
    glVertex2f(boatX +9.5,4.5);
    glVertex2f(boatX +10.5,4.5);
    glVertex2f(boatX +10.5,3.5);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(boatX +12,3.5);
    glVertex2f(boatX +12,4.5);
    glVertex2f(boatX +13,4.5);
    glVertex2f(boatX +13,3.5);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(boatX +14.5,3.5);
    glVertex2f(boatX +14.5,4.5);
    glVertex2f(boatX +15.5,4.5);
    glVertex2f(boatX +15.5,3.5);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(boatX +17,3.5);
    glVertex2f(boatX +17,4.5);
    glVertex2f(boatX +18,4.5);
    glVertex2f(boatX +18,3.5);
    glEnd();
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(boatX +5,10);
    glVertex2f(boatX +5,11);
    glVertex2f(boatX +5.5,10.8);
    glVertex2f(boatX +6.5,10.8);
    glVertex2f(boatX +7,11);
    glVertex2f(boatX +7,10);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(boatX +7,10);
    glVertex2f(boatX +7,11);
    glVertex2f(boatX +7.5,10.8);
    glVertex2f(boatX +8.5,10.8);
    glVertex2f(boatX +9,11);
    glVertex2f(boatX +9,10);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(boatX +9,10);
    glVertex2f(boatX +9,11);
    glVertex2f(boatX +9.5,10.8);
    glVertex2f(boatX +10.5,10.8);
    glVertex2f(boatX +11,11);
    glVertex2f(boatX +11,10);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(boatX +11,10);
    glVertex2f(boatX +11,11);
    glVertex2f(boatX +11.5,10.8);
    glVertex2f(boatX +12.5,10.8);
    glVertex2f(boatX +13,11);
    glVertex2f(boatX +13,10);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(boatX +13,10);
    glVertex2f(boatX +13,11);
    glVertex2f(boatX +13.5,10.8);
    glVertex2f(boatX +14.5,10.8);
    glVertex2f(boatX +15,11);
    glVertex2f(boatX +15,10);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(boatX +15,10);
    glVertex2f(boatX +15,11);
    glVertex2f(boatX +15.5,10.8);
    glVertex2f(boatX +16.5,10.8);
    glVertex2f(boatX +17,11);
    glVertex2f(boatX +17,10);

    glEnd();

 glPopMatrix();
}


void drawLampPost() {
    float positions[] = {450, 250, 50, 650};
    for (int i = 0; i < 4; i++) {
        float pos = positions[i];
        
        // Base using Bresenham
        glColor3ub(20, 20, 20);
        drawBresenhamLine(pos - 10, 150, pos + 10, 150);
        
        // Pole using Bresenham
        glColor3ub(30, 30, 30);
        drawBresenhamLine(pos, 170, pos, 270);
        
        // Lamp head using Bresenham
        glColor3ub(20, 20, 20);
        drawBresenhamLine(pos - 12, 270, pos + 12, 270);
        
        // Light Bulb using Midpoint Circle
        if (isNight) glColor3ub(255, 255, 200);
        else glColor3ub(150, 150, 100);
        drawMidpointCircle(pos, 275, 5);
    }
}














void drawTree() {
  glBegin(GL_TRIANGLE_FAN);  /// Bottom tree trunk  3///
glColor3ub(75, 35, 5);
glVertex3f(550, 150, 0);   // Adjusted to align with the road
glVertex3f(560, 150, 0);
glVertex3f(560, 190, 0);   // Taller trunk
glVertex3f(550, 190, 0);
glEnd();

glBegin(GL_TRIANGLE_FAN);  /// First Layer of Leaves (Bigger & Higher) ///
glColor3ub(0, 102, 0);
glVertex3f(540, 180, 0);
glVertex3f(570, 180, 0);
glVertex3f(555, 230, 0);
glVertex3f(555, 230, 0);
glEnd();

glBegin(GL_TRIANGLE_FAN);  /// Second Layer of Leaves (Bigger & Higher) ///
glColor3ub(0, 102, 0);
glVertex3f(543, 190, 0);
glVertex3f(567, 190, 0);
glVertex3f(555, 240, 0);
glVertex3f(555, 240, 0);
glEnd();






 glBegin(GL_TRIANGLE_FAN);  /// Bottom tree trunk 2///
glColor3ub(75, 35, 5);
glVertex3f(380, 150, 0);   // Adjusted to align with the road
glVertex3f(390, 150, 0);
glVertex3f(390, 190, 0);   // Taller trunk
glVertex3f(380, 190, 0);
glEnd();

glBegin(GL_TRIANGLE_FAN);  /// First Layer of Leaves (Bigger & Higher) ///
glColor3ub(0, 102, 0);
glVertex3f(370, 180, 0);
glVertex3f(400, 180, 0);
glVertex3f(385, 230, 0);
glVertex3f(385, 230, 0);
glEnd();

glBegin(GL_TRIANGLE_FAN);  /// Second Layer of Leaves (Bigger & Higher) ///
glColor3ub(0, 102, 0);
glVertex3f(373, 190, 0);
glVertex3f(397, 190, 0);
glVertex3f(385, 240, 0);
glVertex3f(385, 240, 0);
glEnd();







glBegin(GL_TRIANGLE_FAN);  /// Bottom tree trunk1 ///
glColor3ub(75, 35, 5);
glVertex3f(140, 150, 0);   // Adjusted to align with the road
glVertex3f(150, 150, 0);
glVertex3f(150, 190, 0);   // Taller trunk
glVertex3f(140, 190, 0);
glEnd();

glBegin(GL_TRIANGLE_FAN);  /// First Layer of Leaves (Bigger & Higher) ///
glColor3ub(0, 102, 0);
glVertex3f(130, 180, 0);
glVertex3f(160, 180, 0);
glVertex3f(145, 230, 0);
glVertex3f(145, 230, 0);
glEnd();

glBegin(GL_TRIANGLE_FAN);  /// Second Layer of Leaves (Bigger & Higher) ///
glColor3ub(0, 102, 0);
glVertex3f(133, 190, 0);
glVertex3f(157, 190, 0);
glVertex3f(145, 240, 0);
glVertex3f(145, 240, 0);
glEnd();



}


float humanY = 160;  // Start position at the first stripe
float stepSize = 0.2; // Movement speed
int stepFrame = 0;  // Frame counter for animation

void drawHuman(float x, float y, int step) {
    glPushMatrix();

    glTranslatef(x, y, 0);
    glScalef(2.0, 2.0, 1.0f);

    // Torso
    glColor3f(0.0, 0.0, 1.0); // Blue shirt
    glBegin(GL_POLYGON);
        glVertex2f(-3, 0);
        glVertex2f(3, 0);
        glVertex2f(3, 10);
        glVertex2f(-3, 10);
    glEnd();

    // Head
    glColor3f(1.0, 0.8, 0.6); // Skin color
    glBegin(GL_POLYGON);
        glVertex2f(-2, 10);
        glVertex2f(2, 10);
        glVertex2f(2, 14);
        glVertex2f(-2, 14);
    glEnd();

    // Legs (Alternating movement for animation)
    glColor3f(255, 0.2, 0.2); // Black pants
   // glBegin(GL_LINES);
   glBegin(GL_POLYGON);
        if (step % 20 < 10) { // One leg forward
           // Left leg (rectangle for thickness)
        glVertex2f(-2, 0);    // Top-left
        glVertex2f(-3, 0);    // Top-right
        glVertex2f(-3, -7);   // Bottom-right (longer for full leg)
        glVertex2f(-2, -7);   // Bottom-left
    } else { // Other leg forward
        // Right leg (rectangle for thickness)
        glVertex2f(2, 0);     // Top-left
        glVertex2f(3, 0);     // Top-right
        glVertex2f(3, -7);    // Bottom-right (longer for full leg)
        glVertex2f(2, -7);    // Bottom-left
    }
glEnd();

    // Arms (Swinging motion)
    glColor3f(1.0, 0.8, 0.6); // Skin color
    glBegin(GL_POLYGON);
    //glBegin(GL_LINES);
        if (step % 20 < 10) { // One arm forward

             glVertex2f(-3, 8); glVertex2f(-4, 9); // Top left
        glVertex2f(-5, 5); glVertex2f(-4, 4); // Bottom left
        glVertex2f(-4, 4); glVertex2f(-3, 5); // Bottom right
        glVertex2f(-3, 8); glVertex2f(-4, 9); // Top right
    } else { // Other arm forward
        glVertex2f(3, 8); glVertex2f(4, 9); // Top left
        glVertex2f(5, 5); glVertex2f(4, 4); // Bottom left
        glVertex2f(4, 4); glVertex2f(3, 5); // Bottom right
        glVertex2f(3, 8); glVertex2f(4, 9); // Top right
    }
glEnd();
    glPopMatrix();
}







void updateHuman(int value) {
    int crosswalkPosition = 160;  // Y position of the crosswalk
    int stopPoint = crosswalkPosition - 5; // Stop slightly before the crosswalk

    if ((lightState == 2 || lightState == 1)  && humanY >= stopPoint && humanY <= crosswalkPosition) {
        // Stop moving AND stop animation when the traffic light is RED
    } else {
        humanY += stepSize; // Move upward normally when GREEN or past the crosswalk
        stepFrame++;  // Only animate when moving
    }

    if (humanY > 300) { // Reset when reaching the top
        humanY = 160;
    }

    glutPostRedisplay(); // Redraw scene
    glutTimerFunc(16, updateHuman, 0); // Call timer function again (~60 FPS)
}







// Flag to toggle rain

// Structure for raindrops
struct Raindrop {
    float x, y;  // Position
    float speed; // Falling speed
} rain[MAX_RAIN];

// Initialize raindrops randomly across the full screen
void initRain() {
    for (int i = 0; i < MAX_RAIN; i++) {
        rain[i].x = (rand() % SCREEN_WIDTH) ;  // Spread across full width
        rain[i].y = (rand() % SCREEN_HEIGHT) ; // Spread across full height
        rain[i].speed = (rand() % 5) + 2;  // Random falling speed
    }
}

// Function to draw rain
void drawRain() {
    if (!isRaining) return; // If rain is off, don't draw

    glColor3f(1.0, 1.0, 1.0); // White color for raindrops
    glBegin(GL_LINES);

    for (int i = 0; i < MAX_RAIN; i++) {
        glVertex2f(rain[i].x, rain[i].y);
        glVertex2f(rain[i].x, rain[i].y - 10); // Longer rain streak for visibility
    }

    glEnd();
}

// Update raindrop positions for animation
void updateRain(int value) {
     if (isRaining) {
        for (int i = 0; i < MAX_RAIN; i++) {
            rain[i].y -= rain[i].speed; // Move raindrop down

            // Reset when raindrop reaches the bottom
            if (rain[i].y < 0) {
                rain[i].x = rand() % SCREEN_WIDTH;  // Random X position again
                rain[i].y = SCREEN_HEIGHT; // Reset to top of the screen
            }
        }
    }

    glutPostRedisplay(); // Redraw scene
    glutTimerFunc(30, updateRain, 0); // Calls itself every 30 ms
}


struct Snow {
    float x, y;
    float speed;
    float drift; // Sideways movement
};

Snow snow[MAX_SNOW];
bool isSnowing = false; // Toggle for snowfall

void initSnow() {
    for (int i = 0; i < MAX_SNOW; i++) {
        snow[i].x = rand() % SCREEN_WIDTH;
        snow[i].y = rand() % SCREEN_HEIGHT + SCREEN_HEIGHT / 2; // Start from sky
        snow[i].speed = (rand() % 2) + 1; // Slower than rain
        snow[i].drift = (rand() % 3) - 1; // Random side drift
    }
}

void updateSnow(int value) {
    if (isSnowing) {
        for (int i = 0; i < MAX_SNOW; i++) {
            snow[i].y -= snow[i].speed; // Snow falls down
            snow[i].x += snow[i].drift; // Snow drifts sideways

            // Reset snowflake if it reaches the bottom
            if (snow[i].y < 0) {
                snow[i].x = rand() % SCREEN_WIDTH;
                snow[i].y = SCREEN_HEIGHT; // Reset to top
                snow[i].drift = (rand() % 3) - 1;
            }
        }
    }

    glutPostRedisplay(); // Redraw scene
    glutTimerFunc(50, updateSnow, 0); // Slow animation for soft snow effect
}

void drawSnow() {
    if (isSnowing) {
        glColor3f(1.0, 1.0, 1.0); // White snow
        glPointSize(3); // Make snowflakes bigger than rain
        glBegin(GL_POINTS);
        for (int i = 0; i < MAX_SNOW; i++) {
            glVertex2f(snow[i].x, snow[i].y);
        }
        glEnd();
    }
}


float birdX = -50;     // Bird's starting X position
float birdY = 600;     // Bird's height in the sky
float wingAngle = 0;   // Wing movement animation
bool wingUp = true;    // Wing flapping direction

void drawBird() {
    if (!isNight) { // Only draw during daytime
        glColor3ub(0, 0, 0); // Black bird

        glPushMatrix();
        glTranslatef(birdX, birdY+80, 0); // Move bird

        // Body (Smaller)
        glBegin(GL_POLYGON);
        glVertex2f(-5, 0);
        glVertex2f(5, 2.5);
        glVertex2f(7.5, 0);
        glVertex2f(5, -2.5);
        glVertex2f(-5, 0);
        glEnd();

        // Head (Smaller)
        glBegin(GL_POLYGON);
        glVertex2f(5, 1);
        glVertex2f(7.5, 1.5);
        glVertex2f(7.5, -1.5);
        glVertex2f(5, -1);
        glEnd();

        // Beak (Smaller)
        glColor3ub(255, 165, 0); // Orange beak
        glBegin(GL_TRIANGLES);
        glVertex2f(7.5, 0.5);
        glVertex2f(9, 0);
        glVertex2f(7.5, -0.5);
        glEnd();

        glColor3ub(0, 0, 0); // Black wings

        // Left Wing (Flapping, Smaller)
        glPushMatrix();
        glTranslatef(-2.5, 0, 0);
        glRotatef(wingAngle, 0, 0, 1);
        glBegin(GL_TRIANGLES);
        glVertex2f(-5, 0);
        glVertex2f(-12, 5);
        glVertex2f(-2.5, 2.5);
        glEnd();
        glPopMatrix();

        // Right Wing (Flapping, Smaller)
        glPushMatrix();
        glTranslatef(2.5, 0, 0);
        glRotatef(-wingAngle, 0, 0, 1);
        glBegin(GL_TRIANGLES);
        glVertex2f(5, 0);
        glVertex2f(12, 5);
        glVertex2f(2.5, 2.5);
        glEnd();
        glPopMatrix();

        glPopMatrix();
    }
}

void updateBird(int value) {
    if (!isNight) {
        birdX += 5; // Move right
        if (birdX > 850) birdX = -50; // Reset when out of screen

        // Animate wing flapping
        if (wingUp) wingAngle += 5;
        else wingAngle -= 5;

        if (wingAngle >= 20) wingUp = false;
        if (wingAngle <= -20) wingUp = true;
    }

    glutPostRedisplay();
    glutTimerFunc(50, updateBird, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawStars();
    drawSunAndClouds();
    
    // Algorithm demonstration: Water reflection using reflection transformation
    drawWaterReflection();
    
    drawBus();
    drawCar();
    drawCar2();
    
    // Algorithm demonstration: Sheared car using shear transformation
    drawShearedCar();


    drawTrafficLight();
    drawTrafficLight2();
    drawBoat();
    drawLampPost();
    drawTree();
    drawHuman(325, humanY, stepFrame); // Adjust x-coordinate as needed

    drawbuilding();
    drawBird();

    drawRain();
    drawSnow(); // Draw rain if enabled
    glutSwapBuffers();

    glFlush();
}


void handleKeyboard(unsigned char key, int x, int y) {
     if (key == 'n' || key == 'N') {
        isNight = true;  // Set to night mode
        myInit();  // Reinitialize background color
        glutPostRedisplay(); // Redraw scene
    }
    else if (key == 'd' || key == 'D') {
        isNight = false;  // Set to day mode
        myInit();  // Reinitialize background color
        glutPostRedisplay(); // Redraw scene
    }

    if (key == 'p' || key == 'P') {
        isSnowing = !isSnowing;
        if (isSnowing) {
            isRaining = false; // Stop rain if snow starts
            initSnow();
            glutTimerFunc(50, updateSnow, 0);
        }
    } else if (key == 'b' || key == 'B') { // Enable rain again
        isRaining = !isRaining;
        if (isRaining) {
            isSnowing = false; // Stop snow if rain starts
            initRain();
            glutTimerFunc(30, updateRain, 0);
        }
    }
    if (key == 'w'){
        busSpeed += 0.2;} else if (key == 's') {
        if (busSpeed > 0.2) busSpeed -= 0.2;
    }
    if (key == 'w'){
        CarSpeed += 0.2;} else if (key == 's') {
        if (CarSpeed > 0.2) CarSpeed -= 0.2;
    }
     if (key == 'r' || key == 'R') {
        activeLight = 'R';
        lightState = 0; // Set to Red // Red light
    } else if (key == 'y' || key == 'Y') {
        activeLight = 'Y'; // Yellow light
        lightState = 1; // Set to Red
    } else if (key == 'g' || key == 'G') {
        activeLight = 'G'; // Green light
        lightState = 2; // Set to Red
    }
}



void handleMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON ) {
        busSpeed += 0.2;
    } else if (button == GLUT_RIGHT_BUTTON ) {
        if (busSpeed > 0.2) busSpeed -= 0.2;
    }
}

//#include <GLUT/glut.h>

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // Set the desired window size
    int windowWidth = 1450;
    int windowHeight = 750;
    glutInitWindowSize(windowWidth, windowHeight);

    // Get the screen width and height
    int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    // Calculate the position of the window to center it
    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    // Create the window and position it at the calculated position
    glutInitWindowPosition(posX, posY);
    glutCreateWindow("3D Simulated City");

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

    glutMainLoop();
}
