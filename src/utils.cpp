#pragma once

#include "ofMain.h"

ofVec3f toVec3f(const ofColor& c) {


    return ofVec3f(c.r / 255.0f, c.g / 255.0f, c.b / 255.0f);


}