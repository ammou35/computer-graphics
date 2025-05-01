#pragma once
#include "ofMain.h"
#include "ofxImGui.h"
#include "graph.h"

class GuiManager {
public:
    void setup();
    void draw(ElementScene2D* element2D, ElementScene3D* element3D, const vector<ofImage> &images);
    void update(Graph &graph);
    int get_type_vector_primitive(void) const;
    void set_type_vector_primitive(const int& typeVectorPrimitive);
    ofColor get_vector_stroke_color(void) const;
    ofColor get_vector_fill_color(void) const;
    ofColor get_vector_bg_color(void) const;
    void set_is_visible(const bool& visible);
    bool get_is_visible(void) const;

    std::array<float, 256> normalizedHistogramR;
    std::array<float, 256> normalizedHistogramG;
    std::array<float, 256> normalizedHistogramB;
    void calculate_histogram(void);

    std::array<float, 6> transformation;
    std::array<float, 9> transformation3D;
    bool delimitation;
    bool sky_box;
    float lineWidth;
    ImVec2 tesselation;
    bool projection;
    vector<std::string> imagePath;
    ofImage selected_image;
    bool is_selected_image;
    bool showGraph3DTransformation;
    bool showGraph3DMats;
    bool showGraph3DLights;
    int element3D_material;
    int shader_mode;
    int element3D_texture;
    int element3D_filtre;
    int item_selected3D = 0;
    int item_selected3D_with_material = 0;
    bool transformationBufferIsInitialized = false;
    float exposure;
private:
    ofxImGui::Gui gui;
    int typeVectorPrimitive_;
    ofColor stroke_color;
    ofColor fill_color;
    ofColor bg_color;
    ImVec4 ui_color;
    bool is_visible;
    bool ui_color_picker;
    bool mode_2d;
    int item_selected;
};