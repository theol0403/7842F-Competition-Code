#include "ObjectTrackingMain.hpp"

// main task
void mainObjectTrackingTask(void*)
{

  const int BLUESIG = 1;
  const int REDSIG = 2;
  pros::Vision mainVision(6);

  ObjectDrawing mainScreenDrawing(LV_HOR_RES, LV_VER_RES);

  ObjectContainer rawObjects(6, mainScreenDrawing);
  rawObjects.setContainerStyle(LV_COLOR_BLACK, LV_COLOR_BLACK, LV_OPA_30);
  VisionReading mainVisionReading(mainVision, rawObjects);

  ObjectContainer sortedObjects(20, mainScreenDrawing);
  sortedObjects.setSigStyle(BLUESIG, LV_COLOR_BLUE);
  sortedObjects.setSigStyle(REDSIG, LV_COLOR_RED);
  ObjectSmoothing objectSmoothing(rawObjects, sortedObjects, 30, 0.1, true, 0.1, 20, 10);

  while(true)
  {


    mainVisionReading.getObjects();
    mainVisionReading.filterNoise();
    //mainVisionReading.filterProp();
    mainVisionReading.filterSize();

    mainScreenDrawing.drawSimpleObjects(rawObjects);
    mainVisionReading.discardObjects();

    objectSmoothing.smoothObjects();
    mainScreenDrawing.drawSimpleObjects(sortedObjects);


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
