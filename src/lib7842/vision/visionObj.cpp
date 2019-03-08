#include "visionObj.hpp"

namespace lib7842
{

  ObjectContainer::visionObj ObjectContainer::visionObj::operator+(ObjectContainer::visionObj rhs){
    objX = objX + rhs.objX;
    objY = objY + rhs.objY;
    objWidth = objWidth + rhs.objWidth;
    objHeight = objHeight + rhs.objHeight;
    objArea = objArea + rhs.objArea;
    objCenterX = objCenterX + rhs.objCenterX;
    objCenterY = objCenterY + rhs.objCenterY;
    return *this;
  }

  ObjectContainer::visionObj ObjectContainer::visionObj::operator-(ObjectContainer::visionObj rhs){
    objX = objX - rhs.objX;
    objY = objY - rhs.objY;
    objWidth = objWidth - rhs.objWidth;
    objHeight = objHeight - rhs.objHeight;
    objArea = objArea + rhs.objArea;
    objCenterX = objCenterX - rhs.objCenterX;
    objCenterY = objCenterY - rhs.objCenterY;
    return *this;
  }

  ObjectContainer::visionObj ObjectContainer::visionObj::operator*(ObjectContainer::visionObj rhs){
    objX = objX * rhs.objX;
    objY = objY * rhs.objY;
    objWidth = objWidth * rhs.objWidth;
    objHeight = objHeight * rhs.objHeight;
    objArea = objArea * rhs.objArea;
    objCenterX = objCenterX * rhs.objCenterX;
    objCenterY = objCenterY * rhs.objCenterY;
    return *this;
  }

  ObjectContainer::visionObj ObjectContainer::visionObj::operator/(ObjectContainer::visionObj rhs){
    objX = objX / rhs.objX;
    objY = objY / rhs.objY;
    objWidth = objWidth / rhs.objWidth;
    objHeight = objHeight / rhs.objHeight;
    objArea = objArea / rhs.objArea;
    objCenterX = objCenterX / rhs.objCenterX;
    objCenterY = objCenterY / rhs.objCenterY;
    return *this;
  }

  ObjectContainer::visionObj ObjectContainer::visionObj::operator+(double rhs){
    objX = objX + rhs;
    objY = objY + rhs;
    objWidth = objWidth + rhs;
    objHeight = objHeight + rhs;
    objArea = objArea + rhs;
    objCenterX = objCenterX + rhs;
    objCenterY = objCenterY + rhs;
    return *this;
  }

  ObjectContainer::visionObj ObjectContainer::visionObj::operator-(double rhs){
    objX = objX - rhs;
    objY = objY - rhs;
    objWidth = objWidth - rhs;
    objHeight = objHeight - rhs;
    objArea = objArea + rhs;
    objCenterX = objCenterX - rhs;
    objCenterY = objCenterY - rhs;
    return *this;
  }

  ObjectContainer::visionObj ObjectContainer::visionObj::operator*(double rhs){
    objX = objX * rhs;
    objY = objY * rhs;
    objWidth = objWidth * rhs;
    objHeight = objHeight * rhs;
    objArea = objArea * rhs;
    objCenterX = objCenterX * rhs;
    objCenterY = objCenterY * rhs;
    return *this;
  }

  ObjectContainer::visionObj ObjectContainer::visionObj::operator/(double rhs){
    objX = objX / rhs;
    objY = objY / rhs;
    objWidth = objWidth / rhs;
    objHeight = objHeight / rhs;
    objArea = objArea / rhs;
    objCenterX = objCenterX / rhs;
    objCenterY = objCenterY / rhs;
    return *this;
  }

  double ObjectContainer::visionObj::getAttr(objAttr attr) const {
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

}
