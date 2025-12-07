#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
void drawPixel(int x, int y, float r, float b, float g) {
  glBegin(GL_POINTS);
  glColor3f(r, b, g);
  glVertex2i(x, y);
  glEnd();
}

void MidPointCircle(int x0, int y0, int radius, float r, float b, float g) {
  int p = 1 - radius;
  int x = 0;
  int y = radius;
  while (x <= y) {
    drawPixel(x0 + x, y0 + y, r, b, g);
    drawPixel(x0 + y, y0 + x, r, b, g);
    drawPixel(x0 - y, y0 + x, r, b, g);
    drawPixel(x0 - x, y0 + y, r, b, g);
    drawPixel(x0 - x, y0 - y, r, b, g);
    drawPixel(x0 - y, y0 - x, r, b, g);
    drawPixel(x0 + y, y0 - x, r, b, g);
    drawPixel(x0 + x, y0 - y, r, b, g);

    if (p < 0) {
      x++;
      p += 2 * x + 1;
    } else {
      x++;
      y--;
      p += 2 * (x - y) + 1;
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
    MidPointCircle(width / 2, height / 2, 100, 1, 0, 0);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
