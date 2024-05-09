#include "dagoba/runnable_op/runnable_op.h"
#include "dagoba/runnable_op/register.h"
Status Unique::run(Gremlin *gremlin)
{
    switch (gremlin->type_)
    {
    case GremlinType::Pull:
        return Status::Ok();
    case GremlinType::Data:
        if (gremlin->verties_.empty())
        {
            gremlin->type_ = GremlinType::Pull;
            gremlin->verties_.clear();
            return Status::Ok();
        }
        else
        {
            auto vertex_id = gremlin->verties_[0];
            auto iter = seen_set_.find(vertex_id);
            if (iter == seen_set_.end())
            {
                seen_set_.insert(vertex_id);
                return Status::Ok();
            }
            else
            {
                gremlin->type_ = GremlinType::Pull;
                gremlin->verties_.clear();
                return Status::Ok();
            }
        }
    default:
        return Status::Error("got wrong GremlinType");
    }
};

Status Take::run(Gremlin *gremlin)
{
    switch (gremlin->type_)
    {
    case GremlinType::Pull:
        return Status::Ok();
    case GremlinType::Data:
        if (gremlin->verties_.empty())
        {
            gremlin->type_ = GremlinType::Pull;
            gremlin->verties_.clear();
            return Status::Ok();
        }
        else
        {
            if (count_ < threshold_)
            {
                count_++;
                return Status::Ok();
            }
            else
            {
                gremlin->type_ = GremlinType::Dead;
                gremlin->verties_.clear();
                return Status::Ok();
            }
        }
    default:
        return Status::Error("got wrong GremlinType");
    }
}


Status FakeSource::run(Gremlin *gremlin){
    switch (gremlin->type_)
    {
    case GremlinType::Pull:
    case GremlinType::Data:
        gremlin->type_ = GremlinType::Data; 
        if (gremlin->verties_.empty())
        {
            gremlin->verties_.resize(1);
        }
        gremlin->verties_[0] = 2;
    default:
        return Status::Error("got wrong GremlinType");
    }
};
