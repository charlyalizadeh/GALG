#ifndef  GALG_WIDGET_H_
#define GALG_WIDGET_H_

#include "../lib/imgui_impl_pge.h"
#include "../lib/imgui/imgui_impl_opengl2.h"
#include "../inc/ModifiedPGE.h"
#include <utility>
#include <vector>
#include <string>
#include <map>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

// Inspired by Windows 95 palette
const std::vector<olc::Pixel> COLOR = {
    olc::Pixel("#000000"),
    olc::Pixel("#939393"),
    olc::Pixel("#c0c0c0"),
    olc::Pixel("#d7d7d7"),
    olc::Pixel("#ffffff"),
    olc::Pixel("#fffaa6"),
    olc::Pixel("#17166d"),
    olc::Pixel("#006666"),
    olc::Pixel("#54002c"),
    olc::Pixel("#FF0000")
};

class Widget{
    public:

        // Constructor
        Widget(const std::pair<int, int>& coord, const std::pair<int, int>& dim, boost::uuids::uuid m_id = boost::uuids::uuid());
        Widget();
        virtual ~Widget();

        // Getter methods
        int get_x() const;
        int get_y() const;
        std::pair<int, int> get_coord() const;
        int get_width() const;
        int get_height() const;
        std::pair<int, int> get_dim() const;
        olc::Pixel get_color(const std::string& label) const;
        bool get_is_drag() const;
        std::pair<int, int> get_coord_drag() const;
        bool get_gui_state(const std::string& gui) const;
        std::pair<float, float> get_mouse_pos() const;
        const boost::uuids::uuid& get_id() const;
        std::pair<int, int> get_center() const;
        int get_center_x() const;
        int get_center_y() const;

        // Setter methods
        void set_x(const int& x);
        void set_y(const int& y);
        void set_coord(const std::pair<int, int>& coord);
        void set_coord(const int& x, const int& y);
        void set_width(const int& width);
        void set_height(const int& height);
        void set_dim(const std::pair<int, int>& dim);
        void set_dim(const int& width, const int& height);
        void set_is_drag(const bool& is_drag);
        void set_coord_drag(const std::pair<int, int>& coord_drag);
        void set_gui_state(const std::string& gui, const bool& state);
        void set_mouse_pos(const std::pair<float, float>& mouse_pos);
        void set_id(boost::uuids::uuid id);

        // Drag methods
        void drag(const int& mouse_x, const int& mouse_y);
        void toggle_drag_on(ModifiedPGE& engine);
        void toggle_drag_off(ModifiedPGE& engine);

        // Color methods
        void add_color(const std::string& label, const olc::Pixel& color);
        void add_color(const std::string& label, const std::string& hexadecimal_color);
        void add_color(const std::string& label, const int& red, const int& green, const int& blue);

        // // state methods
        virtual bool is_mouse_over(ModifiedPGE& engine) const;
        virtual bool is_clicked(ModifiedPGE& engine, const int& button = 0) const;
        virtual bool is_held(ModifiedPGE& engine, const int& button = 0) const;
        virtual bool is_shift_held(ModifiedPGE& engine) const;
        virtual bool is_ctrl_held(ModifiedPGE& engine) const;
        virtual bool is_released(ModifiedPGE& engine, const int& button = 0) const;

        // Draw/Update methods
        virtual void draw(ModifiedPGE& engine) const;
        virtual void update(ModifiedPGE& engine);

        // ImGUI methods
        virtual void update_ui(ModifiedPGE& engine);
        void modify_depth(ModifiedPGE& engine);

        // Misc methods
        void move_forward(ModifiedPGE& engine) const;
        void move_backward(ModifiedPGE& engine) const;
        void move_front(ModifiedPGE& engine) const;
        void move_back(ModifiedPGE& engine) const;

    private:
        std::pair<int, int> m_coord, m_dim;
        std::pair<int, int> m_coord_drag;
        bool m_is_drag;
        std::pair<float, float> m_mouse_pos;
        boost::uuids::uuid m_id;
        std::map<std::string, olc::Pixel> m_colors;
        std::map<std::string, bool> m_gui_state;

};
#endif // GALG_WIDGET_H_

