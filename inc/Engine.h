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

        // Getter
        Widget get_widget(const boost::uuids::uuid& id) const;
        Vertex get_vertex(const boost::uuids::uuid& id) const;
        Edge get_edge(const boost::uuids::uuid& id) const;
        int get_nb_vertex() const;
        int get_nb_edge() const;
        Vertex get_default_vertex() const;
        bool get_state(const std::string& state_name) const;
        std::vector<boost::uuids::uuid> get_edge_buffer() const;
        std::string get_type(const boost::uuids::uuid& id) const;
        bool has_edge(const boost::uuids::uuid& vertex_id1, const boost::uuids::uuid& vertex_id2);
        const std::vector<boost::uuids::uuid>& get_vertices_ids() const;
        const std::vector<boost::uuids::uuid>& get_edges_ids() const;

        // Setter
        void set_default_vertex(Vertex default_vertex);

        // Debug
        void draw_debug();

    private:
        // ImGui
        void draw_ui();
        void draw_action_window();
        void draw_debug_window();

        // Getter
        Vertex* get_vertex_ptr(const boost::uuids::uuid& id);
        Edge* get_edge_ptr(const boost::uuids::uuid& id);
        Widget* get_widget_ptr(const boost::uuids::uuid& id);
        std::map<boost::uuids::uuid, Vertex*>& get_vertices();
        std::map<boost::uuids::uuid, Edge*>& get_edges();
        std::map<boost::uuids::uuid, Widget*>& get_widgets();

        // Setter
        // // Add
        void add_edge(boost::uuids::uuid vertex_id1, boost::uuids::uuid vertex_id2);
        void add_vertex(Vertex vertex);
        void add_widget(const boost::uuids::uuid& id, Widget* widget);
        void add_widget_to_edge_buffer(boost::uuids::uuid id);

        // // Modify
        void set_state(const std::string& state_name, bool state);

        // // Delete
        void clear_edge_buffer();

        // Update
        void update_normal();
        void update_add_edge();

    private:
        std::map<boost::uuids::uuid, Widget*> m_widgets;
        std::map<boost::uuids::uuid, Vertex*> m_vertices;
        std::map<boost::uuids::uuid, Edge*> m_edges;
        std::map<std::string, bool> m_state;
        std::vector<boost::uuids::uuid> m_edge_buffer;
        Vertex m_default_vertex;
};


#endif // GALG_ENGINE_H_
