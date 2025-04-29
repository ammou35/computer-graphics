#include "graph.h"

void Graph::setup() {
	buffer_count = 30;

	element2D_buffer_head = 0;
	element2D_buffer_stride = sizeof(ElementScene2D);
	element2D_buffer_size = buffer_count * element2D_buffer_stride;
	element2D = (ElementScene2D*)std::malloc(element2D_buffer_size);

	element3D_buffer_head = 0;
	element3D_buffer_stride = sizeof(ElementScene3D);
	element3D_buffer_size = buffer_count * element3D_buffer_stride;
	element3D = new ElementScene3D[buffer_count];

	dessinVectoriel.setup();
	geometrie.setup();
	draw_mode = 2;

	for (int i = 0; i < buffer_count; i++) {
		element2D[i].type = ElementScene2DType::none;
		element2D[i].is_selected = false;
		element2D[i].image_nb = -1;
		element2D[i].image_position[0] = 0;
		element2D[i].image_position[1] = 0;
		element2D[i].transformation = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f };
		element3D[i].type = ElementScene3DType::none;
		element3D[i].is_selected = false;
		element3D[i].bounding_box = false;
		element3D[i].transformation = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
	}
}

void Graph::update(const ofColor& stroke_color, const ofColor& fill_color, const int& type_vector_primitive, const std::array<float, 6>& transformation, const std::array<float, 9>&transformation3D, const int& element3D_material, const int& element3D_texture) {
	set_draw_shape(type_vector_primitive);
	dessinVectoriel.update(stroke_color, fill_color, type_vector_primitive);
	for (int index = 0; index < buffer_count; index++) {
		if (element2D[index].is_selected) {
			if (element2D[index].type != ElementScene2DType::image) {
				for (int ptr = 0; ptr < 4; ptr++) {
					if (element2D[index].primitives[ptr] != nullptr) {
						dessinVectoriel.apply_transform(element2D[index].primitives[ptr], transformation);
					}
				}
			}
			else {
				element2D[index].transformation = transformation;
			}
		}
		if (element3D[index].is_selected) {
			ElementScene3DMaterial elementScene3DMaterial = ElementScene3DMaterial::none;
			switch (element3D_material) {
			case -1:
				elementScene3DMaterial = ElementScene3DMaterial::none;
				break;
			case 1:
				elementScene3DMaterial = ElementScene3DMaterial::volcanicRock;
				break;
			case 2:
				elementScene3DMaterial = ElementScene3DMaterial::frozenCrystal;
				break;
			case 3:
				elementScene3DMaterial = ElementScene3DMaterial::mossyStone;
				break;
			case 4:
				elementScene3DMaterial = ElementScene3DMaterial::neonTech;
				break;
			case 5:
				elementScene3DMaterial = ElementScene3DMaterial::ancientBronze;
				break;
			}
			element3D[index].material = elementScene3DMaterial;

			ElementScene3DTexture elementScene3DTexture;
			switch (element3D_texture) {
			case -1:
				elementScene3DTexture = ElementScene3DTexture::none;
				break;
			case 1:
				elementScene3DTexture = ElementScene3DTexture::wood;
				break;
			case 2:
				elementScene3DTexture = ElementScene3DTexture::sand;
				break;
			case 3:
				elementScene3DTexture = ElementScene3DTexture::briks;
				break;
			case 4:
				elementScene3DTexture = ElementScene3DTexture::honeycomb;
				break;
			case 5:
				elementScene3DTexture = ElementScene3DTexture::sponge;
				break;
			}
			element3D[index].texture = elementScene3DTexture;

			element3D[index].transformation = transformation3D;

			if (element3D[index].type == ElementScene3DType::point_light || element3D[index].type == ElementScene3DType::directional_light || element3D[index].type == ElementScene3DType::spot_light) {
				element3D[index].lightAttribute.light.setPosition(element3D[index].transformation[0], element3D[index].transformation[1], element3D[index].transformation[2]);
				element3D[index].lightAttribute.light.draw();
			}

			if (bounding_box) {
				element3D[index].bounding_box = true;
			}
			else {
				element3D[index].bounding_box = false;
			}
		}
	}
}

void Graph::draw(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current, const bool& is_mouse_button_pressed) {
	for (int i = 0; i < buffer_count; i++) {
		if (element2D[i].type == ElementScene2DType::image) {
			ofPushMatrix();
			ofTranslate(element2D[i].transformation[0], element2D[i].transformation[1], element2D[i].transformation[2]);
			ofRotateDeg(element2D[i].transformation[3]);
			ofScale(element2D[i].transformation[4], element2D[i].transformation[5]);
			images[element2D[i].image_nb].draw(element2D[i].image_position[0], element2D[i].image_position[1]);
			ofPopMatrix();
		}
	}
	ofPushMatrix();
	//ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);
	for (int i = 0; i < buffer_count; i++) {
		ofPushMatrix();
		ofTranslate(element3D[i].transformation[0], element3D[i].transformation[1], element3D[i].transformation[2]);
		ofRotateXDeg(element3D[i].transformation[3]);
		ofRotateYDeg(element3D[i].transformation[4]);
		ofRotateZDeg(element3D[i].transformation[5]);
		ofScale(element3D[i].transformation[6], element3D[i].transformation[7], element3D[i].transformation[8]);

		ofSetLineWidth(5);
		ofSetColor(0, 0, 0);
		ofNoFill();

		ofMaterial material;
		
		switch (element3D[i].material) {
			case ElementScene3DMaterial::none:
				material = geometrie.material_None;
				break;
			case ElementScene3DMaterial::volcanicRock:
				material = geometrie.material_VolcanicRock;
				break;
			case ElementScene3DMaterial::frozenCrystal:
				material = geometrie.material_FrozenCrystal;
				break;
			case ElementScene3DMaterial::mossyStone:
				material = geometrie.material_MossyStone;
				break;
			case ElementScene3DMaterial::neonTech:
				material = geometrie.material_NeonTech;
				break;
			case ElementScene3DMaterial::ancientBronze:
				material = geometrie.material_AncientBronze;
				break;
			default:
				material = geometrie.material_None;
				break;
		}
		ofImage texture;
		switch (element3D[i].texture) {
		case ElementScene3DTexture::none:
			texture = geometrie.texture_None;
			break;
		case ElementScene3DTexture::wood:
			texture = geometrie.texture_Wood;
			break;
		case ElementScene3DTexture::sand:
			texture = geometrie.texture_Sand;
			break;
		case ElementScene3DTexture::briks:
			texture = geometrie.texture_Briks;
			break;
		case ElementScene3DTexture::honeycomb:
			texture = geometrie.texture_Honeycomb;
			break;
		case ElementScene3DTexture::sponge:
			texture = geometrie.texture_Sponge;
			break;
		default:
			texture = geometrie.texture_None;
			break;
		}
		ofEnableLighting();
		light_on();
		switch (element3D[i].type) {
			case ElementScene3DType::none:
				break;
			case ElementScene3DType::cube:
				geometrie.draw_cube(material, texture);
				break;
			case ElementScene3DType::sphere:
				geometrie.draw_sphere(material, texture);
				break;
			case ElementScene3DType::cylinder:
				geometrie.draw_cylinder();
				break;
			case ElementScene3DType::cone:
				geometrie.draw_cone();
				break;
			case ElementScene3DType::donut:
				geometrie.draw_donut();
				break;
			case ElementScene3DType::plate:
				geometrie.draw_plate();
				break;
			case ElementScene3DType::spaghetti_getter:
				geometrie.draw_spaghetti_getter();
				break;
			case ElementScene3DType::bezier_curve:
				geometrie.draw_bezier_curve();
				break;
			default:
				break;
		}
		light_off();
		ofDisableLighting();
		if (element3D[i].bounding_box && element3D[i].type != ElementScene3DType::none) {
			geometrie.draw_bounding_box();
		}
		ofPopMatrix();
	}
	ofPopMatrix();
}

void Graph::add_element(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current) {
	if (draw_mode == 2) {
		add_element2D(mouse_press, mouse_current);
	}
	else if (draw_mode == 3){
		add_element3D(mouse_press, mouse_current);
	}
}

void Graph::add_element2D(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current) {
	int index;

	if (!free_indices.empty()) {
		index = free_indices.back();
		free_indices.pop_back(); // Réutilise un ancien index
	}
	else {
		index = element2D_buffer_head;
		element2D_buffer_head = (element2D_buffer_head + 1) % buffer_count;
	}
	
	element2D[index].type = get_draw_shape_2D();
	element2D[index].is_selected = false;

	switch (get_draw_shape_2D()) {
	case ElementScene2DType::none:
		break;
	case ElementScene2DType::pixel:
		std::strcpy(element2D[index].name, "Pixel");
		element2D[index].primitives = dessinVectoriel.add_vector_shape(mouse_press, mouse_current);
		break;
	case ElementScene2DType::point:
		std::strcpy(element2D[index].name, "Point");
		element2D[index].primitives = dessinVectoriel.add_vector_shape(mouse_press, mouse_current);
		break;
	case ElementScene2DType::line:
		std::strcpy(element2D[index].name, "Line");
		element2D[index].primitives = dessinVectoriel.add_vector_shape(mouse_press, mouse_current);
		break;
	case ElementScene2DType::rectangle:
		std::strcpy(element2D[index].name, "Rectangle");
		element2D[index].primitives = dessinVectoriel.add_vector_shape(mouse_press, mouse_current);
		break;
	case ElementScene2DType::ellipse:
		std::strcpy(element2D[index].name, "Ellipse");
		element2D[index].primitives = dessinVectoriel.add_vector_shape(mouse_press, mouse_current);
		break;
	case ElementScene2DType::param_curve:
		std::strcpy(element2D[index].name, "Parametric curve");
		element2D[index].primitives = dessinVectoriel.addBezierPoint(mouse_press, mouse_current);
		if (element2D[index].primitives[0] == nullptr) {
			element2D_buffer_head = (element2D_buffer_head - 1) % buffer_count;
			element2D[index].type = ElementScene2DType::none;
		}
		break;
	case ElementScene2DType::polygone:
		std::strcpy(element2D[index].name, "Polygon");
		element2D[index].primitives = dessinVectoriel.addPolyPoint(mouse_press, mouse_current);
		if (element2D[index].primitives[0] == nullptr) {
			element2D_buffer_head = (element2D_buffer_head - 1) % buffer_count;
			element2D[index].type = ElementScene2DType::none;
		}
		break;
	case ElementScene2DType::star:
		std::strcpy(element2D[index].name, "Star");
		dessinVectoriel.set_draw_mode(7);
		element2D[index].primitives = dessinVectoriel.add_star(mouse_press, mouse_current);
		break;
	case ElementScene2DType::heart:
		std::strcpy(element2D[index].name, "Heart");
		dessinVectoriel.set_draw_mode(6);
		element2D[index].primitives = dessinVectoriel.add_heart(mouse_press, mouse_current);
		break;
	case ElementScene2DType::moon:
		std::strcpy(element2D[index].name, "Moon");
		dessinVectoriel.set_draw_mode(6);
		element2D[index].primitives = dessinVectoriel.add_moon(mouse_press, mouse_current);
		break;
	case ElementScene2DType::arrow:
		std::strcpy(element2D[index].name, "Arrow");
		dessinVectoriel.set_draw_mode(7);
		element2D[index].primitives = dessinVectoriel.add_arrow(mouse_press, mouse_current);
		break;
	case ElementScene2DType::clover:
		std::strcpy(element2D[index].name, "Clover");
		dessinVectoriel.set_draw_mode(6);
		element2D[index].primitives = dessinVectoriel.add_clover(mouse_press, mouse_current);
		break;
	case ElementScene2DType::image:
		std::strcpy(element2D[index].name, "Image");
		element2D[index].image_nb = image_nb;
		element2D[index].image_position[0] = mouse_current[0] - images[image_nb].getWidth()/2;
		element2D[index].image_position[1] = mouse_current[1] - images[image_nb].getHeight()/2;
		break;
	default:
		break;
	}
}

void Graph::add_element3D(const std::array<int, 2>& mouse_press, const std::array<int, 2>& mouse_current) {
	int index;

	if (!free_indices.empty()) {
		index = free_indices.back();
		free_indices.pop_back(); // Réutilise un ancien index
	}
	else {
		index = element3D_buffer_head;
		element3D_buffer_head = (element3D_buffer_head + 1) % buffer_count;
	}

	element3D[index].type = get_draw_shape_3D();
	element3D[index].is_selected = false;
	element3D[index].material = ElementScene3DMaterial::none;

	if (get_draw_shape_3D() == ElementScene3DType::spot_light) {
		element3D[index].lightAttribute.diffuseColor = ofColor(191, 191, 191);
		element3D[index].lightAttribute.specularColor = ofColor(191, 191, 191);
		element3D[index].lightAttribute.orientation = ofVec3f(0.0f, 0.0f, 0.0f);
		element3D[index].lightAttribute.concentration = 2.0f;
		element3D[index].lightAttribute.lightCutOff = 30.0f;
		element3D[index].lightAttribute.light.setSpotlight();
		element3D[index].lightAttribute.light.setDiffuseColor(element3D[index].lightAttribute.diffuseColor);
		element3D[index].lightAttribute.light.setSpecularColor(element3D[index].lightAttribute.specularColor);
		element3D[index].lightAttribute.light.setOrientation(element3D[index].lightAttribute.orientation);
		element3D[index].lightAttribute.light.setSpotConcentration(element3D[index].lightAttribute.concentration);
		element3D[index].lightAttribute.light.setSpotlightCutOff(element3D[index].lightAttribute.lightCutOff);
		element3D[index].lightAttribute.light.setSpotlight();
	}
	else if (get_draw_shape_3D() == ElementScene3DType::point_light) {
		element3D[index].lightAttribute.diffuseColor = ofColor(255, 255, 255);
		element3D[index].lightAttribute.specularColor = ofColor(191, 191, 191);
		element3D[index].lightAttribute.orientation = ofVec3f(0.0f, 0.0f, 0.0f);
		element3D[index].lightAttribute.concentration = 0.0f;
		element3D[index].lightAttribute.lightCutOff = 0.0f;
		element3D[index].lightAttribute.light.setDiffuseColor(element3D[index].lightAttribute.diffuseColor);
		element3D[index].lightAttribute.light.setSpecularColor(element3D[index].lightAttribute.specularColor);
		element3D[index].lightAttribute.light.setOrientation(element3D[index].lightAttribute.orientation);
		element3D[index].lightAttribute.light.setPointLight();
	}
	else if (get_draw_shape_3D() == ElementScene3DType::directional_light) {
		element3D[index].lightAttribute.light;
		element3D[index].lightAttribute.diffuseColor = ofColor(31, 255, 31);
		element3D[index].lightAttribute.specularColor = ofColor(191, 191, 191);
		element3D[index].lightAttribute.orientation = ofVec3f(0.0f, 0.0f, 0.0f);
		element3D[index].lightAttribute.concentration = 0.0f;
		element3D[index].lightAttribute.lightCutOff = 0.0f;
		element3D[index].lightAttribute.light.setDiffuseColor(element3D[index].lightAttribute.diffuseColor);
		element3D[index].lightAttribute.light.setSpecularColor(element3D[index].lightAttribute.specularColor);
		element3D[index].lightAttribute.light.setOrientation(element3D[index].lightAttribute.orientation);
		element3D[index].lightAttribute.light.setDirectional();
	}
	else if (get_draw_shape_3D() == ElementScene3DType::ambiant) {
		element3D[index].lightAttribute.diffuseColor = ofColor(127, 127, 127);
		element3D[index].lightAttribute.specularColor = ofColor(191, 191, 191);
		element3D[index].lightAttribute.orientation = ofVec3f(0.0f, 0.0f, 0.0f);
		element3D[index].lightAttribute.concentration = 0.0f;
		element3D[index].lightAttribute.lightCutOff = 0.0f;
	}

	if (bounding_box) {
		element3D[index].bounding_box = true;
	}
}

void Graph::delete_element2D(void) {
	for (int index = 0; index < buffer_count; index++) {
		if (element2D[index].is_selected) {
			element2D[index].type = ElementScene2DType::none; // Marquer comme supprimé
			element2D[index].is_selected = false;
			for (int ptr = 0; ptr < 4; ptr++) {
				if (element2D[index].primitives[ptr] != nullptr) {
					dessinVectoriel.delete_vector_shape(element2D[index].primitives[ptr]);
				}
			}
			element2D[index].primitives = {nullptr, nullptr, nullptr, nullptr};
			free_indices.push_back(index); // Ajouter à la liste des indices libres
		}
	}
}

void Graph::delete_element3D(void) {
	for (int index = 0; index < buffer_count; index++) {
		if (element3D[index].is_selected) {
			element3D[index].type = ElementScene3DType::none; // Marquer comme supprimé
			element3D[index].is_selected = false;
			free_indices.push_back(index); // Ajouter à la liste des indices libres
		}
	}
}

void Graph::set_draw_shape(const int& type_ElementScene) {
	switch (type_ElementScene) {
	case -1: draw_shape_2D = ElementScene2DType::none; break;
	case 1:  draw_shape_2D = ElementScene2DType::pixel; break;
	case 2:  draw_shape_2D = ElementScene2DType::point; break;
	case 3:  draw_shape_2D = ElementScene2DType::line; break;
	case 4:  draw_shape_2D = ElementScene2DType::rectangle; break;
	case 5:  draw_shape_2D = ElementScene2DType::ellipse; break;
	case 6:  draw_shape_2D = ElementScene2DType::param_curve; break;
	case 7:  draw_shape_2D = ElementScene2DType::polygone; break;
	case 8:  draw_shape_2D = ElementScene2DType::star; break;
	case 9:  draw_shape_2D = ElementScene2DType::heart; break;
	case 10:  draw_shape_2D = ElementScene2DType::moon; break;
	case 11:  draw_shape_2D = ElementScene2DType::arrow; break;
	case 12:  draw_shape_2D = ElementScene2DType::clover; break;
	case 13:  draw_shape_3D = ElementScene3DType::cube; break;
	case 14:  draw_shape_3D = ElementScene3DType::sphere; break;
	case 15:  draw_shape_3D = ElementScene3DType::cylinder; break;
	case 16:  draw_shape_3D = ElementScene3DType::cone; break;
	case 17:  draw_shape_3D = ElementScene3DType::donut; break;
	case 18:  draw_shape_3D = ElementScene3DType::plate; break;
	case 19:  draw_shape_3D = ElementScene3DType::spaghetti_getter; break;
	case 20:  draw_shape_3D = ElementScene3DType::bezier_curve; break;
	case 21:  draw_shape_3D = ElementScene3DType::ambiant; break;
	case 22:  draw_shape_3D = ElementScene3DType::directional_light; break;
	case 23:  draw_shape_3D = ElementScene3DType::point_light; break;
	case 24:  draw_shape_3D = ElementScene3DType::spot_light; break;
	default: draw_shape_2D = ElementScene2DType::none; break;
	}

	if (type_ElementScene > 19) {
		draw_shape_2D = ElementScene2DType::image;
		image_nb = type_ElementScene - 20;
	}
	if (type_ElementScene > 12 && type_ElementScene < 25) {
		draw_mode = 3;
	}
	else {
		draw_mode = 2;
	}
}

ElementScene2DType Graph::get_draw_shape_2D(void) const {
	return draw_shape_2D;
}

ElementScene3DType Graph::get_draw_shape_3D(void) const {
	return draw_shape_3D;
}

void Graph::load_image(string path) {
	ofImage image;
	image.load(path);
	images.push_back(image);
}

Graph::~Graph()
{
	std::free(element2D);
	//std::free(element3D);
}

void Graph::light_on() {
	for (int i = 0; i < buffer_count; i++) {
		if (element3D[i].type == ElementScene3DType::point_light || element3D[i].type == ElementScene3DType::directional_light || element3D[i].type == ElementScene3DType::spot_light) {
			element3D[i].lightAttribute.light.enable();
		}
		else if (element3D[i].type == ElementScene3DType::ambiant) {
			ofSetGlobalAmbientColor(element3D[i].lightAttribute.diffuseColor);
		}
	}
}

void Graph::light_off() {
	for (int i = 0; i < buffer_count; i++) {
		if (element3D[i].type == ElementScene3DType::point_light || element3D[i].type == ElementScene3DType::directional_light || element3D[i].type == ElementScene3DType::spot_light) {
			element3D[i].lightAttribute.light.disable();
		}
		else if (element3D[i].type == ElementScene3DType::ambiant) {
			ofSetGlobalAmbientColor(ofColor(0, 0, 0));
		}
	}
}