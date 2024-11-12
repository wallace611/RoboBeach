#pragma once

#include "Engine.h"

#include "utils/ObjContainer.h"
#include "object/Object.h"
#include "utils/String.h"
#include "object/Triangle.h"
#include "object/robot/Robot.h"
#include "object/Floor.h"
#include "input/InputMapper.h"
#include "object/Rock.h"

#include <stdio.h>

#include <cglm/cglm.h>

typedef unsigned char perspective_t;
#define RB_ROBOT	1
#define RB_WORLD	2

perspective_t persp;

Triangle* t1;
Triangle* t2;
Rock* rock1;
Rock* rock2;
Robot* r;
Floor* flr;
Camera* worldCam;
Camera* botCam;

int runTest(int* argc, char** argv);