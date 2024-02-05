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
	std::cout << "SIZE " << size / 8 << '\n';

	// get file data
	buff->sgetn(buffer, size);

	// stbi_set_flip_vertically_on_load(false);

	// int width, height, nrChannels;
	// unsigned char *data =
	// stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

	ssize_t i = (size / 8);
	int pad = (this->infoHeader.imgWidth * (this->infoHeader.bpp / 8)) % 4;
	std::cout << "TEST\n";
	ssize_t temp;

	while (i > 0) {
		i -= pad + ((this->infoHeader.imgWidth * 3) - 1);
		for (std::size_t n = 0; n < this->infoHeader.imgWidth; ++n) {
			std::cout << "R: " << (int)((unsigned char)(buffer[i + 2])) << ", "
					  << "G: " << (int)((unsigned char)(buffer[i + 1])) << ", "
					  << "B: " << (int)((unsigned char)(buffer[i])) << "\n";
			this->data.push_back(buffer[i + 2]);
			this->data.push_back(buffer[i + 1]);
			this->data.push_back(buffer[i]);
			i += 3;
		}
		i -= ((this->infoHeader.imgWidth * 3) + 1);
	}
	// this->data.resize(this)

	// std::cout << "PAD " << pad << '\n';
	// // uint64_t padBuff[4];
	// uint64_t lenRow = this->infoHeader.imgWidth * this->infoHeader.bpp;
	// if (!pad) {
	// 	file.seekg(this->fileHeader.startAddr, std::ios::beg);
	// 	file.read(reinterpret_cast<char *>(this->data.data()), data.size());
	// }
	// else {
	// 	for (uint16_t i = 0; i < std::abs(this->infoHeader.imgHeight); ++i) {
	// 		file.read(
	// 			reinterpret_cast<char *>(this->data.data() + (lenRow * i)),
	// 			lenRow);
	// 		// file.read(reinterpret_cast<char *>(&padBuff), pad);
	// 	}
	// }

	// int strideSize =
	// ((this->infoHeader.bpp * this->infoHeader.imgWidth) / 32) * 4;
	// std::cout << this->infoHeader.bpp << '\n';

	// if (this->infoHeader.imgWidth % 4 == 0) {

	// this->data.assign(buffer, buffer + data.size());
	// }
	// else {
	// 	uint32_t lenRow = this->infoHeader.imgWidth * this->infoHeader.bpp /
	// 8; 	uint32_t lenPad = pad4(lenRow) - lenRow; 	uint64_t padBuff =
	// 0; 	for (uint16_t i = 0; i < std::abs(this->infoHeader.imgHeight);
	// ++i) { 		file.read(reinterpret_cast<char *>(this->data.data() +
	// lenRow
	// * i), 				  lenRow); 		file.read(reinterpret_cast<char
	// *>(&padBuff), lenPad);
	// 	}

	// 	for (int i = 0; i < this->data.size(); i += 3) {
	// 		std::swap(data[i], data[i + 2]);
	// 	}
	// }
	// }
	// for (int row = 0; row < this->infoHeader.imgHeight / 2; ++row) {
	// 	int topOffset =
	// 		row * (this->infoHeader.imgWidth * this->infoHeader.bpp / 8);
	// 	int bottomOffset =
	// 		(this->infoHeader.imgHeight - 1 - row) *
	// 		(this->infoHeader.imgWidth * this->infoHeader.bpp / 8);
	// 	std::swap_ranges(
	// 		this->data.begin() + topOffset,
	// 		this->data.begin() + topOffset +
	// 			(this->infoHeader.imgWidth * this->infoHeader.bpp / 8),
	// 		this->data.begin() + bottomOffset);
	// }

	// if (this->infoHeader.bpp == 24) {
	// for (int i = 0; i < this->data.size() / 2; i += 3) {
	// 	std::swap(this->data[i], this->data[i + 2]);
	// }
	// }
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
