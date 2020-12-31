#include "../inc/Engine.h"

namespace galg {
/*public*/ 
// Constructor
Engine::Engine() {
    sAppName = "GALG";
}

// PixelGameEngine
bool Engine::OnUserCreate() {
    ModifiedPGE::OnUserCreate();
    ImGui::GetStyle().WindowRounding = 0.0f;
    ImGui::GetStyle().ChildRounding = 0.0f;
    ImGui::GetStyle().FrameRounding = 0.0f;
    ImGui::GetStyle().GrabRounding = 0.0f;
    ImGui::GetStyle().PopupRounding = 0.0f;
    ImGui::GetStyle().ScrollbarRounding = 0.0f;
    m_edge_buffer = std::vector<boost::uuids::uuid>();
    set_state("add_edge", false);
    set_state("normal", true);
    set_flag("has_active_popup_menu", false);
    set_state("debug", true);
    m_default_vertex = Vertex({ScreenWidth() / 2, ScreenHeight() / 2}, 50);
    create_layer("edge");
    create_layer("vertex");
    return true;
}
bool Engine::OnUserUpdate(float fElapsedTime) {
    ModifiedPGE::OnUserUpdate(fElapsedTime);
    Clear(olc::Pixel("#ffe9d6"));
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
    for(auto& widget : m_widgets) {
        delete widget.second;
        widget.second = nullptr;
    }
    return true;
}

// Getter
const Widget& Engine::get_widget(const boost::uuids::uuid& id) const {
    return *m_widgets.at(id);
}
const Vertex& Engine::get_vertex(const boost::uuids::uuid& id) const {
    return *m_vertices.at(id);
}
const Edge& Engine::get_edge(const boost::uuids::uuid& id) const {
    return *m_edges.at(id);
}
const Vertex& Engine::get_default_vertex() const {
    return m_default_vertex;
}
int Engine::get_nb_vertex() const {
    return m_vertices.size();
}
int Engine::get_nb_edge() const {
    return m_edges.size();
}
const bool& Engine::get_state(const std::string& state_name) const {
    return m_state.at(state_name);
}
const std::vector<boost::uuids::uuid>& Engine::get_edge_buffer() const {
    return m_edge_buffer;
}
const bool Engine::has_edge(const boost::uuids::uuid& vertex_id1,
                             const boost::uuids::uuid& vertex_id2) {
    for(const auto& el : get_edges()) {
        if((el.second->get_src_id() == vertex_id1 
            && el.second->get_dst_id() == vertex_id2) 
                ||
           (el.second->get_src_id() == vertex_id2 
            && el.second->get_dst_id() == vertex_id1)) {
            return true;
        }
    }
    return false;
}

// Setter
void Engine::set_default_vertex(Vertex default_vertex) {
    m_default_vertex = default_vertex; 
}

// Debug
void Engine::draw_debug() { 
    DrawString(0,
               10,
               "Number edges:" + std::to_string(get_nb_edge()),
               COLOR[0]);
    DrawString(0,
               20,
               "Number vertices:" + std::to_string(get_nb_vertex()),
               COLOR[0]);
}


/*private*/
// ImGui 
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
    for(auto layer = get_layer_order(OrderType::UPDATE).rbegin(); layer != get_layer_order(OrderType::UPDATE).rend(); ++layer) {
        for (auto id = get_order(*layer, OrderType::UPDATE).rbegin(); id != get_order(*layer, OrderType::UPDATE).rend(); ++id) { 
            get_widget_ptr(*id)->update_ui(*this);
        } 
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
        add_vertex(get_default_vertex());
    }
    if(ImGui::Button("Add Edge")) {
        //set_state("normal", false);
        set_state("add_edge", true);
    }
    ImGui::End();
}

// Getter
Vertex* Engine::get_vertex_ptr(const boost::uuids::uuid& id) {
    return m_vertices.at(id);
}
Edge* Engine::get_edge_ptr(const boost::uuids::uuid& id) {
    return m_edges.at(id); 
}
Widget* Engine::get_widget_ptr(const boost::uuids::uuid& id) {
    return m_widgets.at(id); 
}
const std::map<boost::uuids::uuid,Vertex*>& Engine::get_vertices() {
    return m_vertices; 
}
const std::map<boost::uuids::uuid,Edge*>& Engine::get_edges() {
    return m_edges; 
}
const std::map<boost::uuids::uuid,Widget*>& Engine::get_widgets() {
    return m_widgets; 
}

// Setter
// // Add
void Engine::add_edge(const boost::uuids::uuid& vertex_id1,
                      const boost::uuids::uuid& vertex_id2) {
    if(vertex_id1 != vertex_id2 && !has_edge(vertex_id1, vertex_id2)) {
        Edge* new_edge = new Edge(get_vertex_ptr(vertex_id1),
                                  get_vertex_ptr(vertex_id2));
        boost::uuids::uuid id = boost::uuids::random_generator()();
        new_edge->set_id(id);
        new_edge->set_is_directed(true);
        m_edges[id] = new_edge;
        add_widget(id, new_edge);
        push_front(id, "edge");
    }
}
void Engine::add_vertex(const Vertex& vertex) {
    boost::uuids::uuid id = boost::uuids::random_generator()();
    Vertex* new_vertex = new Vertex(vertex);
    new_vertex->set_id(id);
    m_vertices[id] = new_vertex;
    add_widget(id, new_vertex);
    push_front(id, "vertex");
}
void Engine::add_widget(const boost::uuids::uuid& id, Widget* widget) {
    m_widgets[id] = widget;
}
void Engine::add_widget_to_edge_buffer(const boost::uuids::uuid& id) {
    m_edge_buffer.push_back(id);
}

// // Modify
void Engine::set_state(const std::string& state_name, const bool& state) {
    m_state[state_name] = state; 
}

// // Delete
void Engine::clear_edge_buffer() {
    m_edge_buffer.clear();
}

// Update
void Engine::update_normal() {
    for(auto layer: get_layer_order(OrderType::UPDATE)) {
        for(auto id : get_order(layer, OrderType::UPDATE)) {
            get_widget_ptr(id)->update(*this);
        }
    }
    for(auto layer: get_layer_order(OrderType::DRAW)) {
        for(auto id : get_order(layer, OrderType::DRAW)) {
            get_widget_ptr(id)->draw(*this);
        }
    }
}
void Engine::update_add_edge() {
    // TODO: Take into account the draw order
    for (auto id = get_order("vertex", OrderType::DRAW).rbegin(); id != get_order("vertex", OrderType::DRAW).rend(); ++id) { 
        if(get_vertex_ptr(*id)->is_clicked(*this)) {
            add_widget_to_edge_buffer(get_vertex_ptr(*id)->get_id());
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
}
