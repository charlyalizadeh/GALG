#include "../inc/ModifiedPGE.h"

namespace galg {
namespace utility {
template <typename T>
void move_forward(std::vector<T>& vec, T el) {
    auto itr = std::find(vec.begin(), vec.end(), el);
    int index = std::distance(vec.begin(), itr);
    if(index + 1 < vec.size()){
        std::swap(vec[index], vec[index + 1]);
    }
}
template <typename T>
void move_backward(std::vector<T>& vec, T el) {
    auto itr = std::find(vec.begin(), vec.end(), el);
    int index = std::distance(vec.begin(), itr);
    if(index - 1 >= 0) {
        std::swap(vec[index], vec[index - 1]);
    }
}
template <typename T>
void move_front(std::vector<T>& vec, T el) {
    auto itr = std::find(vec.begin(), vec.end(), el);
    int index = std::distance(vec.begin(), itr);
    std::swap(vec[index], vec.front());
}
template <typename T>
void move_back(std::vector<T>& vec, T el) {
    auto itr = std::find(vec.begin(), vec.end(), el);
    int index = std::distance(vec.begin(), itr);
    std::swap(vec[index], vec.back());
}
}


/*Order*/
/*public*/
// Constructor
template <typename T>
Order<T>::Order() : m_draw(std::vector<T>()), m_update(std::vector<T>()) {}

// Getter
template <typename T>
const std::vector<T>& Order<T>::get_order(OrderType order) const { 
    switch(order) {
        default:
        case OrderType::DRAW:
            return get_order_draw();
            break;
        case OrderType::UPDATE:
            return get_order_update();
            break;
    }
}
template <typename T>
const std::vector<T>& Order<T>::get_order_draw() const { 
    return m_draw;
}
template <typename T>
const std::vector<T>& Order<T>::get_order_update() const {
    return m_update;
}

// Setter
template <typename T>
void Order<T>::set_order_draw(const std::vector<T>& draw) {
    m_draw = draw;
}
template <typename T>
void Order<T>::set_order_update(const std::vector<T>& update) {
    m_update = update;
}

// Update/Draw order related
template <typename T>
void Order<T>::move_forward(T el, OrderType order) {
    switch(order) {
        case OrderType::DRAW:
            utility::move_forward(m_draw, el);
            break;
        case OrderType::UPDATE:
            utility::move_forward(m_update, el);
            break;
        case OrderType::ALL:
            utility::move_forward(m_draw, el);
            utility::move_forward(m_update, el);
            break;
    }
}
template <typename T>
void Order<T>::move_backward(T el, OrderType order) {
    switch(order) {
        case OrderType::DRAW:
            utility::move_backward(m_draw, el);
            break;
        case OrderType::UPDATE:
            utility::move_backward(m_update, el);
            break;
        case OrderType::ALL:
            utility::move_backward(m_draw, el);
            utility::move_backward(m_update, el);
            break;
    }
}
template <typename T>
void Order<T>::move_front(T el, OrderType order) {
    switch(order) {
        case OrderType::DRAW:
            utility::move_front(m_draw, el);
            break;
        case OrderType::UPDATE:
            utility::move_front(m_update, el);
            break;
        case OrderType::ALL:
            utility::move_front(m_draw, el);
            utility::move_front(m_update, el);
            break;
    }
}
template <typename T>
void Order<T>::move_back(T el, OrderType order) {
    switch(order) {
        case OrderType::DRAW:
            utility::move_back(m_draw, el);
            break;
        case OrderType::UPDATE:
            utility::move_back(m_update, el);
            break;
        case OrderType::ALL:
            utility::move_back(m_draw, el);
            utility::move_back(m_update, el);
            break;
    }
}
// The name of two following function may be measlinding if you think about
// where `el` is pushed in the vector. You need to think in term of draw
// order.
template <typename T>
void Order<T>::push_front(T el, OrderType order) {
    switch(order) {
        case OrderType::DRAW:
            m_draw.push_back(el);
            break;
        case OrderType::UPDATE:
            m_update.push_back(el);
            break;
        case OrderType::ALL:
            m_draw.push_back(el);
            m_update.push_back(el);
            break;
    }
}
template <typename T>
void Order<T>::push_back(T el, OrderType order) {
    switch(order) {
        case OrderType::DRAW:
            m_draw.insert(m_draw.begin(), el);
            break;
        case OrderType::UPDATE:
            m_update.insert(m_update.begin(), el);
            break;
        case OrderType::ALL:
            m_draw.insert(m_draw.begin(), el);
            m_update.insert(m_update.begin(), el);
            break;
    }
}



/*ModifiedPGE*/
/*public*/
// Constructor
ModifiedPGE::ModifiedPGE() : m_layer_order(Order<std::string>()) {
		sAppName = "Modified Pixel Game Engine";
}

// PixelGameEngine
bool ModifiedPGE::OnUserCreate() {
    ImGui::CreateContext();
    m_pge_imgui = olc::imgui::PGE_ImGUI();
    m_pge_imgui.ImGui_ImplPGE_Init(this);
    ImGui_ImplOpenGL2_Init();
    m_gameLayer = CreateLayer();
    EnableLayer(m_gameLayer, true);
    SetLayerCustomRenderFunction(0, std::bind(&ModifiedPGE::DrawUI, this));
    set_bg_color(olc::Pixel("#54BA99"));
    set_is_dragging(false);
    return true;
}
bool ModifiedPGE::OnUserUpdate(float fElapsedTime) {
    SetDrawTarget((uint8_t)m_gameLayer);
    return true;
}
bool ModifiedPGE::OnUserDestroy() {
    return true;
}

// ImGui
void ModifiedPGE::DrawUI(void) {
    ImGui_ImplOpenGL2_NewFrame();
    m_pge_imgui.ImGui_ImplPGE_NewFrame();
    ImGui::NewFrame();
    draw_ui();
    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

// Getter
const olc::Pixel& ModifiedPGE::get_bg_color() const {
    return m_bg_color; 
}
const bool& ModifiedPGE::get_is_dragging() const {
    return m_flag.at("is_dragging"); 
}
olc::imgui::PGE_ImGUI& ModifiedPGE::get_pge_imgui() {
    return m_pge_imgui; 
}
const std::vector<boost::uuids::uuid>& ModifiedPGE::get_order(const std::string& layer,
                                                              const OrderType& order) const {
    return m_layers.at(layer).get_order(order);
}
const bool& ModifiedPGE::get_flag(const std::string& flag) const {
    return m_flag.at(flag); 
}
const std::vector<std::string>& ModifiedPGE::get_layer_order(const OrderType& order) const {
    return m_layer_order.get_order(order);
}

// Setter
void ModifiedPGE::set_flag(const std::string& flag, bool state) {
    m_flag[flag] = state; 
}
void ModifiedPGE::set_bg_color(const olc::Pixel& color) {
    m_bg_color = color; 
}
void ModifiedPGE::set_is_dragging(bool is_dragging) {
    m_flag["is_dragging"] = is_dragging; 
}
void ModifiedPGE::create_layer(const std::string& layer) {
    m_layers[layer] = Order<boost::uuids::uuid>();
    push_front_layer(layer);
}

// Update/Draw order related
void ModifiedPGE::move_forward(const boost::uuids::uuid& id,
                               const std::string& layer,
                               const OrderType& order) {
    try {
        m_layers.at(layer).move_forward(id, order);
    }
    catch(const std::out_of_range& oor) {
        create_layer(layer);
        m_layers[layer].push_front(id, order);
    }
}
void ModifiedPGE::move_backward(const boost::uuids::uuid& id,
                                const std::string& layer,
                                const OrderType& order) {
    try {
        m_layers.at(layer).move_backward(id, order);
    }
    catch(const std::out_of_range& oor) {
        create_layer(layer);
        m_layers[layer].push_front(id, order);
    }
}
void ModifiedPGE::move_front(const boost::uuids::uuid& id,
                             const std::string& layer,
                             const OrderType& order) {
    try {
        m_layers.at(layer).move_back(id, order);
    }
    catch(const std::out_of_range& oor) {
        create_layer(layer);
        m_layers[layer].push_front(id, order);
    }
}
void ModifiedPGE::move_back(const boost::uuids::uuid& id,
                            const std::string& layer,
                            const OrderType& order) {
    try {
        m_layers.at(layer).move_front(id, order);
    }
    catch(const std::out_of_range& oor) {
        create_layer(layer);
        m_layers[layer].push_front(id, order);
    }
}
void ModifiedPGE::move_forward_layer(const std::string& layer,
                                     const OrderType& order) {
    m_layer_order.move_forward(layer, order);
}
void ModifiedPGE::move_backward_layer(const std::string& layer,
                                      const OrderType& order) {
    m_layer_order.move_backward(layer, order);
}
void ModifiedPGE::move_front_layer(const std::string& layer,
                                   const OrderType& order) {
    m_layer_order.move_back(layer, order);
}
void ModifiedPGE::move_back_layer(const std::string& layer,
                                  const OrderType& order) {
    m_layer_order.move_front(layer, order);
}
void ModifiedPGE::push_front(const boost::uuids::uuid& id,
                             const std::string& layer,
                             const OrderType& order) {
    try {
        m_layers[layer].push_front(id, order);
    }
    catch(const std::out_of_range& oor) {
        create_layer(layer);
        m_layers[layer].push_front(id, order);
    }
}
void ModifiedPGE::push_back(const boost::uuids::uuid& id,
                            const std::string& layer,
                            const OrderType& order) {
    try {
        m_layers[layer].push_back(id, order);
    }
    catch(const std::out_of_range& oor) {
        create_layer(layer);
        m_layers[layer].push_front(id, order);
    }
}
void ModifiedPGE::push_front_layer(const std::string& layer,
                                   const OrderType& order) {
    m_layer_order.push_front(layer, order);
}
void ModifiedPGE::push_back_layer(const std::string& layer,
                                  const OrderType& order) {
    m_layer_order.push_back(layer, order);
}


/*protected*/
// ImGui
void ModifiedPGE::draw_ui() {}
}
