#include "ObjectTracking.hpp"

// main task
void ObjectTrackingTask(void*)
{

  lib7842::ObjectDrawing mainScreenDrawing(LV_HOR_RES, LV_VER_RES);

  const int BLUESIG = 1;
  const int REDSIG = 2;

  const int GREENSIG = 3;

  pros::Vision mainVision(20);
  pros::delay(1000);

  // pros::vision_color_code_t blueCode = mainVision.create_color_code(1, 5);
  // while(true)
  // {
  //   std::cout << mainVision.get_by_code(0, blueCode).top_coord << std::endl;
  //   pros::delay(200);
  // }






//   pros::vision_signature_s_t sig_BLUEFLAG = {1, {1, 0, 0}, 2.9, -2891, -2101, -2496, 9503, 13653, 11578, 0, 1};
//   mainVision.set_signature(1, &sig_BLUEFLAG);
// pros::vision_signature_s_t sig_REDFLAG = {2, {1, 0, 0}, 3.2, 7903, 8235, 8069, -175, 167, -4, 0, 1};
//   mainVision.set_signature(2, &sig_REDFLAG);
//   pros::vision_signature_s_t sig_GREENFLAG = {5, {1, 0, 0}, 7.2, -2845, -2471, -2658, -4967, -4257, -4612, 0, 1};
//   mainVision.set_signature(5, &sig_GREENFLAG);
//   mainVision.set_exposure( (int) ( (30/150 * 100) + 50) / 255 );


  lib7842::ObjectReading mainObjectReading(mainVision);
  lib7842::codeSig_t BLUECODE = mainObjectReading.createCodeSig(BLUESIG, GREENSIG);
  lib7842::codeSig_t REDCODE = mainObjectReading.createCodeSig(BLUESIG, GREENSIG);

  lib7842::ObjectContainer rawObjects(30, mainScreenDrawing);
  rawObjects.setSigStyle(BLUESIG, LV_COLOR_BLUE, LV_COLOR_BLACK, LV_OPA_40);
  rawObjects.setSigStyle(REDSIG, LV_COLOR_RED, LV_COLOR_BLACK, LV_OPA_40);
  rawObjects.setSigStyle(REDSIG, LV_COLOR_GREEN, LV_COLOR_BLACK, LV_OPA_40);
//  rawObjects.setSigStyle(BLUECODE.destSig, LV_COLOR_BLUE, LV_COLOR_LIME, LV_OPA_40);
//  rawObjects.setSigStyle(REDCODE.destSig, LV_COLOR_RED, LV_COLOR_LIME, LV_OPA_40);


  lib7842::ObjectContainer smoothedObjects(20, mainScreenDrawing);
  smoothedObjects.setSigStyle(BLUESIG, LV_COLOR_BLUE, LV_COLOR_YELLOW, LV_OPA_70);
  smoothedObjects.setSigStyle(REDSIG, LV_COLOR_RED, LV_COLOR_YELLOW, LV_OPA_70);
  lib7842::ObjectSmoothing objectSmoothing
  (
    rawObjects, smoothedObjects, // Source, Dest
  {},//  {{BLUECODE.destSig, BLUESIG}, {REDCODE.destSig, REDSIG}}, // Sig merges
    20, 10, 3, // Maxlife, LifeZone, lifeIncrement
    0.5, 0.4, // PosEMA, VelEMA
    {{20, 15}, {15, 30}, {40, 10}, {50, 5}}, // PosThresh, DimThresh
    0 // DebugMode
  );

  while(true)
  {
    rawObjects.shrinkTo(0);
    rawObjects.currentCount = 0;

    mainObjectReading.getCodeObjects(rawObjects, {BLUECODE, REDCODE});
    rawObjects.debugObjects(2);
    pros::delay(200);
    rawObjects.removeRange(lib7842::objArea, 0, 400);
    std::cout << rawObjects.currentCount;
    mainScreenDrawing.drawSimpleObjects(rawObjects);

   //objectSmoothing.smoothObjects();
//   mainScreenDrawing.drawSimpleObjects(smoothedObjects);


    pros::delay(100);
  }
}
