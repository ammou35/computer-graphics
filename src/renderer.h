#pragma once

#include "ofMain.h"
#include "guiManager.h"
#include "graph.h"

struct image2f
{
    ofImage image;
    ofVec2f position;
};

class Renderer
{
    vector<ofImage> image; //img contenue dans le rendu

public:

    ofCamera camera;
    Graph graph;

    float center_x;
    float center_y;

    float offset_x;
    float offset_y;
    float offset_z;
    float rotation_y;
    float rotation_x;

    float delta_x;
    float delta_y;
    float delta_z;
    float delta_r_y;
    float delta_r_x;

    float speed;
    bool sky_box;

    void setup();
    void update(const GuiManager& guiManager);
    void draw();

    void imageExport(const string name, const string extension = "jpeg") const;
    void imageImport(const string path);

    bool get_is_mouse_button_pressed(void) const;
    ofColor get_bg_color(void) const;

    void set_is_mouse_button_pressed(const bool& is_mouse_button_pressed);
    void set_bg_color(const ofColor& color);

    int get_mouse_press_x(void) const;
    int get_mouse_press_y(void) const;
    int get_mouse_current_x(void) const;
    int get_mouse_current_y(void) const;
    const std::array<int, 2>& get_mouse_press(void) const;
    const std::array<int, 2>& get_mouse_current(void) const;

    void set_mouse_press_x(const int& mouse_press_x);
    void set_mouse_press_y(const int& mouse_press_y);
    void set_mouse_current_x(const int& mouse_current_x);
    void set_mouse_current_y(const int& mouse_current_y);
    
    void draw_cursor(float x, float y) const;
    bool is_mouse_in_draw_area(void);

    void mouseReleased(void);
    void draw_font(void);

    ofVec3f screenToScene(int x, int y);
    ofVec3f screenToViewPlane(int x, int y, const ofVec3f& plane_origin, const ofVec3f& plane_normal);

    ofTexture applyFilter(const ofTexture& texture, ofShader& shader);
private:
    ofColor bg_color;

    //Dessin vectoriel
    std::array<int, 2> mouse_press;
    std::array<int, 2> mouse_current;

    bool is_mouse_button_pressed_;
};