#include "main.h"

#include "Include/Libraries/FlagTracking/FlagTrackingMain.hpp"
#include "Include/Libraries/FlagTracking/ScreenDrawingMain.hpp"







screenDrawing::screenDrawing(flagSig_t flagSig, int containerWidth, int containerHeight, int objectCount)
:
m_flagSig(flagSig),
m_widthScale{containerWidth/VISION_FOV_WIDTH},
m_heightScale{containerHeight/VISION_FOV_HEIGHT}
{
  initContainer(containerWidth, containerHeight);
  initVisionObjects(objectCount);
}



void screenDrawing::initContainer(int containerWidth, int containerHeight)
{
  m_objectContainer = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_set_size(m_objectContainer, containerWidth, containerHeight);
  lv_obj_align(m_objectContainer, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);

  // Style for background of screen
  static lv_style_t backgroundStyle; //Static so the style will stay alive
  lv_style_copy(&backgroundStyle, &lv_style_plain_color);
  backgroundStyle.body.main_color = LV_COLOR_GRAY;
  lv_obj_set_style(m_objectContainer, &backgroundStyle);
}




void screenDrawing::initVisionObjects(int objectCount)
{

  m_screenObjects = new lv_obj_t*[objectCount];


  //Generic Object Style
  lv_style_copy(&m_objectStyle, &lv_style_pretty_color);
  m_objectStyle.body.main_color = LV_COLOR_GRAY;
  m_objectStyle.body.grad_color = LV_COLOR_GRAY;
  m_objectStyle.body.radius = 5;
  m_objectStyle.body.border.color = LV_COLOR_BLACK;
  m_objectStyle.body.border.width = 3;
  m_objectStyle.body.border.opa = LV_OPA_100;

  //Blue object style
  lv_style_copy(&m_blueObjectStyle, &m_objectStyle);
  m_blueObjectStyle.body.main_color = LV_COLOR_BLUE;
  m_blueObjectStyle.body.grad_color = LV_COLOR_BLUE;
  m_blueObjectStyle.body.border.color = LV_COLOR_YELLOW;

  //Red object style
  lv_style_copy(&m_redObjectStyle, &m_objectStyle);
  m_redObjectStyle.body.main_color = LV_COLOR_RED;
  m_redObjectStyle.body.grad_color = LV_COLOR_RED;
  m_redObjectStyle.body.border.color = LV_COLOR_YELLOW;

  //Discard object style
  lv_style_copy(&m_discardObjectStyle, &m_objectStyle);
  m_redObjectStyle.body.main_color = LV_COLOR_OLIVE;
  m_redObjectStyle.body.grad_color = LV_COLOR_OLIVE;
  m_redObjectStyle.body.border.color = LV_COLOR_YELLOW;



  for(int objectNum = 0; objectNum < objectCount; objectNum++)
  {
    m_screenObjects[objectNum] = lv_obj_create(m_objectContainer, NULL);
  }
}





void screenDrawing::drawVisionObjects(visionObjects* flagObjects, int objectCount)
{

  for(int objectNum = 0; objectNum < objectCount; objectNum++)
  {
    if(flagObjects[objectNum].objSig != VISION_OBJECT_ERR_SIG)
    {

      // make visible
      lv_obj_set_hidden(m_screenObjects[objectNum], false);

      //Set posisitons and size
      lv_obj_set_x(m_screenObjects[objectNum], flagObjects[objectNum].objX * m_widthScale);
      lv_obj_set_y(m_screenObjects[objectNum], flagObjects[objectNum].objY * m_heightScale);

      lv_obj_set_width(m_screenObjects[objectNum], flagObjects[objectNum].objWidth * m_widthScale);
      lv_obj_set_height(m_screenObjects[objectNum], flagObjects[objectNum].objHeight * m_heightScale);


      if(flagObjects[objectNum].discardObject)
      {
        lv_obj_set_style(m_screenObjects[objectNum], &m_discardObjectStyle);
      }
      else if(flagObjects[objectNum].objSig == m_flagSig.blueSig)
      {
        lv_obj_set_style(m_screenObjects[objectNum], &m_blueObjectStyle); //Give it the style for a blue flagObject
      }
      else if(flagObjects[objectNum].objSig == m_flagSig.redSig)
      {
        lv_obj_set_style(m_screenObjects[objectNum], &m_redObjectStyle); //Give it the style for a red flagObject
      }


    }
    else
    {
      lv_obj_set_hidden(m_screenObjects[objectNum], true);
    }
  }

}
