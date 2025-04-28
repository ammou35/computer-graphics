#include "application.h"

void Application::setup()
{
    ofSetWindowTitle("IFT-3100");
    ofLogToConsole();

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
        renderer.camera.boom(+10);
        //renderer.offset_y += renderer.delta_y * time_elapsed;
    if (is_key_press_down)
        renderer.camera.boom(-10);
        //renderer.offset_y -= renderer.delta_y * time_elapsed;
    if (is_key_press_left)
        renderer.camera.truck(+10);
        //renderer.offset_x += renderer.delta_x * time_elapsed;
    if (is_key_press_right)
        renderer.camera.truck(-10);
        //renderer.offset_x -= renderer.delta_x * time_elapsed;
    if (is_key_press_q)
        renderer.camera.dolly(+10);
    if (is_key_press_w)
        renderer.camera.tilt(+1);
        //renderer.rotation_x -= renderer.delta_r_x * time_elapsed/10;
    if (is_key_press_e)
        renderer.camera.dolly(-10);
        //renderer.offset_z -= renderer.delta_z * time_elapsed;
    if (is_key_press_a)
        renderer.camera.pan(-1);
        //renderer.rotation_y -= renderer.delta_r_y * time_elapsed/10;
    if (is_key_press_s)
        renderer.camera.tilt(-1);
        //renderer.rotation_x += renderer.delta_r_x * time_elapsed/10;
    if (is_key_press_d)
        renderer.camera.pan(+1);
        //renderer.rotation_y += renderer.delta_r_y * time_elapsed/10;

    //--------------------------------------------------------------
// Avant renderer.update(guiManager);
    if (is_key_press_q || is_key_press_e) {
        float zoomFactor = 1.0f + renderer.delta_z * time_elapsed; // Un facteur doux pour le zoom
        int mouseX = renderer.get_mouse_current_x();
        int mouseY = renderer.get_mouse_current_y();

        // Centre de l'écran
        float centerX = ofGetWidth() / 2.0f;
        float centerY = ofGetHeight() / 2.0f;

        // Déplacer la souris proportionnellement au zoom autour du centre
        mouseX = centerX + (mouseX - centerX) * zoomFactor;
        mouseY = centerY + (mouseY - centerY) * zoomFactor;

        renderer.set_mouse_current_x(mouseX);
        renderer.set_mouse_current_y(mouseY);
    }


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

    if (renderer.graph.geometrie.is_bezier_curve) {
        if (renderer.graph.geometrie.is_dragging && renderer.graph.geometrie.selected_i >= 0 && renderer.graph.geometrie.selected_j >= 0)
        {
            float z = renderer.graph.geometrie.control_grid[renderer.graph.geometrie.selected_i][renderer.graph.geometrie.selected_j].z;
            ofVec3f corrected_mouse = renderer.screenToScene(x - 300, y - 24); // Pas besoin de recalcul compliqué si la caméra n'est plus inversée

            renderer.graph.geometrie.control_grid[renderer.graph.geometrie.selected_i][renderer.graph.geometrie.selected_j] = corrected_mouse;
            renderer.graph.geometrie.update_mesh();
        }
    }
}


//--------------------------------------------------------------
void Application::mousePressed(int x, int y, int button) {
    renderer.set_is_mouse_button_pressed(true);

    renderer.set_mouse_current_x(x);
    renderer.set_mouse_current_y(y);
    renderer.set_mouse_press_x(x);
    renderer.set_mouse_press_y(y);

    if (button == 0 && renderer.is_mouse_in_draw_area() && guiManager.get_type_vector_primitive() == 20) {

        ofVec3f corrected_mouse = renderer.screenToScene(x - 300, y - 24);
        corrected_mouse.z = 0;

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j)
            {
                renderer.graph.geometrie.control_grid[i][j].x = corrected_mouse.x + (i - 1.5f) * 100;
                renderer.graph.geometrie.control_grid[i][j].y = corrected_mouse.y + (j - 1.5f) * 100;
                renderer.graph.geometrie.control_grid[i][j].z = corrected_mouse.z;
            }
        }

        renderer.graph.geometrie.update_mesh();
        renderer.graph.geometrie.is_bezier_curve = true;
    }
}

//--------------------------------------------------------------
void Application::mouseReleased(int x, int y, int button){
    renderer.set_is_mouse_button_pressed(false);

    renderer.set_mouse_current_x(x);
    renderer.set_mouse_current_y(y);
    renderer.mouseReleased();

    if (renderer.graph.geometrie.is_bezier_curve) {
        renderer.graph.geometrie.is_dragging = false;
        renderer.graph.geometrie.selected_i = renderer.graph.geometrie.selected_j = -1;
    }
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
void Application::windowResized(int w, int h) {
    renderer.camera.setPosition(0, 0, 600); // on repositionne proprement
    renderer.camera.lookAt(ofVec3f(0, 0, 0));
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
