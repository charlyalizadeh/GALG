#ifndef GALG_ENGINE_H_
#define GALG_ENGINE_H_

#include "../lib/olcPixelGameEngine.h"
#include <boost/uuid/uuid_io.hpp>
#include "ModifiedPGE.h"
#include "Vertex.h"
#include "Edge.h"

class Engine : public ModifiedPGE {
    public:
        // Constructor
        Engine();

        // olc::PixelGameEngine methods
        bool OnUserCreate() override;
        bool OnUserUpdate(float fElapsedTime) override;
        bool OnUserDestroy() override;

        // Getter methods
        Widget* get_widget(const boost::uuids::uuid& id);
        bool get_state(const std::string& state_name) const;
        std::vector<boost::uuids::uuid> get_edge_buffer() const;
        std::string get_type(const boost::uuids::uuid& id) const;
        bool has_edge(const boost::uuids::uuid& vertex_id1, const boost::uuids::uuid& vertex_id2) const;

        // Setter methods
        void add_widget(Widget* widget, const std::string& type="Widget");

        // Debug
        void draw_debug();

    protected:
        // ImGui methods
        void draw_ui();
        void draw_action_window();
        void draw_debug_window();

        // Setter methods
        // // Add
        void add_type(const boost::uuids::uuid& id, const std::string& type);
        void add_edge(boost::uuids::uuid vertex_id1, boost::uuids::uuid vertex_id2);
        void add_vertex();
        void add_widget_to_edge_buffer(boost::uuids::uuid id);
        void add_edge_pair(boost::uuids::uuid vertex_id1, boost::uuids::uuid vertex_id2);

        // // Modify
        void set_state(const std::string& state_name, bool state);

        // // Delete
        void clear_edge_buffer();

        // Update methods
        void update_normal();
        void update_add_edge();

    private:
        std::map<boost::uuids::uuid, Widget*> m_widgets;
        std::map<std::string, bool> m_state;
        std::vector<boost::uuids::uuid> m_edge_buffer;
        std::map<boost::uuids::uuid, std::string> m_types; // I'm not sure that's the best design but it's quite simple to use
        std::vector<std::pair<boost::uuids::uuid, boost::uuids::uuid>> m_edges_pair;
        Vertex m_default_vertex;
};


#endif // GALG_ENGINE_H_
