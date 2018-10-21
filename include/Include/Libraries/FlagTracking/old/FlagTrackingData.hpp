// #include "main.h"
// #pragma once
//
//
// namespace display_C
// {
//   const int OBJECT_CONTAINER_WIDTH(316); // 480
//   const int OBJECT_CONTAINER_HEIGHT(212); // 240
//
//   const int OBJECT_SCALE_WIDTH(OBJECT_CONTAINER_WIDTH / VISION_FOV_WIDTH);
//   const int OBJECT_SCALE_HEIGHT(OBJECT_CONTAINER_HEIGHT / VISION_FOV_HEIGHT);
//
//   const int NUM_COLOR_OBJECTS(2); //max number of objects to see and print
//
//   const int DISPLAY_REFRESH(100); // How fast to poll the vision
// }
//
//
//
// const int VISION_PORT(7);
//
//   const int ERR_SIG(255); //Error signature for non-objects
//   const int FLAG_RED_SIG(0); //The signature for the RED flag
//   const int FLAG_BLUE_SIG(1); //The signature for the RED flag
//
//   const int NUM_VISION_OBJECTS(20); //max number of objects to see and print
//
//
//   const int VISION_REFRESH(40); // How fast to poll the vision
//
//   enum OBJ_COLOR // English for the flag colors
//   {
//     OBJ_FIRST_COLOR = 0,
//     OBJ_BLUE_COLOR = 0,
//     OBJ_RED_COLOR = 1,
//     OBJ_NUM_COLORS = 2
//   };
//
//
//
//
//
//
// struct visionObjects
// {
//    int objColor;
//   int objX;
//    int objY;
//   int objWidth;
//   int objHeight;
//   int objAvgDim; // Avg of width and height
// };
//
// extern visionObjects flagObjects[NUM_VISION_OBJECTS];
//
//
//
//
//
// class filterFunctions;
