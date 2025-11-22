#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cstdlib>
#include <math.h>
#include <vector>

typedef struct {
  int noofsides;
  std::vector<float> x1;
  std::vector<float> y1;
} polygon;

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
polygon setupPolygonsmalltraingle(int screenwidth, int screenheight) {
  polygon p1;
  p1.noofsides = 3;
  float cx = screenwidth / 2.0f;
  float cy = screenheight / 2.0f;
  p1.x1 = {cx, cx + 80, cx + 40, cx};
  p1.y1 = {cy, cy, cy + 80, cy};
  return p1;
}
polygon setupPolygonouterlayer(int screenwidth, int screenheight) {
  polygon p1;
  p1.noofsides = 7;
  float cx = screenwidth / 2.0f;
  float cy = screenheight / 2.0f;
  p1.x1 = {cx - 80, cx + 40, cx + 160, cx + 100,
           cx + 80, cx - 5,  cx - 25,  cx - 80};
  p1.y1 = {cy - 80, cy + 160, cy - 80, cy - 80,
           cy - 30, cy - 30,  cy - 80, cy - 80};
  return p1;
}
void drawpolygon(polygon p1) {
  for (int i = 1; i <= p1.noofsides - 1; ++i) {
    bresenhamLine(p1.x1[i], p1.y1[i], p1.x1[i + 1], p1.y1[i + 1], 1.0f, 1.0f,
                  1.0f);
  }

  bresenhamLine(p1.x1[p1.noofsides], p1.y1[p1.noofsides], p1.x1[1], p1.y1[1],
                1.0f, 1.0f, 1.0f);
}

void scanlinefillmethod(const polygon &p1, float r, float b, float g) {
  int n = p1.noofsides;

  float miny = 1e9, maxy = -1e9;
  for (int i = 0; i < n; i++) {
    miny = std::min(miny, p1.y1[i]);
    maxy = std::max(maxy, p1.y1[i]);
  }

  std::vector<float> invslope(n);

  for (int i = 0; i < n; i++) {
    int j = (i + 1) % n;

    float dy = p1.y1[j] - p1.y1[i];
    float dx = p1.x1[j] - p1.x1[i];

    if (dy == 0) {
      invslope[i] = 0;
    } else {
      invslope[i] = dx / dy;
    }
  }

  for (int y = (int)miny; y <= (int)maxy; y++) {
    std::vector<float> intersections;

    for (int i = 0; i < n; i++) {
      int j = (i + 1) % n;

      float y1 = p1.y1[i];
      float y2 = p1.y1[j];

      if ((y >= std::min(y1, y2)) && (y < std::max(y1, y2))) {
        float x = p1.x1[i] + (y - y1) * invslope[i];
        intersections.push_back(x);
      }
    }

    if (intersections.size() < 2)
      continue;

    std::sort(intersections.begin(), intersections.end());

    for (int k = 0; k < intersections.size() - 1; k += 2) {
      int x1 = (int)std::round(intersections[k]);
      int x2 = (int)std::round(intersections[k + 1]);

      bresenhamLine(x1, y, x2, y, r, b, g);
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
      glfwCreateWindow(width, height, "Character 'A'", NULL, NULL);
  if (window == NULL)
    return -1;

  glfwMakeContextCurrent(window);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, width, 0, height, -1, 1);
  glMatrixMode(GL_MODELVIEW);

  glPointSize(2);
  polygon drawing1 = setupPolygonsmalltraingle(width, height);
  polygon drawing2 = setupPolygonouterlayer(width, height);
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glColor3f(1, 1, 1);
    drawpolygon(drawing1);
    drawpolygon(drawing2);
    scanlinefillmethod(drawing2, 0.0f, 1.0f, 0.705f);
    scanlinefillmethod(drawing1, 0.0f, 0.0f, 0.0f);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
