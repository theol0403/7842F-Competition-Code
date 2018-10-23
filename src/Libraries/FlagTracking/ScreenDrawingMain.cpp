#include "main.h"

#include "Include/Libraries/FlagTracking/FlagTrackingMain.hpp"
#include "Include/Libraries/FlagTracking/ScreenDrawingMain.hpp"







screenDrawing::screenDrawing(flagSig_t flagSig, int containerWidth, int containerHeight)
:
m_flagSig(flagSig),
m_widthScale{containerWidth/VISION_FOV_WIDTH},
m_heightScale{containerHeight/VISION_FOV_HEIGHT}
{
  initContainer(containerWidth, containerHeight);
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
  backgroundStyle.body.grad_color = LV_COLOR_GRAY;
  lv_obj_set_style(m_objectContainer, &backgroundStyle);
}




void screenDrawing::initSimpleObjects(int simpleObjectCount)
{
  m_simpleObjectCount = simpleObjectCount;
  m_simpleObjects = new lv_obj_t*[simpleObjectCount];


  //Generic Object Style
  lv_style_copy(&m_simpleObjectStyle, &lv_style_pretty_color);
  m_simpleObjectStyle.body.main_color = LV_COLOR_GRAY;
  m_simpleObjectStyle.body.grad_color = LV_COLOR_GRAY;
  m_simpleObjectStyle.body.radius = 5;
  m_simpleObjectStyle.body.border.color = LV_COLOR_BLACK;
  m_simpleObjectStyle.body.border.width = 3;
  m_simpleObjectStyle.body.border.opa = LV_OPA_100;

  //Blue object style
  lv_style_copy(&m_blueSimpleObjectStyle, &m_simpleObjectStyle);
  m_blueSimpleObjectStyle.body.main_color = LV_COLOR_BLUE;
  m_blueSimpleObjectStyle.body.grad_color = LV_COLOR_BLUE;
  m_blueSimpleObjectStyle.body.border.color = LV_COLOR_YELLOW;

  //Red object style
  lv_style_copy(&m_redSimpleObjectStyle, &m_simpleObjectStyle);
  m_redSimpleObjectStyle.body.main_color = LV_COLOR_RED;
  m_redSimpleObjectStyle.body.grad_color = LV_COLOR_RED;
  m_redSimpleObjectStyle.body.border.color = LV_COLOR_YELLOW;

  //Discard object style
  lv_style_copy(&m_discardSimpleObjectStyle, &m_simpleObjectStyle);
  m_discardSimpleObjectStyle.body.main_color = LV_COLOR_OLIVE;
  m_discardSimpleObjectStyle.body.grad_color = LV_COLOR_OLIVE;
  m_discardSimpleObjectStyle.body.border.color = LV_COLOR_YELLOW;



  for(int objectNum = 0; objectNum < simpleObjectCount; objectNum++)
  {
    m_simpleObjects[objectNum] = lv_obj_create(m_objectContainer, NULL);
    lv_obj_set_style(m_simpleObjects[objectNum], &m_simpleObjectStyle);
    lv_obj_set_hidden(m_simpleObjects[objectNum], true);
  }
}





void screenDrawing::drawSimpleObjects(simpleObjects* flagObjects)
{

  for(int objectNum = 0; objectNum < m_simpleObjectCount; objectNum++)
  {
    if(flagObjects[objectNum].objSig != VISION_OBJECT_ERR_SIG)
    {

      // make visible
      lv_obj_set_hidden(m_simpleObjects[objectNum], false);

      //Set posisitons and size
      lv_obj_set_x(m_simpleObjects[objectNum], flagObjects[objectNum].objX * m_widthScale);
      lv_obj_set_y(m_simpleObjects[objectNum], flagObjects[objectNum].objY * m_heightScale);

      lv_obj_set_width(m_simpleObjects[objectNum], flagObjects[objectNum].objWidth * m_widthScale);
      lv_obj_set_height(m_simpleObjects[objectNum], flagObjects[objectNum].objHeight * m_heightScale);



      if(flagObjects[objectNum].discardObject)
      {
        lv_obj_set_style(m_simpleObjects[objectNum], &m_discardSimpleObjectStyle);
      }
      else if(flagObjects[objectNum].objSig == m_flagSig.blueSig)
      {
        lv_obj_set_style(m_simpleObjects[objectNum], &m_blueSimpleObjectStyle); //Give it the style for a blue flagObject
      }
      else if(flagObjects[objectNum].objSig == m_flagSig.redSig)
      {
        lv_obj_set_style(m_simpleObjects[objectNum], &m_redSimpleObjectStyle); //Give it the style for a red flagObject
      }
      else
      {

      }


    }
    else
    {
      lv_obj_set_hidden(m_simpleObjects[objectNum], true);
    }
  }

}
