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
    centerX = centerX op rhs.centerX;  \
    centerY = centerY op rhs.centerY;  \
    toCenterX = toCenterX op rhs.toCenterX;  \
    toCenterY = toCenterY op rhs.toCenterY;  \
    return *this; \
  }

  #define doubleOperator(op) \
  const visionObj& visionObj::operator op(const double& rhs) { \
    x = x op rhs; \
    y = y op rhs; \
    width = width op rhs; \
    height = height op rhs; \
    area = area op rhs; \
    centerX = centerX op rhs; \
    centerY = centerY op rhs; \
    toCenterX = toCenterX op rhs; \
    toCenterY = toCenterY op rhs; \
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
      case objAttr::centerX: return centerX; break;
      case objAttr::centerY: return centerY; break;
      case objAttr::toCenterX: return toCenterX; break;
      case objAttr::toCenterY: return toCenterY; break;
    }
    std::cerr << "GetAttr: Invalid Attr" << std::endl;
    return 0;
  }


  void visionObj::print() {
    std::cout << "Sig: " << sig << " | ";
    std::cout << "Width: " << width << " | ";
    std::cout << "Height: " << height << " | ";
    std::cout << "X: " << x << " | ";
    std::cout << "Y: " << y << " | ";
    std::cout << "Area: " << area << " | ";
    std::cout << "CenterX: " << centerX << " | ";
    std::cout << "CenterY: " << centerY << " | ";
    std::cout << "ToCenterX: " << toCenterX << " | ";
    std::cout << "ToCenterY: " << toCenterY << std::endl;
  }


  visionObj::visionObj(pros::vision_object obj) {
    sig = obj.signature;
    x = obj.left_coord;
    y = obj.top_coord;
    width = obj.width;
    height = obj.height;
    area = obj.width * obj.height;
    centerX = obj.x_middle_coord;
    centerY = obj.y_middle_coord;
    toCenterX = centerX - VISION_FOV_WIDTH/2.0;
    toCenterY = centerY - VISION_FOV_WIDTH/2.0;

    //std::cout << "centerX: " << centerX << " | width: " << width << std::endl;
    assert(sig != VISION_OBJECT_ERR_SIG);
    assert((int)centerX == (int)x + ((int)width/2));
    assert((int)centerY == (int)y - ((int)height/2));
  }

}
