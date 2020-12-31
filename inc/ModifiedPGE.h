#ifndef  GALG_MODIFIEDPGE_H_
#define GALG_MODIFIEDPGE_H_

#include "../lib/imgui_impl_pge.h"
#include "../lib/imgui/imgui_impl_opengl2.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>


namespace galg {
enum class OrderType {
    DRAW,
    UPDATE,
    ALL
};

template <typename T>
class Order {
    public:
        Order();

        // Getter
        const std::vector<T>& get_order(OrderType order) const;
        const std::vector<T>& get_order_draw() const;
        const std::vector<T>& get_order_update() const;

        // Setter
        void set_order_draw(const std::vector<T>& draw);
        void set_order_update(const std::vector<T>& update);

        // Update/Draw order related
        void move_forward(T el, OrderType order=OrderType::ALL);
        void move_backward(T el, OrderType order=OrderType::ALL);
        void move_front(T el, OrderType order=OrderType::ALL);
        void move_back(T el, OrderType order=OrderType::ALL);
        void push_front(T el, OrderType order=OrderType::ALL);
        void push_back(T el, OrderType order=OrderType::ALL);

    private:
        std::vector<T> m_draw;
        std::vector<T> m_update;
};

class ModifiedPGE : public olc::PixelGameEngine {
    public:
        // Constructor
        ModifiedPGE();

        // PixelGameEngine
        bool OnUserCreate() override;
        bool OnUserUpdate(float fElapsedTime) override;
        bool OnUserDestroy() override;

        // ImGui
        void DrawUI(void);

        // Getter
        const olc::Pixel& get_bg_color() const;
        const bool& get_is_dragging() const;
        olc::imgui::PGE_ImGUI& get_pge_imgui();
        const std::vector<boost::uuids::uuid>& get_order(const std::string& layer,
                                                         const OrderType& order) const;
        const bool& get_flag(const std::string& flag) const;
        const std::vector<std::string>& get_layer_order(const OrderType& order) const;
        

        // Setter
        void set_flag(const std::string& flag, bool state);
        void set_bg_color(const olc::Pixel& color);
        void set_is_dragging(bool is_dragging); 
        void create_layer(const std::string& layer);
                         

        // Update/Draw order related
        void move_forward(const boost::uuids::uuid& id,
                          const std::string& layer,
                          const OrderType& order=OrderType::ALL);
        void move_backward(const boost::uuids::uuid& id,
                           const std::string& layer,
                           const OrderType& order=OrderType::ALL);
        void move_front(const boost::uuids::uuid& id,
                       const std::string& layer,
                       const OrderType& order=OrderType::ALL);
        void move_back(const boost::uuids::uuid& id,
                       const std::string& layer,
                       const OrderType& order=OrderType::ALL);
        void move_forward_layer(const std::string& layer,
                                const OrderType& order=OrderType::ALL);
        void move_backward_layer(const std::string& layer,
                                 const OrderType& order=OrderType::ALL);
        void move_front_layer(const std::string& layer,
                              const OrderType& order=OrderType::ALL);
        void move_back_layer(const std::string& layer,
                             const OrderType& order=OrderType::ALL);
        void push_front(const boost::uuids::uuid& id,
                        const std::string& layer,
                        const OrderType& order=OrderType::ALL);
        void push_back(const boost::uuids::uuid& id,
                       const std::string& layer,
                       const OrderType& order=OrderType::ALL);
        void push_front_layer(const std::string& layer,
                              const OrderType& order=OrderType::ALL);
        void push_back_layer(const std::string& layer,
                             const OrderType& order=OrderType::ALL);

    protected:
        // ImGui
        virtual void draw_ui();

    private:
        olc::imgui::PGE_ImGUI m_pge_imgui;
        int m_gameLayer;
        olc::Pixel m_bg_color;
        std::pair<float,float> m_pos_temp;
        std::map<std::string,bool> m_flag;
        std::map<std::string,Order<boost::uuids::uuid>> m_layers;
        Order<std::string> m_layer_order;
        // TODO: Mayber use uuid also for layer. By using string I let the user
        // manage the unicity of the id. Also it doesn't allow for two layer 
        // with the same name. For the project use I don't think it will be a 
        // problem.
};
}
#endif // GALG_MODIFIEDPGE_H_
