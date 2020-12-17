#include "../inc/ModifiedPGE.h"

ModifiedPGE::ModifiedPGE() {
		sAppName = "Modified Pixel Game Engine";
}

bool ModifiedPGE::OnUserCreate() {
    //One time initialization of the Dear ImGui library
    ImGui::CreateContext();
    //Create an instance of the Dear ImGui PGE Integration
    m_pge_imgui = olc::imgui::PGE_ImGUI();
    //The vi2d for pixel size must match the values given in Construct()
    //Otherwise the mouse will not work correctly
    m_pge_imgui.ImGui_ImplPGE_Init(this);
    //Initialize the OpenGL2 rendering system
    ImGui_ImplOpenGL2_Init();
    //Create a new Layer which will be used for the game
    m_gameLayer = CreateLayer();
    //The layer is not enabled by default,  so we need to enable it
    EnableLayer(m_gameLayer, true);
    //Set a custom render function on layer 0.  Since DrawUI is a member of
    //our class, we need to use std::bind
    SetLayerCustomRenderFunction(0, std::bind(&ModifiedPGE::DrawUI, this));
    set_bg_color(olc::Pixel("#54BA99"));
    set_is_dragging(false);
    return true;
}
bool ModifiedPGE::OnUserUpdate(float fElapsedTime) {
    //Change the Draw Target to not be Layer 0
    SetDrawTarget((uint8_t)m_gameLayer);
    //Game Drawing code here
    return true;
}
bool ModifiedPGE::OnUserDestroy() {
    return true;
}
void ModifiedPGE::DrawUI(void) {
    //These 3 lines are mandatory per-frame initialization
    //Create and react to your UI here
    ImGui_ImplOpenGL2_NewFrame();
    m_pge_imgui.ImGui_ImplPGE_NewFrame();
    ImGui::NewFrame();

    // render your GUI
    draw_ui();
    
    //This finishes the Dear ImGui and renders it to the screen
    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}
void ModifiedPGE::draw_ui() {}

olc::Pixel ModifiedPGE::get_bg_color() const { return m_bg_color; }
bool ModifiedPGE::get_is_dragging() const { return m_flag.at("is_dragging"); }
olc::imgui::PGE_ImGUI& ModifiedPGE::get_pge_imgui() { return m_pge_imgui; }
std::vector<boost::uuids::uuid>& ModifiedPGE::get_order() { return m_order; }
bool ModifiedPGE::get_flag(const std::string& flag) const { return m_flag.at(flag); }

void ModifiedPGE::set_flag(const std::string& flag, bool state) { m_flag[flag] = state; }
void ModifiedPGE::set_bg_color(const olc::Pixel& color) { m_bg_color = color; }
void ModifiedPGE::set_is_dragging(bool is_dragging) { m_flag["is_dragging"] = is_dragging; }
// The name of the two following methods has been chosen by their meaning it term of drawing order. The last element in `m_order` is the clothest element.
void ModifiedPGE::push_front_order_id(const boost::uuids::uuid& id) { m_order.push_back(id); } 
void ModifiedPGE::push_back_order_id(const boost::uuids::uuid& id) { m_order.insert(m_order.begin(), id); }


void ModifiedPGE::move_forward(const boost::uuids::uuid& id) {
    std::vector<boost::uuids::uuid>::iterator itr = std::find(m_order.begin(), m_order.end(), id);
    int index = std::distance(m_order.begin(), itr);
    if(!index + 1 < m_order.size()){
        std::swap(m_order[index], m_order[index + 1]);
    }
}
void ModifiedPGE::move_backward(const boost::uuids::uuid& id) {
    std::vector<boost::uuids::uuid>::iterator itr = std::find(m_order.begin(), m_order.end(), id);
    int index = std::distance(m_order.begin(), itr);
    if(index - 1 >= 0) {
        std::swap(m_order[index], m_order[index - 1]);
    }
}
void ModifiedPGE::move_front(const boost::uuids::uuid& id) {
    std::vector<boost::uuids::uuid>::iterator itr = std::find(m_order.begin(), m_order.end(), id);
    int index = std::distance(m_order.begin(), itr);
    std::swap(m_order[index], m_order.back());
}
void ModifiedPGE::move_back(const boost::uuids::uuid& id) {
    std::vector<boost::uuids::uuid>::iterator itr = std::find(m_order.begin(), m_order.end(), id);
    int index = std::distance(m_order.begin(), itr);
    std::swap(m_order[index], m_order.front());
}
