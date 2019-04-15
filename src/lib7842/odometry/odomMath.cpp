#include "odomMath.hpp"

namespace lib7842
{
  /**
  * QPoint
  */
  QPoint::QPoint(const QPoint &ipoint) : x(ipoint.x), y(ipoint.y), theta(ipoint.theta) {}

  QPoint::QPoint(QLength ix, QLength iy, QAngle itheta) : x(ix), y(iy), theta(itheta) {}
  QPoint::QPoint(QLength ix, QLength iy) : x(ix), y(iy) {}
  QPoint::QPoint(QAngle itheta) : theta(itheta) {}
  QPoint::QPoint() {}

  QPoint::QPoint(QPoint ipoint, QLength ix, QLength iy) : x(ix), y(iy), theta(ipoint.theta) {}
  QPoint::QPoint(QPoint ipoint, QAngle itheta) : x(ipoint.x), y(ipoint.y), theta(itheta) {}

  QPoint::QPoint(dPoint ipoint) : x(ipoint.x * inch), y(ipoint.y * inch), theta(ipoint.theta * radian) {}

  QPoint QPoint::operator+(QPoint rhs) { return {x + rhs.x, y + rhs.y, theta + rhs.theta}; }

  /**
  * dPoint
  */
  dPoint::dPoint(const dPoint &ipoint) : x(ipoint.x), y(ipoint.y), theta(ipoint.theta) {}

  dPoint::dPoint(double ix, double iy, double itheta) : x(ix), y(iy), theta(itheta) {}
  dPoint::dPoint(double ix, double iy) : x(ix), y(iy) {}
  dPoint::dPoint() {}

  dPoint::dPoint(dPoint ipoint, double ix, double iy) : x(ix), y(iy), theta(ipoint.theta) {}
  dPoint::dPoint(dPoint ipoint, double itheta) : x(ipoint.x), y(ipoint.y), theta(itheta) {}

  dPoint::dPoint(QPoint ipoint) : x(ipoint.x.convert(inch)), y(ipoint.y.convert(inch)), theta(ipoint.theta.convert(radian)) {}

  dPoint dPoint::operator+(dPoint rhs) { return {x + rhs.x, y + rhs.y, theta + rhs.theta}; }

  /**
  * Path
  */
  Path::Path(QPoint point) : wayPoints {point} {}
  Path::Path(std::initializer_list<QPoint> points) : wayPoints {points} {}
  void Path::add(QPoint point) { wayPoints.push_back(point); }
  void Path::add(Path path) { for(const QPoint &point : path.wayPoints) { wayPoints.push_back(point); } }

  /**
  * Sides
  */
  QPoint mirrorSide(QPoint point, autonSides side) {
    if(side == autonSides::blue) {
      point.x = 12_ft - point.x;
      point.theta *= -1;
    }
    return point;
  }

  QAngle mirrorSide(QAngle angle, autonSides side) {
    if(side == autonSides::blue) angle = -angle;
    return angle;
  }

  Path mirrorSide(Path path, autonSides side) {
    if(side == autonSides::blue) { for(auto&& point : path.wayPoints) { point = mirrorSide(point, side); } }
    return path;
  }

  QLength mirrorSide(QLength x, autonSides side) {
    if(side == autonSides::blue) x = 12_ft - x;
    return x;
  }

}


namespace lib7842::OdomMath
{

  double dot(dPoint a, dPoint b) { return a.x * b.x + a.y * b.y; }
  double dot(double x1, double y1, double x2, double y2) { return (x1 * x2) + (y1 * y2); }

  dPoint add(dPoint a, dPoint b) { return { a.x + b.x, a.y + b.y }; }
  dPoint sub(dPoint a, dPoint b) { return { a.x - b.x, a.y - b.y }; }
  dPoint mult(dPoint a, dPoint b) { return { a.x * b.x, a.y * b.y }; }
  dPoint div(dPoint a, dPoint b) { return { a.x / b.x, a.y / b.y }; }

  double mag(dPoint a) { return sqrt((a.x * a.x) + (a.y * a.y)); }

  dPoint normalize(dPoint a) {
    double imag = mag(a);
    if(imag == 0) return a;
    return { a.x / imag, a.y / imag };
  }

  dPoint multScalar(dPoint a, double b) { return { a.x * b, a.y * b }; }

  dPoint closest(dPoint current, dPoint head, dPoint target) {
    dPoint n = normalize(head);
    dPoint v = sub(target, current);
    double d = dot(v.x, v.y, n.x, n.y);
    return add(current, multScalar(n, d));
  }

  QPoint closest(QPoint current, QPoint target) {
    return closest(current, dPoint{sin(current.theta.convert(radian)), cos(current.theta.convert(radian))}, target);
  }


  QAngle rollAngle360(QAngle angle) {
    return angle - 360.0_deg * std::floor(angle.convert(degree) * (1.0 / 360.0));
  }

  QAngle rollAngle180(QAngle angle) {
    QAngle newAngle = angle - 360.0_deg * std::floor((angle.convert(degree) + 180.0) * (1.0 / 360.0));
    //if(newAngle == -180_deg) newAngle = 180_deg;
    return newAngle;
  }

  QAngle rollAngle90(QAngle angle) {
    angle = rollAngle180(angle);
    if(angle.abs() > 90_deg) {
      angle += 180_deg;
      angle = rollAngle180(angle);
    }
    return angle;
  }


  QLength distanceBetweenPoints(QPoint firstPoint, QPoint secondPoint) {
    QLength xDiff = secondPoint.x - firstPoint.x;
    QLength yDiff = secondPoint.y - firstPoint.y;
    return std::sqrt(std::pow(xDiff.convert(inch), 2) + std::pow(yDiff.convert(inch), 2)) * inch;
  }

  QPoint distanceAtHeading(QLength dist, QAngle heading) {
    QLength x = sin(heading.convert(radian)) * dist;
    QLength y = cos(heading.convert(radian)) * dist;
    return {x, y, heading};
  }


}
