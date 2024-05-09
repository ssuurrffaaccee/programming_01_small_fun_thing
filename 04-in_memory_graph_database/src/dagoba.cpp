
#include "dagoba/dagoba.h"
#include "dagoba/runnable_op/register.h"
#include "dagoba/op.h"
#include "dagoba/query.h"
Status Dagoba::compile(Graph *graph, Query *query)
{
    if (nullptr == graph)
    {
        return Status::Error("null graph!!");
    }
    if (nullptr == query)
    {
        return Status::Error("null query!!");
    }
    auto &constructors = OpRegister::get_op_contructors();
    const std::vector<Op> &ops = query->get_operators();
    std::vector<RunableOp *> runnable_ops = query->get_runnable_operators();
    if (!runnable_ops.empty())
    {
        for (auto runnable_op : runnable_ops)
        {
            if (runnable_op != nullptr)
            {
                delete runnable_op;
            }
        }
    }
    runnable_ops.resize(0);
    for (auto &op : ops)
    {
        auto iter = constructors.find(op.name);
        if (iter == constructors.end())
        {
            return Status::Error("op not find : " + op.name);
        }
        auto runable_op = iter->second();
        runable_op->init(op);
        runable_op->set_graph(graph);
        runnable_ops.push_back(runable_op);
    }
    query->reset_dead_boundary();
    return Status::Ok();
}
Status Dagoba::next()
{
    Gremlin *gremlin = new Gremlin(GremlinType::Pull);
    size_t pc = query_->get_op_num() - 1;
    size_t dead_boundary = query_->get_dead_boundary();
    std::vector<RunableOp *> &runnable_ops = query_->get_runnable_operators();
    while (pc < query_->get_op_num())
    {
        auto *cur_runable_op = runnable_ops[pc];
        Status s = cur_runable_op->run(gremlin);
        if (!s.isOk())
        {
            return s;
        }
        switch (gremlin->type_)
        {
        case GremlinType::Pull:
            pc--;
            if (pc < dead_boundary)
            {
                dead_boundary = std::min(dead_boundary + 1, query_->get_op_num());
                pc = dead_boundary;
            }
            break;
        case GremlinType::Data:
            pc++;
            break;
        case GremlinType::Dead:
            pc++;
            dead_boundary = pc;
            gremlin->type_ = GremlinType::Pull;
            break;
        default:
            return Status::Error("unkwon GreminlinType");
        }
    }
    return Status::Ok();
}