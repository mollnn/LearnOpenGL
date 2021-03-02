#ifndef BIN_READER_H
#define BIN_READER_H

#include <fstream>
#include <string>
#include "uint.h"

class BinReader
{
public:
	BinReader();
	BinReader(const std::string& filePath);
	~BinReader();

	void open(const std::string& filePath);
	bool isOpen();

	uint8 read8();
	uint16 read16();
	uint32 read32();

private:
	std::ifstream file;
	bool opened{ false };
	uint8 buffer[4];

	uint32 bytesToInt(uint8 bytes[], int n);
};

BinReader::BinReader()
{

}

BinReader::BinReader(const std::string& filePath)
{
	open(filePath);
}

BinReader::~BinReader()
{
	if (opened) {
		file.close();
	}
}

void BinReader::open(const std::string& filePath)
{
	file.open(filePath, std::ios::binary);
	opened = !file.bad();
}

bool BinReader::isOpen()
{
	return opened;
}

uint8 BinReader::read8()
{
	file.read((char*)buffer, 1);
	return (uint8)bytesToInt(buffer, 1);
}

uint16 BinReader::read16()
{
	file.read((char*)buffer, 2);
	return (uint16)bytesToInt(buffer, 2);
}

uint32 BinReader::read32()
{
	file.read((char*)buffer, 4);
	return (uint32)bytesToInt(buffer, 4);
}

uint32 BinReader::bytesToInt(uint8 bytes[], int n)
{
	//LittleEndian
	switch (n) {
	default:
	case 1: //8 bits
		return bytes[0];
	case 2: //16 bits
		return (bytes[1] << 8) | bytes[0];
	case 4: //32 bits
		return (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0];
	}
}

#endif