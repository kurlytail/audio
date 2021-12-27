
#pragma once

#include <memory>
#include <vector>

namespace bst {

struct Output;
struct Unit;
struct Input;

struct Input {
    std::shared_ptr<Output> src;
    std::shared_ptr<Unit> dst;
};

struct Output {
    std::shared_ptr<Input> dst;
    std::shared_ptr<Unit> src;
};

struct Unit {
    std::vector<std::shared_ptr<Input>> inputs;
    std::vector<std::shared_ptr<Output>> outputs;
};
} // namespace bst