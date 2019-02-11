#include "odomMath.hpp"

namespace lib7842
{
  void QSAngle::setSide(autonSides side) { if(side != currentSide) { value *= -1; currentSide = side; } }
  
  /**
  * qPoint
  */
  qPoint::qPoint(const qPoint &ipoint) : x(ipoint.x), y(ipoint.y), theta(ipoint.theta) {}

  qPoint::qPoint(QLength ix, QLength iy, QAngle itheta) : x(ix), y(iy), theta(itheta) {}
  qPoint::qPoint(QLength ix, QLength iy) : x(ix), y(iy) {}
  qPoint::qPoint(QAngle itheta) : theta(itheta) {}
  qPoint::qPoint() {}

  qPoint::qPoint(qPoint ipoint, QLength ix, QLength iy) : x(ix), y(iy), theta(ipoint.theta) {}
  qPoint::qPoint(qPoint ipoint, QAngle itheta) : x(ipoint.x), y(ipoint.y), theta(itheta) {}

  qPoint::qPoint(dPoint ipoint) : x(ipoint.x * inch), y(ipoint.y * inch), theta(ipoint.theta * radian) {}
  void qPoint::setSide(autonSides side) { if(side != currentSide) { x = 12_ft - x; theta *= -1; currentSide = side; } }

  /**
  * dPoint
  */
  dPoint::dPoint(const dPoint &ipoint) : x(ipoint.x), y(ipoint.y), theta(ipoint.theta) {}

  dPoint::dPoint(double ix, double iy, double itheta) : x(ix), y(iy), theta(itheta) {}
  dPoint::dPoint(double ix, double iy) : x(ix), y(iy) {}
  dPoint::dPoint() {}

  dPoint::dPoint(dPoint ipoint, double ix, double iy) : x(ix), y(iy), theta(ipoint.theta) {}
  dPoint::dPoint(dPoint ipoint, double itheta) : x(ipoint.x), y(ipoint.y), theta(itheta) {}

  dPoint::dPoint(qPoint ipoint) : x(ipoint.x.convert(inch)), y(ipoint.y.convert(inch)), theta(ipoint.theta.convert(radian)) {}

  /**
  * Path
  */
  Path::Path(qPoint point) : wayPoints {point} {}
  Path::Path(std::initializer_list<qPoint> points) : wayPoints {points} {}
  void Path::add(qPoint point) { wayPoints.push_back(point); }
  void Path::add(Path path) { for(const qPoint &point : path.wayPoints) { wayPoints.push_back(point); } }
  void Path::setSide(autonSides side) { for(qPoint &point : wayPoints) { point.setSide(side); } }

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

  qPoint closest(qPoint current, qPoint target) {
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


  QLength computeDistanceBetweenPoints(qPoint firstPoint, qPoint secondPoint)
  {
    QLength xDiff = secondPoint.x - firstPoint.x;
    QLength yDiff = secondPoint.y - firstPoint.y;
    return std::sqrt(std::pow(xDiff.convert(inch), 2) + std::pow(yDiff.convert(inch), 2)) * inch;
  }


}
