#include "lib7842/vision/objectDrawing.hpp"

namespace lib7842
{

  ObjectDrawing::ObjectDrawing(int containerWidth, int containerHeight)
  :
  m_widthScale{containerWidth/VISION_FOV_WIDTH},
  m_heightScale{containerHeight/VISION_FOV_HEIGHT}
  {
    m_drawingContainer = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_size(m_drawingContainer, containerWidth, containerHeight);
    lv_obj_align(m_drawingContainer, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);

    // Style for background of screen
    lv_style_copy(&m_drawingContainerStyle, &lv_style_plain_color);
    m_drawingContainerStyle.body.main_color = LV_COLOR_GRAY;
    m_drawingContainerStyle.body.grad_color = LV_COLOR_GRAY;
    lv_obj_set_style(m_drawingContainer, &m_drawingContainerStyle);

    //Generic Object Style
    lv_style_copy(&m_defaultObjectStyle, &lv_style_pretty_color);
    m_defaultObjectStyle.body.main_color = LV_COLOR_GRAY;
    m_defaultObjectStyle.body.grad_color = LV_COLOR_GRAY;
    m_defaultObjectStyle.body.radius = 5;
    m_defaultObjectStyle.body.border.color = LV_COLOR_BLACK;
    m_defaultObjectStyle.body.border.width = 2;
    m_defaultObjectStyle.body.border.opa = LV_OPA_100;

    //Discard object style
    lv_style_copy(&m_discardObjectStyle, &m_defaultObjectStyle);
    m_discardObjectStyle.body.main_color = LV_COLOR_OLIVE;
    m_discardObjectStyle.body.grad_color = LV_COLOR_OLIVE;
    m_discardObjectStyle.body.border.color = LV_COLOR_YELLOW;
  }

  ObjectDrawing::~ObjectDrawing()
  {
  }


  void ObjectDrawing::drawSimpleObjects(ObjectContainer& objectContainer)
  {

    for (lv_obj_t* screenObject : objectContainer.screenArray) //Hide all objects
    {
      lv_obj_set_hidden(screenObject, true);
    }

    for(int objectNum = 0; objectNum < objectContainer.currentCount; objectNum++)
    {
      if(objectContainer.objectArray.at(objectNum).objSig != VISION_OBJECT_ERR_SIG)
      {
        lv_obj_set_hidden(objectContainer.screenArray.at(objectNum), false); // make visible

        //Set posisitons and size
        lv_obj_set_x(objectContainer.screenArray.at(objectNum), objectContainer.objectArray.at(objectNum).objX * m_widthScale);
        lv_obj_set_y(objectContainer.screenArray.at(objectNum), objectContainer.objectArray.at(objectNum).objY * m_heightScale);

        lv_obj_set_width(objectContainer.screenArray.at(objectNum), objectContainer.objectArray.at(objectNum).objWidth * m_widthScale);
        lv_obj_set_height(objectContainer.screenArray.at(objectNum), objectContainer.objectArray.at(objectNum).objHeight * m_heightScale);

        if(objectContainer.objectArray.at(objectNum).discardObject)
        {
          lv_obj_set_style(objectContainer.screenArray.at(objectNum), &m_discardObjectStyle);
        }
        else
        {
          lv_obj_set_style(objectContainer.screenArray.at(objectNum), &objectContainer.styleArray.at(objectContainer.objectArray.at(objectNum).objSig));
        }
      }
      else
      {
        lv_obj_set_hidden(objectContainer.screenArray.at(objectNum), true);
      }
    }

  }
}
