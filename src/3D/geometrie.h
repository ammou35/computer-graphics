#pragma once
// IFT-3100-H2025/src/3D/geometrie.h
// Classe responsable du rendu 3D.

#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

enum class ElementScene3DType { none, cube, sphere, cylinder, cone, donut, plate, spaghetti_getter, bezier_curve};
enum class ElementScene3DMaterial { none, volcanicRock, frozenCrystal, mossyStone, neonTech, ancientBronze };

struct ElementScene3D {
	ElementScene3DType type;
	std::array<float, 9> transformation; // Translation(3), rotation(3), proportion(3)
	bool is_selected;
	bool bounding_box;
	//texture
	ElementScene3DMaterial material;
};

class Geometrie
{
public:
	ofLight light;
	ofLight light_point;

	ofxAssimpModelLoader donut;
	ofxAssimpModelLoader plate;
	ofxAssimpModelLoader spaghetti_getter;

	ofMaterial material_VolcanicRock;
	ofMaterial material_FrozenCrystal;
	ofMaterial material_MossyStone;
	ofMaterial material_NeonTech;
	ofMaterial material_AncientBronze;
	ofMaterial material_None;

	// shaders ?
	// 	ofShader shader;
	// ofShader shader_lambert;
	// ofShader shader_normal;
	// 
	// ofColor color_ambient;
	// ofColor color_diffuse;

	// Bézier bicubique
	ofVec3f control_grid[4][4];
	int resolution_u = 20;
	int resolution_v = 20;
	ofMesh mesh;

	bool is_dragging;
	int selected_i;
	int selected_j;
	float radius;
	bool is_bezier_curve;

	bool projection_mode = false; // projection orthogonale

	float scale;

	float rotation_speed; // not sure

	bool use_rotation;

	void setup();
	void update();
	void draw();

	void draw_cube(ofMaterial material);   // Fonction pour ajouter un cube
	void draw_sphere() const; // Fonction pour ajouter une sphère
	void draw_cylinder() const; // Fonction pour ajouter un cylindre
	void draw_cone() const; // Fonction pour ajouter un cône

	void draw_donut(); // Fonction pour ajouter un donut
	void draw_plate(); // Fonction pour ajouter une assiette
	void draw_spaghetti_getter(); // Fonction pour ajouter
	void draw_bezier_curve();

	void draw_bounding_box() const;
	void set_projection_mode(bool mode);

	void bezier_bicubic(float u, float v, const ofVec3f control_points[4][4], float& x, float& y, float& z);
	void update_mesh();
};
