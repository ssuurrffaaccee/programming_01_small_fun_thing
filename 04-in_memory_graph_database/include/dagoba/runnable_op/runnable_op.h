#pragma once
#include <unordered_set>
#include "dagoba/graph.h"
#include "dagoba/op.h"
#include "dagoba/runnable_op/register.h"
enum class GremlinType{
    Pull,
    Data,
    Dead,
};
struct Gremlin{
    Gremlin(GremlinType type):type_{type}{}
    GremlinType type_;
    std::vector<ID> verties_;
    std::unordered_map<std::string,ID> as_;
};
struct RunableOp
{
    RunableOp() : graph_(nullptr) {}
    virtual ~RunableOp(){}
    virtual void init(const Op& op) = 0;
    virtual Status run(Gremlin *gremlin) = 0;
    void set_graph(Graph* graph){
        graph_ = graph;
    }
    Graph *graph_;
};

struct Filter : public RunableOp
{
    virtual void init(const Op& op) override{};
    virtual Status run(Gremlin *gremlin) override{
        return Status::Ok();
    };
};
REGISTE_OP(Filter,"filter")

struct Unique : public RunableOp
{
    virtual void init(const Op& op) override {}
    virtual Status run(Gremlin *gremlin) override;
    private:
    std::unordered_set<int32_t> seen_set_;
};
REGISTE_OP(Unique,"unique")

struct Take : public RunableOp
{
    virtual void init(const Op& op) override{};
    virtual Status run(Gremlin *gremlin) override;
    int count_{0};
    int threshold_{0};
};
REGISTE_OP(Take,"take")
struct FakeSource : public RunableOp
{
    virtual void init(const Op& op) override{};
    virtual Status run(Gremlin *gremlin) override;
};
REGISTE_OP(FakeSource,"fake_source")