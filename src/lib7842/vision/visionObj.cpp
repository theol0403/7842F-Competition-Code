#include "visionObj.hpp"
#include <cassert>

namespace lib7842
{

  #define makeOperators(x) \
  x (+) \
  x (-) \
  x (*) \
  x (/)

  #define objOperator(op) \
  const visionObj& visionObj::operator op(const visionObj& rhs) { \
    x = x op rhs.x;  \
    y = y op rhs.y;  \
    width = width op rhs.width;  \
    height = height op rhs.height; \
    area = area op rhs.area; \
    avgDim = avgDim op rhs.avgDim; \
    centerX = centerX op rhs.centerX;  \
    centerY = centerY op rhs.centerY;  \
    fromMidX = fromMidX op rhs.fromMidX;  \
    fromMidY = fromMidY op rhs.fromMidY;  \
    absFromMidX = absFromMidX op rhs.absFromMidX;  \
    absFromMidY = absFromMidY op rhs.absFromMidY;  \
    return *this; \
  }

  #define doubleOperator(op) \
  const visionObj& visionObj::operator op(const double& rhs) { \
    x = x op rhs; \
    y = y op rhs; \
    width = width op rhs; \
    height = height op rhs; \
    area = area op rhs; \
    avgDim = avgDim op rhs; \
    centerX = centerX op rhs; \
    centerY = centerY op rhs; \
    fromMidX = fromMidX op rhs; \
    fromMidY = fromMidY op rhs; \
    absFromMidX = absFromMidX op rhs; \
    absFromMidY = absFromMidY op rhs; \
    return *this; \
  }


  makeOperators(objOperator)
  makeOperators(doubleOperator)


  double visionObj::getAttr(objAttr attr) const {
    switch (attr) {
      case objAttr::sig: return sig; break;
      case objAttr::x: return x; break;
      case objAttr::y: return y; break;
      case objAttr::width: return width; break;
      case objAttr::height: return height; break;
      case objAttr::area: return area; break;
      case objAttr::avgDim: return avgDim; break;
      case objAttr::centerX: return centerX; break;
      case objAttr::centerY: return centerY; break;
      case objAttr::fromMidX: return fromMidX; break;
      case objAttr::fromMidY: return fromMidY; break;
      case objAttr::absFromMidX: return absFromMidX; break;
      case objAttr::absFromMidY: return absFromMidY; break;
    }
    std::cerr << "GetAttr: Invalid Attr" << std::endl;
    return 0;
  }

  void visionObj::setAttr(objAttr attr, double val) {
    switch (attr) {
      case objAttr::sig: sig = val; break;
      case objAttr::x: x = val; break;
      case objAttr::y: y = val; break;
      case objAttr::width: width = val; break;
      case objAttr::height: height = val; break;
      case objAttr::area: area = val; break;
      case objAttr::avgDim: avgDim = val; break;
      case objAttr::centerX: centerX = val; break;
      case objAttr::centerY: centerY = val; break;
      case objAttr::fromMidX: fromMidX = val; break;
      case objAttr::fromMidY: fromMidY = val; break;
      case objAttr::absFromMidX: absFromMidX = val; break;
      case objAttr::absFromMidY: absFromMidY = val; break;
    }
    std::cerr << "GetAttr: Invalid Attr" << std::endl;
  }


  void visionObj::print() {
    std::cout << "Sig: " << sig << " | ";
    std::cout << "Width: " << width << " | ";
    std::cout << "Height: " << height << " | ";
    std::cout << "X: " << x << " | ";
    std::cout << "Y: " << y << " | ";
    std::cout << "Area: " << area << " | ";
    std::cout << "AvgDim: " << avgDim << " | ";
    std::cout << "CenterX: " << centerX << " | ";
    std::cout << "CenterY: " << centerY << " | ";
    std::cout << "FromMidX: " << fromMidX << " | ";
    std::cout << "FromMidY: " << fromMidY << " | ";
    std::cout << "AbsFromMidX: " << absFromMidX << " | ";
    std::cout << "AbsFromMidY: " << absFromMidY << std::endl;
  }


  visionObj::visionObj(pros::vision_object obj) {
    sig = obj.signature;
    x = obj.left_coord;
    y = obj.top_coord;
    width = obj.width;
    height = obj.height;
    area = obj.width * obj.height;
    avgDim = (obj.width + obj.height)/2;
    centerX = x + width/2.0;
    centerY = y + height/2.0;
    fromMidX = centerX - VISION_FOV_WIDTH/2.0;
    fromMidY = centerY - VISION_FOV_WIDTH/2.0;
    absFromMidX = std::abs(fromMidX);
    absFromMidY = std::abs(fromMidY);

    assert(sig != VISION_OBJECT_ERR_SIG);
    assert((int)centerX == (int)x + ((int)width/2));
    assert((int)centerY == (int)y + ((int)height/2));
  }

}
