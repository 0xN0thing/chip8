#include "nt/chip8.h"

#include <memory.h>

constexpr uint32_t gStartAddress = 0x200;
constexpr uint32_t gFontSetStartAddress = 0x50;

const uint8_t nt::chip8::gDefaultFontSet[nt::chip8::gDefaultFontSetSize] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void nt::chip8::IVirtualMachine::Table0()
{
    (this->*vtable0[opcode & 0x000f])();
}

void nt::chip8::IVirtualMachine::Table8()
{
    (this->*vtable8[opcode & 0x000f])();
}

void nt::chip8::IVirtualMachine::TableF()
{
    (this->*vtableF[opcode & 0x00ff])();
}

void nt::chip8::IVirtualMachine::TableE()
{
    (this->*vtableE[opcode & 0x000f])();
}

nt::chip8::IVirtualMachine::IVirtualMachine()
{
    pc = gStartAddress;
    {
        size_t i = 0;
        for (; i <= 0xE; i += 1)
        {
            vtable0[i] = &IVirtualMachine::null_0;
            vtable8[i] = &IVirtualMachine::null_0;
            vtableE[i] = &IVirtualMachine::null_0;
            vtableF[i] = &IVirtualMachine::null_0;
        }

        for (; i <= 0x65; i += 1)
        {
            vtableF[i] = &IVirtualMachine::null_0;
        }
    }

    vtable[0x0] = &IVirtualMachine::Table0;

    {
        vtable0[0x0] = &IVirtualMachine::cls_00e0;
        vtable0[0xe] = &IVirtualMachine::ret_00ee;
    }

    vtable[0x1] = &IVirtualMachine::jp_1nnn;
    vtable[0x2] = &IVirtualMachine::call_2nnn;
    vtable[0x3] = &IVirtualMachine::se_3xkk;
    vtable[0x4] = &IVirtualMachine::sne_4xkk;
    vtable[0x5] = &IVirtualMachine::se_5xy0;
    vtable[0x6] = &IVirtualMachine::ld_6xkk;
    vtable[0x7] = &IVirtualMachine::add_7xkk;
    vtable[0x8] = &IVirtualMachine::Table8;

    {
        vtable8[0x0] = &IVirtualMachine::ld_8xy0;
        vtable8[0x1] = &IVirtualMachine::or_8xy1;
        vtable8[0x2] = &IVirtualMachine::and_8xy2;
        vtable8[0x3] = &IVirtualMachine::xor_8xy3;
        vtable8[0x4] = &IVirtualMachine::add_8xy4;
        vtable8[0x5] = &IVirtualMachine::sub_8xy5;
        vtable8[0x6] = &IVirtualMachine::shr_8xy6;
        vtable8[0x7] = &IVirtualMachine::subn_8xy7;
        vtable8[0xE] = &IVirtualMachine::shl_8xyE;
    }

    vtable[0x9] = &IVirtualMachine::sne_9xy0;
    vtable[0xA] = &IVirtualMachine::ld_annn;
    vtable[0xB] = &IVirtualMachine::jp_bnnn;
    vtable[0xC] = &IVirtualMachine::rnd_cxkk;
    vtable[0xD] = &IVirtualMachine::drw_dxyn;
    vtable[0xE] = &IVirtualMachine::TableE;

    {
        vtableE[0xE] = &IVirtualMachine::skp_ex9e;
        vtableE[0x1] = &IVirtualMachine::sknp_exa1;
    }

    vtable[0xF] = &IVirtualMachine::TableF;

    {
        vtableF[0x07] = &IVirtualMachine::ld_fx07;
        vtableF[0x0a] = &IVirtualMachine::ld_fx0a;
        vtableF[0x15] = &IVirtualMachine::ld_fx15;
        vtableF[0x18] = &IVirtualMachine::ld_fx18;
        vtableF[0x1e] = &IVirtualMachine::ld_fx1e;
        vtableF[0x29] = &IVirtualMachine::ld_fx29;
        vtableF[0x33] = &IVirtualMachine::ld_fx33;
        vtableF[0x55] = &IVirtualMachine::ld_fx55;
        vtableF[0x65] = &IVirtualMachine::ld_fx65;
    }

    LoadFont(gDefaultFontSet, 80);
}

nt::chip8::IVirtualMachine::~IVirtualMachine() {}

void nt::chip8::IVirtualMachine::ResetPC() { pc = gStartAddress; }

void nt::chip8::IVirtualMachine::Update()
{
    opcode = (memory[pc] << 8u) | memory[pc + 1];

    pc += 2;

    (this->*vtable[(opcode & 0xf000u) >> 12u])();

    if (delayTimer > 0)
    {
        delayTimer -= 1;
    }

    if (soundTimer > 0)
    {
        soundTimer -= 1;
    }
}

const uint32_t *nt::chip8::IVirtualMachine::GetVideoBuffer() const
{
    return video;
}

void nt::chip8::IVirtualMachine::LoadRom(const char *bytes, int len)
{
    for (long i = 0; i < len; i += 1)
    {
        memory[gStartAddress + i] = bytes[i];
    }
}

void nt::chip8::IVirtualMachine::LoadFont(const uint8_t *const buffer,
                                          const uint32_t fontSize)
{
    for (uint32_t i = 0; i < fontSize; i += 1)
    {
        memory[gFontSetStartAddress] = buffer[i];
    }
}
