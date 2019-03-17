#include "visionObj.hpp"

namespace lib7842
{

  visionObj visionObj::operator+(visionObj rhs) {
    x = x + rhs.x;
    y = y + rhs.y;
    width = width + rhs.width;
    height = height + rhs.height;
    area = area + rhs.area;
    centerX = centerX + rhs.centerX;
    centerY = centerY + rhs.centerY;
    return *this;
  }

  visionObj visionObj::operator-(visionObj rhs) {
    x = x - rhs.x;
    y = y - rhs.y;
    width = width - rhs.width;
    height = height - rhs.height;
    area = area + rhs.area;
    centerX = centerX - rhs.centerX;
    centerY = centerY - rhs.centerY;
    return *this;
  }

  visionObj visionObj::operator*(visionObj rhs) {
    x = x * rhs.x;
    y = y * rhs.y;
    width = width * rhs.width;
    height = height * rhs.height;
    area = area * rhs.area;
    centerX = centerX * rhs.centerX;
    centerY = centerY * rhs.centerY;
    return *this;
  }

  visionObj visionObj::operator/(visionObj rhs) {
    x = x / rhs.x;
    y = y / rhs.y;
    width = width / rhs.width;
    height = height / rhs.height;
    area = area / rhs.area;
    centerX = centerX / rhs.centerX;
    centerY = centerY / rhs.centerY;
    return *this;
  }

  visionObj visionObj::operator+(double rhs) {
    x = x + rhs;
    y = y + rhs;
    width = width + rhs;
    height = height + rhs;
    area = area + rhs;
    centerX = centerX + rhs;
    centerY = centerY + rhs;
    return *this;
  }

  visionObj visionObj::operator-(double rhs) {
    x = x - rhs;
    y = y - rhs;
    width = width - rhs;
    height = height - rhs;
    area = area + rhs;
    centerX = centerX - rhs;
    centerY = centerY - rhs;
    return *this;
  }

  visionObj visionObj::operator*(double rhs) {
    x = x * rhs;
    y = y * rhs;
    width = width * rhs;
    height = height * rhs;
    area = area * rhs;
    centerX = centerX * rhs;
    centerY = centerY * rhs;
    return *this;
  }

  visionObj visionObj::operator/(double rhs) {
    x = x / rhs;
    y = y / rhs;
    width = width / rhs;
    height = height / rhs;
    area = area / rhs;
    centerX = centerX / rhs;
    centerY = centerY / rhs;
    return *this;
  }

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
    }
    std::cerr << "GetAttr: Invalid Attr\n";
  }


  void visionObj::print() {
    std::cout << "Sig:" << sig << " | ";
    std::cout << "Width:" << width << " | ";
    std::cout << "Height:" << height << " | ";
    std::cout << "X:" << x << " | ";
    std::cout << "Y:" << y << " | ";
    std::cout << "Area:" << area << " | ";
    std::cout << "CenterX:" << centerX << " | ";
    std::cout << "CenterY:" << centerY << " | ";
    std::cout << std::endl;
  }


  visionObj::visionObj(pros::vision_object obj) {
    assert(obj.signature != VISION_OBJECT_ERR_SIG);

    sig = obj.signature;
    x = obj.left_coord;
    y = obj.top_coord;
    width = obj.width;
    height = obj.height;
    area = obj.width * obj.height;
    centerX = obj.x_middle_coord;
    centerY = obj.y_middle_coord;

    assert((int)centerX == (int)x + ((int)width/2));
    assert((int)centerY == (int)y - ((int)height/2));
  }

}
