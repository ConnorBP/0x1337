#pragma once

#include <cstddef>
#include "Platform.h"

#define CONCAT(a, b) a##b
#define PAD_NAME(n) CONCAT(pad, n)

#define PAD(size) \
private: \
    std::byte PAD_NAME(__COUNTER__) [size]; \
public:
