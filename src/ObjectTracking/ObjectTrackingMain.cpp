#include "ObjectTrackingMain.hpp"

// main task
void mainObjectTrackingTask(void*)
{

  const int BLUESIG = 1;
  const int REDSIG = 2;
  pros::Vision mainVision(20);
  pros::delay(1000);

  // pros::vision_signature_s_t blueSig = mainVision.get_signature(1);
  // mainVision.print_signature(blueSig);
  //pros::vision_signature_s_t redSig = mainVision.get_signature(2);
  //mainVision.print_signature(redSig);

  pros::vision_signature_s_t SIG_1 = {1, {1, 0, 0}, 9.300000, -2979, -2215, -2597, 11377, 13517, 12447, 0, 0};
  mainVision.set_signature(1, &SIG_1);
pros::vision_signature_s_t SIG_2 = {2, {1, 0, 0}, 8.00000, 9227, 9651, 9439, -543, -227, -385, 0, 0};
  mainVision.set_signature(2, &SIG_2);

  ObjectDrawing mainScreenDrawing(LV_HOR_RES, LV_VER_RES);

  ObjectContainer rawObjects(6, mainScreenDrawing);
  rawObjects.setContainerStyle(LV_COLOR_BLACK, LV_COLOR_BLACK, LV_OPA_30);
  VisionReading mainVisionReading(mainVision, rawObjects);

  ObjectContainer sortedObjects(20, mainScreenDrawing);
  sortedObjects.setSigStyle(BLUESIG, LV_COLOR_BLUE);
  sortedObjects.setSigStyle(REDSIG, LV_COLOR_RED);
  ObjectSmoothing objectSmoothing(rawObjects, sortedObjects, 30, 0.35, false, 0.3, 40, 20);

  while(true)
  {

    mainVisionReading.getObjects();
    mainVisionReading.filterNoise(20);
    //mainVisionReading.filterProp();
    //mainVisionReading.filterSize();
    mainVisionReading.discardObjects();

    mainScreenDrawing.drawSimpleObjects(rawObjects);


    objectSmoothing.smoothObjects();
    mainScreenDrawing.drawSimpleObjects(sortedObjects);


    pros::delay(50);
  }
}
