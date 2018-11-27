#include "ObjectTrackingMain.hpp"

// main task
void mainObjectTrackingTask(void*)
{

  const int BLUESIG = 1;
  const int REDSIG = 2;
  pros::Vision mainVision(20);
  //pros::delay(1000);
  //pros::vision_signature_s_t ballSig = mainVision.get_signature(1);
  //mainVision.print_signature(ballSig);
  pros::vision_signature_s_t ballSig = {1, {1, 0, 0}, 3.100000, 437, 1051, 744, -4369, -3967, -4168, 0, 0};
  mainVision.set_signature(1, &ballSig);
  //mainVision.set_exposure(55);

  ObjectDrawing mainScreenDrawing(LV_HOR_RES, LV_VER_RES);

  ObjectContainer rawObjects(6, mainScreenDrawing);
  rawObjects.setContainerStyle(LV_COLOR_BLACK, LV_COLOR_BLACK, LV_OPA_30);
  VisionReading mainVisionReading(mainVision, rawObjects);

  ObjectContainer sortedObjects(20, mainScreenDrawing);
  sortedObjects.setSigStyle(BLUESIG, LV_COLOR_BLUE);
  sortedObjects.setSigStyle(REDSIG, LV_COLOR_RED);
  ObjectSmoothing objectSmoothing(rawObjects, sortedObjects, 50, 0.3, false, 0.3, 10, 3);

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

    pros::delay(50);
  }
}
