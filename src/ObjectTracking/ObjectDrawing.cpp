#include "ObjectDrawing.hpp"



ObjectDrawing::ObjectDrawing(objectSig_t flagSig, int containerWidth, int containerHeight)
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


objectContainer_t* ObjectDrawing::createObjectContainer(int arrayLength, bool createScreenObjects)
{
  objectContainer_t* newContainer = new objectContainer_t;

  newContainer->arrayLength = arrayLength;
  newContainer->objectArray = new simpleObjects_t[arrayLength];
  newContainer->objectCount = 0;

  if(createScreenObjects)
  {
    newContainer->screenArray = createSimpleObjects(arrayLength);
  }

  return newContainer;
}

lv_obj_t** ObjectDrawing::createSimpleObjects(int arrayLength)
{
  lv_obj_t** newObjects = new lv_obj_t*[arrayLength];

  for(int objectNum = 0; objectNum < arrayLength; objectNum++)
  {
    newObjects[objectNum] = lv_obj_create(m_objectContainer, NULL);
    lv_obj_set_style(newObjects[objectNum], &m_defaultObjectStyle);
    lv_obj_set_hidden(newObjects[objectNum], true);
  }
  return newObjects;
}



lv_style_t* ObjectDrawing::createStyle(lv_color_t bodyColor, lv_color_t borderColor, lv_opa_t opaNum)
{
  lv_style_t* objectStyle = new lv_style_t;

  lv_style_copy(objectStyle, &m_defaultObjectStyle);
  objectStyle->body.main_color = bodyColor;
  objectStyle->body.grad_color = bodyColor;
  objectStyle->body.border.color = borderColor;
  objectStyle->body.border.color = borderColor;
  objectStyle->body.opa = opaNum;
  objectStyle->body.opa = opaNum;

  return objectStyle;
}






void ObjectDrawing::drawSimpleObjects(objectContainer_t* objectContainer)
{

  for(int objectNum = 0; objectNum < objectContainer->arrayLength; objectNum++) //Hide all objects
  {
    lv_obj_set_hidden(objectContainer->screenArray[objectNum], true);
  }

  for(int objectNum = 0; objectNum < objectContainer->objectCount; objectNum++)
  {
    if(objectContainer->objectArray[objectNum].objSig != VISION_OBJECT_ERR_SIG)
    {
      lv_obj_set_hidden(objectContainer->screenArray[objectNum], false); // make visible

      //Set posisitons and size
      lv_obj_set_x(objectContainer->screenArray[objectNum], objectContainer->objectArray[objectNum].objX * m_widthScale);
      lv_obj_set_y(objectContainer->screenArray[objectNum], objectContainer->objectArray[objectNum].objY * m_heightScale);

      lv_obj_set_width(objectContainer->screenArray[objectNum], objectContainer->objectArray[objectNum].objWidth * m_widthScale);
      lv_obj_set_height(objectContainer->screenArray[objectNum], objectContainer->objectArray[objectNum].objHeight * m_heightScale);

      if(objectContainer->objectArray[objectNum].discardObject)
      {
        lv_obj_set_style(objectContainer->screenArray[objectNum], &m_discardObjectStyle);
      }
      else if(objectContainer->objectArray[objectNum].objSig == m_flagSig.blueSig)
      {
        lv_obj_set_style(objectContainer->screenArray[objectNum], objectContainer->blueStyle); //Give it the style for a blue flagObject
      }
      else if(objectContainer->objectArray[objectNum].objSig == m_flagSig.redSig)
      {
        lv_obj_set_style(objectContainer->screenArray[objectNum], objectContainer->redStyle); //Give it the style for a red flagObject
      }
      else
      {

      }
    }
    else
    {
      lv_obj_set_hidden(objectContainer->screenArray[objectNum], true);
    }
  }

}
