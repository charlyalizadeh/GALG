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
bool ModifiedPGE::get_is_dragging() const {
    return m_flag.at("is_dragging"); 
}
olc::imgui::PGE_ImGUI& ModifiedPGE::get_pge_imgui() {
    return m_pge_imgui; 
}
const std::vector<boost::uuids::uuid>& ModifiedPGE::get_order() const {
    return m_order; 
}
bool ModifiedPGE::get_flag(const std::string& flag) const {
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
// The last element in `m_order` is the clothest element.
void ModifiedPGE::push_front_order_id(const boost::uuids::uuid& id) {
    m_order.push_back(id); 
} 
void ModifiedPGE::push_back_order_id(const boost::uuids::uuid& id) {
    m_order.insert(m_order.begin(), id); 
}

// Update/Draw order related
void ModifiedPGE::move_forward(const boost::uuids::uuid& id) {
    auto itr = std::find(m_order.begin(), m_order.end(), id);
    int index = std::distance(m_order.begin(), itr);
    if(!index + 1 < m_order.size()){
        std::swap(m_order[index], m_order[index + 1]);
    }
}
void ModifiedPGE::move_backward(const boost::uuids::uuid& id) {
    auto itr = std::find(m_order.begin(), m_order.end(), id);
    int index = std::distance(m_order.begin(), itr);
    if(index - 1 >= 0) {
        std::swap(m_order[index], m_order[index - 1]);
    }
}
void ModifiedPGE::move_front(const boost::uuids::uuid& id) {
    auto itr = std::find(m_order.begin(), m_order.end(), id);
    int index = std::distance(m_order.begin(), itr);
    std::swap(m_order[index], m_order.back());
}
void ModifiedPGE::move_back(const boost::uuids::uuid& id) {
    auto itr = std::find(m_order.begin(), m_order.end(), id);
    int index = std::distance(m_order.begin(), itr);
    std::swap(m_order[index], m_order.front());
}

/*protected*/
// ImGui
void ModifiedPGE::draw_ui() {}
