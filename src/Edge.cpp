#include "../inc/Edge.h"

Edge::Edge(Widget* src, Widget* dst, boost::uuids::uuid id): Widget(src->get_coord(), {0, 0}, id), m_src(src), m_dst(dst) {
    set_width(abs(src->get_x() - dst->get_x()));
    set_height(abs(src->get_y() - dst->get_y()));
    add_color("fg_over", COLOR[9]);
}
Edge::Edge() {}

const Widget& Edge::get_src() const { return *m_src; }
const Widget& Edge::get_dst() const { return *m_dst; }

void Edge::set_src(Widget* src) { m_src = std::unique_ptr<Widget>(src); }
void Edge::set_dst(Widget* dst) { m_dst = std::unique_ptr<Widget>(dst); }

void Edge::draw(ModifiedPGE& engine) const {
    std::pair<int, int> src_center = get_src().get_center();
    std::pair<int, int> dst_center = get_dst().get_center();
    olc::Pixel fg = is_mouse_over(engine) ? COLOR[9] : get_color("fg");
    engine.DrawLine(src_center.first, src_center.second, dst_center.first, dst_center.second, fg);
}
void Edge::update(ModifiedPGE& engine) {
    set_coord(get_src().get_center());
    set_width(abs(get_src().get_center_x()- get_dst().get_center_x()));
    set_height(abs(get_src().get_center_y()- get_dst().get_center_y()));
}
void Edge::update_ui(ModifiedPGE& engine) {
}

bool Edge::is_mouse_over(ModifiedPGE& engine) const {
    int x0 = engine.GetMouseX();
    int y0 = engine.GetMouseY();

    if(get_src().is_mouse_over(engine) || get_dst().is_mouse_over(engine)) {
        return false;
    }

    int x1 = get_src().get_center_x();
    int y1 = get_src().get_center_y();
    int x2 = get_dst().get_center_x();
    int y2 = get_dst().get_center_y();

    // Check the normal distance to the line
    float distance = abs((x2 - x1) * (y1 - y0) - (x1 - x0) * (y2 - y1)) / (float)sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    if(distance > 8){
        return false;
    }

    int dxl = x2 - x1;
    int dyl = y2 - y1;

    // Check if (x0, y0) is between src and dst : https://stackoverflow.com/questions/11907947/how-to-check-if-a-point-lies-on-a-line-between-2-other-points
    if (abs(dxl) >= abs(dyl)) {
      return dxl > 0 ? 
        x1 <= x0 && x0 <= x2 :
        x2 <= x0 && x0 <= x1;
    }
    else {
      return dyl > 0 ? 
        y1 <= y0 && y0 <= y2 :
        y2 <= y0 && y0 <= y1;
    }
}
