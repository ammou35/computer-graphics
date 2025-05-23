#pragma once
// IFT-3100-H2025/src/3D/geometrie.h
// Classe responsable du rendu 3D.

#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "skybox.h"


enum class ElementScene3DType { none, cube, sphere, cylinder, cone, donut, plate, spaghetti_getter, bezier_curve, ambiant, directional_light, point_light, spot_light };
enum class ElementScene3DMaterial { none, volcanicRock, frozenCrystal, mossyStone, neonTech, ancientBronze };
enum class ElementScene3DTexture { none, wood, sand, briks, honeycomb, sponge, checkboard };
enum class ElementScene3DFiltre { none, blur, grayscale, vignette, mexico, invert };

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
	bool normal_mapping;
	ElementScene3DMaterial material;
	ElementScene3DTexture texture;
	ElementScene3DFiltre filtre;
	LightAttribute lightAttribute;
	float metallic;
	float roughness;
};

class Geometrie
{
public:
	ofxAssimpModelLoader donut;
	ofxAssimpModelLoader plate;
	ofxAssimpModelLoader spaghetti_getter;

	ofImage texture_None;
	ofImage texture_Wood;
	ofImage texture_Sand;
	ofImage texture_Briks;
	ofImage texture_Honeycomb;
	ofImage texture_Sponge;
	ofImage texture_Checkerboard;

	ofImage normal_Wood;
	ofImage normal_Sand;
	ofImage normal_Briks;
	ofImage normal_Honeycomb;
	ofImage normal_Sponge;

	ofMaterial material_None;
	ofMaterial material_VolcanicRock;
	ofMaterial material_FrozenCrystal;
	ofMaterial material_MossyStone;
	ofMaterial material_NeonTech;
	ofMaterial material_AncientBronze;

	ofShader filtre_None;
	ofShader filtre_Blur;
	ofShader filtre_Grayscale;
	ofShader filtre_Vignette;
	ofShader filtre_Mexico;
	ofShader filtre_Invert;

	ofShader phong_shader;
	ofShader blinn_phong_shader;
	ofShader lambert_shader;
	ofShader gouraud_shader;
	ofShader flat_shader;
	ofShader pbr_shader;
	ofShader nm_shader;

	ofFbo objectFbo;

	std::vector<ofVec3f> light_positions;
	std::vector<ofVec3f> light_colors;

	int shader_mode;
	ofShader* shader_active = nullptr;
	string shader_name;
	bool useNormalMap;

	float selected_point_depth = 0.0f;

	skybox skybox;

	// B�zier bicubique
	ofVec3f control_grid[4][4];
	int resolution_u = 20;
	int resolution_v = 20;
	ofMesh mesh;

	bool use_tessellation_gpu = false;
	ofShader tessellation_shader;
	ofVboMesh tessellation_patch;

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
	void update(ElementScene3D* element3D);
	void draw();

	void draw_cube(ofMaterial material, ofImage img, ElementScene3DFiltre filtre, ElementScene3D* element3D, ElementScene3D current_element3D);
	void draw_sphere(ofMaterial material, ofImage img, ElementScene3DFiltre filtre, ElementScene3D* element3D, ElementScene3D current_element3D);
	void draw_cylinder(ofMaterial material, ofImage img, ElementScene3DFiltre filtre, ElementScene3D* element3D, ElementScene3D current_element3D);
	void draw_cone(ofMaterial material, ofImage img, ElementScene3DFiltre filtre, ElementScene3D* element3D, ElementScene3D current_element3D);
	void draw_primitive(of3dPrimitive& primitive, ofMaterial material, ofImage img, ElementScene3DFiltre filtre, ElementScene3D* element3D, ElementScene3D current_element3D);
	void draw_model(ofxAssimpModelLoader& model, ofMaterial material, ofImage img, ElementScene3DFiltre filtre, ElementScene3D* element3D, ElementScene3D current_element3D);
	void draw_donut(ofMaterial material, ofImage img, ElementScene3DFiltre filtre, ElementScene3D* element3D, ElementScene3D current_element3D);
	void draw_plate(ofMaterial material, ofImage img, ElementScene3DFiltre filtre, ElementScene3D* element3D, ElementScene3D current_element3D);
	void draw_spaghetti_getter(ofMaterial material, ofImage img, ElementScene3DFiltre filtre, ElementScene3D* element3D, ElementScene3D current_element3D);
	void draw_bezier_curve();
	void setup_tessellation_surface();
	void draw_bezier_surface_cpu();
	void draw_bezier_surface_gpu();
	void update_tessellation_patch();
	void draw_skybox();
	
	ofShader* get_filter_shader(ElementScene3DFiltre filtre);
	void draw_bounding_box() const;
	void set_projection_mode(bool mode);

	void bezier_bicubic(float u, float v, const ofVec3f control_points[4][4], float& x, float& y, float& z);
	void update_mesh();

	void addTangentsToBox(ofBoxPrimitive& box);
	ofBufferObject vertexBuffer;
	ofBufferObject normalBuffer;
	ofBufferObject texcoordBuffer;

	ofVbo tangentVbo;
	ofBufferObject tangentBuffer;

	ofVec3f toVec3f(const ofColor& c);
	void send_common_matrices(ofShader* shader);
	ofTexture getRelief(ElementScene3DTexture texture) const;
	void computeNormals(ofMesh& mesh);

};
