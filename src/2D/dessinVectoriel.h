#pragma once
#include "ofMain.h"

// énumération des différents types de primitives vectorielles
enum class VectorPrimitiveType {none, pixel, point, line, rectangle, ellipse, param_curve, polygone};

// structure de primitive vectorielle générique
struct VectorPrimitive
{
	VectorPrimitiveType type;
	float				center[2];
	float               position1[2];
	float               position2[2];
	float               position3[2];
	float               position4[2];
	float               stroke_width;
	unsigned char       stroke_color[4];
	unsigned char       fill_color[4];
	std::array<float, 6> transformation; // Translation(2), rotation(1), proportion(2)

	int vertex_start_index;
	int vertex_count;
};

class DessinVectoriel {
public :

	VectorPrimitive* shapes;

	int index;

	int buffer_count;
	int buffer_stride;
	int buffer_size;
	int buffer_head;
	std::vector<int> free_indices;

	float stroke_width_default;
	float radius;

	ofColor stroke_color;
	ofColor fill_color;

	int shape_scale = 10;

	void setup();
	void update(const ofColor& stroke_color, const ofColor& fill_color, const int& type_vector_primitive);
	void draw(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current, const bool& is_mouse_button_pressed);
	std::array<VectorPrimitive*, 4> add_vector_shape(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current);
	void delete_vector_shape(VectorPrimitive* ptr);

	void draw_pixel(float x, float y) const;
	void draw_point(float x, float y, float radius) const;
	void draw_line(float x1, float y1, float x2, float y2) const;
	void draw_rectangle(float x1, float y1, float x2, float y2) const;
	void draw_ellipse(float x1, float y1, float x2, float y2) const;
	void draw_curve(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) const;
	void DessinVectoriel::draw_polygone(const int& vertex_start_index, const int& vertex_count,
	const ofColor& fill_color, const ofColor& outline_color) const;

	std::array<VectorPrimitive*, 4> add_star(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current);
	std::array<VectorPrimitive*, 4> add_heart(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current);
	std::array<VectorPrimitive*, 4> add_moon(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current);
	std::array<VectorPrimitive*, 4> add_arrow(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current);
	std::array<VectorPrimitive*, 4> add_clover(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current);

	void random_color_stroke();
	void random_color_fill();
	void set_stroke_color(const ofColor& color);
	void set_fill_color(const ofColor& color);

	void draw_zone(float x1, float y1, float x2, float y2) const;

	void reset();

	void set_draw_mode(const int& typeVectorPrimitive);
	VectorPrimitiveType get_draw_mode(void) const;

	std::array<VectorPrimitive*, 4> addBezierPoint(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current);
	float* get_param_curve_buffer(void) const;
	int get_param_curve_buffer_head(void) const;
	void increment_param_curve_buffer_head(void);
	void reset_param_curve_buffer(void);

	void init_poly(void);
	std::array<VectorPrimitive*, 4> addPolyPoint(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current);
	float* get_poly_pool(void) const;
	int get_poly_points(void) const;
	int get_poly_pool_head(void) const;
	void increment_poly(void);
	void reset_poly_points(void);

	bool is_point1_close_to_point2(float x1, float y1, float x2, float y2) const;

	void apply_transform(VectorPrimitive* ptr, const std::array<float, 6>& transformation);

	~DessinVectoriel();

	ofVec2f mouse_press_w;
	ofVec2f mouse_current_w;
private:
	VectorPrimitiveType draw_mode;
	float* param_curve_buffer;
	int param_curve_buffer_head;

	int poly_points;
	float* poly_pool;
	int poly_pool_head;
	int max_point_per_poly;
	int max_poly;
	int poly_pool_len;
};