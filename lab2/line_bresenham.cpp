#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
void drawPixel(int x, int y, float r, float b, float g) {
  glBegin(GL_POINTS);
  glColor3f(r, b, g);
  glVertex2i(x, y);
  glEnd();
}

void bresenhamLine(int x1, int y1, int x2, int y2, float r, float b, float g) {
  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int sx = (x1 < x2) ? 1 : -1;
  int sy = (y1 < y2) ? 1 : -1;
  int err = dx - dy;

  while (1) {
    drawPixel(x1, y1, r, b, g);
    if (x1 == x2 && y1 == y2)
      break;

    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y1 += sy;
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
    bresenhamLine(width / 2, height / 2, 500, 600, 1, 0, 0);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
