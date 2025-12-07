#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
void drawPixel(int x, int y, float r, float b, float g) {
  glBegin(GL_POINTS);
  glColor3f(r, b, g);
  glVertex2i(x, y);
  glEnd();
}

void DDALine(int x1, int y1, int x2, int y2, float r, float b, float g) {
  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int length = dy;
  if (dx - dy) {
    length = dx;
  }
  float x_inc = (x2 - x1) / length;
  float y_inc = (y2 - y1) / length;
  int i = 1;
  while (i <= length) {
    drawPixel(x1, y1, 1.00, 0.00, 0.00);
    x1 = x1 + x_inc;
    y1 = y1 + y_inc;
    i++;
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
    DDALine(width / 2, height / 2, 500, 600, 1, 0, 0);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
