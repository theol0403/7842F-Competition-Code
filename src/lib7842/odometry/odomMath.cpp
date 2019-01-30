#include "odomMath.hpp"

namespace lib7842::OdomMath
{

  dPoint toDPoint(qPoint point) { return {point.x.convert(inch), point.y.convert(inch)}; }
  qPoint toPoint(dPoint point) { return {point.x * inch, point.y * inch}; }

  double dot(dPoint a, dPoint b) { return a.x * b.x + a.y * b.y; }
  double dot(double x1, double y1, double x2, double y2) {
    return (x1 * x2) + (y1 * y2);
  }

  dPoint add(dPoint a, dPoint b) {
    return { a.x + b.x, a.y + b.y };
  }
  dPoint sub(dPoint a, dPoint b) {
    return { a.x - b.x, a.y - b.y };
  }
  dPoint mult(dPoint a, dPoint b) {
    return { a.x * b.x, a.y * b.y };
  }
  dPoint div(dPoint a, dPoint b) {
    return { a.x / b.x, a.y / b.y };
  }

  double mag(dPoint a) {
    return sqrt((a.x * a.x) + (a.y * a.y));
  }

  dPoint normalize(dPoint a) {
    if(mag(a) == 0) return a;
    return { a.x / mag(a), a.y / mag(a) };
  }

  dPoint multScalar(dPoint a, double b) {
    return { a.x * b, a.y * b };
  }

  dPoint closest(dPoint current, dPoint head, dPoint target) {
    dPoint n = normalize(head);
    dPoint v = sub(target, current);
    double d = dot(v.x, v.y, n.x, n.y);
    return add(current, multScalar(n, d));
  }

  qPoint closest(qPoint current, qPoint target) {
    //current.theta = rollAngle180(current.theta);
    return toPoint(closest(toDPoint(current), dPoint{sin(current.theta.convert(radian)), cos(current.theta.convert(radian))}, toDPoint(target)));
  }


  QAngle rollAngle360(QAngle angle)
  {
    return angle - 360.0_deg * std::floor(angle.convert(degree) * (1.0 / 360.0));
  }

  QAngle rollAngle180(QAngle angle)
  {
    QAngle newAngle = angle - 360.0_deg * std::floor((angle.convert(degree) + 180.0) * (1.0 / 360.0));
    return newAngle;
  }


  QLength computeDistanceBetweenPoints(qPoint firstPoint, qPoint secondPoint)
  {
    const QLength xDiff = secondPoint.x - firstPoint.x;
    const QLength yDiff = secondPoint.y - firstPoint.y;
    return std::sqrt(std::pow(xDiff.convert(inch), 2) + std::pow(yDiff.convert(inch), 2)) * inch;
  }



}
