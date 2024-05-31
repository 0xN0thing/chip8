#include "nt/chip8_std_impl.h"
#include <chrono>
#include <fstream>

nt::chip8::VMImpl::VMImpl()
    : randGen(std::chrono::system_clock::now().time_since_epoch().count())
{
    randByte = std::uniform_int_distribution<uint16_t>(0, 255U);
}

nt::chip8::VMImpl::~VMImpl() {}

void nt::chip8::VMImpl::LoadRomFromFile(const char *filename)
{
    std::ifstream stream;
    stream.open(filename, std::ios::binary | std::ios::ate);

    if (stream.is_open())
    {
        const std::streampos size = stream.tellg();
        char *buffer = new char[size];

        stream.seekg(0, std::ios::beg);
        stream.read(buffer, size);
        stream.close();

        LoadRom(buffer, size);

        delete[] buffer;
    }

    stream.close();
}


uint8_t nt::chip8::VMImpl::GenRandom() { return randByte(randGen); }
