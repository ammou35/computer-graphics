#include "geometrie.h"




void Geometrie::setup()
{
    // Paramètres généraux
    //scale_teapot = 1.5f;
    //rotation_speed = 0.3f;
    //use_rotation = true;
    //bounding_box = true;       // Affichage de la boîte de délimitation
    //animation_speed = 0.02f;   // Vitesse d’animation
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

    // Génération des points de contrôle en grille 4x4
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

    // Sélectionner le shader courant
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
    ofLogNotice() << "Blinn-Phong loaded: " << blinn_phong_shader.isLoaded();

    // Default shader active
    shader_active = &blinn_phong_shader;
    shader_mode = -1;

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
            float hue = fmod((tileX + tileY) * 30.0f, 255.0f); // 30° par case diagonale
            ofColor color;
            color.setHsb(hue, 255, 255); // teinte vive

            texture_Checkerboard.setColor(x, y, color);
        }
    }

    texture_Checkerboard.update(); // Envoie la texture au GPU

    skybox.setup("textures/skybox.hdr");
}

void Geometrie::update()
{
    switch (shader_mode)
    {
    case 1:
        shader_active = &lambert_shader;
        shader_name = "Lambert";
        shader_active->begin();
        shader_active->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
        shader_active->setUniform3f("color_diffuse", 0.6f, 0.6f, 0.6f);
        shader_active->setUniform1f("brightness", 0.0f);
        shader_active->setUniform3f("light_position", light_point.getGlobalPosition());
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
        shader_active->setUniform3f("light_position", light_point.getGlobalPosition());
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
        shader_active->setUniform3f("light_position", light_point.getGlobalPosition());
        shader_active->end();
        break;

    case 4:
        shader_active = &blinn_phong_shader;
        shader_name = "Blinn-Phong";
        shader_active->begin();
        shader_active->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
        shader_active->setUniform3f("color_diffuse", 0.0f, 0.6f, 0.6f);
        shader_active->setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
        shader_active->setUniform1f("brightness", 10.0f);
        shader_active->setUniform3f("light_position", light_point.getGlobalPosition());
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

void Geometrie::draw_cube(ofMaterial material, ofImage img, ElementScene3DFiltre filtre) {
    ofEnableDepthTest();

    ofBoxPrimitive box;
    box.set(200);
    box.getMesh().enableNormals();
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
            shader_active->begin();

            send_common_matrices(shader_active);
            shader_active->setUniform3f("color_ambient", toVec3f(material.getAmbientColor()));
            shader_active->setUniform3f("color_diffuse", toVec3f(material.getDiffuseColor()));
            shader_active->setUniform3f("color_specular", toVec3f(material.getSpecularColor()));
            shader_active->setUniform1f("brightness", material.getShininess());
            shader_active->setUniform3f("light_position", light_point.getGlobalPosition());
            shader_active->setUniformTexture("tex", img.getTexture(), 0);

            img.getTexture().bind();
            box.drawFaces();
            img.getTexture().unbind();

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
}





// fonction qui dessine une sphère
void Geometrie::draw_sphere(ofMaterial material, ofImage img)
{
    ofEnableDepthTest();

    ofSpherePrimitive sphere;
    sphere.set(50, 10);
    
    if (shader_active) {
        shader_active->begin();

        // propriétés du matériau
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

// fonction qui dessine un cône
void Geometrie::draw_cone() const
{
    ofDrawCone(0, 0, 0, 50, 100);
}

// fonction qui dessine un donut
void Geometrie::draw_donut()
{
    // configurer le matériau du teapot
    ofEnableDepthTest();

    donut.draw(OF_MESH_FILL);
    //donutImage.getTexture().bind();
    //donut.drawFaces();
    //donutImage.getTexture().unbind();

    // désactiver le matériau
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

// fonction qui dessine un cône
void Geometrie::draw_spaghetti_getter()
{
    ofEnableDepthTest();
    spaghetti_getter.draw(OF_MESH_FILL);
    ofDisableDepthTest();
}

void Geometrie::draw_bezier_curve() {
    ofSetColor(39, 107, 5);
    mesh.drawWireframe();
    update_mesh();
    // Dessiner les points de contrôle
    ofSetColor(255, 0, 0);
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            ofDrawEllipse(control_grid[i][j], radius, radius);
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

    // Génération des sommets
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
    ofMatrix4x4 modelViewProjectionMatrix = modelViewMatrix * projectionMatrix;

    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelViewMatrix)));
    shader->setUniformMatrix3f("normalMatrix", normalMatrix);
    shader->setUniformMatrix4f("modelViewMatrix", modelViewMatrix);
    shader->setUniformMatrix4f("projectionMatrix", projectionMatrix);

    // Light position en espace vue
    ofVec3f lightPosWorld = light_point.getGlobalPosition();
    ofVec3f lightPosView = lightPosWorld * modelViewMatrix;
    shader->setUniform3f("light_position", lightPosView);
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
