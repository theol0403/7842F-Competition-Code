#include "odomMath.hpp"

namespace lib7842::OdomMath
{

  dPoint toDPoint(Point point) { return {point.x.convert(inch), point.y.convert(inch)}; }
  Point toPoint(dPoint point) { return {point.x * inch, point.y * inch}; }

  dPoint add(dPoint a, dPoint b) { return {a.x + b.x, a.y + b.y}; }
  dPoint sub(dPoint a, dPoint b) { return {a.x - b.x, a.y - b.y}; }
  dPoint mult(dPoint a, dPoint b) { return {a.x * b.x, a.y * b.y}; }
  dPoint div(dPoint a, dPoint b) { return {a.x / b.x, a.y / b.y}; }
  double mag(dPoint a) { return sqrt(a.x * a.x + a.y * a.y); }
  dPoint normalize(dPoint a) { return {a.x / mag(a), a.y / mag(a)}; }
  dPoint multScalar(dPoint a, double b) { return {a.x * b, a.y * b}; }
  double dot(dPoint a, dPoint b) { return a.x * b.x + a.y * b.y; }


  QAngle rollAngle360(QAngle angle)
  {
    return angle - 360.0_deg * std::floor(angle.convert(degree) * (1.0 / 360.0));
  }

  QAngle rollAngle180(QAngle angle)
  {
    return angle - 360.0_deg * std::floor((angle.convert(degree) + 180.0) * (1.0 / 360.0));
  }

  bool shouldDriveBackwards(QAngle angle)
  {
    if(rollAngle180(angle).abs() > 90_deg) { return true; }
    else { return false; }
  }


  QLength computeDistanceBetweenPoints(Point firstPoint, Point secondPoint)
  {
    const QLength xDiff = secondPoint.x - firstPoint.x;
    const QLength yDiff = secondPoint.y - firstPoint.y;
    return std::sqrt(std::pow(xDiff.convert(inch), 2) + std::pow(yDiff.convert(inch), 2)) * inch;
  }


  Point computeClosestPoint(Point current, Point target)
  {
    double a = current.theta.convert(radian);
    dPoint n = normalize({cos(a), sin(a)});
    dPoint v = sub(toDPoint(target), toDPoint(current));
    double d = dot(v, n);
    return toPoint(add(toDPoint(current), multScalar(n, d)));
  }


}
