#pragma once

#include "Color.hpp"
#include <cmath>
#include <cstdint>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <vector>

#pragma pack(push, 2)

// clang-format off
struct BmpFileHeader {
	char magicWord[2];
	uint32_t fileSize;
	uint16_t reservedBlank[2];
	uint32_t startAddr;
};

struct BmpInfoHeader {
	uint32_t size;
	int32_t imgWidth;
	int32_t imgHeight;
	uint16_t planes;
	uint16_t bpp;
	uint32_t compression;
	uint32_t imgSize;
	int32_t hRes;
	int32_t vRes;
	uint32_t ColorUsed;
	uint32_t colorImportant;
};

// clang-format on

#pragma pack(pop)

class BmpImage {
	private:
		BmpFileHeader fileHeader;
		BmpInfoHeader infoHeader;
		std::vector<ColorU> data;

	public:
		void extractData(std::string const &filePath);
		BmpFileHeader const &getFileHeader();
		BmpInfoHeader const &getInfoHeader();
		ColorU const *getData() const;
		void reverse();
		void clear();
};
