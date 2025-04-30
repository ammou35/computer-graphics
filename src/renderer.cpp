#include "renderer.h"

void Renderer::setup()
{
    ofSetFrameRate(60);
    ofLog() << "renderer::setup";

    speed = 100;

    offset_x = 0;
    offset_y = 0;
    offset_z = 0;
    rotation_y = 0;
    rotation_x = 0;

    delta_x = speed;
    delta_y = speed;
    delta_z = speed;
    delta_r_y = speed;
    delta_r_x = speed;

    set_mouse_press_x(0);
    set_mouse_press_y(0);
    set_mouse_current_x(0);
    set_mouse_current_y(0);
    set_is_mouse_button_pressed(false);
    graph.setup();
    camera.setNearClip(0.1f);   // how close something can be before clipping
    camera.setFarClip(10000.f); // how far something can be before clipping
    camera.setPosition(0, 0, 600);     // set manually position
    camera.lookAt(ofVec3f(0, 0, 0)); // look at the origin
}

void Renderer::update(const GuiManager& guiManager) {
    graph.update(guiManager.get_vector_stroke_color(), guiManager.get_vector_fill_color(), guiManager.get_type_vector_primitive(), guiManager.transformation, guiManager.transformation3D, guiManager.element3D_material, guiManager.element3D_texture, guiManager.element3D_filtre);
    graph.geometrie.set_projection_mode(guiManager.projection);
    graph.geometrie.shader_mode = guiManager.shader_mode;
    graph.bounding_box = guiManager.delimitation;
    sky_box = guiManager.sky_box;
    graph.dessinVectoriel.stroke_width_default = guiManager.lineWidth;
    set_bg_color(guiManager.get_vector_bg_color());
    ofSetBackgroundColor(bg_color);
    center_x = ofGetWidth() / 2.0f;
    center_y = ofGetHeight() / 2.0f;
    graph.geometrie.update(graph.element3D);
    ofVec3f worldPress = screenToScene(mouse_press[0], mouse_press[1]);
    ofVec3f worldCurrent = screenToScene(mouse_current[0], mouse_current[1]);
}

void Renderer::draw()
{
    camera.begin();
    if (sky_box) {
        graph.geometrie.skybox.draw(camera);
    }
    if (graph.geometrie.shader_active != nullptr)
        graph.geometrie.shader_active->begin();

    ofSetColor(255, 255, 255);
    for (const auto& i : image)
        i.draw(300, 24, 0);

    graph.draw(get_mouse_press(), get_mouse_current(), get_is_mouse_button_pressed());
    graph.dessinVectoriel.draw(get_mouse_press(), get_mouse_current(), get_is_mouse_button_pressed());

    if (graph.geometrie.shader_active != nullptr)
        graph.geometrie.shader_active->end();
    camera.end();
}

void Renderer::imageExport(const string name, const string extension) const
{
    ofImage img;
    img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    img.save(name + "." + extension);
}

void Renderer::imageImport(const string path)
{
    image.emplace_back(path);

    if (image.back().getWidth() > 0 && image.back().getHeight() > 0)
        ofSetWindowShape(image.back().getWidth(), image.back().getHeight());
}

bool Renderer::get_is_mouse_button_pressed(void) const {
    return is_mouse_button_pressed_;
}

int Renderer::get_mouse_press_x(void) const {
    return mouse_press[0];
}

int Renderer::get_mouse_press_y(void) const {
    return mouse_press[1];
}

int Renderer::get_mouse_current_x(void) const {
    return mouse_current[0];
}

int Renderer::get_mouse_current_y(void) const {
    return mouse_current[1];
}

const std::array<int, 2>& Renderer::get_mouse_press(void) const {
    return mouse_press;
}

const std::array<int, 2>& Renderer::get_mouse_current(void) const {
    return mouse_current;
}

ofColor Renderer::get_bg_color(void) const {
    return bg_color;
}

void Renderer::set_is_mouse_button_pressed(const bool& is_mouse_button_pressed) {
    is_mouse_button_pressed_ = is_mouse_button_pressed;
}

void Renderer::set_mouse_press_x(const int& mouse_press_x) {
    mouse_press[0] = mouse_press_x;
}

void Renderer::set_mouse_press_y(const int& mouse_press_y) {
    mouse_press[1] = mouse_press_y;
}

void Renderer::set_mouse_current_x(const int& mouse_current_x) {
    mouse_current[0] = mouse_current_x;
}

void Renderer::set_mouse_current_y(const int& mouse_current_y) {
    mouse_current[1] = mouse_current_y;
}

void Renderer::set_bg_color(const ofColor& color) {
    bg_color = color;
}

bool Renderer::is_mouse_in_draw_area(void) {
    return (get_mouse_current_x() >= 300 && get_mouse_current_x() <= 300 + ofGetWindowWidth() &&
            get_mouse_current_y() >= 24 && get_mouse_current_y() <= 24 + ofGetWindowHeight());
}

void Renderer::draw_font(void) {

    ofSetColor(255, 0, 0);
}

void Renderer::mouseReleased(void) {
    if (is_mouse_in_draw_area()) {
        ofVec3f plane_origin(0, 0, 0);
        ofVec3f plane_normal = camera.getLookAtDir();
        ofVec3f worldPress = screenToViewPlane(get_mouse_press()[0], get_mouse_press()[1], plane_origin, plane_normal);
        ofVec3f worldCurrent = screenToViewPlane(get_mouse_current()[0], get_mouse_current()[1], plane_origin, plane_normal);

        std::array<int, 2> mouse_press_w = { worldPress.x, worldPress.y };
        std::array<int, 2> mouse_current_w = { worldCurrent.x, worldCurrent.y };
        graph.add_element(mouse_press_w, mouse_current_w);
    }
}

ofVec3f Renderer::screenToScene(int x, int y) {
    ofVec3f screenPosNear(x, y, 0.0f); // Z proche
    ofVec3f screenPosFar(x, y, 1.0f);  // Z loin

    ofVec3f worldNear = camera.screenToWorld(screenPosNear);
    ofVec3f worldFar = camera.screenToWorld(screenPosFar);

    ofVec3f rayDirection = (worldFar - worldNear).normalized();

    // intersection avec le plan Y=0
    float t = -worldNear.y / rayDirection.y;
    ofVec3f intersection = worldNear + t * rayDirection;

    return intersection;
}

ofVec3f Renderer::screenToViewPlane(int x, int y, const ofVec3f& plane_origin, const ofVec3f& plane_normal) {
    ofVec3f screenPosNear(x, y, 0.0f); // Z = proche
    ofVec3f screenPosFar(x, y, 1.0f);  // Z = loin

    ofVec3f worldNear = camera.screenToWorld(screenPosNear);
    ofVec3f worldFar = camera.screenToWorld(screenPosFar);

    ofVec3f rayDirection = (worldFar - worldNear).normalized();

    float denom = plane_normal.dot(rayDirection);
    if (abs(denom) > 1e-6) {
        float t = plane_normal.dot(plane_origin - worldNear) / denom;
        return worldNear + rayDirection * t;
    }
    else {
        // Rayon parall�le au plan : retourner worldNear
        return worldNear;
    }
}
