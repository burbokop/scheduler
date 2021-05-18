#include "time.h"

#include <chrono>

size_t Time::nowMillis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()
                ).count();
}
