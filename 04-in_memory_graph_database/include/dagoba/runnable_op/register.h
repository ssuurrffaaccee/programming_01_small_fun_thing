#pragma once
#include <unordered_map>
class RunableOp;
class OpRegister
{
public:
    static std::unordered_map<std::string, std::function<RunableOp *()>> &get_op_contructors()
    {
        static std::unordered_map<std::string, std::function<RunableOp *()>> constructors;
        return constructors;
    }
};
#define REGISTE_OP(Class, Name)                                      \
    struct OpRegister_##Class                                        \
    {                                                                \
        OpRegister_##Class()                                         \
        {                                                            \
            auto &op_contructors = OpRegister::get_op_contructors(); \
            op_contructors[Name] = []() -> RunableOp * {             \
                return new Class();                                  \
            };                                                       \
        }                                                            \
    };                                                               \
    static OpRegister_##Class OpRegister_##Class##_instance;
