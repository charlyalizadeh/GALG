#include "../inc/Widget.h"

namespace galg {
/*public*/
// Constructor
Widget::Widget(const std::pair<int,int>& coord,
               const std::pair<int,int>& dim,
               const boost::uuids::uuid& id):
    m_coord(coord),
    m_dim(dim),
    m_coord_drag(coord),
    m_is_drag(false),
    m_mouse_pos(0, 0),
    m_id(id),
    m_colors(std::map<std::string,olc::Pixel>()),
    m_gui_state(std::map<std::string,bool>()) 
{
        set_gui_state("modifyDepth", false);
        add_color("bg", COLOR[4]);
        add_color("bg_over", COLOR[3]);
        add_color("bg_drag", COLOR[2]);
        add_color("fg", COLOR[7]);
        add_color("fg_over", COLOR[7]);
        add_color("fg_drag", COLOR[7]);
        add_color("border", COLOR[7]);
        add_color("border_over", COLOR[9]);
        add_color("border_drag", COLOR[8]);
}
Widget::Widget(): 
    m_coord(0, 0),
    m_dim(0, 0),
    m_coord_drag(0, 0),
    m_is_drag(false),
    m_mouse_pos(0, 0),
    m_id(boost::uuids::random_generator()()),
    m_colors(std::map<std::string,olc::Pixel>()),
    m_gui_state(std::map<std::string,bool>()){}
Widget::~Widget() {}

// Getter
const int& Widget::get_x() const {
    return m_coord.first; 
}
const int& Widget::get_y() const {
    return m_coord.second; 
}
const std::pair<int,int>& Widget::get_coord() const {
    return m_coord; 
}
const int& Widget::get_width() const {
    return m_dim.first; 
}
const int& Widget::get_height() const {
    return m_dim.second; 
}
const std::pair<int,int>& Widget::get_dim() const {
    return m_dim; 
}
const olc::Pixel& Widget::get_color(const std::string& label) const {
    return m_colors.at(label); 
}
const bool& Widget::get_is_drag() const {
    return m_is_drag; 
}
const std::pair<int,int>& Widget::get_coord_drag() const {
    return m_coord_drag; 
}
const bool& Widget::get_gui_state(const std::string& gui) const {
    return m_gui_state.at(gui); 
}
const std::pair<float,float>& Widget::get_mouse_pos() const {
    return m_mouse_pos; 
}
const boost::uuids::uuid& Widget::get_id() const {
    return m_id; 
}
std::pair<int,int> Widget::get_center() const { 
    return {get_x() + get_width() / 2, get_y() + get_height() / 2}; 
}
int Widget::get_center_x() const {
    return get_x() + get_width() / 2;
}
int Widget::get_center_y() const {
    return get_y() + get_height() / 2;
}

// Setter
void Widget::set_x(const int& x) {
    m_coord.first = x; 
}
void Widget::set_y(const int& y) {
    m_coord.second = y; 
}
void Widget::set_coord(const std::pair<int,int>& coord) {
    m_coord = coord; 
}
void Widget::set_coord(const int& x, const int& y) {
    m_coord = {x, y
    }; }
void Widget::set_width(const int& width) {
    m_dim.first = width; 
}
void Widget::set_height(const int& height) {
    m_dim.second = height; 
}
void Widget::set_dim(const std::pair<int,int>& dim) {
    m_dim = dim; 
}
void Widget::set_dim(const int& width, const int& height) {
    m_dim = {width, height
    }; }
void Widget::set_is_drag(const bool& drag) {
    m_is_drag = drag; 
}
void Widget::set_coord_drag(const std::pair<int,int>& coord_drag) {
    m_coord_drag = coord_drag; 
}
void Widget::set_gui_state(const std::string& gui, const bool& state) {
    m_gui_state[gui] = state; 
}
void Widget::set_mouse_pos(const std::pair<float,float>& mouse_pos) {
    m_mouse_pos = mouse_pos; 
}
void Widget::set_id(const boost::uuids::uuid& id) {
    m_id = id; 
}

// Drag
void Widget::drag(const int& mouse_x, const int& mouse_y) {
    int x = mouse_x - get_coord_drag().first;
    int y = mouse_y - get_coord_drag().second;
    set_coord({x, y});
}
void Widget::toggle_drag_on(ModifiedPGE& engine) { 
    if(!engine.get_is_dragging()) {
        int mouse_x = engine.GetMouseX();
        int mouse_y = engine.GetMouseY();
        set_is_drag(true);
        set_coord_drag({mouse_x - get_x(), mouse_y - get_y()});
        engine.set_is_dragging(true);
    }
}
void Widget::toggle_drag_off(ModifiedPGE& engine) {
    set_is_drag(false);
    engine.set_is_dragging(false);
}

// Color
void Widget::add_color(const std::string& label, const olc::Pixel& color) {
    m_colors[label] = color;
}
void Widget::add_color(const std::string& label, const std::string& hexcol) {
    m_colors[label] = olc::Pixel(hexcol);
}
void Widget::add_color(const std::string& label,
                       const int& red,
                       const int& green,
                       const int& blue) {
    m_colors[label] = olc::Pixel(red, green, blue);
}

// State
bool Widget::is_mouse_over(ModifiedPGE& engine) const  {
    int mouse_x = engine.GetMouseX(); 
    int mouse_y = engine.GetMouseY();
    return mouse_x >= m_coord.first &&
           mouse_y >= m_coord.second &&
           mouse_x <= m_coord.first + m_dim.first &&
           mouse_y <= m_coord.second + m_dim.second;
}
bool Widget::is_clicked(ModifiedPGE& engine, const int& button) const {
    return is_mouse_over(engine) && engine.GetMouse(button).bPressed;
}
bool Widget::is_held(ModifiedPGE& engine, const int& button) const {
    return is_mouse_over(engine) && engine.GetMouse(button).bHeld;
}
bool Widget::is_shift_held(ModifiedPGE& engine) const {
    return engine.GetKey(olc::Key::SHIFT).bHeld ||
           engine.GetKey(olc::Key::SHIFT).bPressed;
}
bool Widget::is_ctrl_held(ModifiedPGE& engine) const {
    return engine.GetKey(olc::Key::CTRL).bHeld ||
           engine.GetKey(olc::Key::CTRL).bPressed;
}
bool Widget::is_released(ModifiedPGE& engine, const int& button) const {
    return is_mouse_over(engine) && engine.GetMouse(button).bReleased;
}

// Draw/Update
void Widget::draw(ModifiedPGE& engine) const {
    engine.DrawString(0, 0, "ah"); 
}
void Widget::update(ModifiedPGE& engine) { }

// ImGui
void Widget::update_ui(ModifiedPGE& engine) { }
void Widget::modify_depth(ModifiedPGE& engine) {
    ImGui::SetNextWindowPos({get_mouse_pos().first, get_mouse_pos().second});
    ImGui::Begin("Depth");
    if(ImGui::Button("Move forward")) {
        move_forward(engine);
    }
    if(ImGui::Button("Move backward")) {
        move_backward(engine);
    }
    if(ImGui::Button("Move to front")) {
        move_front(engine);
    }
    if(ImGui::Button("Move to back")) {
        move_back(engine);
    }
    if(!ImGui::IsWindowFocused()) {
        set_gui_state("modifyDepth", false);
        engine.set_flag("has_active_popup_menu", false);
    }
    ImGui::End();
}

// Update/Draw order related
void Widget::move_forward(ModifiedPGE& engine) const {
    engine.move_forward(get_id(), "widget");
}
void Widget::move_backward(ModifiedPGE& engine) const {
    engine.move_backward(get_id(), "widget");
}
void Widget::move_front(ModifiedPGE& engine) const {
    engine.move_front(get_id(), "widget");
}
void Widget::move_back(ModifiedPGE& engine) const {
    engine.move_back(get_id(), "widget");
}
}
