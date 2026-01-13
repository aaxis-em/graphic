#include <GLFW/glfw3.h>
#include <math.h>

#define PI 3.141592653589793

// Rectangle (homogeneous coordinates)
float rect[3][4] = {{0, 100, 100, 0}, {0, 0, 100, 100}, {1, 1, 1, 1}};

void multiply(float A[3][3], float B[3][4], float R[3][4]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
      R[i][j] = 0;
      for (int k = 0; k < 3; k++)
        R[i][j] += A[i][k] * B[k][j];
    }
  }
}

void offset(float dx, float dy, float P[3][4]) {
  for (int i = 0; i < 4; i++) {
    P[0][i] += dx;
    P[1][i] += dy;
  }
}

void drawShape(float P[3][4]) {
  glBegin(GL_LINE_LOOP);
  for (int i = 0; i < 4; i++)
    glVertex2f(P[0][i], P[1][i]);
  glEnd();
}

int main() {
  glfwInit();
  GLFWwindow *window =
      glfwCreateWindow(800, 600, "2D Transformations – GLFW", NULL, NULL);
  glfwMakeContextCurrent(window);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 800, 0, 600, -1, 1);
  glMatrixMode(GL_MODELVIEW);

  float T[3][3] = {{1, 0, 80}, {0, 1, 80}, {0, 0, 1}};
  float S[3][3] = {{1.5, 0, 0}, {0, 1.5, 0}, {0, 0, 1}};
  float R[3][3] = {
      {cos(PI / 6), -sin(PI / 6), 0}, {sin(PI / 6), cos(PI / 6), 0}, {0, 0, 1}};
  float Ref[3][3] = {{1, 0, 0}, {0, -1, 0}, {0, 0, 1}};
  float Sh[3][3] = {{1, 0.5, 0}, {0, 1, 0}, {0, 0, 1}};

  float A[3][4], B[3][4], C[3][4], D[3][4];

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    // 1. Original (White)
    glColor3f(1, 1, 1);
    float T_vis[3][3] = {{1, 0, 200}, {0, 1, 400}, {0, 0, 1}};
    multiply(T_vis, rect, A);
    drawShape(A);

    // 2. Translation (Red)
    glColor3f(1, 0, 0);
    multiply(T, rect, A);
    offset(400, 400, A);
    drawShape(A);

    // 3. Scaling (Green)
    glColor3f(0, 1, 0);
    multiply(S, rect, A);
    offset(200, 250, A);
    drawShape(A);

    // 4. Rotation (Blue)
    glColor3f(0, 0, 1);
    multiply(R, rect, A);
    offset(450, 250, A);
    drawShape(A);

    // 5. Reflection (Yellow)
    glColor3f(1, 1, 0);
    multiply(Ref, rect, A);
    offset(200, 100, A);
    drawShape(A);

    // 6. Shearing (Light Blue)
    glColor3f(0, 1, 1);
    multiply(Sh, rect, A);
    offset(450, 100, A);
    drawShape(A);

    // 7. Composite (Pink)
    glColor3f(1, 0, 1);
    multiply(T, rect, A);
    multiply(S, A, B);
    multiply(R, B, C);
    multiply(Sh, C, D);
    offset(325, 20, D);
    drawShape(D);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
