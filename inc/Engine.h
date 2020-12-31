#ifndef GALG_ENGINE_H_
#define GALG_ENGINE_H_

#include "ModifiedPGE.h"
#include "Vertex.h"
#include "Edge.h"
#include "../lib/olcPixelGameEngine.h"
#include <boost/uuid/uuid_io.hpp>

namespace galg {
class Engine : public ModifiedPGE {
    public:
        // Constructor
        Engine();

        // PixelGameEngine
        bool OnUserCreate() override;
        bool OnUserUpdate(float fElapsedTime) override;
        bool OnUserDestroy() override;

        // Getter
        const Widget& get_widget(const boost::uuids::uuid& id) const;
        const Vertex& get_vertex(const boost::uuids::uuid& id) const;
        const Edge& get_edge(const boost::uuids::uuid& id) const;
        const Vertex& get_default_vertex() const;
        int get_nb_vertex() const;
        int get_nb_edge() const;
        const bool& get_state(const std::string& state_name) const;
        const std::vector<boost::uuids::uuid>& get_edge_buffer() const;
        const bool has_edge(const boost::uuids::uuid& vertex_id1,
                             const boost::uuids::uuid& vertex_id2);

        // Setter
        void set_default_vertex(Vertex default_vertex);

        // Debug
        void draw_debug();

    private:
        // ImGui
        void draw_ui() override;
        void draw_action_window();

        // Getter
        Vertex* get_vertex_ptr(const boost::uuids::uuid& id);
        Edge* get_edge_ptr(const boost::uuids::uuid& id);
        Widget* get_widget_ptr(const boost::uuids::uuid& id);
        const std::map<boost::uuids::uuid,Vertex*>& get_vertices();
        const std::map<boost::uuids::uuid,Edge*>& get_edges();
        const std::map<boost::uuids::uuid,Widget*>& get_widgets();

        // Setter
        // // Add
        void add_edge(const boost::uuids::uuid& vertex_id1,
                      const boost::uuids::uuid& vertex_id2);
        void add_vertex(const Vertex& vertex);
        void add_widget(const boost::uuids::uuid& id, Widget* widget);
        void add_widget_to_edge_buffer(const boost::uuids::uuid& id);

        // // Modify
        void set_state(const std::string& state_name, const bool& state);

        // // Delete
        void clear_edge_buffer();

        // Update
        void update_normal();
        void update_add_edge();

    private:
        std::map<boost::uuids::uuid,Widget*> m_widgets;
        std::map<boost::uuids::uuid,Vertex*> m_vertices;
        std::map<boost::uuids::uuid,Edge*> m_edges;
        std::map<std::string,bool> m_state;
        std::vector<boost::uuids::uuid> m_edge_buffer;
        Vertex m_default_vertex;
};
}


#endif // GALG_ENGINE_H_
