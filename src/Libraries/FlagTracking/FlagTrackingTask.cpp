#include "main.h"

#include "Include/Libraries/FlagTracking/FlagTrackingMain.hpp"
#include "Include/Libraries/FlagTracking/ScreenDrawingMain.hpp"



// main task
void mainFlagTrackingTask(void*ignore)
{

  visionTracking mainVisionTracking(9, 30);
  screenDrawing mainScreenDrawing(316, 212, 30);

  while(true)
  {
    mainVisionTracking.getObjects(); //Calculates Objects

    mainVisionTracking.filterObjectSize();
    mainVisionTracking.filterObjectProp();


  //  mainScreenDrawing.drawFlagObjects(mainVisionTracking.exportObjects(););




    pros::delay(100);
  }
}
