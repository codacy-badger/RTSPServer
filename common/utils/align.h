#ifndef PROJECT_ALIGN_H
#define PROJECT_ALIGN_H

#include <cstddef>

namespace memory {
namespace align {
inline bool is_aligned(std::size_t value, std::size_t alignment) {
    return (value & (alignment - 1));
}

inline std::size_t align_down(std::size_t value, std::size_t alignment) {
    return (value & ~(alignment - 1));
}

inline std::size_t align_up(std::size_t value, std::size_t alignment) {
    return ((value + alignment - 1) & ~(alignment - 1));
}
}
}

#endif //PROJECT_ALIGN_H
