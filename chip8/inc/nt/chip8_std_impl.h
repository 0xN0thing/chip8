#pragma once

#include <nt/chip8.h>
#include <random>

namespace nt
{
    namespace chip8
    {
        class VMImpl : public IVirtualMachine
        {
          public:
            VMImpl();

            ~VMImpl();

          public: // implementation
            void LoadRomFromFile(const char *filename);

            uint8_t GenRandom() override;

          private:
            std::default_random_engine randGen;
            std::uniform_int_distribution<uint16_t> randByte;
        };
    } // namespace chip8
} // namespace nt