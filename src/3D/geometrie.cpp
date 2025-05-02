#include "geometrie.h"




void Geometrie::setup()
{
    // Param�tres g�n�raux
    //scale_teapot = 1.5f;
    //rotation_speed = 0.3f;
    //use_rotation = true;
    //bounding_box = true;       // Affichage de la bo�te de d�limitation
    //animation_speed = 0.02f;   // Vitesse d�animation
    //instance_count = 5;        // Nombre d'instances pour la duplication

    // Activer les effets graphiques
    //ofEnableDepthTest();
    //ofEnableLighting();
    //light.enable();
    //shader.begin();

    //donut.loadModel("donut.obj");
    plate.loadModel("plate.obj");
    spaghetti_getter.loadModel("spaghetti_getter.obj");

    radius = 10;

    // G�n�ration des points de contr�le en grille 4x4
    float centerX = 0;
    float centerY = 0;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            control_grid[i][j].x = centerX;
            control_grid[i][j].y = centerY;
            control_grid[i][j].z = 0;
        }
    }
    is_bezier_curve = true;

    /*light.setup();
    light.setPosition(300, 300, 300);
    light.enable();*/

    // Chargement des shaders
    //shader_lambert.load("lambert_330_vs.glsl", "lambert_330_fs.glsl");
    //shader_normal.load("draw_normal_330_vs.glsl", "draw_normal_330_fs.glsl");

    // S�lectionner le shader courant
    //shader = shader_lambert;

    filtre_None.load("shaders/passthrough_vs.glsl", "shaders/passthrough_fs.glsl");
    filtre_Blur.load("shaders/passthrough_vs.glsl", "shaders/blur_fs.glsl");
    filtre_Grayscale.load("shaders/passthrough_vs.glsl", "shaders/grayscale_fs.glsl");
    filtre_Vignette.load("shaders/passthrough_vs.glsl", "shaders/vignette_fs.glsl");
    filtre_Mexico.load("shaders/passthrough_vs.glsl", "shaders/mexico_fs.glsl");
    filtre_Invert.load("shaders/passthrough_vs.glsl", "shaders/invert_fs.glsl");

    lambert_shader.load("shaders/lambert_vs.glsl", "shaders/lambert_fs.glsl");
    gouraud_shader.load("shaders/gouraud_vs.glsl", "shaders/gouraud_fs.glsl");
    phong_shader.load("shaders/phong_vs.glsl", "shaders/phong_fs.glsl");
    blinn_phong_shader.load("shaders/blinn_phong_vs.glsl", "shaders/blinn_phong_fs.glsl");
    flat_shader.load("shaders/flat_vs.glsl", "shaders/flat_fs.glsl");
    pbr_shader.load("shaders/pbr_330_vs.glsl", "shaders/pbr_330_fs.glsl");
    nm_shader.setupShaderFromFile(GL_VERTEX_SHADER, "shaders/bump_vs.glsl");
    nm_shader.setupShaderFromFile(GL_FRAGMENT_SHADER, "shaders/bump_fs.glsl");

    GLuint progID = nm_shader.getProgram();
    glBindAttribLocation(progID, 0, "position");
    glBindAttribLocation(progID, 1, "normal");
    glBindAttribLocation(progID, 2, "texcoord");
    glBindAttribLocation(progID, 6, "tangent");

    nm_shader.linkProgram();

    // Default shader active
    shader_active = &blinn_phong_shader;
    shader_mode = -1;

    tessellation_shader.setupShaderFromFile(GL_VERTEX_SHADER, "shaders/tessellation/bezier.vert");
    tessellation_shader.setupShaderFromFile(GL_FRAGMENT_SHADER, "shaders/tessellation/bezier.frag");
    tessellation_shader.setupShaderFromFile(GL_TESS_CONTROL_SHADER, "shaders/tessellation/bezier_tcs.glsl");
    tessellation_shader.setupShaderFromFile(GL_TESS_EVALUATION_SHADER, "shaders/tessellation/bezier_tes.glsl");
    tessellation_shader.linkProgram();
    setup_tessellation_surface();

    // Volcanic Magma - Bright, hot, glowing
    material_VolcanicRock.setAmbientColor(ofColor(80, 10, 0));
    material_VolcanicRock.setDiffuseColor(ofColor(255, 60, 10));
    material_VolcanicRock.setEmissiveColor(ofColor(150, 20, 0));  // Strong internal glow
    material_VolcanicRock.setSpecularColor(ofColor(255, 180, 130));
    material_VolcanicRock.setShininess(20.0f);

    // Frozen Crystal - Cold, sharp reflections
    material_FrozenCrystal.setAmbientColor(ofColor(200, 230, 255));
    material_FrozenCrystal.setDiffuseColor(ofColor(180, 220, 255));
    material_FrozenCrystal.setEmissiveColor(ofColor(20, 40, 80));  // Very subtle glow
    material_FrozenCrystal.setSpecularColor(ofColor(255, 255, 255));
    material_FrozenCrystal.setShininess(100.0f);  // Very sharp specular

    // Mossy Stone - Very soft, matte, natural
    material_MossyStone.setAmbientColor(ofColor(30, 50, 30));
    material_MossyStone.setDiffuseColor(ofColor(50, 80, 50));
    material_MossyStone.setEmissiveColor(ofColor(5, 10, 5));      // Minimal
    material_MossyStone.setSpecularColor(ofColor(20, 20, 20));    // Very low specular
    material_MossyStone.setShininess(2.0f);  // Dead matte

    // Neon Tech - Bright, glowing, artificial
    material_NeonTech.setAmbientColor(ofColor(10, 10, 50));
    material_NeonTech.setDiffuseColor(ofColor(0, 255, 180));
    material_NeonTech.setEmissiveColor(ofColor(0, 200, 180));     // Super glow
    material_NeonTech.setSpecularColor(ofColor(0, 255, 255));
    material_NeonTech.setShininess(80.0f);  // Shiny but not too sharp

    // Ancient Bronze - Warm, subtle metal
    material_AncientBronze.setAmbientColor(ofColor(90, 70, 40));
    material_AncientBronze.setDiffuseColor(ofColor(160, 120, 60));
    material_AncientBronze.setEmissiveColor(ofColor(30, 20, 10));
    material_AncientBronze.setSpecularColor(ofColor(200, 150, 80));
    material_AncientBronze.setShininess(25.0f);  // Soft metallic shine

    material_None.setAmbientColor(ofColor(255, 255, 255));
    material_None.setDiffuseColor(ofColor(255, 255, 255));
    material_None.setSpecularColor(ofColor(0, 0, 0));
    material_None.setEmissiveColor(ofColor(0, 0, 0));
    material_None.setShininess(0.0f);
    
	texture_None.load("textures/none.jpg");
	texture_Wood.load("textures/wood.jpg");
	texture_Sand.load("textures/sand.jpg");
	texture_Briks.load("textures/brick.jpg");
	texture_Honeycomb.load("textures/honeycomb.png");
	texture_Sponge.load("textures/sponge.jpg");

    normal_Wood.load("textures/wood_nm.png");
    normal_Sand.load("textures/sand_nm.png");
    normal_Briks.load("textures/brick_nm.png");
    normal_Honeycomb.load("textures/honeycomb_nm.png");
    normal_Sponge.load("textures/sponge_nm.png");

    normal_Wood.load("textures/wood_nm.png");
    normal_Sand.load("textures/sand_nm.png");
    normal_Briks.load("textures/brick_nm.png");
    normal_Honeycomb.load("textures/honeycomb_nm.png");
    normal_Sponge.load("textures/sponge_nm.png");

    // Paramétrage des textures de diffusion
    texture_Wood.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    texture_Wood.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);

    texture_Sand.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    texture_Sand.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);

    texture_Briks.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    texture_Briks.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);

    texture_Honeycomb.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    texture_Honeycomb.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);

    texture_Sponge.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    texture_Sponge.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);

    texture_Checkerboard.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    texture_Checkerboard.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);

    // Paramétrage des normal maps associées
    normal_Wood.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    normal_Wood.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);

    normal_Sand.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    normal_Sand.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);

    normal_Briks.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    normal_Briks.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);

    normal_Honeycomb.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    normal_Honeycomb.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);

    normal_Sponge.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    normal_Sponge.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);

    int width = 256;
    int height = 256;
    int tileSize = 32;

    texture_Checkerboard.allocate(width, height, OF_IMAGE_COLOR);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int tileX = x / tileSize;
            int tileY = y / tileSize;

            // Calcul d'une couleur en arc-en-ciel en fonction de la position
            float hue = fmod((tileX + tileY) * 30.0f, 255.0f); // 30� par case diagonale
            ofColor color;
            color.setHsb(hue, 255, 255); // teinte vive

            texture_Checkerboard.setColor(x, y, color);
        }
    }

    texture_Checkerboard.update(); // Envoie la texture au GPU

    skybox.setup("textures/skybox.hdr");
}

void Geometrie::update(ElementScene3D* element3D)
{

    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(ofGetCurrentMatrix(OF_MATRIX_MODELVIEW))));
    switch (shader_mode)
    {
    case 1:
        shader_active = &lambert_shader;
        shader_name = "Lambert";
        shader_active->begin();
        shader_active->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
        shader_active->setUniform3f("color_diffuse", 0.6f, 0.6f, 0.6f);
        shader_active->setUniform1f("brightness", 0.0f);
        //shader_active->setUniform3f("light_position", light_point.getGlobalPosition());
        shader_active->end();
        break;

    case 2:
        shader_active = &gouraud_shader;
        shader_name = "Gouraud";
        shader_active->begin();
        shader_active->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
        shader_active->setUniform3f("color_diffuse", 0.6f, 0.6f, 0.0f);
        shader_active->setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
        shader_active->setUniform1f("brightness", 10.0f);
        //shader_active->setUniform3f("light_position", light_point.getGlobalPosition());
        shader_active->end();
        break;

    case 3:
        shader_active = &phong_shader;
        shader_name = "Phong";
        shader_active->begin();
        shader_active->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
        shader_active->setUniform3f("color_diffuse", 0.6f, 0.0f, 0.6f);
        shader_active->setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
        shader_active->setUniform1f("brightness", 10.0f);
        //->setUniform3f("light_position", light_point.getGlobalPosition());
        shader_active->end();
        break;

    case 4: // Blinn-Phong
        shader_active = &blinn_phong_shader;
        shader_name = "Blinn-Phong";

        shader_active->begin();
        shader_active->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
        shader_active->setUniform3f("color_diffuse", 0.0f, 0.6f, 0.6f);
        shader_active->setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
        shader_active->setUniform1f("brightness", 10.0f);

        shader_active->end();
        break;

    case 5:
        shader_active = &pbr_shader;
        shader_name = "PBR";

        shader_active->begin();
        send_common_matrices(shader_active);

        shader_active->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
        shader_active->setUniform3f("color_diffuse", 0.0f, 0.6f, 0.6f);
        shader_active->setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
        shader_active->setUniform1f("brightness", 10.0f);

        shader_active->setUniform1f("material_roughness", 0.3f); // Valeur test ; tu peux ajuster via GUI
        shader_active->setUniform1f("material_metallic", 0.1f);

        // Gestion des lumières comme dans draw_cube (préparation des vecteurs)
        // Tu n'as pas besoin de les re-calculer ici si tu les calcules dans draw_cube()

        shader_active->end();
        break;

    case 6:

        shader_active = &flat_shader;
        shader_name = "Flat_shader";
        shader_active->begin();
        shader_active->setUniformMatrix4f("modelViewMatrix", ofGetCurrentMatrix(OF_MATRIX_MODELVIEW));
        shader_active->setUniformMatrix4f("projectionMatrix", ofGetCurrentMatrix(OF_MATRIX_PROJECTION));
        
        shader_active->setUniformMatrix3f("normalMatrix", normalMatrix);

        shader_active->setUniform3f("light_position", ofVec3f(100, 200, 300));
        shader_active->setUniform3f("color_diffuse", ofVec3f(0.8, 0.5, 0.3));
        shader_active->setUniform3f("color_ambient", ofVec3f(0.1, 0.1, 0.1));
        shader_active->end();
        break;

    default:
        shader_active = nullptr;
        break;
    }
}

void Geometrie::draw()
{
    ofPushMatrix();

    if (!projection_mode) {
        ofSetupScreenPerspective(60, 0.1f, 1000.0f);
    }
    else { // Orthogonal
        float width = ofGetWidth();
        float height = ofGetHeight();
        ofSetupScreenOrtho(width, height, -1000, 1000);
    }

    ofPopMatrix();

}

void Geometrie::draw_bounding_box() const {
    ofNoFill();
    ofSetColor(255, 0, 0);
    ofDrawBox(0, 0, 0, 100);
}

void Geometrie::draw_primitive(of3dPrimitive& primitive, ofMaterial material, ofImage img, ElementScene3DFiltre filtre, ElementScene3D* element3D, ElementScene3D current_element3D) {
    ofEnableDepthTest();
    primitive.getMesh().enableNormals();
    primitive.mapTexCoordsFromTexture(img.getTexture());

    bool useSpecialFilter = (filtre != ElementScene3DFiltre::none);

    if (useSpecialFilter) {
        ofShader* fshader = get_filter_shader(filtre);
        if (fshader && fshader->isLoaded()) {
            fshader->begin();
            fshader->setUniformMatrix4f("modelViewProjectionMatrix", ofGetCurrentMatrix(OF_MATRIX_MODELVIEW) * ofGetCurrentMatrix(OF_MATRIX_PROJECTION));
            fshader->setUniformTexture("tex0", img.getTexture(), 0);
            primitive.drawFaces();
            fshader->end();
            return;
        }
    }

    if (current_element3D.normal_mapping) {
        std::vector<ofVec3f> light_positions;
        std::vector<ofVec3f> light_colors;
        std::vector<ofVec3f> light_directions;
        std::vector<int> light_types;
        std::vector<float> spot_cutoffs;

        int light_count = 0;

        for (int i = 0; i < 30 && light_count < 8; ++i) {
            const auto& e = element3D[i];
            if (e.type == ElementScene3DType::point_light ||
                e.type == ElementScene3DType::directional_light ||
                e.type == ElementScene3DType::spot_light) {

                ofVec3f world_pos = e.lightAttribute.light.getGlobalPosition();
                glm::vec4 view_pos = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW) * glm::vec4(world_pos.x, world_pos.y, world_pos.z, 1.0);
                light_positions.push_back(glm::vec3(view_pos));
                light_colors.push_back(e.lightAttribute.diffuseColor / 255.0f);

                glm::vec3 dir = glm::vec3(e.lightAttribute.orientation);
                glm::vec3 view_dir = glm::normalize(glm::mat3(ofGetCurrentMatrix(OF_MATRIX_MODELVIEW)) * dir);
                light_directions.push_back(view_dir);

                if (e.type == ElementScene3DType::point_light) {
                    light_types.push_back(0);
                    spot_cutoffs.push_back(0.0f);
                }
                else if (e.type == ElementScene3DType::directional_light) {
                    light_types.push_back(1);
                    spot_cutoffs.push_back(0.0f);
                }
                else if (e.type == ElementScene3DType::spot_light) {
                    light_types.push_back(2);
                    spot_cutoffs.push_back(cos(ofDegToRad(e.lightAttribute.lightCutOff)));
                }

                ++light_count;
            }
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        nm_shader.begin();

        send_common_matrices(&nm_shader);

        nm_shader.setUniform1i("numLights", light_count);
        if (light_count < 0) {
            nm_shader.setUniform3fv("lightPositions", &light_positions[0].x, light_count);
            nm_shader.setUniform3fv("lightColors", &light_colors[0].x, light_count);
            nm_shader.setUniform3fv("lightDirections", &light_directions[0].x, light_count);
            nm_shader.setUniform1iv("lightTypes", &light_types[0], light_count);
            nm_shader.setUniform1fv("spotCutoffs", &spot_cutoffs[0], light_count);
        }

        nm_shader.setUniformTexture("uTex", img.getTexture(), 0);
        nm_shader.setUniformTexture("uNormalMap", getRelief(current_element3D.texture), 1);

        nm_shader.setUniform3f("uViewPos", glm::vec3(0, 0, 0));  // en espace vue

        tangentVbo.drawElements(GL_TRIANGLES, tangentVbo.getNumIndices());
        nm_shader.end();
        return;
    }

    else if (shader_active && shader_active->isLoaded()) {

        std::vector<ofVec3f> light_positions;
        std::vector<ofVec3f> light_colors;
        std::vector<ofVec3f> light_directions;
        std::vector<int> light_types;
        std::vector<float> spot_cutoffs;
        ofVec3f ambient_sum(0, 0, 0);

        ofMatrix4x4 modelViewMatrix = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW);

        for (int i = 0; i < 30; ++i) {
            const auto& e = element3D[i];
            if (e.type == ElementScene3DType::point_light ||
                e.type == ElementScene3DType::directional_light ||
                e.type == ElementScene3DType::spot_light) {

                ofVec3f world_pos = e.lightAttribute.light.getGlobalPosition();
                ofVec3f view_pos = world_pos * modelViewMatrix;
                glm::vec3 dir = glm::vec3(e.lightAttribute.orientation);

                glm::vec3 view_dir = glm::mat3(modelViewMatrix) * dir;
                light_directions.push_back(ofVec3f(view_dir)); // transforme pour spot et point

                light_positions.push_back(view_pos);
                light_colors.push_back(e.lightAttribute.diffuseColor / 255.0f); // normalize

                if (e.type == ElementScene3DType::point_light) {
                    light_types.push_back(0);
                    spot_cutoffs.push_back(0.0f);
                }
                else if (e.type == ElementScene3DType::directional_light) {
                    light_types.push_back(1);
                    spot_cutoffs.push_back(0.0f);
                }
                else if (e.type == ElementScene3DType::spot_light) {
                    light_types.push_back(2);
                    spot_cutoffs.push_back(cos(ofDegToRad(e.lightAttribute.lightCutOff)));
                }
            }
            else if (e.type == ElementScene3DType::ambiant) {
                ambient_sum += e.lightAttribute.diffuseColor / 255.0f;
            }
        }

        shader_active->begin();

        send_common_matrices(shader_active);
        shader_active->setUniform3f("mat_ambient", toVec3f(material.getAmbientColor()));
        shader_active->setUniform3f("color_diffuse", toVec3f(material.getDiffuseColor()));
        shader_active->setUniform3f("color_specular", toVec3f(material.getSpecularColor()));
        shader_active->setUniform1f("brightness", 50.0f);
        shader_active->setUniform3f("light_ambient", ambient_sum);
        shader_active->setUniform1f("material_roughness", current_element3D.roughness);
        shader_active->setUniform1f("material_metallic", current_element3D.metallic);

        int count = std::min((int)light_positions.size(), 30);
        shader_active->setUniform1i("num_lights", count);
        if (count > 0) {
            shader_active->setUniform3fv("light_positions", &light_positions[0].x, count);
            shader_active->setUniform3fv("light_colors", &light_colors[0].x, count);
            shader_active->setUniform3fv("light_directions", &light_directions[0].x, count);
            shader_active->setUniform1iv("light_type", &light_types[0], count);
            shader_active->setUniform1fv("spot_cutoffs", &spot_cutoffs[0], count);
        }

        shader_active->setUniformTexture("tex0", img.getTexture(), 0);

        primitive.drawFaces();

        shader_active->end();
    }
    else {
        material.begin();
        img.bind();
        primitive.draw();
        img.unbind();
        material.end();
    }
}


void Geometrie::draw_cube(ofMaterial material, ofImage img, ElementScene3DFiltre filtre,
    ElementScene3D* element3D, ElementScene3D current_element3D) {
    ofBoxPrimitive box;
    box.set(200);

    if (current_element3D.normal_mapping) {
        addTangentsToBox(box);
    }

    draw_primitive(box, material, img, filtre, element3D, current_element3D);
}


// fonction qui dessine une sph�re
void Geometrie::draw_sphere(ofMaterial material, ofImage img, ElementScene3DFiltre filtre, ElementScene3D* element3D, ElementScene3D current_element3D) {
    ofSpherePrimitive sphere;
    sphere.setRadius(100);
    draw_primitive(sphere, material, img, filtre, element3D, current_element3D);
}

// fonction qui dessine un cylindre
void Geometrie::draw_cylinder(ofMaterial material, ofImage img, ElementScene3DFiltre filtre, ElementScene3D* element3D, ElementScene3D current_element3D) {
    ofCylinderPrimitive cyl;
    cyl.set(100, 200);
    draw_primitive(cyl, material, img, filtre, element3D, current_element3D);
}

// fonction qui dessine un c�ne
void Geometrie::draw_cone(ofMaterial material, ofImage img, ElementScene3DFiltre filtre, ElementScene3D* element3D, ElementScene3D current_element3D) {
    ofConePrimitive cone;
    cone.set(100, 200);
    draw_primitive(cone, material, img, filtre, element3D, current_element3D);
}

void Geometrie::draw_model(ofxAssimpModelLoader& model, ofMaterial material, ofImage img, ElementScene3DFiltre filtre, ElementScene3D* element3D, ElementScene3D current_element3D) {
    model.disableTextures();
    model.disableMaterials();
    ofEnableDepthTest();

    bool useSpecialFilter = (filtre != ElementScene3DFiltre::none);

    if (useSpecialFilter) {
        ofShader* fshader = get_filter_shader(filtre);
        if (fshader && fshader->isLoaded()) {
            fshader->begin();
            fshader->setUniformMatrix4f("modelViewProjectionMatrix", ofGetCurrentMatrix(OF_MATRIX_MODELVIEW) * ofGetCurrentMatrix(OF_MATRIX_PROJECTION));
            fshader->setUniformTexture("tex0", img.getTexture(), 0);
            model.drawFaces();
            fshader->end();
            return;
        }
    }

    if (current_element3D.normal_mapping) {}
    else if (shader_active && shader_active->isLoaded()) {
        std::vector<ofVec3f> light_positions;
        std::vector<ofVec3f> light_colors;
        std::vector<ofVec3f> light_directions;
        std::vector<int> light_types;
        std::vector<float> spot_cutoffs;
        ofVec3f ambient_sum(0, 0, 0);

        ofMatrix4x4 modelViewMatrix = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW);

        for (int i = 0; i < 30; ++i) {
            const auto& e = element3D[i];
            if (e.type == ElementScene3DType::point_light ||
                e.type == ElementScene3DType::directional_light ||
                e.type == ElementScene3DType::spot_light) {

                ofVec3f world_pos = e.lightAttribute.light.getGlobalPosition();
                ofVec3f view_pos = world_pos * modelViewMatrix;
                glm::vec3 dir = glm::vec3(e.lightAttribute.orientation);

                glm::vec3 view_dir = glm::mat3(modelViewMatrix) * dir;
                light_directions.push_back(ofVec3f(view_dir)); // transforme pour spot et point

                light_positions.push_back(view_pos);
                light_colors.push_back(e.lightAttribute.diffuseColor / 255.0f); // normalize

                if (e.type == ElementScene3DType::point_light) {
                    light_types.push_back(0);
                    spot_cutoffs.push_back(0.0f);
                }
                else if (e.type == ElementScene3DType::directional_light) {
                    light_types.push_back(1);
                    spot_cutoffs.push_back(0.0f);
                }
                else if (e.type == ElementScene3DType::spot_light) {
                    light_types.push_back(2);
                    spot_cutoffs.push_back(cos(ofDegToRad(e.lightAttribute.lightCutOff)));
                }
            }
            else if (e.type == ElementScene3DType::ambiant) {
                ambient_sum += e.lightAttribute.diffuseColor / 255.0f;
            }
        }

        shader_active->begin();

        send_common_matrices(shader_active);
        shader_active->setUniform3f("mat_ambient", toVec3f(material.getAmbientColor()));
        shader_active->setUniform3f("color_diffuse", toVec3f(material.getDiffuseColor()));
        shader_active->setUniform3f("color_specular", toVec3f(material.getSpecularColor()));
        shader_active->setUniform1f("brightness", 50.0f);
        shader_active->setUniform3f("light_ambient", ambient_sum);
        shader_active->setUniform1f("material_roughness", current_element3D.roughness);
        shader_active->setUniform1f("material_metallic", current_element3D.metallic);

        int count = std::min((int)light_positions.size(), 30);
        shader_active->setUniform1i("num_lights", count);
        if (count > 0) {
            shader_active->setUniform3fv("light_positions", &light_positions[0].x, count);
            shader_active->setUniform3fv("light_colors", &light_colors[0].x, count);
            shader_active->setUniform3fv("light_directions", &light_directions[0].x, count);
            shader_active->setUniform1iv("light_type", &light_types[0], count);
            shader_active->setUniform1fv("spot_cutoffs", &spot_cutoffs[0], count);
        }

        shader_active->setUniformTexture("tex0", img.getTexture(), 0);
        model.drawFaces();
        shader_active->end();
    }
    else {
        model.enableTextures();
        model.enableMaterials();
        material.begin();
        img.bind();
        model.drawFaces();
        img.unbind();
        material.end();
    }
}


void Geometrie::draw_donut(ofMaterial material, ofImage img, ElementScene3DFiltre filtre, ElementScene3D* element3D, ElementScene3D current_element3D) {
    draw_model(donut, material, img, filtre, element3D, current_element3D);
}

void Geometrie::draw_plate(ofMaterial material, ofImage img, ElementScene3DFiltre filtre, ElementScene3D* element3D, ElementScene3D current_element3D) {
    draw_model(plate, material, img, filtre, element3D, current_element3D);
}

void Geometrie::draw_spaghetti_getter(ofMaterial material, ofImage img, ElementScene3DFiltre filtre, ElementScene3D* element3D, ElementScene3D current_element3D) {
    draw_model(spaghetti_getter, material, img, filtre, element3D, current_element3D);
}

void Geometrie::draw_bezier_curve() {
    if (use_tessellation_gpu)
        draw_bezier_surface_gpu();
    else {
        ofSetColor(39, 107, 5);
        mesh.drawWireframe();
        
        
    }
    update_mesh();
    // Dessiner les points de contr�le
    ofSetColor(255, 0, 0);
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            ofDrawEllipse(control_grid[i][j], radius, radius);
}

void Geometrie::setup_tessellation_surface() {
    tessellation_patch.clear();
    tessellation_patch.setMode(OF_PRIMITIVE_PATCHES);

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            tessellation_patch.addVertex(control_grid[i][j]); // 16 sommets

    glPatchParameteri(GL_PATCH_VERTICES, 16);
}

void Geometrie::draw_bezier_surface_gpu() {
    glPatchParameteri(GL_PATCH_VERTICES, 16);
    update_tessellation_patch();
    tessellation_shader.begin();
    send_common_matrices(&tessellation_shader); // si tu veux projeter
    ofPushStyle();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // mode wireframe
    tessellation_patch.draw();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // retour au mode normal
    ofPopStyle();
    tessellation_shader.end();
}

void Geometrie::update_tessellation_patch() {
    tessellation_patch.clear();
    tessellation_patch.setMode(OF_PRIMITIVE_PATCHES);

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            tessellation_patch.addVertex(control_grid[i][j]); // points actualisés
}

void Geometrie::set_projection_mode(bool mode) {
    projection_mode = mode;
}

void Geometrie::bezier_bicubic(
    float u, float v,
    const ofVec3f control_points[4][4],
    float& x, float& y, float& z)
{
    float bu[4] = {
      (1 - u) * (1 - u) * (1 - u),
      3 * u * (1 - u) * (1 - u),
      3 * u * u * (1 - u),
      u * u * u
    };

    float bv[4] = {
      (1 - v) * (1 - v) * (1 - v),
      3 * v * (1 - v) * (1 - v),
      3 * v * v * (1 - v),
      v * v * v
    };

    x = y = z = 0.0f;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
        {
            float b = bu[i] * bv[j];
            x += b * control_points[i][j].x;
            y += b * control_points[i][j].y;
            z += b * control_points[i][j].z;
        }
}

void Geometrie::update_mesh()
{
    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);

    // G�n�ration des sommets
    for (int i = 0; i <= resolution_u; ++i)
    {
        float u = i / (float)resolution_u;
        for (int j = 0; j <= resolution_v; ++j)
        {
            float v = j / (float)resolution_v;
            float x, y, z;
            bezier_bicubic(u, v, control_grid, x, y, z);
            mesh.addVertex(ofVec3f(x, y, z));
        }
    }

    // Indexation du maillage
    for (int i = 0; i < resolution_u; ++i)
    {
        for (int j = 0; j < resolution_v; ++j)
        {
            int idx1 = i * (resolution_v + 1) + j;
            int idx2 = (i+1) * (resolution_v + 1) + j;
            int idx3 = idx1 + 1;
            int idx4 = idx2 + 1;

            mesh.addTriangle(idx1, idx2, idx4);
            mesh.addTriangle(idx1, idx4, idx3);
        }
    }
}

ofVec3f Geometrie::toVec3f(const ofColor& c) {
    return ofVec3f(c.r / 255.0f, c.g / 255.0f, c.b / 255.0f);
}

void Geometrie::send_common_matrices(ofShader* shader)
{
    ofMatrix4x4 modelViewMatrix = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW);
    ofMatrix4x4 projectionMatrix = ofGetCurrentMatrix(OF_MATRIX_PROJECTION);

    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelViewMatrix)));

    shader->setUniformMatrix4f("modelViewMatrix", modelViewMatrix);
    shader->setUniformMatrix4f("projectionMatrix", projectionMatrix);
    shader->setUniformMatrix3f("normalMatrix", normalMatrix);
}

ofShader* Geometrie::get_filter_shader(ElementScene3DFiltre filtre) {
    switch (filtre) {
    case ElementScene3DFiltre::blur: return &filtre_Blur;
    case ElementScene3DFiltre::grayscale: return &filtre_Grayscale;
    case ElementScene3DFiltre::vignette: return &filtre_Vignette;
    case ElementScene3DFiltre::mexico: return &filtre_Mexico;
    case ElementScene3DFiltre::invert: return &filtre_Invert;
    case ElementScene3DFiltre::none:
    default:
        return &filtre_None;
    }
}

ofTexture Geometrie::getRelief(ElementScene3DTexture tex) const {
    switch (tex) {
    case ElementScene3DTexture::wood: return normal_Wood.getTexture();
    case ElementScene3DTexture::sand: return normal_Sand.getTexture();
    case ElementScene3DTexture::briks: return normal_Briks.getTexture();
    case ElementScene3DTexture::honeycomb: return normal_Honeycomb.getTexture();
    case ElementScene3DTexture::sponge: return normal_Sponge.getTexture();
    default: return texture_None.getTexture();
    }
}

void Geometrie::addTangentsToBox(ofBoxPrimitive& box) {
    ofMesh& mesh = box.getMesh();
    computeNormals(mesh);
    const auto& vertices = mesh.getVertices();
    const auto& texcoords = mesh.getTexCoords();
    const auto& indices = mesh.getIndices();

    std::vector<ofVec3f> tangents(vertices.size(), ofVec3f(0, 0, 0));

    for (size_t i = 0; i + 2 < indices.size(); i += 3) {
        int i0 = indices[i];
        int i1 = indices[i + 1];
        int i2 = indices[i + 2];

        const ofVec3f& v0 = vertices[i0];
        const ofVec3f& v1 = vertices[i1];
        const ofVec3f& v2 = vertices[i2];

        const ofVec2f& uv0 = texcoords[i0];
        const ofVec2f& uv1 = texcoords[i1];
        const ofVec2f& uv2 = texcoords[i2];

        ofVec3f edge1 = v1 - v0;
        ofVec3f edge2 = v2 - v0;
        ofVec2f deltaUV1 = uv1 - uv0;
        ofVec2f deltaUV2 = uv2 - uv0;

        float f = deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y;
        if (fabs(f) < 1e-6f) continue;

        float invF = 1.0f / f;
        ofVec3f tangent = invF * (deltaUV2.y * edge1 - deltaUV1.y * edge2);

        tangents[i0] += tangent;
        tangents[i1] += tangent;
        tangents[i2] += tangent;
    }

    // Normalisation des tangentes
    for (auto& t : tangents)
        t.normalize();

    // Allocation GPU des buffers
    vertexBuffer.allocate(vertices, GL_STATIC_DRAW);
    normalBuffer.allocate(mesh.getNormals(), GL_STATIC_DRAW);
    texcoordBuffer.allocate(texcoords, GL_STATIC_DRAW);
    tangentBuffer.allocate(tangents, GL_STATIC_DRAW);

    // Liaison des données au VBO
    tangentVbo.setMesh(mesh, GL_STATIC_DRAW);
    tangentVbo.setIndexData(indices.data(), indices.size(), GL_STATIC_DRAW); // Correction ici

    tangentVbo.setAttributeBuffer(0, vertexBuffer, 3, sizeof(ofVec3f), 0);   // position
    tangentVbo.setAttributeBuffer(1, normalBuffer, 3, sizeof(ofVec3f), 0);   // normal
    tangentVbo.setAttributeBuffer(2, texcoordBuffer, 2, sizeof(ofVec2f), 0); // texcoord
    tangentVbo.setAttributeBuffer(6, tangentBuffer, 3, sizeof(ofVec3f), 0);  // tangent
}

void Geometrie::computeNormals(ofMesh& mesh) {
    auto& vertices = mesh.getVertices();
    auto& indices = mesh.getIndices();
    std::vector<ofVec3f> normals(vertices.size(), ofVec3f(0, 0, 0));

    for (size_t i = 0; i < indices.size(); i += 3) {
        int ia = indices[i];
        int ib = indices[i + 1];
        int ic = indices[i + 2];

        ofVec3f a = vertices[ia];
        ofVec3f b = vertices[ib];
        ofVec3f c = vertices[ic];

        ofVec3f normal = (b - a).cross(c - a).normalize();
        normals[ia] += normal;
        normals[ib] += normal;
        normals[ic] += normal;
    }

    for (auto& n : normals)
        n.normalize();

    mesh.clearNormals();
    for (const auto& n : normals)
        mesh.addNormal(n);
}


