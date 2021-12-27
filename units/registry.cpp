
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "unit.h"

namespace bst {
struct registry {
    std::unordered_map<std::string, std::shared_ptr<Unit>> units;
    std::mutex mutex;

    bool add_unit(const std::string &name, std::shared_ptr<Unit> &unit);
    void remove_unit(const std::string &name);
};

bool registry::add_unit(const std::string &name, std::shared_ptr<Unit> &unit)
{
    std::scoped_lock lock(mutex);
    return units.insert(std::pair(name, unit)).second;
}

void registry::remove_unit(const std::string &name)
{
    std::scoped_lock lock(mutex);
    units.erase(name);
}
} // namespace bst