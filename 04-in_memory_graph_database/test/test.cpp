#include <iostream>
#include "dagoba/dagoba.h"
#include "dagoba/op.h"
int main(){
    Graph* graph = new Graph; 
    graph->add_vertex(Vertex{0});
    graph->add_vertex(Vertex{1});
    graph->add_edge(Edge{0,1});
    Query* query = new Query;
    query->add_op(Op{"fake_source"});
    query->add_op(Op{"double"});
    query->add_op(Op{"double"});
    query->add_op(Op{"double"});
    query->add_op(Op{"double"});
    query->add_op(Op{"print"});
    Dagoba dagoba;
    Status s = dagoba.compile(graph,query);
    if(!s.isOk()){
        std::cout<<s.to_string()<<"\n";
    }
    return 0;
}