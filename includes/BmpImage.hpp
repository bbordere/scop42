#pragma once

#include <cmath>
#include <cstdint>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <vector>

#define pad4(x) (((x >> 2) + 1) << 2)

#pragma pack(push, 2)

// clang-format off
struct BmpFileHeader {
	char magicWord[2];
	uint32_t fileSize;
	uint16_t reservedBlank[2];
	uint32_t startAddr;
};

struct BmpInfoHeader {
	uint32_t infoHeaderSize;
	int32_t imgWidth;
	int32_t imgHeight;
	uint16_t planes;
	uint16_t bpp;
	uint32_t compression;
	uint32_t imgSize;
	int32_t hRes;
	int32_t vRes;
	uint32_t colorUsed;
	uint32_t colorImportant;
};
// clang-format on

#pragma pack(pop)

class BmpImage {
	private:
	public:
		BmpFileHeader fileHeader = {0};
		BmpInfoHeader infoHeader = {0};
		std::vector<uint8_t> data;

	public:
		void extractData(std::string const &filePath);
		uint8_t *getPixelBuffer();
		BmpFileHeader const &getFileHeader();
		BmpInfoHeader const &getInfoHeader();
};
