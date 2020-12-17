#ifndef  GALG_MODIFIEDPGE_H_
#define GALG_MODIFIEDPGE_H_

#include "../lib/imgui_impl_pge.h"
#include "../lib/imgui/imgui_impl_opengl2.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

class ModifiedPGE : public olc::PixelGameEngine
{
    public:
        ModifiedPGE();
    public:
        bool OnUserCreate() override;
        bool OnUserUpdate(float fElapsedTime) override;
        bool OnUserDestroy() override;
        void DrawUI(void);

        olc::Pixel get_bg_color() const;
        bool get_is_dragging() const;
        olc::imgui::PGE_ImGUI& get_pge_imgui();
        std::vector<boost::uuids::uuid>& get_order();
        bool get_flag(const std::string& flag) const;

        void set_flag(const std::string& flag, bool state);
        void set_bg_color(const olc::Pixel& color);
        void set_is_dragging(bool is_dragging); 
        void push_front_order_id(const boost::uuids::uuid& id);
        void push_back_order_id(const boost::uuids::uuid& id);

        void move_forward(const boost::uuids::uuid& id);
        void move_backward(const boost::uuids::uuid& id);
        void move_front(const boost::uuids::uuid& id);
        void move_back(const boost::uuids::uuid& id);


    protected:
        virtual void draw_ui();

    private:
        olc::imgui::PGE_ImGUI m_pge_imgui;
        int m_gameLayer;
        olc::Pixel m_bg_color;
        std::pair<float, float> m_pos_temp;
        std::map<std::string, bool> m_flag;
        std::vector<boost::uuids::uuid> m_order;
};



#endif // GALG_MODIFIEDPGE_H_
