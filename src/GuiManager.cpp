#include "guiManager.h"

float lineWidth = 3.0f;

void GuiManager::setup() {
    set_type_vector_primitive(-1);
    set_is_visible(true);
    ui_color_picker = false;
    gui.setup();
    ImGuiStyle& style = ImGui::GetStyle();
    ui_color = ImVec4(0.035f, 0.102f, 0.027f, 1.0f);
    style.Colors[ImGuiCol_WindowBg] = ui_color;
    style.Colors[ImGuiCol_MenuBarBg] = ui_color;
    style.Colors[ImGuiCol_Tab] = ImVec4(0.2f, 0.2f, 0.8f, 1.0f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.3f, 0.3f, 0.9f, 1.0f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.1f, 0.1f, 0.7f, 1.0f);
    item_selected = 0;
    item_selected3D = 0;
    delimitation = false;
    sky_box = false;
    projection = false;
    lineWidth = 3.0f;
    tesselation = ImVec2(20.0f, 20.0f);
    is_selected_image = false;
    showGraph3DTransformation = false;
    showGraph3DMats = false;
    showGraph3DLights = false;
    element3D_material = -1;
    shader_mode = -1;
    element3D_texture = -1;
    element3D_filtre = -1;
    exposure = 1.0f;
}

void GuiManager::update(Graph& graph) {
    if (imagePath.size() > 0) {
        graph.load_image(imagePath[0]);
        imagePath.pop_back();
    }
    int existe_image_selectionne = 0;
    for (int i = 0; i < 30; i++) {
        if (graph.element2D[i].type == ElementScene2DType::image && graph.element2D[i].is_selected) {
            existe_image_selectionne = 1;
            if (!is_selected_image) {
                is_selected_image = 1;
                selected_image = graph.images[graph.element2D[i].image_nb];
                calculate_histogram();
            }
        }
    }
    if (existe_image_selectionne == 0) {
        is_selected_image = false;
    }
    graph.geometrie.resolution_u = tesselation.x;
    graph.geometrie.resolution_v = tesselation.y;
}


void GuiManager::draw(ElementScene2D* element2D, ElementScene3D* element3D, const vector<ofImage>& images) {
    gui.begin();

    // Panneau du dessu
    // Save the original font size
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    // Increase the font size temporarily for the menu bar
    ImGui::GetIO().FontGlobalScale = 1.5f;
    // Menu bar without extra space
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open", "Ctrl+O")) { /* Do stuff */ }
            if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
            if (ImGui::MenuItem("Exit")) { /* Do stuff */ }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) { /* Do stuff */ }
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) { /* Do stuff */ }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Image")) {
            if (ImGui::MenuItem("Import")) { 
                ofFileDialogResult result = ofSystemLoadDialog("Sélectionner une image", false);
                if (result.bSuccess) {
                    // Si l'utilisateur a choisi un fichier, on récupère son chemin
                    imagePath.push_back(result.getPath());
                }
            }
            if (ImGui::MenuItem("Export")) { /* Do stuff */ }

            //if (image.isAllocated()) {
            //    image.draw(10, 100);  // Afficher l'image dans la fenêtre
            //}
            //else {
            //    ImGui::Text("Erreur lors du chargement de l'image !");
            //}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Hide UI", "Ctrl+H")) { set_is_visible(false); }
            if (ImGui::MenuItem("UI color picker")) { ui_color_picker = true; }
            if (ImGui::MenuItem("Projection orthogonale")) { projection = false; }
            if (ImGui::MenuItem("Projection perspective")) { projection = true; }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    ImGui::PopFont(); // Restore the original font

    // Panneau de gauche
    //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // Remove padding
    ImGui::SetNextWindowPos(ImVec2(0, 24));  // Below top panel
    ImGui::SetNextWindowSize(ImVec2(300, ofGetHeight() - 24));  // 200px width, full height minus top panel
    ImGui::Begin("Left Panel", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
    //ImGui::Text("This is a fixed left panel");

    if (ImGui::CollapsingHeader("Images")) {
        if (ImGui::BeginTabBar("ImagesTabBar")) {
            if (ImGui::BeginTabItem("Added images")) {
                for (int i = 0; i < images.size(); i++) {
                    ImGui::PushID(i);
                    int currentType = get_type_vector_primitive();
                    bool isSelected = (currentType == 20 + i);
                    if (ImGui::RadioButton("Image", isSelected)) {
                        set_type_vector_primitive(20 + i);
                    }
                    ImGui::PopID();
                }
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }

    if (ImGui::CollapsingHeader("Vector drawing")) {
        if (ImGui::BeginTabBar("Tabs")) {
            if (ImGui::BeginTabItem("Primitives")) {
                if (ImGui::RadioButton("Pixel", get_type_vector_primitive() == 1)) {
                    set_type_vector_primitive(1);
                }
                if (ImGui::RadioButton("Point", get_type_vector_primitive() == 2)) {
                    set_type_vector_primitive(2);
                }
                if (ImGui::RadioButton("Line", get_type_vector_primitive() == 3)) {
                    set_type_vector_primitive(3);
                }
                if (ImGui::RadioButton("Rectangle", get_type_vector_primitive() == 4)) {
                    set_type_vector_primitive(4);
                }
                if (ImGui::RadioButton("Ellipses", get_type_vector_primitive() == 5)) {
                    set_type_vector_primitive(5);
                }
                if (ImGui::RadioButton("Courbe parametrique", get_type_vector_primitive() == 6)) {
                    set_type_vector_primitive(6);
                }
                if (ImGui::RadioButton("Polygone", get_type_vector_primitive() == 7)) {
                    set_type_vector_primitive(7);
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Shapes")) {
                if (ImGui::RadioButton("Star", get_type_vector_primitive() == 8)) {
                    set_type_vector_primitive(8);
                }
                if (ImGui::RadioButton("Heart", get_type_vector_primitive() == 9)) {
                    set_type_vector_primitive(9);
                }
                if (ImGui::RadioButton("Moon", get_type_vector_primitive() == 10)) {
                    set_type_vector_primitive(10);
                }
                if (ImGui::RadioButton("Arrow", get_type_vector_primitive() == 11)) {
                    set_type_vector_primitive(11);
                }
                if (ImGui::RadioButton("Clover", get_type_vector_primitive() == 12)) {
                    set_type_vector_primitive(12);
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Colors")) {
                // Convert ofColor to ImVec4 for ImGui
                ImVec4 im_bg_color = ImVec4(
                    bg_color.r / 255.0f,
                    bg_color.g / 255.0f,
                    bg_color.b / 255.0f,
                    bg_color.a / 255.0f
                );

                ImVec4 im_stroke_color = ImVec4(
                    stroke_color.r / 255.0f,
                    stroke_color.g / 255.0f,
                    stroke_color.b / 255.0f,
                    stroke_color.a / 255.0f
                );

                ImVec4 im_fill_color = ImVec4(
                    fill_color.r / 255.0f,
                    fill_color.g / 255.0f,
                    fill_color.b / 255.0f,
                    fill_color.a / 255.0f
                );

                // Display a small color button
                if (ImGui::ColorButton("Bg Color", im_bg_color)) {
                    ImGui::OpenPopup("Bg Color Picker"); // Open the color picker popup
                }

                ImGui::SameLine();
                ImGui::Text("Background Color");

                // Display the color picker in a popup
                if (ImGui::BeginPopup("Bg Color Picker")) {
                    if (ImGui::ColorPicker4("##Picker", (float*)&im_bg_color,
                        ImGuiColorEditFlags_DisplayRGB | // Show RGB values
                        ImGuiColorEditFlags_DisplayHSV | // Show HSV values
                        ImGuiColorEditFlags_PickerHueWheel | // Use a hue wheel
                        ImGuiColorEditFlags_AlphaPreview)) { // Show alpha preview
                        // Update the background color when the color picker changes
                        bg_color.set(
                            im_bg_color.x * 255,
                            im_bg_color.y * 255,
                            im_bg_color.z * 255,
                            im_bg_color.w * 255
                        );
                    }
                    ImGui::EndPopup();
                }

                // Display a small color button
                if (ImGui::ColorButton("Stroke Color", im_stroke_color)) {
                    ImGui::OpenPopup("strk Color Picker"); // Open the color picker popup
                }

                ImGui::SameLine();
                ImGui::Text("Stroke Color");

                // Display the color picker in a popup
                if (ImGui::BeginPopup("strk Color Picker")) {
                    if (ImGui::ColorPicker4("##Picker", (float*)&im_stroke_color,
                        ImGuiColorEditFlags_DisplayRGB | // Show RGB values
                        ImGuiColorEditFlags_DisplayHSV | // Show HSV values
                        ImGuiColorEditFlags_PickerHueWheel | // Use a hue wheel
                        ImGuiColorEditFlags_AlphaPreview)) { // Show alpha preview
                        stroke_color.set(
                            im_stroke_color.x * 255,
                            im_stroke_color.y * 255,
                            im_stroke_color.z * 255,
                            im_stroke_color.w * 255
                        );
                    }
                    ImGui::EndPopup();
                }

                // Display a small color button
                if (ImGui::ColorButton("Fill Color", im_fill_color)) {
                    ImGui::OpenPopup("Fill Color"); // Open the color picker popup
                }

                ImGui::SameLine();
                ImGui::Text("Fill Color");

                // Display the color picker in a popup
                if (ImGui::BeginPopup("Fill Color")) {
                    if (ImGui::ColorPicker4("##Picker", (float*)&im_fill_color,
                        ImGuiColorEditFlags_DisplayRGB | // Show RGB values
                        ImGuiColorEditFlags_DisplayHSV | // Show HSV values
                        ImGuiColorEditFlags_PickerHueWheel | // Use a hue wheel
                        ImGuiColorEditFlags_AlphaPreview)) { // Show alpha preview
                        fill_color.set(
                            im_fill_color.x * 255,
                            im_fill_color.y * 255,
                            im_fill_color.z * 255,
                            im_fill_color.w * 255
                        );
                    }
                    ImGui::EndPopup();
                }

                // Slider to adjust line width
                ImGui::Text("Adjust Line Width:");
                ImGui::SliderFloat("##lineWidth", &lineWidth, 1.0f, 30.0f, "%.1f px");

                // Get window draw list for custom rendering inside ImGui
                ImDrawList* drawList = ImGui::GetWindowDrawList();

                // Define preview area size
                ImVec2 p = ImGui::GetCursorScreenPos();
                float previewWidth = ImGui::GetContentRegionAvail().x;
                float previewHeight = 40.0f; // Fixed height for a clean look

                // Background rectangle for preview
                drawList->AddRectFilled(
                    ImVec2(p.x, p.y),
                    ImVec2(p.x + previewWidth, p.y + previewHeight),
                    IM_COL32(50, 50, 50, 255) // Dark grey background
                );

                // Draw the line preview
                drawList->AddLine(
                    ImVec2(p.x + 10, p.y + previewHeight / 2),  // Start point
                    ImVec2(p.x + previewWidth - 10, p.y + previewHeight / 2), // End point
                    IM_COL32(255, 255, 255, 255), // White color
                    lineWidth // Dynamic width
                );

                // Add spacing so the next widget doesn't overlap
                ImGui::Dummy(ImVec2(0, previewHeight + 5));
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();  // Close tab bar
        }
    }
    if (ImGui::CollapsingHeader("3D"), ImGuiTreeNodeFlags_DefaultOpen) {
        if (ImGui::BeginTabBar("Tab_lights")) {
            if (ImGui::BeginTabItem("Lights")) {
                if (ImGui::RadioButton("Ambient light", get_type_vector_primitive() == 21)) {
                    set_type_vector_primitive(21);
                }
                if (ImGui::RadioButton("Directionnal light", get_type_vector_primitive() == 22)) {
                    set_type_vector_primitive(22);
                }
                if (ImGui::RadioButton("Point light", get_type_vector_primitive() == 23)) {
                    set_type_vector_primitive(23);
                }
                if (ImGui::RadioButton("Spot light", get_type_vector_primitive() == 24)) {
                    set_type_vector_primitive(24);
                }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Light models")) {
                if (ImGui::RadioButton("Default", shader_mode == -1)) {
                    shader_mode = -1;
                }
                if (ImGui::RadioButton("Lambert", shader_mode == 1)) {
                    shader_mode = 1;
                }
                if (ImGui::RadioButton("Gouraud", shader_mode == 2)) {
                    shader_mode = 2;
                }
                if (ImGui::RadioButton("Phong", shader_mode == 3)) {
                    shader_mode = 3;
                }
                if (ImGui::RadioButton("Blinn-Phong", shader_mode == 4)) {
                    shader_mode = 4;
                }
                if (ImGui::RadioButton("Pbr", shader_mode == 5)) {
                    shader_mode = 5;
                }
                if (ImGui::RadioButton("Flat shading", shader_mode == 6)) {
                    shader_mode = 6;
                }
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
        if (ImGui::Checkbox("Skybox", &sky_box)) {}
        ImGui::Text("Exposition :");
        ImGui::SliderFloat("##Exposure", &exposure, 0.0f, 5.0f, "%.01f");
        if (ImGui::Checkbox("Delimitation box", &delimitation)) {}
        if (ImGui::BeginTabBar("Tabs2")) {
            if (ImGui::BeginTabItem("Primitives")) {
                if (ImGui::RadioButton("Cube", get_type_vector_primitive() == 13)) {
                    set_type_vector_primitive(13);
                }
                if (ImGui::RadioButton("Sphere", get_type_vector_primitive() == 14)) {
                    set_type_vector_primitive(14);
                }
                if (ImGui::RadioButton("Cylindre", get_type_vector_primitive() == 15)) {
                    set_type_vector_primitive(15);
                }
                if (ImGui::RadioButton("Cone", get_type_vector_primitive() == 16)) {
                    set_type_vector_primitive(16);
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Model")) {
                if (ImGui::RadioButton("Donut", get_type_vector_primitive() == 17)) {
                    set_type_vector_primitive(17);
                }
                if (ImGui::RadioButton("Plate", get_type_vector_primitive() == 18)) {
                    set_type_vector_primitive(18);
                }
                if (ImGui::RadioButton("Spaghetti Getter", get_type_vector_primitive() == 19)) {
                    set_type_vector_primitive(19);
                }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Surfaces")) {
                if (ImGui::RadioButton("Bezier bicubic surface", get_type_vector_primitive() == 20)) {
                    if (get_type_vector_primitive() == 20) {
                        set_type_vector_primitive(-1); // Décocher si déjà sélectionné
                    }
                    else {
                        set_type_vector_primitive(20);  // Sinon sélectionner
                    }

                }
                ImGui::EndTabItem();
                ImGui::Text("Adjust tesselation x:");
                ImGui::SliderFloat("##tesselation1", &tesselation.x, 1.0f, 100.0f, "%.1f");
                ImGui::Text("Adjust tesselation y:");
                ImGui::SliderFloat("##tesselation2", &tesselation.y, 1.0f, 100.0f, "%.1f");
            }
            ImGui::EndTabBar();
        }
    }

    if (ImGui::CollapsingHeader("Graph", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::TreeNode("2D elements")) {
            for (int i = 0; i < 30; i++) {
                if (element2D[i].type != ElementScene2DType::none) {
                    ImGui::PushID(i);
                    if (ImGui::Selectable(element2D[i].name, element2D[i].is_selected)) {
                        if (element2D[i].is_selected) {
                            item_selected -= 1;
                        }
                        else {
                            item_selected += 1;
                            if (item_selected == 1) {
                                if (element2D[i].type != ElementScene2DType::image) {
                                    transformation = element2D[i].primitives[0]->transformation;
                                }
                                else {
                                    transformation = element2D[i].transformation;
                                }
                            }
                        }
                        element2D[i].is_selected = !element2D[i].is_selected; // Highlight this item
                    }
                    ImGui::PopID();
                }
            }
            ImGui::PushItemWidth(125.0f);
            ImGui::SliderFloat("Translation X", &transformation[0], -1000.0f, 1000.0f, "%.1f px");
            ImGui::SliderFloat("Translation Y", &transformation[1], -1000.0f, 1000.0f, "%.1f px");
            ImGui::SliderFloat("Translation Z", &transformation[2], -1000.0f, 1000.0f, "%.1f px");
            ImGui::SliderFloat("Rotation", &transformation[3], -360.0f, 360.0f, "%.1f px");
            ImGui::SliderFloat("Proportion X", &transformation[4], -10.0f, 10.0f, "%.1f px");
            ImGui::SliderFloat("Proportion Y", &transformation[5], -10.0f, 10.0f, "%.1f px");
            ImGui::PopItemWidth();
            ImGui::TreePop(); // End of Section 1
            if (ImGui::CollapsingHeader("Histogram")) {
                if (is_selected_image) {
                    ImGui::PlotHistogram("Red Channel", normalizedHistogramR.data(), 256, 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
                    ImGui::PlotHistogram("Green Channel", normalizedHistogramG.data(), 256, 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
                    ImGui::PlotHistogram("Blue Channel", normalizedHistogramB.data(), 256, 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
                }
            }
        }
        ImGui::SetNextItemOpen(true, ImGuiCond_Always);
        if (ImGui::TreeNode("3D elements")) {
            for (int i = 0; i < 30; i++) {
                if (element3D[i].type != ElementScene3DType::none) {
                    ImGui::PushID(i);
                    char nom[20];
                    switch (element3D[i].type) {
                    case ElementScene3DType::none:
                        break;
                    case ElementScene3DType::cube:
                        strcpy(nom, "Cube");
                        break;
                    case ElementScene3DType::sphere:
                        strcpy(nom, "Sphere");
                        break;
                    case ElementScene3DType::cylinder:
                        strcpy(nom, "Cylinder");
                        break;
                    case ElementScene3DType::cone:
                        strcpy(nom, "Cone");
                        break;
                    case ElementScene3DType::donut:
                        strcpy(nom, "Donut");
                        break;
                    case ElementScene3DType::plate:
                        strcpy(nom, "Plate");
                        break;
                    case ElementScene3DType::spaghetti_getter:
                        strcpy(nom, "Spaghetti Getter");
                        break;
                    case ElementScene3DType::bezier_curve:
                        strcpy(nom, "Bezier curve");
                        break;
                    case ElementScene3DType::ambiant:
                        strcpy(nom, "Ambiant light");
                        break;
                    case ElementScene3DType::directional_light:
                        strcpy(nom, "Directional light");
                        break;
                    case ElementScene3DType::point_light:
                        strcpy(nom, "Point light");
                        break;
                    case ElementScene3DType::spot_light:
                        strcpy(nom, "Spot light");
                        break;
                    default:
                        break;
                    }
                    if (ImGui::Selectable(nom, element3D[i].is_selected)) {
                        bool hadMaterial = (
                            element3D[i].type == ElementScene3DType::cube ||
                            element3D[i].type == ElementScene3DType::sphere ||
                            element3D[i].type == ElementScene3DType::cylinder ||
                            element3D[i].type == ElementScene3DType::cone
                            );

                        bool isLight = (
                            element3D[i].type == ElementScene3DType::directional_light ||
                            element3D[i].type == ElementScene3DType::point_light ||
                            element3D[i].type == ElementScene3DType::spot_light ||
                            element3D[i].type == ElementScene3DType::ambiant
                            );

                        bool was_selected = element3D[i].is_selected;
                        element3D[i].is_selected = !was_selected;

                        if (element3D[i].is_selected) {
                            item_selected3D++;
                            if (hadMaterial) item_selected3D_with_material++;

                            if (!isLight)
                                showGraph3DTransformation = true;
                            else
                                showGraph3DLights = true;

                            if (item_selected3D_with_material > 0)
                                showGraph3DMats = true;

                            if (!transformationBufferIsInitialized) {
                                transformation3D = element3D[i].transformation;
                                transformationBufferIsInitialized = true;
                            }
                        }
                        else {
                            item_selected3D--;
                            if (hadMaterial) item_selected3D_with_material--;

                            // Check if anything is still selected
                            bool anySelected = false;
                            bool anyLightSelected = false;

                            for (int j = 0; j < 30; ++j) {
                                if (element3D[j].is_selected) {
                                    anySelected = true;
                                    if (element3D[j].type == ElementScene3DType::directional_light ||
                                        element3D[j].type == ElementScene3DType::point_light ||
                                        element3D[j].type == ElementScene3DType::spot_light ||
                                        element3D[j].type == ElementScene3DType::ambiant) {
                                        anyLightSelected = true;
                                    }
                                }
                            }

                            showGraph3DLights = anyLightSelected;

                            if (!anySelected) {
                                showGraph3DTransformation = false;
                                showGraph3DMats = false;
                                showGraph3DLights = false;
                                transformation3D.fill(0.0f);
                                transformationBufferIsInitialized = false;
                            }

                            if (item_selected3D_with_material <= 0)
                                showGraph3DMats = false;
                        }
                    }
                    ImGui::PopID();
                }
            }
            if (showGraph3DTransformation){
                if (ImGui::CollapsingHeader("Transformations")) {
                    ImGui::PushItemWidth(125.0f);
                    ImGui::SliderFloat("Translation X", &transformation3D[0], -1000.0f, 1000.0f, "%.1f px");
                    ImGui::SliderFloat("Translation Y", &transformation3D[1], -1000.0f, 1000.0f, "%.1f px");
                    ImGui::SliderFloat("Translation Z", &transformation3D[2], -1000.0f, 1000.0f, "%.1f px");
                    ImGui::SliderFloat("Rotation X", &transformation3D[3], -360.0f, 360.0f, "%.1f px");
                    ImGui::SliderFloat("Rotation Y", &transformation3D[4], -360.0f, 360.0f, "%.1f px");
                    ImGui::SliderFloat("Rotation Z", &transformation3D[5], -360.0f, 360.0f, "%.1f px");
                    ImGui::SliderFloat("Proportion X", &transformation3D[6], -10.0f, 10.0f, "%.1f px");
                    ImGui::SliderFloat("Proportion Y", &transformation3D[7], -10.0f, 10.0f, "%.1f px");
                    ImGui::SliderFloat("Proportion Z", &transformation3D[8], -10.0f, 10.0f, "%.1f px");
                    ImGui::PopItemWidth();
                }
            }
            if (showGraph3DLights) {
                if (ImGui::CollapsingHeader("Light Settings")) {
                    for (int i = 0; i < 30; ++i) {
                        if (element3D[i].is_selected &&
                            (element3D[i].type == ElementScene3DType::directional_light ||
                                element3D[i].type == ElementScene3DType::point_light ||
                                element3D[i].type == ElementScene3DType::spot_light ||
                                element3D[i].type == ElementScene3DType::ambiant)) {

                            ImGui::PushID(i);
                            ImGui::Text("Light %d", i);

                            // Show editable position vector
                            ImGui::InputFloat3("Position", element3D[i].lightAttribute.position.getPtr(), "%.1f");
                            element3D[i].lightAttribute.light.setGlobalPosition(element3D[i].lightAttribute.position);

                            // Optionally also allow rotation or direction
                            if (element3D[i].type == ElementScene3DType::directional_light ||
                                element3D[i].type == ElementScene3DType::spot_light) {
                                ImGui::InputFloat3("Direction x (Euler)", element3D[i].lightAttribute.orientation.getPtr(), "%.1f");
                                element3D[i].lightAttribute.light.lookAt(element3D[i].lightAttribute.orientation);
                            }

                            if (element3D[i].type == ElementScene3DType::directional_light ||
                                element3D[i].type == ElementScene3DType::spot_light ||
                                element3D[i].type == ElementScene3DType::point_light) {
                                ImGui::InputFloat3("Diffuse", element3D[i].lightAttribute.diffuseColor.getPtr(), "%.1f");
                                ImGui::InputFloat3("Specular", element3D[i].lightAttribute.specularColor.getPtr(), "%.1f");
                                element3D[i].lightAttribute.light.setDiffuseColor(ofColor(element3D[i].lightAttribute.diffuseColor.x, element3D[i].lightAttribute.diffuseColor.y, element3D[i].lightAttribute.diffuseColor.z));
                                element3D[i].lightAttribute.light.setSpecularColor(ofColor(element3D[i].lightAttribute.specularColor.x, element3D[i].lightAttribute.specularColor.y, element3D[i].lightAttribute.specularColor.z));
                            }
                            else {
                                ImGui::InputFloat3("Ambient", element3D[i].lightAttribute.diffuseColor.getPtr(), "%.1f");
                            }
                            ImGui::PopID();
                        }
                    }
                }
            }
            if (showGraph3DMats) {
                if (ImGui::CollapsingHeader("Proprieties")) {
                    ImGui::PushItemWidth(125.0f);
                    if (ImGui::BeginTabBar("Tabs2")) {
                        if (ImGui::BeginTabItem("Materials")) {
                            if (ImGui::RadioButton("None", element3D_material == -1)) {
                                element3D_material = -1;
                            }
                            if (ImGui::RadioButton("Volcanic Rock", element3D_material == 1)) {
                                element3D_material = 1;
                            }
                            if (ImGui::RadioButton("Frozen Crystal", element3D_material == 2)) {
                                element3D_material = 2;
                            }
                            if (ImGui::RadioButton("Mossy Stone", element3D_material == 3)) {
                                element3D_material = 3;
                            }
                            if (ImGui::RadioButton("Neon Tech", element3D_material == 4)) {
                                element3D_material = 4;
                            }
                            if (ImGui::RadioButton("Ancient Bronze", element3D_material == 5)) {
                                element3D_material = 5;
                            }
                            ImGui::EndTabItem();
                        }
                        if (ImGui::BeginTabItem("Textures")) {
                            if (ImGui::RadioButton("None", element3D_texture == -1)) {
                                element3D_texture = -1;
                            }
                            if (ImGui::RadioButton("Wood", element3D_texture == 1)) {
                                element3D_texture = 1;
                            }
                            if (ImGui::RadioButton("Sand", element3D_texture == 2)) {
                                element3D_texture = 2;
                            }
                            if (ImGui::RadioButton("Bricks", element3D_texture == 3)) {
                                element3D_texture = 3;
                            }
                            if (ImGui::RadioButton("Honeycomb", element3D_texture == 4)) {
                                element3D_texture = 4;
                            }
                            if (ImGui::RadioButton("Spong", element3D_texture == 5)) {
                                element3D_texture = 5;
                            }
                            if (ImGui::RadioButton("Checker", element3D_texture == 6)) {
                                element3D_texture = 6;
                            }
                            ImGui::EndTabItem();
                        }
                        if (ImGui::BeginTabItem("Filtres")) {
                            if (ImGui::RadioButton("None", element3D_filtre == -1)) {
                                element3D_filtre = -1;
                            }
                            if (ImGui::RadioButton("Blur", element3D_filtre == 1)) {
                                element3D_filtre = 1;
                            }
                            if (ImGui::RadioButton("Grayscale", element3D_filtre == 2)) {
                                element3D_filtre = 2;
                            }
                            if (ImGui::RadioButton("Vignette", element3D_filtre == 3)) {
                                element3D_filtre = 3;
                            }
                            if (ImGui::RadioButton("Mexico", element3D_filtre == 4)) {
                                element3D_filtre = 4;
                            }
                            if (ImGui::RadioButton("Invert", element3D_filtre == 5)) {
                                element3D_filtre = 5;
                            }
                            ImGui::EndTabItem();
                        }
                        ImGui::EndTabBar();
                    }
                    ImGui::PopItemWidth();
                }
            }
            ImGui::TreePop(); // End of Section 1
        }
    }

    ImGui::End();
    //ImGui::PopStyleVar(); // Restore padding

    // UI color picker
    if (ui_color_picker) {
        ImGui::OpenPopup("UI color picker");  // Open the pop-up
    }

    if (ImGui::BeginPopupModal("UI color picker", &ui_color_picker, ImGuiWindowFlags_AlwaysAutoResize)) {
        if (ImGui::ColorEdit4("Background Color", (float*)&ui_color)) {
            ImGuiStyle& style = ImGui::GetStyle();
            style.Colors[ImGuiCol_WindowBg] = ui_color;
            style.Colors[ImGuiCol_MenuBarBg] = ui_color;
        }

        ImGui::Separator();

        float buttonWidth = 100.0f;  // Width of the button
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - buttonWidth - ImGui::GetStyle().WindowPadding.x);
        if (ImGui::Button("Ok", ImVec2(buttonWidth, 0))) {
            ui_color_picker = false;  // Close the pop-up
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    gui.end();
}

int GuiManager::get_type_vector_primitive(void) const {
    return typeVectorPrimitive_;
}

void GuiManager::set_type_vector_primitive(const int& typeVectorPrimitive) {
    typeVectorPrimitive_ = typeVectorPrimitive;
}

ofColor GuiManager::get_vector_stroke_color(void) const {
    return stroke_color;
}

ofColor GuiManager::get_vector_fill_color(void) const {
    return fill_color;
}

ofColor GuiManager::get_vector_bg_color(void) const {
    return bg_color;
}

void GuiManager::set_is_visible(const bool& visible) {
    is_visible = visible;
}

bool GuiManager::get_is_visible(void) const {
    return is_visible;
}

void GuiManager::calculate_histogram(void) {
    int width = selected_image.getWidth();
    int height = selected_image.getHeight();
    std::vector<int> histogramR(256, 0);
    std::vector<int> histogramG(256, 0);
    std::vector<int> histogramB(256, 0);

    // Calculate the histograms
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            ofColor color = selected_image.getColor(x, y);
            histogramR[color.r]++;
            histogramG[color.g]++;
            histogramB[color.b]++;
        }
    }

    // Normalize the histograms (optional)
    int maxR = *std::max_element(histogramR.begin(), histogramR.end());
    int maxG = *std::max_element(histogramG.begin(), histogramG.end());
    int maxB = *std::max_element(histogramB.begin(), histogramB.end());

    std::vector<float> normalizedHistogramR(256);
    std::vector<float> normalizedHistogramG(256);
    std::vector<float> normalizedHistogramB(256);

    for (int i = 0; i < 256; i++) {
        normalizedHistogramR[i] = static_cast<float>(histogramR[i]) / maxR;
        normalizedHistogramG[i] = static_cast<float>(histogramG[i]) / maxG;
        normalizedHistogramB[i] = static_cast<float>(histogramB[i]) / maxB;
    }
}