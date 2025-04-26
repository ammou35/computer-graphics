#include "application.h"

void Application::setup()
{
    ofSetWindowTitle("IFT-3100");
    ofLogToConsole();
    ofHideCursor();

    is_key_press_up = false;
    is_key_press_down = false;
    is_key_press_left = false;
    is_key_press_right = false;
    is_key_press_q = false;
    is_key_press_w = false;
    is_key_press_e = false;
    is_key_press_a = false;
    is_key_press_s = false;
    is_key_press_d = false;

    ofLog() << "app::setup";
    guiManager.setup();
    renderer.setup();

}

//--------------------------------------------------------------
void Application::update()
{
    time_current = ofGetElapsedTimef();
    time_elapsed = time_current - time_last;
    time_last = time_current;

    if (is_key_press_up)
        renderer.offset_y += renderer.delta_y * time_elapsed;
    if (is_key_press_down)
        renderer.offset_y -= renderer.delta_y * time_elapsed;
    if (is_key_press_left)
        renderer.offset_x += renderer.delta_x * time_elapsed;
    if (is_key_press_right)
        renderer.offset_x -= renderer.delta_x * time_elapsed;
    if (is_key_press_q)
        renderer.offset_z += renderer.delta_z * time_elapsed;
    if (is_key_press_w)
        renderer.rotation_x -= renderer.delta_r_x * time_elapsed/10;
    if (is_key_press_e)
        renderer.offset_z -= renderer.delta_z * time_elapsed;
    if (is_key_press_a)
        renderer.rotation_y -= renderer.delta_r_y * time_elapsed/10;
    if (is_key_press_s)
        renderer.rotation_x += renderer.delta_r_x * time_elapsed/10;
    if (is_key_press_d)
        renderer.rotation_y += renderer.delta_r_y * time_elapsed/10;

    renderer.update(guiManager);
    guiManager.update(renderer.graph);
    set_ui_visible(guiManager.get_is_visible());
}

//--------------------------------------------------------------
void Application::draw()
{
    renderer.draw();

    if (get_ui_visible()) {
        guiManager.draw(renderer.graph.element2D, renderer.graph.element3D, renderer.graph.images);
    }
    renderer.draw_cursor(renderer.get_mouse_current_x(), renderer.get_mouse_current_y());
    //renderer.draw_font();
}

//--------------------------------------------------------------
void Application::keyPressed(int key)
{
    switch (key)
    {
    case OF_KEY_LEFT: // key ←
        is_key_press_left = true;
        break;

    case OF_KEY_UP: // key ↑
        is_key_press_up = true;
        break;

    case OF_KEY_RIGHT: // key →
        is_key_press_right = true;
        break;

    case OF_KEY_DOWN: // key ↓
        is_key_press_down = true;
        break;

    case 113: // key q
        is_key_press_q = true;
        break;

    case 119: // key w
        is_key_press_w = true;
        break;

    case 101: // key e
        is_key_press_e = true;
        break;

    case 97: // key a
        is_key_press_a = true;
        break;

    case 115: // key s
        is_key_press_s = true;
        break;

    case 100: // key d
        is_key_press_d = true;
        break;

    case OF_KEY_DEL:
        renderer.graph.delete_element2D();
        renderer.graph.delete_element3D();

    default:
        break;
    }

    if (key == 'p') {
        ofImage screenshot;
        screenshot.grabScreen(300, 24, ofGetWindowWidth() - 300, ofGetWindowHeight() - 24); // Capture the entire screen
        string fileName = "screenshot_" + ofGetTimestampString() + ".png";
        screenshot.save(fileName); // Save as PNG
        ofLog() << "Screenshot saved as screenshot.png";
    }
}

//--------------------------------------------------------------
void Application::keyReleased(int key)
{
    ofLog() << "<app::keyReleased: " << key << ">";

    if (key == ' ')
        renderer.imageExport("render", "png");

    switch (key)
    {
    case OF_KEY_LEFT: // key ←
        is_key_press_left = false;
        break;

    case OF_KEY_UP: // key ↑
        is_key_press_up = false;
        break;

    case OF_KEY_RIGHT: // key →
        is_key_press_right = false;
        break;

    case OF_KEY_DOWN: // key ↓
        is_key_press_down = false;
        break;

    case 113: // key q
        is_key_press_q = false;
        break;

    case 119: // key w
        is_key_press_w = false;
        break;

    case 101: // key e
        is_key_press_e = false;
        break;

    case 97: // key a
        is_key_press_a = false;
        break;

    case 115: // key s
        is_key_press_s = false;
        break;

    case 100: // key d
        is_key_press_d = false;
        break;
    }
}

//--------------------------------------------------------------
void Application::mouseMoved(int x, int y ){
    renderer.set_mouse_current_x(x);
    renderer.set_mouse_current_y(y);
}

//--------------------------------------------------------------
void Application::mouseDragged(int x, int y, int button){
    renderer.set_mouse_current_x(x);
    renderer.set_mouse_current_y(y);
}

//--------------------------------------------------------------
void Application::mousePressed(int x, int y, int button){
    renderer.set_is_mouse_button_pressed(true);

    renderer.set_mouse_current_x(x);
    renderer.set_mouse_current_y(y);

    renderer.set_mouse_press_x(x);
    renderer.set_mouse_press_y(y);
}

//--------------------------------------------------------------
void Application::mouseReleased(int x, int y, int button){
    renderer.set_is_mouse_button_pressed(false);

    renderer.set_mouse_current_x(x);
    renderer.set_mouse_current_y(y);
    renderer.mouseReleased();
}

//--------------------------------------------------------------
void Application::mouseEntered(int x, int y){
    renderer.set_mouse_current_x(x);
    renderer.set_mouse_current_y(y);
}

//--------------------------------------------------------------
void Application::mouseExited(int x, int y){
    renderer.set_mouse_current_x(x);
    renderer.set_mouse_current_y(y);
}

//--------------------------------------------------------------
void Application::windowResized(int w, int h){

}

//--------------------------------------------------------------
void Application::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void Application::dragEvent(ofDragInfo dragInfo)
{
    ofLog() << "<app::ofDragInfo" << dragInfo.files.at(0) << " at : " << dragInfo.position << ">";

    renderer.imageImport(dragInfo.files.at(0));
}

//--------------------------------------------------------------
void Application::exit()
{
    
}

void Application::set_ui_visible(const bool& is_visible) {
    ui_visible = is_visible;
}

bool Application::get_ui_visible() const{
    return ui_visible;
}
