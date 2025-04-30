#pragma once
// IFT-3100-H2025/src/3D/geometrie.h
// Classe responsable du rendu 3D.

#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "skybox.h"


enum class ElementScene3DType { none, cube, sphere, cylinder, cone, donut, plate, spaghetti_getter, bezier_curve, ambiant, directional_light, point_light, spot_light };
enum class ElementScene3DMaterial { none, volcanicRock, frozenCrystal, mossyStone, neonTech, ancientBronze };
enum class ElementScene3DTexture { none, wood, sand, briks, honeycomb, sponge };

struct LightAttribute {
	ofLight light;
	ElementScene3DType type;
	ofVec3f diffuseColor;
	ofVec3f specularColor;
	ofVec3f position;
	ofVec3f orientation;
	float concentration;
	float lightCutOff;
};

struct ElementScene3D {
	ElementScene3DType type;
	std::array<float, 9> transformation; // Translation(3), rotation(3), proportion(3)
	bool is_selected;
	bool bounding_box;
	ElementScene3DMaterial material;
	ElementScene3DTexture texture;
	LightAttribute lightAttribute;
};

class Geometrie
{
public:
	ofLight light;
	ofLight light_point;

	ofxAssimpModelLoader donut;
	ofxAssimpModelLoader plate;
	ofxAssimpModelLoader spaghetti_getter;

	ofImage texture_None;
	ofImage texture_Wood;
	ofImage texture_Sand;
	ofImage texture_Briks;
	ofImage texture_Honeycomb;
	ofImage texture_Sponge;

	ofMaterial material_VolcanicRock;
	ofMaterial material_FrozenCrystal;
	ofMaterial material_MossyStone;
	ofMaterial material_NeonTech;
	ofMaterial material_AncientBronze;
	ofMaterial material_None;

	ofShader phong_shader;
	ofShader blinn_phong_shader;
	ofShader lambert_shader;
	ofShader gouraud_shader;

	int shader_mode;
	ofShader* shader_active = nullptr;
	string shader_name;

	float selected_point_depth = 0.0f;

	skybox skybox;

	// B�zier bicubique
	ofVec3f control_grid[4][4];
	int resolution_u = 20;
	int resolution_v = 20;
	ofMesh mesh;

	bool is_dragging;
	int selected_i;
	int selected_j;
	float radius;
	ofVec3f drag_offset; // pour stocker l'offset initial du clic
	ofVec3f initial_drag_point;
	bool is_bezier_curve;

	bool projection_mode = false; // projection orthogonale

	float scale;

	float rotation_speed; // not sure

	bool use_rotation;

	void setup();
	void update();
	void draw();

	void draw_cube(ofMaterial material, ofImage img);   // Fonction pour ajouter un cube
	void draw_sphere(ofMaterial material, ofImage img); // Fonction pour ajouter une sph�re
	void draw_cylinder() const; // Fonction pour ajouter un cylindre
	void draw_cone() const; // Fonction pour ajouter un c�ne

	void draw_donut(); // Fonction pour ajouter un donut
	void draw_plate(); // Fonction pour ajouter une assiette
	void draw_spaghetti_getter(); // Fonction pour ajouter
	void draw_bezier_curve();
	void draw_skybox();

	void draw_bounding_box() const;
	void set_projection_mode(bool mode);

	void bezier_bicubic(float u, float v, const ofVec3f control_points[4][4], float& x, float& y, float& z);
	void update_mesh();
};
