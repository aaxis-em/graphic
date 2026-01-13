#include <GLFW/glfw3.h>
#include <stdio.h>

#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
#define TOP 8

float xmin = 100, ymin = 100, xmax = 400, ymax = 400;

int computeCode(float x, float y) {
  int code = 0;
  if (x < xmin)
    code |= LEFT;
  if (x > xmax)
    code |= RIGHT;
  if (y < ymin)
    code |= BOTTOM;
  if (y > ymax)
    code |= TOP;
  return code;
}

int cohenSutherland(float *x1, float *y1, float *x2, float *y2) {
  int code1 = computeCode(*x1, *y1);
  int code2 = computeCode(*x2, *y2);

  while (1) {
    if ((code1 | code2) == 0)
      return 1;
    else if (code1 & code2)
      return 0;

    float x, y;
    int out = code1 ? code1 : code2;

    if (out & TOP) {
      x = *x1 + (*x2 - *x1) * (ymax - *y1) / (*y2 - *y1);
      y = ymax;
    } else if (out & BOTTOM) {
      x = *x1 + (*x2 - *x1) * (ymin - *y1) / (*y2 - *y1);
      y = ymin;
    } else if (out & RIGHT) {
      y = *y1 + (*y2 - *y1) * (xmax - *x1) / (*x2 - *x1);
      x = xmax;
    } else {
      y = *y1 + (*y2 - *y1) * (xmin - *x1) / (*x2 - *x1);
      x = xmin;
    }

    if (out == code1) {
      *x1 = x;
      *y1 = y;
      code1 = computeCode(*x1, *y1);
    } else {
      *x2 = x;
      *y2 = y;
      code2 = computeCode(*x2, *y2);
    }
  }
}

int main() {
  glfwInit();
  GLFWwindow *window =
      glfwCreateWindow(600, 600, "Cohen-Sutherland Line Clipping", NULL, NULL);
  glfwMakeContextCurrent(window);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 600, 0, 600, -1, 1);

  float x1 = 50, y1 = 500, x2 = 500, y2 = 50;
  float cx1 = x1, cy1 = y1, cx2 = x2, cy2 = y2;

  int visible = cohenSutherland(&cx1, &cy1, &cx2, &cy2);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Clipping window
    glColor3f(1, 1, 1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xmin, ymin);
    glVertex2f(xmax, ymin);
    glVertex2f(xmax, ymax);
    glVertex2f(xmin, ymax);
    glEnd();

    // Original line (red)
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();

    // Clipped line (green)
    if (visible) {
      glColor3f(0, 1, 0);
      glBegin(GL_LINES);
      glVertex2f(cx1, cy1);
      glVertex2f(cx2, cy2);
      glEnd();
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
