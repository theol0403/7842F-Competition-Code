#include "ObjectTrackingMain.hpp"

// main task
void mainObjectTrackingTask(void*)
{

  const int BLUESIG = 1;
  const int REDSIG = 2;

  ObjectDrawing mainScreenDrawing(LV_HOR_RES, LV_VER_RES);

  ObjectContainer rawObjects(6, mainScreenDrawing);
  rawObjects.setContainerStyle(LV_COLOR_BLACK, LV_COLOR_BLACK, LV_OPA_30);

  ObjectContainer sortedObjects(20, mainScreenDrawing);
  sortedObjects.setSigStyle(BLUESIG, LV_COLOR_BLUE);
  sortedObjects.setSigStyle(REDSIG, LV_COLOR_RED);


  while(true)
  {

    mainScreenDrawing.drawSimpleObjects(rawObjects);

    // mainVisionReading.getObjects(); //Calculates Objects
    // mainVisionReading.filterNoise();
    // mainVisionReading.filterProp();
    // mainVisionReading.filterSize();
    // mainVisionReading.discardObjects();
    // mainVisionReading.exportObjects(rawObjects);
    // mainScreenDrawing.drawSimpleObjects(rawObjects);

    // testInput.outputObjects();
    // mainScreenDrawing.drawSimpleObjects(rawObjects);
    //
    // rawObjectSmoothing.smoothObjects(rawObjects, sortedObjects);
    // mainScreenDrawing.drawSimpleObjects(sortedObjects);
    // rawObjectSmoothing.exportObjects(ghostObjects, 0, 20);
    // mainScreenDrawing.drawSimpleObjects(ghostObjects);

    pros::delay(100);
  }
}
