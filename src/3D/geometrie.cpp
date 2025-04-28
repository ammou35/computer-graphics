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

    int w = ofGetWidth();
    int h = ofGetHeight();

    radius = 10;

    // Génération des points de contrôle en grille 4x4
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
        {
            float x = w * (0.2f + 0.2f * i);
            float y = h * (0.2f + 0.2f * j);
            control_grid[i][j] = ofVec3f(x, y, 0);
        }
    is_bezier_curve = true;

    /*light.setup();
    light.setPosition(300, 300, 300);
    light.enable();*/

    ofColor light_ambient;
    light_ambient.set(127, 127, 127);

    ofSetGlobalAmbientColor(light_ambient);

    // Chargement des shaders
    //shader_lambert.load("lambert_330_vs.glsl", "lambert_330_fs.glsl");
    //shader_normal.load("draw_normal_330_vs.glsl", "draw_normal_330_fs.glsl");

    // Sélectionner le shader courant
    //shader = shader_lambert;

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

    light_point.setDiffuseColor(ofColor(255, 255, 255));
    light_point.setSpecularColor(ofColor(191, 191, 191));
    light_point.setPointLight();
    light_point.setPosition(300, 300, 300);
}

void Geometrie::update()
{
    //// Animation du teapot (ex : oscillation)
    //float anim_offset = sin(ofGetElapsedTimef() * animation_speed) * 20.0f;


    //if (use_rotation)
    //    teapot.setRotation(0, ofGetFrameNum() * rotation_speed, 0.0f, 1.0f, 0.0f);

    // Configuration de la lumière
    //light.setPointLight();
    //light.setDiffuseColor(255);
    //light.setGlobalPosition(center_x, center_y, 255.0f);
}

void Geometrie::draw()
{
//    // Passer les attributs uniformes au shader
//    shader.setUniform3f("color_ambient", color_ambient.r / 255.0f, color_ambient.g / 255.0f, color_ambient.b / 255.0f);
//    shader.setUniform3f("color_diffuse", color_diffuse.r / 255.0f, color_diffuse.g / 255.0f, color_diffuse.b / 255.0f);
//    shader.setUniform3f("light_position", light.getGlobalPosition());
//
//
//    // Instanciation : Duplication du teapot avec variations
//    for (int i = 0; i < instance_count; i++)
//    {
//        float offset = i * 50.0f;
//        float scale_variation = 1.0f + sin(ofGetElapsedTimef() + i) * 0.5f;
//        teapot.setScale(scale_teapot * scale_variation, scale_teapot * scale_variation, scale_teapot * scale_variation);
//        teapot.setPosition(center_x + offset, center_y + 90, 0);
//        teapot.draw(OF_MESH_FILL);
//    }
//
//    // Fin du rendu
//    shader.end();
//    light.disable();
//    ofDisableLighting();
//    ofDisableDepthTest();

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

// fonction qui dessine un cube
void Geometrie::draw_cube(ofMaterial material)
{
    ofEnableDepthTest();

    ofBoxPrimitive box;
    box.set(200);

    light_point.enable();
    ofEnableLighting();

    material.begin();
    box.draw();
    material.end();

    ofDisableLighting();
    light_point.disable();
    ofDisableDepthTest();

    ofDisableDepthTest();
}


// fonction qui dessine une sphère
void Geometrie::draw_sphere() const
{
    ofDrawSphere(0, 0, 0, 50);
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


    // dessiner un teapot
    donut.draw(OF_MESH_FILL);
    // désactiver le matériau
    //material_donut.end();
    //donut.drawWireframe();
    //donut.drawFaces();
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
            int idx2 = (i + 1) * (resolution_v + 1) + j;
            int idx3 = idx1 + 1;
            int idx4 = idx2 + 1;

            mesh.addTriangle(idx1, idx2, idx3);
            mesh.addTriangle(idx2, idx4, idx3);
        }
    }
}