#include "main.h"

#include "Include/FlagTracking/FlagTracking.hpp"
#include "Include/FlagTracking/FlagSorting.hpp"
#include "Include/FlagTracking/ScreenDrawing.hpp"





ScreenDrawing::ScreenDrawing(flagSig_t flagSig, int containerWidth, int containerHeight)
:
m_flagSig(flagSig),
m_widthScale{containerWidth/VISION_FOV_WIDTH},
m_heightScale{containerHeight/VISION_FOV_HEIGHT}
{

  m_objectContainer = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_set_size(m_objectContainer, containerWidth, containerHeight);
  lv_obj_align(m_objectContainer, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);


  // Style for background of screen
  lv_style_copy(&m_objectContainerStyle, &lv_style_plain_color);
  m_objectContainerStyle.body.main_color = LV_COLOR_GRAY;
  m_objectContainerStyle.body.grad_color = LV_COLOR_GRAY;
  lv_obj_set_style(m_objectContainer, &m_objectContainerStyle);






  //Generic Object Style
  lv_style_copy(&m_defaultObjectStyle, &lv_style_pretty_color);
  m_defaultObjectStyle.body.main_color = LV_COLOR_GRAY;
  m_defaultObjectStyle.body.grad_color = LV_COLOR_GRAY;
  m_defaultObjectStyle.body.radius = 5;
  m_defaultObjectStyle.body.border.color = LV_COLOR_BLACK;
  m_defaultObjectStyle.body.border.width = 3;
  m_defaultObjectStyle.body.border.opa = LV_OPA_100;

  //Blue object style
  lv_style_copy(&m_blueObjectStyle, &m_defaultObjectStyle);
  m_blueObjectStyle.body.main_color = LV_COLOR_BLUE;
  m_blueObjectStyle.body.grad_color = LV_COLOR_BLUE;
  m_blueObjectStyle.body.border.color = LV_COLOR_YELLOW;

  //Red object style
  lv_style_copy(&m_redObjectStyle, &m_defaultObjectStyle);
  m_redObjectStyle.body.main_color = LV_COLOR_RED;
  m_redObjectStyle.body.grad_color = LV_COLOR_RED;
  m_redObjectStyle.body.border.color = LV_COLOR_YELLOW;

  //Discard object style
  lv_style_copy(&m_discardObjectStyle, &m_defaultObjectStyle);
  m_discardObjectStyle.body.main_color = LV_COLOR_OLIVE;
  m_discardObjectStyle.body.grad_color = LV_COLOR_OLIVE;
  m_discardObjectStyle.body.border.color = LV_COLOR_YELLOW;

}


ScreenDrawing::~ScreenDrawing()
{

}


screenObjects_t ScreenDrawing::initSimpleObjects(int objectCount)
{
  screenObjects_t screenObjects = {};
  screenObjects.screenObjectsArray = new lv_obj_t*[objectCount];
  screenObjects.objectCount = objectCount;

  for(int objectNum = 0; objectNum < objectCount; objectNum++)
  {
    screenObjects.screenObjectsArray[objectNum] = lv_obj_create(m_objectContainer, NULL);
    lv_obj_set_style(screenObjects.screenObjectsArray[objectNum], &m_defaultObjectStyle);
    lv_obj_set_hidden(screenObjects.screenObjectsArray[objectNum], true);
  }
  return screenObjects;
}






void ScreenDrawing::drawSimpleObjects(screenObjects_t screenObjects, simpleObjects_t* flagObjects, int objectCount)
{

  for(int objectNum = 0; objectNum < screenObjects.objectCount; objectNum++)
  {
    lv_obj_set_hidden(screenObjects.screenObjectsArray[objectNum], true);
  }

  for(int objectNum = 0; objectNum < objectCount; objectNum++)
  {
    if(flagObjects[objectNum].objSig != VISION_OBJECT_ERR_SIG)
    {
      lv_obj_set_hidden(screenObjects.screenObjectsArray[objectNum], false); // make visible

      //Set posisitons and size
      lv_obj_set_x(screenObjects.screenObjectsArray[objectNum], flagObjects[objectNum].objX * m_widthScale);
      lv_obj_set_y(screenObjects.screenObjectsArray[objectNum], flagObjects[objectNum].objY * m_heightScale);

      lv_obj_set_width(screenObjects.screenObjectsArray[objectNum], flagObjects[objectNum].objWidth * m_widthScale);
      lv_obj_set_height(screenObjects.screenObjectsArray[objectNum], flagObjects[objectNum].objHeight * m_heightScale);

      if(flagObjects[objectNum].discardObject)
      {
        lv_obj_set_style(screenObjects.screenObjectsArray[objectNum], &m_discardObjectStyle);
      }
      else if(flagObjects[objectNum].objSig == m_flagSig.blueSig)
      {
        lv_obj_set_style(screenObjects.screenObjectsArray[objectNum], &m_blueObjectStyle); //Give it the style for a blue flagObject
      }
      else if(flagObjects[objectNum].objSig == m_flagSig.redSig)
      {
        lv_obj_set_style(screenObjects.screenObjectsArray[objectNum], &m_redObjectStyle); //Give it the style for a red flagObject
      }
      else
      {

      }
    }
    else
    {
      lv_obj_set_hidden(screenObjects.screenObjectsArray[objectNum], true);
    }
  }

}
