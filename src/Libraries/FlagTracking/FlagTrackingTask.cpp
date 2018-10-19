#include "main.h"

#include "Include/Libraries/FlagTracking/FlagTrackingMain.hpp"



// main task
void mainFlagTrackingTask(void*ignore)
{
  colorObjects *flagExport;

  visionTracking mainVisionTracking(9, 30);
  //screenDrawing mainScreenDrawing(OBJECT_CONTAINER_WIDTH, OBJECT_CONTAINER_HEIGHT, OBJECT_SCALE_WIDTH, OBJECT_SCALE_HEIGHT);

  while(true)
  {
    mainVisionTracking.getObjects(); //Calculates Objects

    mainVisionTracking.filterObjectSize();
    mainVisionTracking.filterObjectProp();

    flagExport = mainVisionTracking.exportArray();
    flagExport[1].objSig = 2;
    //mainScreenDrawing.drawFlagObjects(flagExport);




    pros::delay(100);
  }
}
