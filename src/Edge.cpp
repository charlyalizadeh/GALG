#include "../inc/Edge.h"

/*public*/
// Constructor
Edge::Edge(Vertex* src,
           Vertex* dst,
           const boost::uuids::uuid& id,
           const bool& is_directed,
           const ArrowShape& arrow_shape):
                Widget(src->get_coord(), {0, 0}, id),
                m_src(src),
                m_dst(dst),
                m_old_src_coord({0, 0}),
                m_old_dst_coord({0, 0}),
                m_is_directed(is_directed),
                m_arrow_shape(arrow_shape),
                m_arrow_dim(10, 5),
                m_arrow_offset(0),
                m_debug(false)
{
    set_width(abs(src->get_x() - dst->get_x()));
    set_height(abs(src->get_y() - dst->get_y()));
    add_color("fg_over", COLOR[9]);
    add_color("arrow_bg", COLOR[7]);
    add_color("arrow_fg", COLOR[7]);
}
Edge::Edge():
                Widget({0, 0}, {0, 0}, boost::uuids::random_generator()()),
                m_src(nullptr),
                m_dst(nullptr),
                m_old_src_coord({0, 0}),
                m_old_dst_coord({0, 0}),
                m_is_directed(false),
                m_arrow_shape(ArrowShape::CIRCLE),
                m_arrow_dim({12, 6}),
                m_arrow_offset(0),
                m_debug(false)
{}
Edge::Edge(const Edge& edge):
                Widget(edge.get_coord(), edge.get_dim()),
                m_src(nullptr),
                m_dst(nullptr),
                m_old_src_coord({0, 0}),
                m_old_dst_coord({0, 0}),
                m_is_directed(edge.get_is_directed()),
                m_arrow_shape(edge.get_arrow_shape()),
                m_arrow_dim(edge.get_arrow_dim()),
                m_arrow_offset(edge.get_arrow_offset()),
                m_debug(false)
{}

// Getter
const Vertex& Edge::get_src() const {
    return *m_src; 
}
const Vertex& Edge::get_dst() const {
    return *m_dst; 
}
const boost::uuids::uuid& Edge::get_src_id() const {
    return get_src().get_id(); 
}
const boost::uuids::uuid& Edge::get_dst_id() const {
    return get_dst().get_id(); 
}
const bool& Edge::get_is_directed() const {
    return m_is_directed; 
}
const ArrowShape& Edge::get_arrow_shape() const {
    return m_arrow_shape; 
}
const std::pair<int,int>& Edge::get_arrow_dim() const  {
    return m_arrow_dim; 
}
const int& Edge::get_arrow_offset() const {
    return m_arrow_offset; 
}
const bool& Edge::get_debug() const {
    return m_debug; 
}

// Setter
void Edge::set_src(Vertex* src) {
    m_src = std::unique_ptr<Vertex>(src); 
}
void Edge::set_dst(Vertex* dst) {
    m_dst = std::unique_ptr<Vertex>(dst); 
}
void Edge::set_is_directed(const bool& is_directed) {
    m_is_directed = is_directed; 
}
void Edge::set_arrow_shape(const ArrowShape& arrow_shape) {
    m_arrow_shape = arrow_shape; 
}
void Edge::set_arrow_dim(const std::pair<int,int>& arrow_dim) {
    m_arrow_dim = arrow_dim; 
}
void Edge::set_arrow_offset(const int& arrow_offset) {
    m_arrow_offset = arrow_offset; 
}
void Edge::set_debug(const bool& debug) {
    m_debug = debug; 
}

// Update/Draw
void Edge::draw(ModifiedPGE& engine) const {
    std::pair<int,int> src_center = get_src().get_center();
    std::pair<int,int> dst_center = get_dst().get_center();
    olc::Pixel fg = is_mouse_over(engine) ? COLOR[9] : get_color("fg");
    engine.DrawLine(src_center.first,
                    src_center.second,
                    dst_center.first,
                    dst_center.second,
                    fg);
    draw_arrow(engine);
    if(get_debug()) {
        draw_debug(engine);
    }
}
void Edge::draw_arrow(ModifiedPGE& engine) const {
    switch(get_arrow_shape()) {
        case ArrowShape::CIRCLE:
            draw_arrow_circle(engine);
            break;
        case ArrowShape::TRIANGLE:
            draw_arrow_triangle(engine);
            break;
    }
}
void Edge::draw_arrow_circle(ModifiedPGE& engine) const {
    std::pair<int,int> intersection_coord = get_intersection_coord();
    engine.FillCircle(intersection_coord.first,
                      intersection_coord.second,
                      get_arrow_dim().first,
                      get_color("arrow_bg"));
    engine.DrawCircle(intersection_coord.first,
                      intersection_coord.second,
                      get_arrow_dim().first,
                      get_color("arrow_fg"));
}
void Edge::draw_arrow_square(ModifiedPGE& engine) const {
    // TODO
}
void Edge::draw_arrow_diamond(ModifiedPGE& engine) const {
    // TODO
}
// TODO: better name, or maybe not I don't know
void Edge::draw_arrow_triangle(ModifiedPGE& engine) const {
    
    int base_distance = get_arrow_dim().first;
    int arrow_distance = get_arrow_dim().second;
    std::pair<int,int> intersection = get_intersection_coord();
    int vec_edge_x =  get_src().get_center_x() - intersection.first;
    int vec_edge_y =  get_src().get_center_y() - intersection.second;

    int x_base, y_base;
    if(vec_edge_x != 0) {
        float coefficient = -sqrt((base_distance * base_distance) /
                            (float)(vec_edge_x * vec_edge_x + vec_edge_y * vec_edge_y));
        x_base = intersection.first - coefficient * vec_edge_x;
        y_base = intersection.second - coefficient * vec_edge_y;
        float vec_perp_y = sqrt((arrow_distance * arrow_distance) /
                           (float)((vec_edge_y * vec_edge_y) / (float)(vec_edge_x * vec_edge_x) + 1));
        float vec_perp_x = -(vec_edge_y / (float)vec_edge_x) * vec_perp_y;
        engine.FillTriangle(intersection.first,
                            intersection.second,
                            x_base + vec_perp_x,
                            y_base + vec_perp_y,
                            x_base - vec_perp_x,
                            y_base - vec_perp_y,
                            olc::RED);
    }
    else {
        if(vec_edge_y != 0) {
            x_base = intersection.first;
            y_base = intersection.second +
                     (vec_edge_y / abs(vec_edge_y)) * base_distance;
            float vec_perp_x = arrow_distance;
            float vec_perp_y = 0;
            engine.FillTriangle(intersection.first,
                                intersection.second,
                                x_base + vec_perp_x,
                                y_base + vec_perp_y,
                                x_base - vec_perp_x,
                                y_base - vec_perp_y,
                                olc::RED);
        }
    }
}
void Edge::update(ModifiedPGE& engine) {
    set_coord(get_src().get_center());
    set_width(abs(get_src().get_center_x() - get_dst().get_center_x()));
    set_height(abs(get_src().get_center_y() - get_dst().get_center_y()));
}
void Edge::update_ui(ModifiedPGE& engine) {
}

// State
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
    float distance = abs((x2 - x1) * (y1 - y0) - (x1 - x0) * (y2 - y1)) /
                     (float)sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    if(distance > 8) {
        return false;
    }

    int dxl = x2 - x1;
    int dyl = y2 - y1;

    // Check if (x0, y0) is between src and dst.
    // Source: https://stackoverflow.com/questions/11907947/how-to-check-if-a-point-lies-on-a-line-between-2-other-points
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

// Debug
void Edge::draw_debug(ModifiedPGE& engine) const {
    if(is_mouse_over(engine)) {
        draw_is_mouse_over(engine);
    }
    draw_debug_arrow(engine);
}
void Edge::draw_is_mouse_over(ModifiedPGE& engine) const {
    for(int x0 = 0; x0 < engine.ScreenWidth(); x0++) {
        for(int y0 = 0; y0 < engine.ScreenHeight(); y0++) {
            int x1 = get_src().get_center_x();
            int y1 = get_src().get_center_y();
            int x2 = get_dst().get_center_x();
            int y2 = get_dst().get_center_y();

            // Check the normal distance to the line
            float distance = abs((x2 - x1) * (y1 - y0) -
                                 (x1 - x0) * (y2 - y1)) /
                            (float)sqrt((x2 - x1) * (x2 - x1) +
                                        (y2 - y1) * (y2 - y1));
            if(distance > 8){
                continue;
            }

            int dxl = x2 - x1;
            int dyl = y2 - y1;

            bool is_over = false;
            if (abs(dxl) >= abs(dyl)) {
              is_over = dxl > 0 ?  x1 <= x0 && x0 <= x2 : x2 <= x0 && x0 <= x1;
              
            }
            else {
              is_over = dyl > 0 ?  y1 <= y0 && y0 <= y2 : y2 <= y0 && y0 <= y1;
            }
            if(is_over) {
                engine.Draw(x0, y0, COLOR[6]);
            }
        }
    }
}
void Edge::draw_debug_arrow(ModifiedPGE& engine) const {
    std::pair<int,int> intersection = get_intersection_coord();
    engine.DrawCircle(intersection.first, intersection.second, 2, olc::BLUE);
    switch(get_arrow_shape()) {
        case ArrowShape::TRIANGLE:
            draw_debug_arrow_triangle(engine);
            break;
    }
}
void Edge::draw_debug_arrow_triangle(ModifiedPGE& engine) const {
    int base_distance = get_arrow_dim().first;
    int arrow_distance = get_arrow_dim().second;
    std::pair<int,int> intersection = get_intersection_coord();
    int vec_edge_x =  get_src().get_center_x() - intersection.first;
    int vec_edge_y =  get_src().get_center_y() - intersection.second;
    int x_base, y_base;
    float vec_perp_x, vec_perp_y;
    if(vec_edge_x != 0) {
        float coefficient = -sqrt((base_distance * base_distance) /
                            (float)(vec_edge_x * vec_edge_x +
                                    vec_edge_y * vec_edge_y));
        x_base = intersection.first - coefficient * vec_edge_x;
        y_base = intersection.second - coefficient * vec_edge_y;
        vec_perp_y = sqrt((arrow_distance * arrow_distance) /
                     (float)((vec_edge_y * vec_edge_y) / 
                     (float)(vec_edge_x * vec_edge_x) + 1));
        vec_perp_x = -(vec_edge_y / (float)vec_edge_x) * vec_perp_y;
    }
    else {
        if(vec_edge_y != 0) {
            x_base = intersection.first;
            y_base = intersection.second + (vec_edge_y/abs(vec_edge_y)) *
                     base_distance;
            vec_perp_x = arrow_distance;
            vec_perp_y = 0;
        }
    }
    engine.DrawCircle(x_base, y_base, 2, olc::BLUE);
    engine.DrawCircle(x_base + vec_perp_x, y_base + vec_perp_y, 2, olc::BLUE);
    engine.DrawCircle(x_base - vec_perp_x, y_base - vec_perp_y, 2, olc::BLUE);
}

// Misc
std::pair<int,int> Edge::get_intersection_coord() const {
    std::pair<int,int> dst_center = get_dst().get_center();
    std::pair<int,int> src_center = get_src().get_center();
    std::pair<int,int> relative_src_center = {dst_center.first 
                                              - src_center.first,
                                              dst_center.second -
                                              src_center.second};
    float teta = atan2(relative_src_center.second, relative_src_center.first) 
                 + PI;
    std::pair<int,int> intersection_coord = { -100, -100};
    switch(get_dst().get_shape()) {
        case VertexShape::CIRCLE:
            intersection_coord.first = dst_center.first +
                                       (get_dst().get_width() / 2) * cos(teta);
            intersection_coord.second = dst_center.second +
                                        (get_dst().get_width() / 2) * sin(teta);
            break;
        case VertexShape::RECTANGLE:
            int x1 = get_src().get_center_x();
            int y1 = get_src().get_center_y();
            int x2 = get_dst().get_center_x();
            int y2 = get_dst().get_center_y();
            int x3, y3, x4, y4;
            if((teta >  (7.0 * PI) / 4.0 && teta < 2 * PI) ||
               (teta > 0 && teta < PI / 4.0)) {
                x3 = get_dst().get_x() + get_dst().get_width();
                y3 = get_dst().get_y();
                x4 = get_dst().get_x() + get_dst().get_width();
                y4 = get_dst().get_y() + get_dst().get_height();
            }
            else if(teta > PI / 4.0 && teta <= (3.0 * PI) / 4.0) {
                x3 = get_dst().get_x();
                y3 = get_dst().get_y() + get_dst().get_height();
                x4 = get_dst().get_x() + get_dst().get_width();
                y4 = get_dst().get_y() + get_dst().get_height();
            }
            else if(teta > (3.0 * PI) / 4.0 && teta < (5.0 * PI) / 4.0) {
                x3 = get_dst().get_x();
                y3 = get_dst().get_y();
                x4 = get_dst().get_x();
                y4 = get_dst().get_y() + get_dst().get_height();
            }
            else {
                x3 = get_dst().get_x();
                y3 = get_dst().get_y();
                x4 = get_dst().get_x() + get_dst().get_width();
                y4 = get_dst().get_y();
            }
            if(!(((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)) == 0 ||
               ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)) == 0)) {
                intersection_coord.first = ((x1 * y2 - y1 * x2) * (x3 - x4) -
                                           (x1 - x2) * (x3 * y4 - y3 * x4)) / 
                                           ((x1 - x2) * (y3 - y4) - 
                                           (y1 - y2) * (x3 - x4));
                intersection_coord.second = ((x1 * y2 - y1 * x2) * (y3 - y4) -
                                            (y1 - y2) * (x3 * y4 - y3 * x4)) / 
                                            ((x1 - x2) * (y3 - y4) -
                                            (y1 - y2) * (x3 - x4));
            }
            break;
    }
    return intersection_coord;
}
