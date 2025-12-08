#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <vector>
void drawPixel(int x, int y, float r, float b, float g) {
  glBegin(GL_POINTS);
  glColor3f(r, b, g);
  glVertex2i(x, y);
  glEnd();
}

typedef struct {
  int noofsides;
  std::vector<float> x1;
  std::vector<float> y1;
} plot;
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
void drawaxis(int originx, int originy, int sizex, int sizey, float r, float b,
              float g) {
  bresenhamLine(originx, originy, originx + sizex, originy, r, b, g);
  bresenhamLine(originx, originy, originx, originy + sizey, r, b, g);
}
plot setupplot() {
  // 400, 200, 400+500, 200+500,
  plot p1;
  p1.noofsides = 3;
  p1.x1 = {450, 500, 600, 700};
  p1.y1 = {300, 400, 350, 370};
  return p1;
}
void drawplot(plot p1) {
  for (int i = 0; i < p1.noofsides; ++i) {
    bresenhamLine(p1.x1[i], p1.y1[i], p1.x1[i + 1], p1.y1[i + 1], 1.0f, 0.0f,
                  0.0f);
  }
}
int main() {
  glfwInit();
  GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);

  int width = mode->width;
  int height = mode->height;

  GLFWwindow *window = glfwCreateWindow(width, height, "Plot", NULL, NULL);
  if (window == NULL)
    return -1;

  glfwMakeContextCurrent(window);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, width, 0, height, -1, 1);
  glMatrixMode(GL_MODELVIEW);

  glPointSize(2);
  plot p1 = setupplot();
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glColor3f(1, 1, 1);
    drawaxis(400, 200, 500, 500, 1, 1, 1);
    drawplot(p1);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
