#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdlib>
void drawPixel(int x, int y, float r, float b, float g) {
  glBegin(GL_POINTS);
  glColor3f(r, b, g);
  glVertex2i(x, y);
  glEnd();
}

void MidPointEllipse(int centerx, int centery, int majoraxis, int minoraxis,
                     float r, float b, float g) {
  int x0 = 0;
  int y0 = minoraxis;

  int a2 = majoraxis * majoraxis;
  int b2 = minoraxis * minoraxis;

  int p1 = b2 - a2 * minoraxis + a2 / 4;

  while (b2 * x0 <= a2 * y0) {
    drawPixel(centerx + x0, centery + y0, r, b, g);
    drawPixel(centerx - x0, centery + y0, r, b, g);
    drawPixel(centerx + x0, centery - y0, r, b, g);
    drawPixel(centerx - x0, centery - y0, r, b, g);
    if (p1 < 0) {
      x0++;
      p1 += 2 * b2 * x0 + b2;
    } else {
      x0++;
      y0--;
      p1 += 2 * b2 * x0 - 2 * a2 * y0 + b2;
    }
  }

  int p2 = b2 * (x0 + 0.5) * (x0 + 0.5) + a2 * (y0 - 1) * (y0 - 1) - a2 * b2;
  while (y0 >= 0) {
    drawPixel(centerx + x0, centery + y0, r, b, g);
    drawPixel(centerx - x0, centery + y0, r, b, g);
    drawPixel(centerx + x0, centery - y0, r, b, g);
    drawPixel(centerx - x0, centery - y0, r, b, g);

    if (p2 > 0) {
      y0--;
      p2 -= 2 * a2 * y0 + a2;
    } else {
      x0++;
      y0--;
      p2 += 2 * b2 * x0 - 2 * a2 * y0 + a2;
    }
  }
}
int main() {
  glfwInit();
  GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);

  int width = mode->width;
  int height = mode->height;

  GLFWwindow *window =
      glfwCreateWindow(width, height, "Drawing line", NULL, NULL);
  if (window == NULL)
    return -1;

  glfwMakeContextCurrent(window);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, width, 0, height, -1, 1);
  glMatrixMode(GL_MODELVIEW);

  glPointSize(2);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glColor3f(1, 1, 1);
    MidPointEllipse(width / 2, height / 2, 300, 100, 1, 0, 0);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
