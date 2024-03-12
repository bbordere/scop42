#include "BmpImage.hpp"
#include <algorithm>
#include <array>

void BmpImage::reverse() {
	int32_t w = this->infoHeader.imgWidth;
	int32_t h = this->infoHeader.imgHeight;

	std::vector<ColorU> tmp(w);

	for (int i = 0; i < h / 2; ++i) {
		std::copy(this->data.begin() + i * w, this->data.begin() + (i + 1) * w,
				  tmp.begin());
		std::copy(this->data.begin() + (h - 1 - i) * w,
				  this->data.begin() + (h - i) * w, this->data.begin() + i * w);
		std::copy(tmp.begin(), tmp.end(), this->data.begin() + (h - 1 - i) * w);
	}
}

void BmpImage::extractData(std::string const &filePath) {
	this->data.clear();
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

	std::streambuf *buff = file.rdbuf();
	std::size_t size =
		buff->pubseekoff(this->fileHeader.startAddr, file.end, file.in);
	buff->pubseekpos(this->fileHeader.startAddr, file.in);
	this->data.reserve(size);
	char *buffer = new char[size];

	buff->sgetn(buffer, size);
	size_t i = 0;
	size_t rowSizeInBytes =
		((infoHeader.bpp * infoHeader.imgWidth + 31) / 32) * 4;
	while (i < rowSizeInBytes * infoHeader.imgHeight) {
		for (int32_t n = 0; n < infoHeader.imgWidth; ++n) {
			ColorU color = {static_cast<uint8_t>(buffer[i + 2]),
							static_cast<uint8_t>(buffer[i + 1]),
							static_cast<uint8_t>(buffer[i]), 1};

			this->data.emplace_back(color);
			i += 3;
		}
		i += rowSizeInBytes - (infoHeader.imgWidth * (infoHeader.bpp / 8));
	}
	delete[] buffer;
}

ColorU const *BmpImage::getData() const {
	return (this->data.data());
}

BmpFileHeader const &BmpImage::getFileHeader() {
	return (this->fileHeader);
}

BmpInfoHeader const &BmpImage::getInfoHeader() {
	return (this->infoHeader);
}

void BmpImage::clear() {
	this->data.clear();
}
