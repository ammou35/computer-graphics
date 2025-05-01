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
    ofLogNotice() << "Blinn-Phong loaded: " << blinn_phong_shader.isLoaded();

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
        //shader_active->setUniform1i("num_lights", light_positions.size());

        shader_active->end();
        break;

    case 5:
        break;

    case 6:

        shader_active = &flat_shader;
        shader_name = "Blinn-Phong";
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

void Geometrie::draw_cube(ofMaterial material, ofImage img, ElementScene3DFiltre filtre, ElementScene3D* element3D) {

    ofMatrix4x4 modelViewMatrix = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW); // AJOUT�
    ofEnableDepthTest();
    ofBoxPrimitive box;
    box.set(200);
    box.getMesh().enableNormals();
    box.mapTexCoords(0, 0, 1, 1);
    box.mapTexCoordsFromTexture(img.getTexture()); // ici correct

    bool useSpecialFilter = (filtre != ElementScene3DFiltre::none);

    if (useSpecialFilter) {
        ofShader* fshader = get_filter_shader(filtre);

        if (fshader && fshader->isLoaded()) {
            fshader->begin();

            fshader->setUniformMatrix4f("modelViewProjectionMatrix", ofGetCurrentMatrix(OF_MATRIX_MODELVIEW) * ofGetCurrentMatrix(OF_MATRIX_PROJECTION));
            fshader->setUniformTexture("tex0", img.getTexture(), 0);

            box.drawFaces();

            fshader->end();
        }
    }
    else {
        if (shader_active && shader_active->isLoaded()) {

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
                    light_directions.push_back(ofVec3f(view_dir)); // convert back if needed

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
                } else if (e.type == ElementScene3DType::ambiant) {
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

            box.drawFaces();

            shader_active->end();
        }
        else {
            material.begin();
            img.getTexture().bind();
            box.drawFaces();
            img.getTexture().unbind();
            material.end();
        }
    }

    ofDisableDepthTest();
    light_positions.clear();
    light_colors.clear();
}






// fonction qui dessine une sph�re
void Geometrie::draw_sphere(ofMaterial material, ofImage img)
{
    ofEnableDepthTest();

    ofSpherePrimitive sphere;
    sphere.set(50, 10);
    
    if (shader_active) {
        shader_active->begin();

        // propri�t�s du mat�riau
        shader_active->setUniform3f("color_ambient", toVec3f(material.getAmbientColor()));
        shader_active->setUniform3f("color_diffuse", toVec3f(material.getDiffuseColor()));
        shader_active->setUniform3f("color_specular", toVec3f(material.getSpecularColor()));
        shader_active->setUniform1f("brightness", material.getShininess());

        // texture
        shader_active->setUniformTexture("tex", img.getTexture(), 0);

        send_common_matrices(shader_active);

        sphere.drawFaces();
        shader_active->end();
    }

    ofDisableDepthTest();
}

// fonction qui dessine un cylindre
void Geometrie::draw_cylinder() const
{
    ofDrawCylinder(0, 0, 0, 50, 100);
}

// fonction qui dessine un c�ne
void Geometrie::draw_cone() const
{
    ofDrawCone(0, 0, 0, 50, 100);
}

// fonction qui dessine un donut
void Geometrie::draw_donut()
{
    // configurer le mat�riau du teapot
    ofEnableDepthTest();

    donut.draw(OF_MESH_FILL);
    //donutImage.getTexture().bind();
    //donut.drawFaces();
    //donutImage.getTexture().unbind();

    // d�sactiver le mat�riau
    //material_donut.end();
    //donut.drawWireframe();
    ofDisableDepthTest();
}

// fonction qui dessine une assiette
void Geometrie::draw_plate()
{
    ofEnableDepthTest();
    plate.drawWireframe();
    ofDisableDepthTest();
}

// fonction qui dessine un c�ne
void Geometrie::draw_spaghetti_getter()
{
    ofEnableDepthTest();
    spaghetti_getter.draw(OF_MESH_FILL);
    ofDisableDepthTest();
}

void Geometrie::draw_bezier_curve() {
    if (use_tessellation_gpu)
        draw_bezier_surface_gpu();
    else {
        ofSetColor(39, 107, 5);
        mesh.drawWireframe();
        update_mesh();
        // Dessiner les points de contr�le
        ofSetColor(255, 0, 0);
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                ofDrawEllipse(control_grid[i][j], radius, radius);
    }
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
    tessellation_shader.begin();
    send_common_matrices(&tessellation_shader); // si tu veux projeter
    ofLogNotice() << "tessellation_patch has " << tessellation_patch.getNumVertices() << " vertices.";
    ofLogNotice() << "Mesh mode: " << tessellation_patch.getMode();
    tessellation_patch.draw(); // contient 16 sommets = 1 patch
    tessellation_shader.end();
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
