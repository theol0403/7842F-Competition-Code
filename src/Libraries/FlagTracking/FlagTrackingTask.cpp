#include "main.h"

#include "Include/Libraries/FlagTracking/FlagTrackingMain.hpp"



// main task
void mainFlagTrackingTask(void*ignore)
{
  visionObjects *flagExport;

  visionTracking mainVisionTracking(9, 30);
  //screenDrawing mainScreenDrawing(flagExport, OBJECT_CONTAINER_WIDTH, OBJECT_CONTAINER_HEIGHT);

  while(true)
  {
    mainVisionTracking.getObjects(); //Calculates Objects

    mainVisionTracking.filterObjectSize();
    mainVisionTracking.filterObjectProp();

    flagExport = mainVisionTracking.exportArray();

    mainScreenDrawing.drawFlagObjects();




    pros::delay(100);
  }
}
