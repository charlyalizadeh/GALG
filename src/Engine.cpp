#include "../inc/Engine.h"

// Constructor
Engine::Engine() {
    sAppName = "GALG";
}

// olc::PixelGameEngine methods
bool Engine::OnUserCreate() {
    ModifiedPGE::OnUserCreate();
    ImGui::GetStyle().WindowRounding = 0.0f;
    ImGui::GetStyle().ChildRounding = 0.0f;
    ImGui::GetStyle().FrameRounding = 0.0f;
    ImGui::GetStyle().GrabRounding = 0.0f;
    ImGui::GetStyle().PopupRounding = 0.0f;
    ImGui::GetStyle().ScrollbarRounding = 0.0f;
    m_edge_buffer = std::vector<boost::uuids::uuid>();
    m_edges_pair = std::vector<std::pair<boost::uuids::uuid, boost::uuids::uuid>>();

    set_state("add_edge", false);
    set_state("normal", true);
    set_flag("has_active_popup_menu", false);
    set_state("debug", true);

    m_default_vertex = Vertex({ScreenWidth() / 2, ScreenHeight() / 2}, 50);

    return true;
}
bool Engine::OnUserUpdate(float fElapsedTime) {
    ModifiedPGE::OnUserUpdate(fElapsedTime);
    Clear(olc::Pixel("#ffe9d6"));
    //m_default_vertex.update(*this);
    //m_default_vertex.draw(*this);
    if(get_state("normal")) {
        update_normal();
    }
    if(get_state("add_edge")) {
        update_add_edge();
    }
    if(get_state("debug")) {
        draw_debug();
    }
    return true;
}
bool Engine::OnUserDestroy() {
    for(const auto& id : get_order()) {
        delete m_widgets[id];
    }
    return true;
}

// Getter methods
Widget* Engine::get_widget(const boost::uuids::uuid& id) { return m_widgets.at(id); }
bool Engine::get_state(const std::string& state_name) const {
    return m_state.at(state_name);
}
std::vector<boost::uuids::uuid> Engine::get_edge_buffer() const {
    return m_edge_buffer;
}
std::string Engine::get_type(const boost::uuids::uuid& id) const {
    return m_types.at(id);
}
bool Engine::has_edge(const boost::uuids::uuid& vertex_id1, const boost::uuids::uuid& vertex_id2) const {
    return std::find(m_edges_pair.begin(), m_edges_pair.end(), std::make_pair(vertex_id1, vertex_id2)) != m_edges_pair.end();
}

// Setter methods
void Engine::set_state(const std::string& state_name, bool state) { m_state[state_name] = state; }
void Engine::add_edge(boost::uuids::uuid vertex_id1, boost::uuids::uuid vertex_id2) {
    if(get_type(vertex_id1) == "Vertex" && 
        get_type(vertex_id2) == "Vertex" &&
        vertex_id1 != vertex_id2 &&
        !has_edge(vertex_id1, vertex_id2)
        ) {
        add_widget(new Edge(get_widget(vertex_id1), get_widget(vertex_id2)), "Edge");
        add_edge_pair(vertex_id1, vertex_id2);
    }
}
void Engine::add_widget(Widget* widget, const std::string& type) {
    boost::uuids::uuid id = boost::uuids::random_generator()();
    widget->set_id(id);
    m_widgets[id] = widget;
    if(type == "Edge") {
        push_back_order_id(id);
    }
    else{
        push_front_order_id(id);
    }
    add_type(id, type);
}
void Engine::add_widget_to_edge_buffer(boost::uuids::uuid id) {
    m_edge_buffer.push_back(id);
}
void Engine::clear_edge_buffer() {
    m_edge_buffer.clear();
}
void Engine::add_vertex() {
    add_widget(new Vertex(m_default_vertex), "Vertex");
}
void Engine::add_type(const boost::uuids::uuid& id, const std::string& type) {
    m_types[id] = type;
}
void Engine::add_edge_pair(boost::uuids::uuid vertex_id1, boost::uuids::uuid vertex_id2) {
    m_edges_pair.push_back({vertex_id1, vertex_id2});
}

// Debug
void Engine::draw_debug() {
    int nb_edge = m_edges_pair.size();
    int nb_vertex = m_widgets.size() - nb_edge;
    DrawString(0, 10, "Number vertices: " + std::to_string(nb_vertex), COLOR[0]);
    DrawString(0, 18, "Number edges: " + std::to_string(nb_edge), COLOR[0]);
    if(get_state("add_edge")) {
        DrawString(0, 26, "Adding edge...", COLOR[0]);
        for(size_t i = 0; i < m_edge_buffer.size(); i++) {
            DrawString(0, 34 + i * 8, std::to_string(i) + " vertex: " + boost::uuids::to_string(m_edge_buffer[i]), COLOR[0]);
        }
    }
}

// ImGui methods
void Engine::draw_ui() {
    // Main ui
    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            ImGui::MenuItem("Import graph");
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    draw_action_window();

    // Widget ui
    for(const auto& id : get_order()) {
        get_widget(id)->update_ui(*this);
    }
}
void Engine::draw_action_window() {
    float pos_x =  0.8 * (ScreenWidth() * GetPixelSize().x);
    float pos_y =  0;
    float size_x = (ScreenWidth() * GetPixelSize().x) - pos_x;
    float size_y = (ScreenHeight() * GetPixelSize().y) - pos_y;
    ImGui::SetNextWindowSize({size_x, size_y});
    ImGui::SetNextWindowPos({pos_x, pos_y});
    ImGui::Begin("Action");
    if(ImGui::Button("Add Vertex")) {
        add_vertex();
    }
    if(ImGui::Button("Add Edge")) {
        //set_state("normal", false);
        set_state("add_edge", true);
    }
    ImGui::End();
}

// Update methods
void Engine::update_normal() {
    for (std::vector<boost::uuids::uuid>::reverse_iterator id = get_order().rbegin(); id != get_order().rend(); ++id) { 
        get_widget(*id)->update(*this);
    } 
    for(const auto& id : get_order()) {
        get_widget(id)->draw(*this);
    }
}
void Engine::update_add_edge() {
    for (std::vector<boost::uuids::uuid>::reverse_iterator id = get_order().rbegin(); id != get_order().rend(); ++id) { 
        if(get_widget(*id)->is_clicked(*this)) {
            add_widget_to_edge_buffer(get_widget(*id)->get_id());
            break;
        }
    } 
    if(get_edge_buffer().size() == 2) {
        add_edge(get_edge_buffer()[0], get_edge_buffer()[1]);
        clear_edge_buffer();
        set_state("add_edge", false);
        set_state("normal", true);
    }
}
