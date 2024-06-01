#include <memory.h>
#include <nt/chip8.h>

#define REG_VF 0xF

#define VIDEO_HEIGHT 32
#define VIDEO_WIDTH 64

constexpr uint32_t gFontSetStartAddress = 0x50;

void nt::chip8::IVirtualMachine::cls_00e0() { memset(video, 0, sizeof(video)); }

void nt::chip8::IVirtualMachine::ret_00ee()
{
    --sp;
    pc = stack[sp];
}

void nt::chip8::IVirtualMachine::jp_1nnn()
{
    uint16_t address = opcode & 0x0FFFu;
    pc = address;
}

void nt::chip8::IVirtualMachine::call_2nnn()
{
    uint16_t address = opcode & 0x0FFFu;

    stack[sp] = pc;
    sp += 1;

    pc = address;
}

void nt::chip8::IVirtualMachine::se_3xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t kk = opcode & 0x00FFu;

    if (registers[Vx] == kk)
    {
        pc += 2;
    }
}

void nt::chip8::IVirtualMachine::sne_4xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t kk = opcode & 0x00FFu;

    if (registers[Vx] != kk)
    {
        pc += 2;
    }
}

void nt::chip8::IVirtualMachine::se_5xy0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if (registers[Vx] == registers[Vy])
    {
        pc += 2;
    }
}

void nt::chip8::IVirtualMachine::ld_6xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t kk = opcode & 0x00FFu;

    registers[Vx] = kk;
}

void nt::chip8::IVirtualMachine::add_7xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t kk = opcode & 0x00FFu;

    registers[Vx] += kk;
}

void nt::chip8::IVirtualMachine::ld_8xy0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] = registers[Vy];
}

void nt::chip8::IVirtualMachine::or_8xy1()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] |= registers[Vy];
}

void nt::chip8::IVirtualMachine::and_8xy2()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] &= registers[Vy];
}

void nt::chip8::IVirtualMachine::xor_8xy3()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] ^= registers[Vy];
}

void nt::chip8::IVirtualMachine::add_8xy4()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    uint16_t res = registers[Vx] + registers[Vy];

    registers[REG_VF] = res > 255;
    registers[Vx] = res & 0x00FF;
}

void nt::chip8::IVirtualMachine::sub_8xy5()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[REG_VF] = registers[Vx] > registers[Vy];
    registers[Vx] -= registers[Vy];
}

void nt::chip8::IVirtualMachine::shr_8xy6()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    // Save LSB in VF
    registers[REG_VF] = (registers[Vx] & 0x1u);

    registers[Vx] >>= 1;
}

void nt::chip8::IVirtualMachine::subn_8xy7()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[REG_VF] = registers[Vy] > registers[Vx];
    registers[Vx] = registers[Vy] - registers[Vx];
}

void nt::chip8::IVirtualMachine::shl_8xyE()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    registers[REG_VF] = (registers[Vx] & 0x80u) >> 7u;

    registers[Vx] <<= 1;
}

void nt::chip8::IVirtualMachine::sne_9xy0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if (registers[Vx] != registers[Vy])
    {
        pc += 2;
    }
}

void nt::chip8::IVirtualMachine::ld_annn() { index = opcode & 0x0FFFu; }

void nt::chip8::IVirtualMachine::jp_bnnn()
{
    uint16_t address = opcode & 0x0FFFu;
    pc = registers[0] + address;
}

void nt::chip8::IVirtualMachine::rnd_cxkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t kk = opcode & 0x00FFu;

    registers[Vx] = GenRandom() & kk;
}

void nt::chip8::IVirtualMachine::drw_dxyn()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    uint8_t height = opcode & 0x000Fu;

    // Wrap if going beyond screen boundaries
    uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
    uint8_t yPos = registers[Vy] % VIDEO_HEIGHT;

    registers[0xF] = 0;

    for (unsigned int row = 0; row < height; ++row)
    {
        uint8_t spriteByte = memory[index + row];

        for (unsigned int col = 0; col < 8; ++col)
        {
            uint8_t spritePixel = spriteByte & (0x80u >> col);

            // Calculate the screen position with wrapping
            uint32_t screenX = (xPos + col) % VIDEO_WIDTH;
            uint32_t screenY = (yPos + row) % VIDEO_HEIGHT;

            uint32_t *screenPixel = &video[screenY * VIDEO_WIDTH + screenX];

            // Sprite pixel is on
            if (spritePixel)
            {
                // Screen pixel also on - collision
                if (*screenPixel == 0xFFFFFFFF)
                {
                    registers[0xF] = 1;
                }

                // Effectively XOR with the sprite pixel
                *screenPixel ^= 0xFFFFFFFF;
            }
        }
    }
}

void nt::chip8::IVirtualMachine::skp_ex9e()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t key = registers[Vx];

    if (keypad[registers[Vx]])
    {
        pc += 2;
    }
}

void nt::chip8::IVirtualMachine::sknp_exa1()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t key = registers[Vx];

    if (!keypad[key])
    {
        pc += 2;
    }
}

void nt::chip8::IVirtualMachine::ld_fx07()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    registers[Vx] = delayTimer;
}

void nt::chip8::IVirtualMachine::ld_fx0a()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    if (keypad[0])
    {
        registers[Vx] = 0;
    }
    else if (keypad[1])
    {
        registers[Vx] = 1;
    }
    else if (keypad[2])
    {
        registers[Vx] = 2;
    }
    else if (keypad[3])
    {
        registers[Vx] = 3;
    }
    else if (keypad[4])
    {
        registers[Vx] = 4;
    }
    else if (keypad[5])
    {
        registers[Vx] = 5;
    }
    else if (keypad[6])
    {
        registers[Vx] = 6;
    }
    else if (keypad[7])
    {
        registers[Vx] = 7;
    }
    else if (keypad[8])
    {
        registers[Vx] = 8;
    }
    else if (keypad[9])
    {
        registers[Vx] = 9;
    }
    else if (keypad[10])
    {
        registers[Vx] = 10;
    }
    else if (keypad[11])
    {
        registers[Vx] = 11;
    }
    else if (keypad[12])
    {
        registers[Vx] = 12;
    }
    else if (keypad[13])
    {
        registers[Vx] = 13;
    }
    else if (keypad[14])
    {
        registers[Vx] = 14;
    }
    else if (keypad[15])
    {
        registers[Vx] = 15;
    }
    else
    {
        pc -= 2;
    }
}

void nt::chip8::IVirtualMachine::ld_fx15()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    delayTimer = registers[Vx];
}

void nt::chip8::IVirtualMachine::ld_fx18()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    soundTimer = registers[Vx];
}

void nt::chip8::IVirtualMachine::ld_fx1e()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    index += registers[Vx];
}

void nt::chip8::IVirtualMachine::ld_fx29()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t digit = registers[Vx];

    index = gFontSetStartAddress + (5 * digit);
}

void nt::chip8::IVirtualMachine::ld_fx33()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t value = registers[Vx];

    memory[index + 2] = value % 10;
    value /= 10;

    memory[index + 1] = value % 10;
    value /= 10;

    memory[index] = value % 10;
}

void nt::chip8::IVirtualMachine::ld_fx55()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    for (uint32_t i = 0; i <= Vx; i += 1)
    {
        memory[index + i] = registers[i];
    }
}

void nt::chip8::IVirtualMachine::ld_fx65()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    for (uint32_t i = 0; i <= Vx; i += 1)
    {
        registers[i] = memory[index + i];
    }
}

void nt::chip8::IVirtualMachine::null_0() {}
