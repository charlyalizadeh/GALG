#include "../inc/ModifiedPGE.h"

/*public*/
// Constructor
ModifiedPGE::ModifiedPGE() {
		sAppName = "Modified Pixel Game Engine";
}

// PixelGameEngine
bool ModifiedPGE::OnUserCreate() {
    ImGui::CreateContext();
    m_pge_imgui = olc::imgui::PGE_ImGUI();
    m_pge_imgui.ImGui_ImplPGE_Init(this);
    ImGui_ImplOpenGL2_Init();
    m_gameLayer = CreateLayer();
    EnableLayer(m_gameLayer, true);
    SetLayerCustomRenderFunction(0, std::bind(&ModifiedPGE::DrawUI, this));
    set_bg_color(olc::Pixel("#54BA99"));
    set_is_dragging(false);
    create_layer("main");
    return true;
}
bool ModifiedPGE::OnUserUpdate(float fElapsedTime) {
    SetDrawTarget((uint8_t)m_gameLayer);
    return true;
}
bool ModifiedPGE::OnUserDestroy() {
    return true;
}

// ImGui
void ModifiedPGE::DrawUI(void) {
    ImGui_ImplOpenGL2_NewFrame();
    m_pge_imgui.ImGui_ImplPGE_NewFrame();
    ImGui::NewFrame();
    draw_ui();
    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

// Getter
const olc::Pixel& ModifiedPGE::get_bg_color() const {
    return m_bg_color; 
}
const bool& ModifiedPGE::get_is_dragging() const {
    return m_flag.at("is_dragging"); 
}
olc::imgui::PGE_ImGUI& ModifiedPGE::get_pge_imgui() {
    return m_pge_imgui; 
}
const std::vector<boost::uuids::uuid>& ModifiedPGE::get_order(const std::string& layer) const {
    return m_layers.at(layer);
}
const bool& ModifiedPGE::get_flag(const std::string& flag) const {
    return m_flag.at(flag); 
}

// Setter
void ModifiedPGE::set_flag(const std::string& flag, bool state) {
    m_flag[flag] = state; 
}
void ModifiedPGE::set_bg_color(const olc::Pixel& color) {
    m_bg_color = color; 
}
void ModifiedPGE::set_is_dragging(bool is_dragging) {
    m_flag["is_dragging"] = is_dragging; 
}
// The name of the two following methods has been chosen by their meaning 
// in term of drawing order.
// The last element in `m_layers[layer]` is the clothest element.
void ModifiedPGE::push_front_order_id(const boost::uuids::uuid& id,
                                      const std::string& layer) {
    m_layers[layer].push_back(id); 
} 
void ModifiedPGE::push_back_order_id(const boost::uuids::uuid& id,
                                      const std::string& layer) {
    m_layers[layer].insert(m_layers[layer].begin(), id); 
}
void ModifiedPGE::create_layer(const std::string& layer) {
    m_layers[layer] = {};
}

// Update/Draw order related
void ModifiedPGE::move_forward(const boost::uuids::uuid& id,
                               const std::string& layer) {
    try {
        auto itr = std::find(m_layers[layer].begin(), m_layers[layer].end(), id);
        int index = std::distance(m_layers[layer].begin(), itr);
        if(index + 1 < m_layers[layer].size()){
            std::swap(m_layers[layer][index], m_layers[layer][index + 1]);
        }
    }
    catch(const std::out_of_range& oor) {
        create_layer(layer);
        m_layers[layer].push_back(id);
    }
}
void ModifiedPGE::move_backward(const boost::uuids::uuid& id,
                                const std::string& layer) {
    try {
        auto itr = std::find(m_layers[layer].begin(), m_layers[layer].end(), id);
        int index = std::distance(m_layers[layer].begin(), itr);
        if(index - 1 >= 0) {
            std::swap(m_layers[layer][index], m_layers[layer][index - 1]);
        }
    }
    catch(const std::out_of_range& oor) {
        create_layer(layer);
        m_layers[layer].push_back(id);
    }
}
void ModifiedPGE::move_front(const boost::uuids::uuid& id,
                             const std::string& layer) {
    try {
        auto itr = std::find(m_layers[layer].begin(), m_layers[layer].end(), id);
        int index = std::distance(m_layers[layer].begin(), itr);
        std::swap(m_layers[layer][index], m_layers[layer].back());
    }
    catch(const std::out_of_range& oor) {
        create_layer(layer);
        m_layers[layer].push_back(id);
    }
}
void ModifiedPGE::move_back(const boost::uuids::uuid& id,
                            const std::string& layer) {
    try {
        auto itr = std::find(m_layers[layer].begin(), m_layers[layer].end(), id);
        int index = std::distance(m_layers[layer].begin(), itr);
        std::swap(m_layers[layer][index], m_layers[layer].front());
    }
    catch(const std::out_of_range& oor) {
        create_layer(layer);
        m_layers[layer].push_back(id);
    }
}

/*protected*/
// ImGui
void ModifiedPGE::draw_ui() {}
