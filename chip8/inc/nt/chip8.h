#pragma once

#include <stdint.h>

namespace nt
{
    namespace chip8
    {
        constexpr uint32_t gDefaultFontSetSize = 80;
        extern const uint8_t gDefaultFontSet[gDefaultFontSetSize];

        class IVirtualMachine
        {
          protected:
            uint8_t registers[16]{};
            uint8_t memory[4096]{};
            uint16_t index{};
            uint16_t pc{};
            uint16_t stack[16]{};
            uint8_t sp{};
            uint8_t delayTimer{};
            uint8_t soundTimer{};
            uint8_t keypad[16]{};
            uint32_t video[64 * 32]{};
            uint16_t opcode;

          protected:
            IVirtualMachine();


            void cls_00e0();
            void ret_00ee();
            void jp_1nnn();
            void call_2nnn();
            void se_3xkk();
            void sne_4xkk();
            void se_5xy0();
            void ld_6xkk();
            void add_7xkk();
            void ld_8xy0();
            void or_8xy1();
            void and_8xy2();
            void xor_8xy3();
            void add_8xy4();
            void sub_8xy5();
            void shr_8xy6();
            void subn_8xy7();
            void shl_8xyE();
            void sne_9xy0();
            void ld_annn();
            void jp_bnnn();
            void rnd_cxkk();
            void drw_dxyn();
            void skp_ex9e();
            void sknp_exa1();
            void ld_fx07();
            void ld_fx0a();
            void ld_fx15();
            void ld_fx18();
            void ld_fx1e();
            void ld_fx29();
            void ld_fx33();
            void ld_fx55();
            void ld_fx65();

            void null_0();

            virtual uint8_t GenRandom() = 0;
          public:
            virtual ~IVirtualMachine();

            const uint32_t *GetVideoBuffer() const;
            void Update();
            void ResetPC();

            const uint16_t GetCurrentOpcode() const;
            const uint16_t GetPC() const;
            const uint16_t* const GetStack() const;
            const uint8_t* const GetRegisters() const;

            void LoadRom(const char* bytes, int len);
            void LoadFont(const uint8_t* const buffer, const uint32_t fontSize);

            virtual void LoadRomFromFile(const char* path) = 0;

            void ClearScreen();

            uint8_t* KeyPad();

          private:
            typedef void (IVirtualMachine::*Chip8Instruction)();
            Chip8Instruction vtable[17];
            Chip8Instruction vtable0[15];
            Chip8Instruction vtable8[15];
            Chip8Instruction vtableF[0x65 + 1];
            Chip8Instruction vtableE[15];

            void Table0();
            void Table8();
            void TableF();
            void TableE();
        };

    } // namespace chip8
} // namespace nt
