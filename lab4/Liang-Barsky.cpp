#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>

struct Point {
  float x, y;
  Point(float x = 0, float y = 0) : x(x), y(y) {}
};

// Check if point is inside the edge (left side of the edge vector)
bool isInside(Point p, Point edgeStart, Point edgeEnd) {
  return (edgeEnd.x - edgeStart.x) * (p.y - edgeStart.y) -
             (edgeEnd.y - edgeStart.y) * (p.x - edgeStart.x) >=
         0;
}

// Compute intersection of line segment with clipping edge
Point getIntersection(Point p1, Point p2, Point edgeStart, Point edgeEnd) {
  float dx1 = p2.x - p1.x;
  float dy1 = p2.y - p1.y;
  float dx2 = edgeEnd.x - edgeStart.x;
  float dy2 = edgeEnd.y - edgeStart.y;

  float denom = dx1 * dy2 - dy1 * dx2;
  if (fabs(denom) < 1e-10)
    return p1;

  float t = ((edgeStart.x - p1.x) * dy2 - (edgeStart.y - p1.y) * dx2) / denom;

  return Point(p1.x + t * dx1, p1.y + t * dy1);
}

// Clip polygon against a single edge
std::vector<Point> clipAgainstEdge(const std::vector<Point> &poly,
                                   Point edgeStart, Point edgeEnd) {
  std::vector<Point> output;

  for (size_t i = 0; i < poly.size(); i++) {
    Point curr = poly[i];
    Point next = poly[(i + 1) % poly.size()];

    bool currInside = isInside(curr, edgeStart, edgeEnd);
    bool nextInside = isInside(next, edgeStart, edgeEnd);

    if (currInside && nextInside) {
      // Both inside: add next vertex
      output.push_back(next);
    } else if (currInside && !nextInside) {
      // Leaving: add intersection
      output.push_back(getIntersection(curr, next, edgeStart, edgeEnd));
    } else if (!currInside && nextInside) {
      // Entering: add intersection and next vertex
      output.push_back(getIntersection(curr, next, edgeStart, edgeEnd));
      output.push_back(next);
    }
    // Both outside: add nothing
  }

  return output;
}

// Sutherland-Hodgman algorithm: clip polygon against convex clipping region
std::vector<Point> sutherlandHodgman(const std::vector<Point> &subject,
                                     const std::vector<Point> &clipPoly) {
  std::vector<Point> output = subject;

  // Clip against each edge of the clipping polygon
  for (size_t i = 0; i < clipPoly.size(); i++) {
    if (output.empty())
      break;

    Point edgeStart = clipPoly[i];
    Point edgeEnd = clipPoly[(i + 1) % clipPoly.size()];

    output = clipAgainstEdge(output, edgeStart, edgeEnd);
  }

  return output;
}

// Draw a polygon
void drawPolygon(const std::vector<Point> &poly, float r, float g, float b,
                 bool fill = true) {
  if (poly.empty())
    return;

  if (fill) {
    glBegin(GL_POLYGON);
  } else {
    glBegin(GL_LINE_LOOP);
  }

  glColor3f(r, g, b);
  for (const auto &p : poly) {
    glVertex2f(p.x, p.y);
  }
  glEnd();
}

int main() {
  if (!glfwInit()) {
    return -1;
  }

  GLFWwindow *window = glfwCreateWindow(
      800, 600, "Sutherland-Hodgman Polygon Clipping", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  // Define subject polygon (to be clipped)
  std::vector<Point> subject = {Point(-0.5f, -0.5f), Point(0.7f, -0.3f),
                                Point(0.6f, 0.6f), Point(-0.2f, 0.8f),
                                Point(-0.7f, 0.3f)};

  // Define clipping rectangle
  std::vector<Point> clipWindow = {Point(-0.4f, -0.4f), Point(0.4f, -0.4f),
                                   Point(0.4f, 0.4f), Point(-0.4f, 0.4f)};

  // Perform clipping
  std::vector<Point> clipped = sutherlandHodgman(subject, clipWindow);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw original polygon (semi-transparent blue outline)
    drawPolygon(subject, 0.3f, 0.3f, 1.0f, false);

    // Draw clipping window (yellow outline)
    drawPolygon(clipWindow, 1.0f, 1.0f, 0.0f, false);

    // Draw clipped polygon (filled green)
    drawPolygon(clipped, 0.0f, 0.8f, 0.0f, true);

    // Draw clipped polygon outline (darker green)
    drawPolygon(clipped, 0.0f, 0.5f, 0.0f, false);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
