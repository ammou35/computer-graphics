#include "dessinVectoriel.h"

void DessinVectoriel::setup() {

	// nombre maximal de primitives vectorielles dans le tableau
	buffer_count = 1000;

	// position de la prochaine primitive
	buffer_head = 0;

	// calculer la taille de la structure générique commune à toutes les primitives vectorielles
	buffer_stride = sizeof(VectorPrimitive);

	// calculer le nombre d'octets à allouer en mémoire pour contenir le tableau de primitives vectorielles
	buffer_size = buffer_count * buffer_stride;

	// allocation d'un espace mémoire suffisamment grand pour contenir les données de l'ensemble des primitives vectorielles
	shapes = (VectorPrimitive*)std::malloc(buffer_size);

    for (int i = 0; i < buffer_count; i++) {
        shapes[i].type = VectorPrimitiveType::none;
        shapes[i].vertex_start_index = -1;
        shapes[i].vertex_count = 0;
        shapes[i].transformation = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f };
    }

    reset_poly_points();

    param_curve_buffer_head = 0;

    reset_param_curve_buffer();

    init_poly();

	// largeur de la ligne de contour
	stroke_width_default = 2;

	// couleur de la ligne de contour
	random_color_stroke();

	// couleur de la zone de remplissage
	random_color_fill();

	radius = 4.0f;
}

void DessinVectoriel::update(const ofColor &stroke_color, const ofColor& fill_color, const int& type_vector_primitive) {
    set_stroke_color(stroke_color);
    set_fill_color(fill_color);
    set_draw_mode(type_vector_primitive);
}

void DessinVectoriel::draw(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current, const bool &is_mouse_button_pressed) {
    for (index = 0; index < buffer_count; ++index)
    {
        ofPushMatrix();
        ofTranslate(shapes[index].center[0], shapes[index].center[1]);
        ofTranslate(shapes[index].transformation[0], shapes[index].transformation[1], shapes[index].transformation[2]);
        ofRotateDeg(shapes[index].transformation[3]);
        ofScale(shapes[index].transformation[4], shapes[index].transformation[5]);
        ofTranslate(-shapes[index].center[0], -shapes[index].center[1]);
        switch (shapes[index].type)
        {
        case VectorPrimitiveType::pixel:

            ofFill();
            ofSetLineWidth(0);
            ofSetColor(
                shapes[index].stroke_color[0],
                shapes[index].stroke_color[1],
                shapes[index].stroke_color[2]);
            draw_pixel(
                shapes[index].position2[0],
                shapes[index].position2[1]);
            break;

        case VectorPrimitiveType::point:

            ofFill();
            ofSetLineWidth(0);
            ofSetColor(
                shapes[index].stroke_color[0],
                shapes[index].stroke_color[1],
                shapes[index].stroke_color[2]);
            draw_point(
                shapes[index].position2[0],
                shapes[index].position2[1],
                shapes[index].stroke_width);
            break;

        case VectorPrimitiveType::line:

            ofNoFill();
            ofSetLineWidth(shapes[index].stroke_width);
            ofSetColor(
                shapes[index].stroke_color[0],
                shapes[index].stroke_color[1],
                shapes[index].stroke_color[2]);
            draw_line(
                shapes[index].position1[0],
                shapes[index].position1[1],
                shapes[index].position2[0],
                shapes[index].position2[1]);
            break;

        case VectorPrimitiveType::rectangle:

            ofFill();
            ofSetLineWidth(0);
            ofSetColor(
                shapes[index].fill_color[0],
                shapes[index].fill_color[1],
                shapes[index].fill_color[2]);
            draw_rectangle(
                shapes[index].position1[0],
                shapes[index].position1[1],
                shapes[index].position2[0],
                shapes[index].position2[1]);
            ofNoFill();
            ofSetLineWidth(shapes[index].stroke_width);
            ofSetColor(
                shapes[index].stroke_color[0],
                shapes[index].stroke_color[1],
                shapes[index].stroke_color[2]);
            draw_rectangle(
                shapes[index].position1[0],
                shapes[index].position1[1],
                shapes[index].position2[0],
                shapes[index].position2[1]);
            break;

        case VectorPrimitiveType::ellipse:

            ofFill();
            ofSetLineWidth(0);
            ofSetCircleResolution(48);
            ofSetColor(
                shapes[index].fill_color[0],
                shapes[index].fill_color[1],
                shapes[index].fill_color[2]);
            draw_ellipse(
                shapes[index].position1[0],
                shapes[index].position1[1],
                shapes[index].position2[0],
                shapes[index].position2[1]);
            ofNoFill();
            ofSetLineWidth(shapes[index].stroke_width);
            ofSetColor(
                shapes[index].stroke_color[0],
                shapes[index].stroke_color[1],
                shapes[index].stroke_color[2]);
            draw_ellipse(
                shapes[index].position1[0],
                shapes[index].position1[1],
                shapes[index].position2[0],
                shapes[index].position2[1]);
            break;

        case VectorPrimitiveType::param_curve:
            ofNoFill();
            ofSetLineWidth(shapes[index].stroke_width);
            ofSetColor(
                shapes[index].stroke_color[0],
                shapes[index].stroke_color[1],
                shapes[index].stroke_color[2]);
            draw_curve(
                shapes[index].position1[0],
                shapes[index].position1[1],
                shapes[index].position2[0],
                shapes[index].position2[1],
                shapes[index].position3[0],
                shapes[index].position3[1],
                shapes[index].position4[0],
                shapes[index].position4[1]);
            break;

        case VectorPrimitiveType::polygone:
            draw_polygone(
                shapes[index].vertex_start_index,
                shapes[index].vertex_count,
                ofColor(shapes[index].fill_color[0],
                        shapes[index].fill_color[1],
                        shapes[index].fill_color[2]),
                ofColor(shapes[index].stroke_color[0],
                        shapes[index].stroke_color[1],
                        shapes[index].stroke_color[2]));
            break;

        default:
            break;
        }
        ofPopMatrix();
    }

    // afficher la zone de sélection
    if (is_mouse_button_pressed) {
        if (get_draw_mode() == VectorPrimitiveType::rectangle || get_draw_mode() == VectorPrimitiveType::ellipse)
        {
            draw_zone(
                mouse_press[0],
                mouse_press[1],
                mouse_current[0],
                mouse_current[1]);
        }
    }

    if (get_draw_mode() == VectorPrimitiveType::param_curve) {
        for (int i = 0; i < get_param_curve_buffer_head(); i += 2) {
            ofFill();
            ofSetLineWidth(0);
            ofSetColor(73, 70, 176);
            draw_point(get_param_curve_buffer()[i], get_param_curve_buffer()[i+1], 10);
        }
    } else if (get_draw_mode() == VectorPrimitiveType::polygone) {
        for (int i = 0; i < get_poly_points(); i += 2) {
            ofFill();
            ofSetLineWidth(0);
            ofSetColor(73, 70, 176);
            draw_point(get_poly_pool()[get_poly_pool_head() - get_poly_points() + i],
                       get_poly_pool()[get_poly_pool_head() - get_poly_points() + i + 1], 10);
        }
    }
}

void DessinVectoriel::set_stroke_color(const ofColor& color) {
    stroke_color = color;
}

void DessinVectoriel::set_fill_color(const ofColor& color) {
    fill_color = color;
}

void DessinVectoriel::set_draw_mode(const int& typeVectorPrimitive) {
    switch (typeVectorPrimitive) {
        case -1: draw_mode = VectorPrimitiveType::none; break;
        case 1:  draw_mode = VectorPrimitiveType::pixel; break;
        case 2:  draw_mode = VectorPrimitiveType::point; break;
        case 3:  draw_mode = VectorPrimitiveType::line; break;
        case 4:  draw_mode = VectorPrimitiveType::rectangle; break;
        case 5:  draw_mode = VectorPrimitiveType::ellipse; break;
        case 6:  draw_mode = VectorPrimitiveType::param_curve; break;
        case 7:  draw_mode = VectorPrimitiveType::polygone; break;
        default: draw_mode = VectorPrimitiveType::none; break;
    }
}

VectorPrimitiveType DessinVectoriel::get_draw_mode(void) const {
    return draw_mode;
}

// fonction qui vide le tableau de primitives vectorielles
void DessinVectoriel::reset()
{
    for (index = 0; index < buffer_count; ++index)
        shapes[index].type = VectorPrimitiveType::none;

    buffer_head = 0;

    ofLog() << "<reset>";
}

// fonction qui ajoute une primitive vectorielle au tableau
std::array<VectorPrimitive*, 4> DessinVectoriel::add_vector_shape(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current)
{
    int index;

    if (!free_indices.empty()) {
        index = free_indices.back();
        free_indices.pop_back(); // Réutilisation d’un ancien index
    }
    else {
        index = buffer_head;
        buffer_head = (buffer_head + 1) % buffer_count;
    }

    std::array<VectorPrimitive*, 4> primitives = { nullptr, nullptr, nullptr , nullptr};
    shapes[index].type = get_draw_mode();

    if (get_draw_mode() == VectorPrimitiveType::param_curve) {
        shapes[index].position1[0] = get_param_curve_buffer()[0];
        shapes[index].position1[1] = get_param_curve_buffer()[1];

        shapes[index].position2[0] = get_param_curve_buffer()[2];
        shapes[index].position2[1] = get_param_curve_buffer()[3];

        shapes[index].position3[0] = get_param_curve_buffer()[4];
        shapes[index].position3[1] = get_param_curve_buffer()[5];

        shapes[index].position4[0] = get_param_curve_buffer()[6];
        shapes[index].position4[1] = get_param_curve_buffer()[7];
        
        shapes[index].center[0] = get_param_curve_buffer()[6];
        shapes[index].center[1] = get_param_curve_buffer()[7];
    } else if(get_draw_mode() == VectorPrimitiveType::polygone) {
        shapes[index].position1[0] = NULL;
        shapes[index].position1[1] = NULL;

        shapes[index].position2[0] = NULL;
        shapes[index].position2[1] = NULL;

        shapes[index].position3[0] = NULL;
        shapes[index].position3[1] = NULL;

        shapes[index].position4[0] = NULL;
        shapes[index].position4[1] = NULL;
        shapes[index].vertex_start_index = get_poly_pool_head() - get_poly_points();
        shapes[index].vertex_count = get_poly_points();

        shapes[index].center[0] = get_poly_pool()[get_poly_pool_head() - get_poly_points()];
        shapes[index].center[1] = get_poly_pool()[get_poly_pool_head() - get_poly_points() + 1];
    } else {
        shapes[index].position1[0] = mouse_press[0];
        shapes[index].position1[1] = mouse_press[1];

        shapes[index].position2[0] = mouse_current[0];
        shapes[index].position2[1] = mouse_current[1];

        shapes[index].position3[0] = NULL;
        shapes[index].position3[1] = NULL;

        shapes[index].position4[0] = NULL;
        shapes[index].position4[1] = NULL;

        shapes[index].center[0] = (mouse_current[0] + mouse_press[0])/2;
        shapes[index].center[1] = (mouse_current[1] + mouse_press[1])/2;
    }

    shapes[index].stroke_color[0] = stroke_color.r;
    shapes[index].stroke_color[1] = stroke_color.g;
    shapes[index].stroke_color[2] = stroke_color.b;
    shapes[index].stroke_color[3] = stroke_color.a;

    shapes[index].fill_color[0] = fill_color.r;
    shapes[index].fill_color[1] = fill_color.g;
    shapes[index].fill_color[2] = fill_color.b;
    shapes[index].fill_color[3] = fill_color.a;

    switch (shapes[index].type)
    {
    case VectorPrimitiveType::point:
        shapes[index].stroke_width = ofRandom(1, 64);
        break;

    case VectorPrimitiveType::line:
        shapes[index].stroke_width = ofRandom(1, 16);
        break;

    case VectorPrimitiveType::rectangle:
        shapes[index].stroke_width = stroke_width_default;
        break;

    case VectorPrimitiveType::ellipse:
        shapes[index].stroke_width = stroke_width_default;
        break;

    case VectorPrimitiveType::param_curve:
        shapes[index].stroke_width = stroke_width_default;
        break;

    case VectorPrimitiveType::polygone:
        shapes[index].stroke_width = stroke_width_default;
        break;

    default:
        shapes[index].stroke_width = stroke_width_default;
        break;
    }

    ofLog() << "<new primitive at index: " << index << ">";

    primitives[0] = &shapes[index];
    return primitives;
}

void DessinVectoriel::delete_vector_shape(VectorPrimitive* ptr) {
    if (ptr < shapes || ptr >= shapes + buffer_count) return;

    int index = ptr - shapes;
    shapes[index].type = VectorPrimitiveType::none;
    free_indices.push_back(index);
}


// fonction qui dessine un pixel
void DessinVectoriel::draw_pixel(float x, float y) const
{
    int pixel_x = floorf(x);
    int pixel_y = floorf(y);

    ofDrawRectangle(pixel_x, pixel_y, 1, 1);
}

// fonction qui dessine un point
void DessinVectoriel::draw_point(float x, float y, float radius) const
{
    ofDrawEllipse(x, y, radius, radius);
}

// fonction qui dessine une ligne
void DessinVectoriel::draw_line(float x1, float y1, float x2, float y2) const
{
    ofDrawLine(x1, y1, x2, y2);
}

// fonction qui dessine un rectangle
void DessinVectoriel::draw_rectangle(float x1, float y1, float x2, float y2) const
{
    ofDrawRectangle(x1, y1, x2 - x1, y2 - y1);
}

// fonction qui dessine une ellipse
void DessinVectoriel::draw_ellipse(float x1, float y1, float x2, float y2) const
{
    float diameter_x = x2 - x1;
    float diameter_y = y2 - y1;

    ofDrawEllipse(x1 + diameter_x / 2.0f, y1 + diameter_y / 2.0f, diameter_x, diameter_y);
}

// fonction qui dessine une curve
void DessinVectoriel::draw_curve(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) const {
    ofPolyline bezier;
    bezier.addVertex(x1, y1);
    bezier.bezierTo(x2, y2, x3, y3, x4, y4);
    bezier.draw();
}

void DessinVectoriel::draw_polygone(const int& vertex_start_index, const int& vertex_count, 
                                    const ofColor& fill_color, const ofColor& outline_color) const {
    ofPath polygone;
    polygone.moveTo(get_poly_pool()[vertex_start_index], get_poly_pool()[vertex_start_index + 1]);
    for (int i = 0; i < vertex_count; i += 2) {
        polygone.lineTo(get_poly_pool()[vertex_start_index + i], get_poly_pool()[vertex_start_index + i + 1]);
    }
    polygone.close();
    polygone.setFillColor(fill_color);
    polygone.setFilled(true);
    polygone.setStrokeColor(outline_color);
    polygone.setStrokeWidth(2);
    polygone.draw();
}

// fonction qui détermine une couleur aléatoire pour les lignes de contour
void DessinVectoriel::random_color_stroke()
{
    stroke_color = ((int)ofRandom(0, 255), (int)ofRandom(0, 255), (int)ofRandom(0, 255), 255);
    ofLog() << "<random color stroke>";
}

// fonction qui détermine une couleur aléatoire pour les zones de remplissage
void DessinVectoriel::random_color_fill()
{
    fill_color = ((int)ofRandom(0, 255), (int)ofRandom(0, 255), (int)ofRandom(0, 255), 255);
    ofLog() << "<random color fill>";
}

void DessinVectoriel::draw_zone(float x1, float y1, float x2, float y2) const
{
    float x2_clamp = min(max(0.0f, x2), (float)ofGetWidth());
    float y2_clamp = min(max(0.0f, y2), (float)ofGetHeight());

    ofSetLineWidth(radius);
    ofSetColor(255);
    ofNoFill();
    ofDrawRectangle(x1, y1, x2_clamp - x1, y2_clamp - y1);
    ofFill();
    ofDrawEllipse(x1, y1, radius, radius);
    ofDrawEllipse(x1, y2_clamp, radius, radius);
    ofDrawEllipse(x2_clamp, y1, radius, radius);
    ofDrawEllipse(x2_clamp, y2_clamp, radius, radius);
}

std::array<VectorPrimitive*, 4> DessinVectoriel::addBezierPoint(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current) {
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0];
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1];
    increment_param_curve_buffer_head();
    if (get_param_curve_buffer_head() > 7) {
        std::array<VectorPrimitive*, 4> primitives;
        primitives = add_vector_shape(mouse_press, mouse_current);
        reset_param_curve_buffer();
        return primitives;
    }
    return {nullptr, nullptr, nullptr, nullptr};
}

float* DessinVectoriel::get_param_curve_buffer(void) const {
    return param_curve_buffer;
}
int DessinVectoriel::get_param_curve_buffer_head(void) const {
    return param_curve_buffer_head;
}

void DessinVectoriel::increment_param_curve_buffer_head(void) {
    param_curve_buffer_head += 2;
}

void DessinVectoriel::reset_param_curve_buffer(void) {
    if (param_curve_buffer_head == 0) {
        param_curve_buffer = (float*)std::malloc(8);
    }
    param_curve_buffer_head = 0;
}

void DessinVectoriel::init_poly(void) {
    max_point_per_poly = 20;
    max_poly = 200;
    poly_pool_len = max_point_per_poly * max_poly * 2;
    poly_pool = (float*)std::malloc(poly_pool_len);
    poly_pool_head = 0;
}

std::array<VectorPrimitive*, 4> DessinVectoriel::addPolyPoint(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current) {
    if (get_param_curve_buffer_head() > 100 || (get_poly_points() > 2) &&
        is_point1_close_to_point2(get_poly_pool()[get_poly_pool_head() - get_poly_points()], 
                                  get_poly_pool()[get_poly_pool_head() - get_poly_points() + 1], 
                                  mouse_current[0], mouse_current[1])) {
        std::array<VectorPrimitive*, 4> primitives = add_vector_shape(mouse_press, mouse_current);
        reset_poly_points();
        return primitives;
    } else {
        get_poly_pool()[get_poly_pool_head()] = mouse_current[0];
        get_poly_pool()[get_poly_pool_head() + 1] = mouse_current[1];
        increment_poly();
        return { nullptr, nullptr, nullptr, nullptr };
    }
}

float* DessinVectoriel::get_poly_pool(void) const {
    return poly_pool;
}


int DessinVectoriel::get_poly_points(void) const {
    return poly_points;
}

void DessinVectoriel::increment_poly(void) {
    poly_points += 2;
    poly_pool_head += 2;
}

int DessinVectoriel::get_poly_pool_head(void) const {
    return poly_pool_head;
}

void DessinVectoriel::reset_poly_points(void) {
    poly_points = 0;
}

bool DessinVectoriel::is_point1_close_to_point2(float x1, float y1, float x2, float y2) const {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float threshold = 15.0f;
    return dx * dx + dy * dy <= threshold * threshold;
}

std::array<VectorPrimitive*, 4> DessinVectoriel::add_star(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current) {
    get_poly_pool()[get_poly_pool_head()] = mouse_current[0] + shape_scale * 0;
    get_poly_pool()[get_poly_pool_head() + 1] = mouse_current[1] + shape_scale * -1;
    increment_poly();
    get_poly_pool()[get_poly_pool_head()] = mouse_current[0] + shape_scale * -0.22;
    get_poly_pool()[get_poly_pool_head() + 1] = mouse_current[1] + shape_scale * -0.3;
    increment_poly();
    get_poly_pool()[get_poly_pool_head()] = mouse_current[0] + shape_scale * -0.95;
    get_poly_pool()[get_poly_pool_head() + 1] = mouse_current[1] + shape_scale * -0.3;
    increment_poly();
    get_poly_pool()[get_poly_pool_head()] = mouse_current[0] + shape_scale * -0.36;
    get_poly_pool()[get_poly_pool_head() + 1] = mouse_current[1] + shape_scale * 0.11;
    increment_poly();
    get_poly_pool()[get_poly_pool_head()] = mouse_current[0] + shape_scale * -0.585;
    get_poly_pool()[get_poly_pool_head() + 1] = mouse_current[1] + shape_scale * 0.8;
    increment_poly();
    get_poly_pool()[get_poly_pool_head()] = mouse_current[0] + shape_scale * 0;
    get_poly_pool()[get_poly_pool_head() + 1] = mouse_current[1] + shape_scale * 0.38;
    increment_poly();
    get_poly_pool()[get_poly_pool_head()] = mouse_current[0] + shape_scale * 0.585;
    get_poly_pool()[get_poly_pool_head() + 1] = mouse_current[1] + shape_scale * 0.81;
    increment_poly();
    get_poly_pool()[get_poly_pool_head()] = mouse_current[0] + shape_scale * 0.36;
    get_poly_pool()[get_poly_pool_head() + 1] = mouse_current[1] + shape_scale * 0.11;
    increment_poly();
    get_poly_pool()[get_poly_pool_head()] = mouse_current[0] + shape_scale * 0.95;
    get_poly_pool()[get_poly_pool_head() + 1] = mouse_current[1] + shape_scale * -0.3;
    increment_poly();
    get_poly_pool()[get_poly_pool_head()] = mouse_current[0] + shape_scale * 0.22;
    get_poly_pool()[get_poly_pool_head() + 1] = mouse_current[1] + shape_scale * -0.3;
    increment_poly();
    get_poly_pool()[get_poly_pool_head()] = mouse_current[0] + shape_scale * 0;
    get_poly_pool()[get_poly_pool_head() + 1] = mouse_current[1] + shape_scale * -1;
    increment_poly();
    std::array<VectorPrimitive*, 4> primitives = add_vector_shape(mouse_press, mouse_current);
    primitives[0]->center[0] = mouse_current[0];
    primitives[0]->center[1] = mouse_current[1];
    reset_poly_points();
    return primitives;
}

std::array<VectorPrimitive*, 4> DessinVectoriel::add_heart(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current) {
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0];
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1];
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0];
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] - 30;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] - 50;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] - 30;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] - 50;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1];
    increment_param_curve_buffer_head();
    std::array<VectorPrimitive*, 4> primitives = add_vector_shape(mouse_press, mouse_current);
    reset_param_curve_buffer();

    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] - 50;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1];
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] - 50;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + 30;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0];
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + 35;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0];
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + 60;
    increment_param_curve_buffer_head();
    primitives[1] = add_vector_shape(mouse_press, mouse_current)[0];
    reset_param_curve_buffer();

    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0];
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + 60;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0];
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + 35;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + 50;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + 30;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + 50;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1];
    increment_param_curve_buffer_head();
    primitives[2] = add_vector_shape(mouse_press, mouse_current)[0];
    reset_param_curve_buffer();

    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + 50;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1];
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + 50;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] - 30;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0];
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] - 30;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0];
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1];
    increment_param_curve_buffer_head();
    primitives[3] = add_vector_shape(mouse_press, mouse_current)[0];
    reset_param_curve_buffer();

    for (int i = 0; i < 4; i++) {
        primitives[i]->center[0] = mouse_current[0];
        primitives[i]->center[1] = mouse_current[1];
    }

    return primitives;
}

std::array<VectorPrimitive*, 4> DessinVectoriel::add_moon(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current) {
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * -2;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * 2;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * 0.5;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * 2.5;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * 2.5;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * 0.5;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * 2;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * -2;
    increment_param_curve_buffer_head();
    std::array<VectorPrimitive*, 4> primitives = add_vector_shape(mouse_press, mouse_current);
    reset_param_curve_buffer();

    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * -2;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * 2;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * 0;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * 2;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * 2;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * 0;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * 2;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * -2;
    increment_param_curve_buffer_head();
    primitives[1] = add_vector_shape(mouse_press, mouse_current)[0];
    reset_param_curve_buffer();

    for (int i = 0; i < 2; i++) {
        primitives[i]->center[0] = mouse_current[0];
        primitives[i]->center[1] = mouse_current[1];
    }

    return primitives;
}

std::array<VectorPrimitive*, 4> DessinVectoriel::add_arrow(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current) {
    get_poly_pool()[get_poly_pool_head()] = mouse_current[0] + shape_scale * 3;
    get_poly_pool()[get_poly_pool_head() + 1] = mouse_current[1] + shape_scale * 0;
    increment_poly();
    get_poly_pool()[get_poly_pool_head()] = mouse_current[0] + shape_scale * 1;
    get_poly_pool()[get_poly_pool_head() + 1] = mouse_current[1] + shape_scale * -2;
    increment_poly();
    get_poly_pool()[get_poly_pool_head()] = mouse_current[0] + shape_scale * 1;
    get_poly_pool()[get_poly_pool_head() + 1] = mouse_current[1] + shape_scale * -1;
    increment_poly();
    get_poly_pool()[get_poly_pool_head()] = mouse_current[0] + shape_scale * -2;
    get_poly_pool()[get_poly_pool_head() + 1] = mouse_current[1] + shape_scale * -1;
    increment_poly();
    get_poly_pool()[get_poly_pool_head()] = mouse_current[0] + shape_scale * -2;
    get_poly_pool()[get_poly_pool_head() + 1] = mouse_current[1] + shape_scale * 1;
    increment_poly();
    get_poly_pool()[get_poly_pool_head()] = mouse_current[0] + shape_scale * 1;
    get_poly_pool()[get_poly_pool_head() + 1] = mouse_current[1] + shape_scale * 1;
    increment_poly();
    get_poly_pool()[get_poly_pool_head()] = mouse_current[0] + shape_scale * 1;
    get_poly_pool()[get_poly_pool_head() + 1] = mouse_current[1] + shape_scale * 2;
    increment_poly();
    get_poly_pool()[get_poly_pool_head()] = mouse_current[0] + shape_scale * 3;
    get_poly_pool()[get_poly_pool_head() + 1] = mouse_current[1] + shape_scale * 0;
    increment_poly();
    std::array<VectorPrimitive*, 4> primitives = add_vector_shape(mouse_press, mouse_current);
    reset_poly_points();

    primitives[0]->center[0] = mouse_current[0];
    primitives[0]->center[1] = mouse_current[1];

    return primitives;
}

std::array<VectorPrimitive*, 4> DessinVectoriel::add_clover(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current) {
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * 1;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * 1;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * 4;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * 2;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * 4;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * -2;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * 1;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * -1;
    increment_param_curve_buffer_head();
    std::array<VectorPrimitive*, 4> primitives = add_vector_shape(mouse_press, mouse_current);
    reset_param_curve_buffer();

    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * -1;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * 1;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * -2;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * 4;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * 2;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * 4;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * 1;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * 1;
    increment_param_curve_buffer_head();
    primitives[1] = add_vector_shape(mouse_press, mouse_current)[0];
    reset_param_curve_buffer();

    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * -1;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * 1;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * -4;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * 2;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * -4;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * -2;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * -1;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * -1;
    increment_param_curve_buffer_head();
    primitives[2] = add_vector_shape(mouse_press, mouse_current)[0];
    reset_param_curve_buffer();

    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * -1;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * -1;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * -2;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * -4;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * 2;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * -4;
    increment_param_curve_buffer_head();
    param_curve_buffer[get_param_curve_buffer_head()] = mouse_current[0] + shape_scale * 1;
    param_curve_buffer[get_param_curve_buffer_head() + 1] = mouse_current[1] + shape_scale * -1;
    increment_param_curve_buffer_head();
    primitives[3] = add_vector_shape(mouse_press, mouse_current)[0];
    reset_param_curve_buffer();

    for (int i = 0; i < 4; i++) {
        primitives[i]->center[0] = mouse_current[0];
        primitives[i]->center[1] = mouse_current[1];
    }

    return primitives;
}

void DessinVectoriel::apply_transform(VectorPrimitive* ptr, const std::array<float, 6>& transformation) {
    if (ptr < shapes || ptr >= shapes + buffer_count) return;

    int index = ptr - shapes;
    shapes[index].transformation = transformation;
}

DessinVectoriel::~DessinVectoriel()
{
    std::free(shapes);
    std::free(poly_pool);
}