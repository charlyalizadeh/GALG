#ifndef  GALG_EDGE_H_
#define GALG_EDGE_H_

#include "../lib/imgui_impl_pge.h"
#include "../lib/imgui/imgui_impl_opengl2.h"
#include "ModifiedPGE.h"
#include "Widget.h"
#include "Vertex.h"
#include <string>
#include <cstdlib>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <math.h>

#define PI 3.14159265359


enum class ArrowShape {
    CIRCLE,
    SQUARE,
    DIAMOND,
    TRIANGLE,
};

class Edge : public Widget {
    public:
        // Constructor
        Edge(Vertex* src,
             Vertex* dst,
             boost::uuids::uuid id = boost::uuids::uuid(),
             bool is_directed=false,
             ArrowShape arrow_shape=ArrowShape::TRIANGLE,
             std::pair<int, int> arrow_dim = {10, 6},
             int arrow_offset = 0
             );
        Edge(const Edge& edge);
        Edge();

        // Getter
        const Vertex& get_src() const;
        const Vertex& get_dst() const;
        const boost::uuids::uuid& get_src_id() const;
        const boost::uuids::uuid& get_dst_id() const;
        bool get_is_directed() const;
        ArrowShape get_arrow_shape() const;
        std::pair<int, int> get_arrow_dim() const;
        int get_arrow_offset() const;
        bool get_debug() const;

        // Setter
        void set_src(Vertex* src);
        void set_dst(Vertex* dst);
        void set_is_directed(bool is_directed);
        void switch_direction();
        void set_arrow_shape(ArrowShape arrow_shape);
        void set_arrow_dim(std::pair<int, int> arrow_dim);
        void set_arrow_offset(int arrow_offset);
        void set_debug(bool debug);

        // Update/Draw
        void draw(ModifiedPGE& engine) const;
        void draw_arrow(ModifiedPGE& engine) const;
        void draw_arrow_circle(ModifiedPGE& engine) const;
        void draw_arrow_square(ModifiedPGE& engine) const;
        void draw_arrow_diamond(ModifiedPGE& engine) const;
        void draw_arrow_triangle(ModifiedPGE& engine) const;
        void update(ModifiedPGE& engine);
        void update_ui(ModifiedPGE& engine);

        // State
        bool is_mouse_over(ModifiedPGE& engine) const;

        // Debug
        void draw_debug(ModifiedPGE& engine) const;
        void draw_is_mouse_over(ModifiedPGE& engine) const;
        void draw_debug_arrow(ModifiedPGE& engine) const;
        void draw_debug_arrow_triangle(ModifiedPGE& engine) const;

    private:
        std::pair<int, int> get_intersection_coord() const;
        std::unique_ptr<Vertex> m_src;
        std::unique_ptr<Vertex> m_dst;
        std::pair<int, int> m_old_src_coord;
        std::pair<int, int> m_old_dst_coord;
        bool m_is_directed;
        ArrowShape m_arrow_shape;
        std::pair<int, int> m_arrow_dim;
        int m_arrow_offset;
        bool m_debug;
};

#endif // GALG_EDGE_H_
