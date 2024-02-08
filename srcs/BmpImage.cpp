#include "BmpImage.hpp"
#include "stb_image.h"
#include <algorithm>
#include <array>

void BmpImage::extractData(std::string const &filePath) {
	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open())
		throw std::runtime_error("Failed to open file (" + filePath + ") !");
	if (!file.read(reinterpret_cast<char *>(&this->fileHeader),
				   sizeof(this->fileHeader)))
		throw std::runtime_error("Failed to read file !");
	if (!file.read(reinterpret_cast<char *>(&this->infoHeader),
				   sizeof(this->infoHeader)))
		throw std::runtime_error("Failed to read file !");
	if (this->infoHeader.size < 40)
		throw std::runtime_error("Header type not handled !");
	if (this->infoHeader.compression)
		throw std::runtime_error("Compression not handled !");
	auto buff = file.rdbuf();
	std::size_t size =
		buff->pubseekoff(this->fileHeader.startAddr, file.end, file.in);
	buff->pubseekpos(this->fileHeader.startAddr, file.in);

	char *buffer = new char[size];

	buff->sgetn(buffer, size);

	size_t i = 0;
	size_t rowSizeInBytes =
		((infoHeader.bpp * infoHeader.imgWidth + 31) / 32) * 4;
	while (i < rowSizeInBytes * infoHeader.imgHeight) {
		for (int32_t n = 0; n < this->infoHeader.imgWidth; ++n) {
			this->data.push_back(buffer[i]);
			this->data.push_back(buffer[i + 1]);
			this->data.push_back(buffer[i + 2]);
			i += 3;
		}
	}
	delete[] buffer;
}

uint8_t *BmpImage::getPixelBuffer() {
	uint8_t *res =
		new uint8_t[(this->infoHeader.imgWidth * this->infoHeader.imgHeight)];
	// for (std::size_t i = 0; i < (this->infoHeader.imgHeight) / 2; ++i) {
	// 	for (std::size_t j = 0; j < (this->infoHeader.imgWidth * 3); ++j) {
	// 		std::swap(this->data[i * this->infoHeader.imgWidth * 3 + j],
	// 				  this->data[((this->infoHeader.imgHeight - i) *
	// 							  this->infoHeader.imgWidth * 3) +
	// 							 j]);
	// 	}
	// }
	// std::size_t x = -1;
	// for (std::size_t i = 0; i < this->data.size(); i += 3) {
	// 	res[++x] = this->data[i];
	// 	res[++x] = this->data[i + 1];
	// 	res[++x] = this->data[i + 2];
	// 	// res[++x] = 255;
	// }

	return (res);
}

BmpFileHeader const &BmpImage::getFileHeader() {
	return (this->fileHeader);
}

BmpInfoHeader const &BmpImage::getInfoHeader() {
	return (this->infoHeader);
}
