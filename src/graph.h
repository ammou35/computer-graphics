#pragma once
#include "2D\dessinVectoriel.h"
#include "3D\geometrie.h"

// énumération des différents types de primitives vectorielles
enum class ElementScene2DType { none, pixel, point, line, rectangle, ellipse, param_curve, polygone, star, heart, moon, arrow, clover, image };

struct ElementScene2D {
	std::array<VectorPrimitive*, 4> primitives;
	ElementScene2DType type;
	char name[20];
	bool is_selected;
	int image_nb;
	float image_position[2];
	std::array<float, 6> transformation; // Translation(2), rotation(1), proportion(2)
};

class Graph {
public:
	vector<ofImage> images;

	DessinVectoriel dessinVectoriel;
	Geometrie geometrie;

	ElementScene2D* element2D;
	ElementScene3D* element3D;

	int buffer_count;
	// Buffer 2D
	int element2D_buffer_head;
	int element2D_buffer_stride;
	int element2D_buffer_size;
	// Buffer 3D
	int element3D_buffer_head;
	int element3D_buffer_stride;
	int element3D_buffer_size;

	void draw(const std::array<int, 2> &mouse_press, const std::array<int, 2> &mouse_current, const bool &is_mouse_button_pressed);
	void update(const ofColor &stroke_color, const ofColor &fill_color, const int &type_vector_primitive, const std::array<float, 6> &transformation, const std::array<float, 9>& transformation3D);
	void setup();
	void add_element(const std::array<int, 2> &mouse_press, const std::array<int, 2> &mouse_current);
	void add_element2D(const std::array<int, 2> &mouse_press, const std::array<int, 2> &mouse_current);
	void add_element3D(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current);

	ElementScene2DType Graph::get_draw_shape_2D(void) const;
	void set_draw_shape(const int& ElementSceneType);
	void Graph::delete_element2D(void);

	ElementScene3DType Graph::get_draw_shape_3D(void) const;
	void set_draw_shape_3D(const int& ElementScene3DType);
	void Graph::delete_element3D(void);

	void load_image(string path);

	bool bounding_box;
	int image_nb;

	~Graph();
private:
	int draw_mode;
	ElementScene2DType draw_shape_2D;
	ElementScene3DType draw_shape_3D;
	std::vector<int> free_indices;
};