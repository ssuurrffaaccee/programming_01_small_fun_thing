#pragma once
#include <vector>
#include <unordered_map>
#include <string>
struct Op
{
    std::string name;
    std::vector<std::string> args;
    std::unordered_map<std::string, std::string> kv_args;
};