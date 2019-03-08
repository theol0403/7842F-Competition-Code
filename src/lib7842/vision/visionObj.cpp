#include "visionObj.hpp"

namespace lib7842
{

  visionObj visionObj::operator+(visionObj rhs){
    objX = objX + rhs.objX;
    objY = objY + rhs.objY;
    objWidth = objWidth + rhs.objWidth;
    objHeight = objHeight + rhs.objHeight;
    objArea = objArea + rhs.objArea;
    objCenterX = objCenterX + rhs.objCenterX;
    objCenterY = objCenterY + rhs.objCenterY;
    return *this;
  }

  visionObj visionObj::operator-(visionObj rhs){
    objX = objX - rhs.objX;
    objY = objY - rhs.objY;
    objWidth = objWidth - rhs.objWidth;
    objHeight = objHeight - rhs.objHeight;
    objArea = objArea + rhs.objArea;
    objCenterX = objCenterX - rhs.objCenterX;
    objCenterY = objCenterY - rhs.objCenterY;
    return *this;
  }

  visionObj visionObj::operator*(visionObj rhs){
    objX = objX * rhs.objX;
    objY = objY * rhs.objY;
    objWidth = objWidth * rhs.objWidth;
    objHeight = objHeight * rhs.objHeight;
    objArea = objArea * rhs.objArea;
    objCenterX = objCenterX * rhs.objCenterX;
    objCenterY = objCenterY * rhs.objCenterY;
    return *this;
  }

  visionObj visionObj::operator/(visionObj rhs){
    objX = objX / rhs.objX;
    objY = objY / rhs.objY;
    objWidth = objWidth / rhs.objWidth;
    objHeight = objHeight / rhs.objHeight;
    objArea = objArea / rhs.objArea;
    objCenterX = objCenterX / rhs.objCenterX;
    objCenterY = objCenterY / rhs.objCenterY;
    return *this;
  }

  visionObj visionObj::operator+(double rhs){
    objX = objX + rhs;
    objY = objY + rhs;
    objWidth = objWidth + rhs;
    objHeight = objHeight + rhs;
    objArea = objArea + rhs;
    objCenterX = objCenterX + rhs;
    objCenterY = objCenterY + rhs;
    return *this;
  }

  visionObj visionObj::operator-(double rhs){
    objX = objX - rhs;
    objY = objY - rhs;
    objWidth = objWidth - rhs;
    objHeight = objHeight - rhs;
    objArea = objArea + rhs;
    objCenterX = objCenterX - rhs;
    objCenterY = objCenterY - rhs;
    return *this;
  }

  visionObj visionObj::operator*(double rhs){
    objX = objX * rhs;
    objY = objY * rhs;
    objWidth = objWidth * rhs;
    objHeight = objHeight * rhs;
    objArea = objArea * rhs;
    objCenterX = objCenterX * rhs;
    objCenterY = objCenterY * rhs;
    return *this;
  }

  visionObj visionObj::operator/(double rhs){
    objX = objX / rhs;
    objY = objY / rhs;
    objWidth = objWidth / rhs;
    objHeight = objHeight / rhs;
    objArea = objArea / rhs;
    objCenterX = objCenterX / rhs;
    objCenterY = objCenterY / rhs;
    return *this;
  }

  double visionObj::getAttr(objAttr attr) const {
    switch (attr) {
      case objAttr::objSig: return objSig; break;
      case objAttr::objX: return objX; break;
      case objAttr::objY: return objY; break;
      case objAttr::objWidth: return objWidth; break;
      case objAttr::objHeight: return objHeight; break;
      case objAttr::objArea: return objArea; break;
      case objAttr::objCenterX: return objCenterX; break;
      case objAttr::objCenterY: return objCenterY; break;
    }
    std::cerr << "GetAttr: Invalid Attr\n";
  }


  void visionObj::print() {
    std::cout << "Sig:" << objSig << " | ";
    std::cout << "Width:" << objWidth << " | ";
    std::cout << "Height:" << objHeight << " | ";
    std::cout << "X:" << objX << " | ";
    std::cout << "Y:" << objY << " | ";
    std::cout << "Area:" << objArea << " | ";
    std::cout << "CenterX:" << objCenterX << " | ";
    std::cout << "CenterY:" << objCenterY << " | ";
    std::cout << std::endl;
  }

}
