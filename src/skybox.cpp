#include "skybox.h"

void skybox::setup(const std::string& hdrPath) {
    ofDisableArbTex();

    // Load HDR
    if (!hdrImage.load(hdrPath)) {
        ofLogError() << "Failed to load HDR image: " << hdrPath;
        return;
    }

    // Load shaders
    convertShader.load("shaders/skybox/skybox_convert_vs.glsl", "shaders/skybox/skybox_convert_fs.glsl");
    drawShader.load("shaders/skybox/skybox_draw_vs.glsl", "shaders/skybox/skybox_draw_fs.glsl");

    // Directions for 6 cube faces
    targets[0] = { 1, 0, 0 };  ups[0] = { 0, -1, 0 }; // +X
    targets[1] = { -1, 0, 0 }; ups[1] = { 0, -1, 0 }; // -X
    targets[2] = { 0, -1, 0 }; ups[2] = { 0, 0, -1 }; // -Y
    targets[3] = { 0, 1, 0 };  ups[3] = { 0, 0, 1 };  // +Y
    targets[4] = { 0, 0, 1 };  ups[4] = { 0, -1, 0 }; // +Z
    targets[5] = { 0, 0, -1 }; ups[5] = { 0, -1, 0 }; // -Z

    // Allocate cubemap
    glGenTextures(1, &cubemapID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    captureFbo.allocate(512, 512, GL_RGB32F);

    cube.set(1.0f);
    cube.mapTexCoordsFromTexture(ofTexture());

    convertToCubemap();
    isInitialized = true;
}

void skybox::convertToCubemap() {
    for (int i = 0; i < 6; ++i) {
        ofCamera cam;
        cam.setPosition(0, 0, 0);
        cam.lookAt(targets[i], ups[i]);
        cam.setNearClip(0.1f);
        cam.setFarClip(10.0f);
        cam.setAspectRatio(1.0f);
        cam.setFov(90.0f);

        captureFbo.begin();
        ofClear(0);
        cam.begin();
        convertShader.begin();
        convertShader.setUniformTexture("equirectangularMap", hdrImage.getTexture(), 0);
        convertShader.setUniformMatrix4f("modelViewProjectionMatrix", cam.getModelViewProjectionMatrix());
        cube.draw();
        convertShader.end();
        cam.end();
        captureFbo.end();

        ofPixels pixels;
        captureFbo.readToPixels(pixels);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
        glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, pixels.getWidth(), pixels.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, pixels.getData());
    }
}

void skybox::draw(const ofCamera& cam) {
    if (!isInitialized) return;

    ofPushMatrix();
    ofDisableDepthTest();

    drawShader.begin();
    drawShader.setUniformMatrix4f("view", glm::mat4(glm::mat3(cam.getModelViewMatrix()))); // remove translation
    drawShader.setUniformMatrix4f("projection", cam.getProjectionMatrix());
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
    drawShader.setUniform1i("environmentMap", 0);

    cube.draw();

    drawShader.end();
    ofEnableDepthTest();
    ofPopMatrix();
}
