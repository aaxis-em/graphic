#include <GL/glut.h>

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // move the "camera" back a little (not really necessary in ortho)
  glTranslatef(0, 0, -50);

  // left cube (normal)
  glPushMatrix();
  glTranslatef(-20, 0, 0);
  glColor3f(0.7f, 0.7f, 0.9f);
  glutSolidCube(10);
  glPopMatrix();

  // right cube (scaled)
  glPushMatrix();
  glTranslatef(20, 0, 0);
  glScalef(1, 2, 1);
  glColor3f(0.9f, 0.7f, 0.7f);
  glutSolidCube(10);
  glPopMatrix();

  glutSwapBuffers();
}

void reshape(int w, int h) {
  if (h == 0)
    h = 1;
  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // General orthographic box
  float aspect = (float)w / h;
  if (w >= h)
    glOrtho(-50 * aspect, 50 * aspect, -50, 50, 1, 100);
  else
    glOrtho(-50, 50, -50 / aspect, 50 / aspect, 1, 100);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Orthographic Projection Example");

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMainLoop();
}
