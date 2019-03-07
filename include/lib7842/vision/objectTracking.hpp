#pragma once
#ifndef TRACKING_EMULATOR
#include "main.h"
#endif

#ifdef TRACKING_EMULATOR
#define VISION_OBJECT_ERR_SIG 255
#define VISION_FOV_WIDTH 316
#define VISION_FOV_HEIGHT 212
namespace pros {void delay(int);}
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "lvgl/lvgl.h"
#endif

#include <vector>
#include <array>
#include <functional>
#include <algorithm>

#ifndef TRACKING_EMULATOR
//#include "objectReading.hpp"
#endif

//#include "objectSmoothing.hpp"
//#include "objectDrawing.hpp"
#include "objectContainer.hpp"
