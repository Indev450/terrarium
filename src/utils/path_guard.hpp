#ifndef UTIL_PATH_GUARD_HPP
#define UTIL_PATH_GUARD_HPP

#include <filesystem>

namespace fs = std::filesystem;

// Saves directory path and changes current path to it in destructor
class PathGuard {
    const fs::path restore_path;

public:
    PathGuard(const fs::path &_restore_path):
        restore_path(_restore_path)
    {}

    ~PathGuard() {
        fs::current_path(restore_path);
    }
};

#endif
