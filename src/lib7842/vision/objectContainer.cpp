#include "lib7842/vision/objectContainer.hpp"

namespace lib7842
{


  ObjectContainer::ObjectContainer(int objectCount)
  : arrayLength(objectCount)
  {
    objectArray.resize(arrayLength);
  }


  ObjectContainer::ObjectContainer(int objectCount, ObjectDrawing& screenParent)
  :
  arrayLength(objectCount)
  {
    objectArray.resize(arrayLength);

    screenArray.resize(arrayLength);
    for(int objectNum = 0; objectNum < arrayLength; objectNum++)
    {
      screenArray.at(objectNum) = lv_obj_create(screenParent.m_drawingContainer, NULL);
      lv_obj_set_style(screenArray.at(objectNum), &screenParent.m_defaultObjectStyle);
      lv_obj_set_hidden(screenArray.at(objectNum), true);
    }

    for (lv_style_t &sigStyle : styleArray)
    {
      lv_style_copy(&sigStyle, &screenParent.m_defaultObjectStyle);
    }

  }

  ObjectContainer::~ObjectContainer()
  {

  }


  void ObjectContainer::setContainerStyle(lv_color_t bodyColor, lv_color_t borderColor, lv_opa_t opaNum)
  {
    for (lv_style_t &sigStyle : styleArray)
    {
      sigStyle.body.main_color = bodyColor;
      sigStyle.body.grad_color = bodyColor;
      sigStyle.body.border.color = borderColor;
      sigStyle.body.opa = opaNum;
    }
  }

  void ObjectContainer::setSigStyle(int sigNum, lv_color_t bodyColor, lv_color_t borderColor, lv_opa_t opaNum)
  {
    styleArray.at(sigNum).body.main_color = bodyColor;
    styleArray.at(sigNum).body.grad_color = bodyColor;
    styleArray.at(sigNum).body.border.color = borderColor;
    styleArray.at(sigNum).body.opa = opaNum;
  }


  int ObjectContainer::getArrayLength()
  {
    return arrayLength;
  }

}
