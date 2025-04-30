#pragma once
#include "ofMain.h"

class skybox {
public:
    void setup(const std::string& hdrPath);
    void draw(const ofCamera& cam);

private:
    void convertToCubemap();

    ofShader convertShader;
    ofShader drawShader;
    ofFloatImage hdrImage;
    ofFbo captureFbo;
    GLuint cubemapID = 0;
    ofBoxPrimitive cube;
    glm::vec3 targets[6];
    glm::vec3 ups[6];
    bool isInitialized = false;
};
