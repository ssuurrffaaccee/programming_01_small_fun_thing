#pragma once
#include <vector>
#include <unordered_map>
#include "common.h"
struct Vertex
{
    int32_t ID_;
    std::unordered_map<std::string, std::string> properties_;
    std::vector<size_t> out_edges_;
    std::vector<size_t> in_edges_;
};
struct Edge
{
    int32_t source_ID_;
    int32_t target_ID_;
    std::unordered_map<std::string, std::string> properties_;
};
class Graph
{
public:
    Graph() {}
    Status load_graph(const std::string &file_path)
    {
        return Status::Ok();
    }
    void add_vertex(const Vertex &v)
    {
        verties_.push_back(v);
        ID_to_vertex_address_map[v.ID_] = verties_.size() - 1;
    }
    void add_edge(const Edge &e)
    {
        检查 ID_to_vertex_address_map[e.source_ID_]
        检查 ID_to_vertex_address_map[e.target_ID_]
        edges_.push_back(e);
        Vertex &source_vertex = verties_[ID_to_vertex_address_map[e.source_ID_]];
        source_vertex.out_edges_.push_back(edges_.size());
        Vertex &target_vertex = verties_[ID_to_vertex_address_map[e.source_ID_]];
        target_vertex.in_edges_.push_back(edges_.size());
    }
    const Vertex &find_vertex_by_id(ID id)
    {
        return verties_[ID_to_vertex_address_map[id]];
    }

private:
    std::vector<Vertex> verties_;
    std::vector<Edge> edges_;
    std::unordered_map<ID, size_t> ID_to_vertex_address_map;
    DISALLOW_COPY_AND_ASSIGNMENT_CONSTRUCTOR(Graph)
};