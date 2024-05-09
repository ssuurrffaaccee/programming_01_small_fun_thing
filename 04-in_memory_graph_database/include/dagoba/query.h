#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "dagoba/common.h"
#include "dagoba/op.h"
#include "dagoba/runnable_op/runnable_op.h"
class Query
{
public:
    Query() {}
    Query &add_op(const Op &op)
    {
        operators_.push_back(op);
        return *this;
    }
    const std::vector<Op> &get_operators()
    {
        return operators_;
    }
    std::vector<RunableOp*>& get_runnable_operators()
    {
        return runnable_ops_;
    }
    void reset_dead_boundary()
    {
        dead_boundary = 0;
    }
    size_t get_op_num(){
        return operators_.size();
    }
    size_t get_dead_boundary(){
        return dead_boundary;
    }
private:
    std::vector<Op> operators_;
    std::vector<RunableOp *> runnable_ops_;
    size_t dead_boundary;
    DISALLOW_COPY_AND_ASSIGNMENT_CONSTRUCTOR(Query)
};