#ifndef  GALG_EDGE_H_
#define GALG_EDGE_H_

#include "../lib/imgui_impl_pge.h"
#include "../lib/imgui/imgui_impl_opengl2.h"
#include "ModifiedPGE.h"
#include "Widget.h"
#include <string>
#include <cstdlib>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

class Edge : public Widget {
    public:
        Edge(Widget* src, Widget* dst, boost::uuids::uuid id = boost::uuids::uuid());
        Edge();

        const Widget& get_src() const;
        const Widget& get_dst() const;

        void set_src(Widget* src);
        void set_dst(Widget* dst);

        void draw(ModifiedPGE& engine) const;
        void update(ModifiedPGE& engine);
        void update_ui(ModifiedPGE& engine);

        bool is_mouse_over(ModifiedPGE& engine) const;

    private:
        std::unique_ptr<Widget> m_src;
        std::unique_ptr<Widget> m_dst;
        //Vertex& m_src;
        //Vertex& m_dst;
        std::pair<int, int> m_old_src_coord;
        std::pair<int, int> m_old_dst_coord;
};

#endif // GALG_EDGE_H_
