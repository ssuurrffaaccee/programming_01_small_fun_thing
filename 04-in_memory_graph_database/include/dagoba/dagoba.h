#pragma once
#include "dagoba/graph.h"
#include "dagoba/query.h"
class Dagoba{
    public:
    Status compile(Graph* graph, Query* query);
    Status next();
    private:
        Graph* graph_{nullptr};
        Query* query_{nullptr};
};