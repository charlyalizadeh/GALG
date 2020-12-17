#ifndef  GALG_VERTEX_H_
#define GALG_VERTEX_H_

#include "../lib/imgui_impl_pge.h"
#include "../lib/imgui/imgui_impl_opengl2.h"
#include "ModifiedPGE.h"
#include "Widget.h"
#include <string>

enum class Shape {
    CIRCLE,
    RECTANGLE,
    DIAMOND
};

class Vertex : public Widget {
    public:
        Vertex(std::pair<int, int> coord, int size, std::string m_label = "", Shape shape = Shape::CIRCLE, boost::uuids::uuid id = boost::uuids::uuid());
        Vertex(std::pair<int, int> coord, std::pair<int, int> dim, std::string m_label = "", Shape shape = Shape::RECTANGLE, boost::uuids::uuid id = boost::uuids::uuid());
        Vertex(const Vertex& vertex);
        Vertex();

        // Getter
        Shape get_shape() const;
        std::string get_label() const;

        // Setter
        void set_shape(Shape shape);
        void set_label(std::string label);

        // Draw/Update functions
        void draw(ModifiedPGE& engine) const;
        void update(ModifiedPGE& engine);
        void update_ui(ModifiedPGE& engine);

        // State function
        bool is_mouse_over(ModifiedPGE& engine) const;

        // GUI functions
        void main_menu(ModifiedPGE& engine);
        void modify_label(ModifiedPGE& engine);
        void modify_shape(ModifiedPGE& engine);

    private:
        std::string m_label;
        Shape m_shape;
};

#endif // GALG_VERTEX_H_
