#include "main.h"

#include "Include/Libraries/FlagTracking/FlagTrackingMain.hpp"




// main task
void mainFlagTrackingTask(void*ignore)
{
  colorObjects flagExport[OBJECT_NUM];

  visionTracking mainVisionTracking(9);
  //screenDrawing mainScreenDrawing(OBJECT_CONTAINER_WIDTH, OBJECT_CONTAINER_HEIGHT, OBJECT_SCALE_WIDTH, OBJECT_SCALE_HEIGHT);

  while(true)
  {
    mainVisionTracking.getObjects(); //Calculates Objects

    mainVisionTracking.filterObjectSize();
    mainVisionTracking.filterObjectProp();

    mainVisionTracking.exportArray(flagExport);
    //mainScreenDrawing.drawFlagObjects(flagExport);




    pros::delay(100);
  }
}
