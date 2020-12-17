#include "../inc/Vertex.h"

Vertex::Vertex(std::pair<int, int> coord,
                int size,
                std::string label,
                Shape shape,
                boost::uuids::uuid id): Widget(coord, {size, size}, id), m_label(label), m_shape(shape) {
    set_gui_state("mainMenu", false);
    set_gui_state("modifyLabel", false);
    set_gui_state("modifyShape", false);
}
Vertex::Vertex(std::pair<int, int> coord,
                std::pair<int, int> dim,
                std::string label,
                Shape shape,
                boost::uuids::uuid id): Widget(coord, dim, id), m_label(label), m_shape(shape) {
    set_gui_state("mainMenu", false);
    set_gui_state("modifyLabel", false);
    set_gui_state("modifyShape", false);
}
Vertex::Vertex(const Vertex& vertex): Widget(vertex.get_coord(), vertex.get_dim()), m_label(vertex.get_label()), m_shape(vertex.get_shape()){
    set_gui_state("mainMenu", false);
    set_gui_state("modifyLabel", false);
    set_gui_state("modifyShape", false);
}
Vertex::Vertex() {}

Shape Vertex::get_shape() const { return m_shape; }
std::string Vertex::get_label() const { return m_label; }

void Vertex::set_shape(Shape shape) { m_shape = shape; }
void Vertex::set_label(std::string label) { m_label = label; }

bool Vertex::is_mouse_over(ModifiedPGE& engine) const {
    int mouse_x = engine.GetMouseX();
    int mouse_y = engine.GetMouseY();
    int origin_x = get_x() + get_width() / 2;
    int origin_y = get_y() + get_height() / 2;
    switch(m_shape){
        case Shape::CIRCLE:
            return (origin_x - mouse_x) * (origin_x - mouse_x) + (origin_y - mouse_y) * (origin_y - mouse_y)   <= (get_width() / 2) * (get_width() / 2) ;
            break;
        case Shape::RECTANGLE:
            return Widget::is_mouse_over(engine);
        case Shape::DIAMOND:
            break;
    }
    return false;
}

void Vertex::draw(ModifiedPGE& engine) const {
    int label_x = get_x() + get_width() / 2 - (get_label().size() / 2) * 8;
    int label_y = get_y() + get_width() / 2 - 4;
    std::string state = get_is_drag() ? "_drag" : is_mouse_over(engine) ? "_over" : "";
    olc::Pixel bg = get_color("bg" + state);
    olc::Pixel fg = get_color("fg" + state);
    olc::Pixel border = get_color("border" + state);
    switch(m_shape) {
        case Shape::CIRCLE:
            engine.FillCircle(get_x() + get_width() / 2, get_y() + get_width() / 2, get_width() / 2, bg);
            engine.DrawCircle(get_x() + get_width() / 2, get_y() + get_width() / 2, get_width() / 2, border);
            engine.DrawString(label_x, label_y, get_label(), fg);
            break;
        case Shape::RECTANGLE:
            engine.FillRect(get_x(), get_y(), get_width(), get_height(), bg);
            engine.DrawRect(get_x(), get_y(), get_width(), get_height(), border);
            engine.DrawString(label_x, label_y, get_label(), fg);
            break;
        case Shape::DIAMOND:
            // TODO: implement diamond shape.
            break;
    }
}
void Vertex::update(ModifiedPGE& engine) {
    if(is_clicked(engine, 0)) {
        toggle_drag_on(engine); 
        engine.DrawString(0, 0, "DRAG ON");
    }
    if(get_is_drag()) {
        drag(engine.GetMouseX(), engine.GetMouseY());
        if(engine.GetMouse(0).bReleased) {
            toggle_drag_off(engine);
        }
    }
    if(is_clicked(engine, 1) && !engine.get_flag("has_active_popup_menu")) {
        engine.set_flag("has_active_popup_menu", true);
        set_gui_state("mainMenu", true);
        set_mouse_pos({engine.GetMouseX() * engine.GetPixelSize().x, engine.GetMouseY() * engine.GetPixelSize().y});
    }
}
void Vertex::update_ui(ModifiedPGE& engine) {
    if(get_gui_state("mainMenu")) {
        main_menu(engine);
    }
    if(get_gui_state("modifyLabel")) {
        modify_label(engine);
    }
    if(get_gui_state("modifyShape")) {
        modify_shape(engine);
    }
    if(get_gui_state("modifyDepth")) {
        Widget::modify_depth(engine);
    }
}

void Vertex::main_menu(ModifiedPGE& engine) {
    ImGui::SetNextWindowPos({get_mouse_pos().first, get_mouse_pos().second});
    ImGui::Begin("Options");
    if(ImGui::Button("Modify Label")) {
        set_gui_state("mainMenu", false);
        set_gui_state("modifyLabel", true);
    }
    if(ImGui::Button("Modify Shape")) {
        set_gui_state("mainMenu", false);
        set_gui_state("modifyShape", true);
    }
    if(ImGui::Button("Modify Depth")) {
        set_gui_state("mainMenu", false);
        set_gui_state("modifyDepth", true);
    }
    if(!ImGui::IsWindowFocused()) {
        set_gui_state("mainMenu", false);
        engine.set_flag("has_active_popup_menu", false);
    }
    ImGui::End();
}
void Vertex::modify_label(ModifiedPGE& engine) {
    ImGui::SetNextWindowPos({get_mouse_pos().first, get_mouse_pos().second});
    ImGui::Begin("Label");
    char buf[30]; 
    strcpy(buf, get_label().c_str());
    if(ImGui::InputText("UTF-8 input", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_EnterReturnsTrue) || (!ImGui::IsWindowFocused())) {
        set_gui_state("modifyLabel", false);
        engine.set_flag("has_active_popup_menu", false);
    } else {
        set_label(buf);
    }
    ImGui::End();
}
void Vertex::modify_shape(ModifiedPGE& engine) {
    ImGui::SetNextWindowPos({get_mouse_pos().first, get_mouse_pos().second});
    ImGui::Begin("Shape");
    const char* items[] = { "CIRCLE", "SQUARE" };
    int item_current = static_cast<int>(get_shape());
    if(ImGui::ListBox("listbox\n(single select)", &item_current, items, IM_ARRAYSIZE(items), 4)) {
        set_gui_state("modifyShape", false);
        engine.set_flag("has_active_popup_menu", false);
    }
    set_shape(static_cast<Shape>(item_current));
    ImGui::End();
}
