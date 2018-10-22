#include "main.h"

#include "Include/Libraries/FlagTracking/FlagTrackingMain.hpp"
#include "Include/Libraries/FlagTracking/ScreenDrawingMain.hpp"



// main task
void mainFlagTrackingTask(void*ignore)
{
  flagSig_t mainFlagSig {1, 2};

  visionTracking mainVisionTracking(9, 30);
  screenDrawing mainScreenDrawing(mainFlagSig, 316, 212, 30);

  visionObjects* objectExport;

  while(true)
  {
    mainVisionTracking.getObjects(); //Calculates Objects

    mainVisionTracking.filterObjectSize();
    mainVisionTracking.filterObjectProp();

    objectExport = mainVisionTracking.exportObjects();

    mainScreenDrawing.drawVisionObjects(objectExport, 20);




    pros::delay(100);
  }
}
