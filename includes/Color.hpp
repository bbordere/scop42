#pragma once

#include <cstdint>

// clang-format off

template<typename T>
struct Color {
	T b;
	T g;
	T r;
	T a;
};

// clang-format on

typedef Color<float> ColorF;
typedef Color<uint8_t> ColorU;
