#pragma once
#include "objectTracking.hpp"

namespace lib7842
{

  class ObjectDrawing
  {
  private:

    const int m_widthScale;
    const int m_heightScale;

    lv_obj_t* m_drawingContainer = nullptr;
    lv_style_t m_drawingContainerStyle;

    lv_style_t m_defaultObjectStyle;
    lv_style_t m_discardObjectStyle;


  public:

    ObjectDrawing(int, int);
    ~ObjectDrawing();


    void drawSimpleObjects(lib7842::ObjectContainer&);

    friend class lib7842::ObjectContainer;

  };

}
