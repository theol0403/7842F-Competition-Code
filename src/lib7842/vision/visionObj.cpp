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
    return *this; \
  }

  #define doubleOperator(op) \
  const visionObj& visionObj::operator op(const double& rhs) { \
    x = x op rhs; \
    y = y op rhs; \
    width = width op rhs; \
    height = height op rhs; \
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
      case objAttr::isCode: return isCode; break;

      case objAttr::area: return width * height; break;
      case objAttr::avgDim: return (width + height)/2.0; break;
      case objAttr::centerX: return x + width/2.0; break;
      case objAttr::centerY: return y + height/2.0; break;
      case objAttr::fromMidX: return getAttr(objAttr::centerX) - VISION_FOV_WIDTH/2.0; break;
      case objAttr::fromMidY: return getAttr(objAttr::centerY) - VISION_FOV_HEIGHT/2.0; break;
      case objAttr::absFromMidX: return std::abs(getAttr(objAttr::fromMidX)); break;
      case objAttr::absFromMidY: return std::abs(getAttr(objAttr::fromMidY)); break;
      default: std::cerr << "getAttr: Invalid Attr" << std::endl;
    }
    return 0;
  }

  void visionObj::setAttr(objAttr attr, double val) {
    switch (attr) {
      case objAttr::sig: sig = val; break;
      case objAttr::x: x = val; break;
      case objAttr::y: y = val; break;
      case objAttr::width: width = val; break;
      case objAttr::height: height = val; break;
      case objAttr::isCode: isCode = val; break;
      default: std::cerr << "setAttr: Invalid Attr" << std::endl;
    }
  }


  void visionObj::snapTo(objAttr attr, double increment) {
    x = std::round(x / increment) * increment;
  }

  void visionObj::snapToGrid(objAttr attr, int grid, double max) {
    snapTo(attr, max / grid);
  }

  void visionObj::snapToGridX(int grid) {
    snapToGrid(objAttr::x, grid, VISION_FOV_WIDTH);
  }

  void visionObj::snapToGridY(int grid) {
    snapToGrid(objAttr::y, grid, VISION_FOV_HEIGHT);
  }


  void visionObj::print() {
    std::cout << "Sig:" << sig << " | ";
    std::cout << "IsCode:" << isCode << " | ";
    std::cout << "Width:" << width << " | ";
    std::cout << "Height:" << height << " | ";
    std::cout << "X:" << x << " | ";
    std::cout << "Y:" << y << std::endl;
  }


  visionObj::visionObj(pros::vision_object obj) {
    sig = obj.signature;
    x = obj.left_coord;
    y = obj.top_coord;
    width = obj.width;
    height = obj.height;

    if(obj.type == pros::E_VISION_OBJECT_COLOR_CODE) {
      isCode = true;
    }

    // for(uint16_t id = obj.signature; id >> 3;) {
    //   std::cout << "Id: " << id << " to: ";
    //   id = id >> 3;
    //   std::cout << id << std::endl;
    //   sig = id;
    // }

    assert(sig != VISION_OBJECT_ERR_SIG);
  }

}
