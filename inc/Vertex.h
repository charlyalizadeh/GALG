#ifndef  GALG_VERTEX_H_
#define GALG_VERTEX_H_

#include "ModifiedPGE.h"
#include "Widget.h"
#include "../lib/imgui_impl_pge.h"
#include "../lib/imgui/imgui_impl_opengl2.h"
#include <string>

enum class VertexShape {
    CIRCLE,
    RECTANGLE,
    DIAMOND
};

class Vertex : public Widget {
    public:
        // Constructor
        Vertex(const std::pair<int,int>& coord,
               const int& size,
               const std::string& m_label = "",
               const VertexShape& shape = VertexShape::CIRCLE,
               const boost::uuids::uuid& id = boost::uuids::uuid());
        Vertex(const std::pair<int,int>& coord,
               const std::pair<int,int>& dim,
               const std::string& m_label = "",
               const VertexShape& shape = VertexShape::RECTANGLE,
               const boost::uuids::uuid& id = boost::uuids::uuid());
        Vertex(const Vertex& vertex);
        Vertex();

        // Getter
        const VertexShape& get_shape() const;
        const std::string& get_label() const;

        // Setter
        void set_shape(const VertexShape& shape);
        void set_label(const std::string& label);

        // Draw/Update
        void draw(ModifiedPGE& engine) const;
        void update(ModifiedPGE& engine);
        void update_ui(ModifiedPGE& engine);

        // State
        bool is_mouse_over(ModifiedPGE& engine) const;

        // ImGui
        void main_menu(ModifiedPGE& engine);
        void modify_label(ModifiedPGE& engine);
        void modify_shape(ModifiedPGE& engine);

    private:
        std::string m_label;
        VertexShape m_shape;
};

#endif // GALG_VERTEX_H_
