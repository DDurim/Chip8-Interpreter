#include "../lib/Chip8.hpp"

#include <fstream>
#include <iostream>
#include <random>


std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> distrib(0, 255);

uint8_t lFontset[80]
{
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

Chip8::Chip8()
	: mProgramCounter{ 0x200 } {

	for (uint8_t a{}; a != 80; ++a)
	{
		mMemory[0x50 + a] = lFontset[a];
	}
}

Chip8::Chip8(const char* const iSource)
	: Chip8() {
	LoadRom(iSource);
}

void Chip8::LoadRom(const char* const iSource)
{
	std::ifstream lSource(iSource, std::ios::binary | std::ios::ate);

	if (lSource.is_open())
	{
		std::cout << "File Opened" << std::endl;

		std::streampos lSize{ lSource.tellg() };

		if (lSize < 3584)
		{
			lSource.seekg(0, std::ios::beg);
			lSource.read(reinterpret_cast<char*>(&mMemory[0x200]), lSize);
			lSource.close();

			std::cout << "Succesfully Loaded" << std::endl;
		}
		else {
			std::cout << "Too Big" << std::endl;
		}

	}

}

void Chip8::EmulateCycle()
{
	mOpcode = (mMemory[mProgramCounter] << 8u) | mMemory[mProgramCounter + 1];
	mProgramCounter += 2;

	switch (mOpcode & 0xF000u)
	{
	case 0x0000:
	{
		switch (mOpcode & 0x000Fu)
		{
		case 0x0000:
			OP_00E0();
			break;

		case 0x000E:
			OP_00EE();
			break;
		}
	}break;

	case 0x1000:
		OP_1NNN();
		break;

	case 0x2000:
		OP_2NNN();
		break;

	case 0x3000:
		OP_3XNN();
		break;

	case 0x4000:
		OP_4XNN();
		break;

	case 0x5000:
		OP_5XY0();
		break;

	case 0x6000:
		OP_6XNN();
		break;

	case 0x7000:
		OP_7XNN();
		break;

	case 0x8000:
	{
		switch (mOpcode & 0x000Fu)
		{
		case 0x0000:
			OP_8XY0();
			break;

		case 0x0001:
			OP_8XY1();
			break;

		case 0x0002:
			OP_8XY2();
			break;

		case 0x0003:
			OP_8XY3();
			break;

		case 0x0004:
			OP_8XY4();
			break;

		case 0x0005:
			OP_8XY5();
			break;

		case 0x0006:
			OP_8XY6();
			break;

		case 0x0007:
			OP_8XY7();
			break;

		case 0x000E:
			OP_8XYE();
			break;
		}
	}break;

	case 0x9000:
		OP_9XY0();
		break;

	case 0xA000:
		OP_ANNN();
		break;

	case 0xB000:
		OP_BNNN();
		break;

	case 0xC000:
		OP_CXKK();
		break;

	case 0xD000:
		OP_DXYN();
		break;

	case 0xE000:
	{
		switch (mOpcode & 0x00FFu)
		{
		case 0x00A1:
			OP_EXA1();
			break;

		case 0x009E:
			OP_EX9E();
			break;
		}
	}break;

	case 0xF000:
	{
		switch (mOpcode & 0x00FFu)
		{
		case 0x0007:
			OP_Fx07();
			break;

		case 0x000A:
			OP_FX0A();
			break;

		case 0x0015:
			OP_FX15();
			break;

		case 0x001E:
			OP_FX1E();
			break;

		case 0x0029:
			OP_FX29();
			break;

		case 0x0033:
			OP_FX33();
			break;

		case 0x0055:
			OP_FX55();
			break;

		case 0x0065:
			OP_FX65();
			break;
		}
	}break;

	}
}

uint8_t* Chip8::getRegisters()
{
	return mRegisters;
}

uint8_t* Chip8::getMemory()
{
	return mMemory;
}

uint8_t* Chip8::getVideo()
{
	return mVideo;
}

uint8_t* Chip8::getKeypad()
{
	return mKeypad;
}

uint8_t* Chip8::getDelayTimer()
{
	return &mDelayTimer;
}

uint16_t Chip8::getProgramCounter()
{
	return mProgramCounter;
}

uint16_t Chip8::getIndexRegister()
{
	return mIndexRegister;
}

uint16_t Chip8::getOpcode()
{
	return mOpcode;
}

uint16_t* Chip8::getStack()
{
	return mStack;
}

uint8_t Chip8::getStackPointer()
{
	return mStackPointer;
}


void Chip8::OP_00E0()
{
	for (int a{}; a != 32 * 64; ++a)
	{
		mVideo[a] = 0;
	}
}

void Chip8::OP_00EE()
{
	mStackPointer--;
	mProgramCounter = mStack[mStackPointer];
}

void Chip8::OP_1NNN()
{
	const uint16_t lAddress = mOpcode & 0x0FFFu;

	mProgramCounter = lAddress;
}

void Chip8::OP_2NNN()
{
	const uint16_t lAddress = mOpcode & 0x0FFFu;

	mStack[mStackPointer] = mProgramCounter;
	mStackPointer++;

	mProgramCounter = lAddress;
}

void Chip8::OP_3XNN()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;
	const uint8_t lValue = (mOpcode & 0x00FFu);

	if (mRegisters[lRegisterX] == lValue)
	{
		mProgramCounter += 2;
	}
}

void Chip8::OP_4XNN()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;
	const uint8_t lValue = (mOpcode & 0x00FFu);

	if (mRegisters[lRegisterX] != lValue)
	{
		mProgramCounter += 2;
	}
}

void Chip8::OP_5XY0()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;
	const uint8_t lRegisterY = (mOpcode & 0x00F0u) >> 4u;

	if (mRegisters[lRegisterX] == mRegisters[lRegisterY])
	{
		mProgramCounter += 2;
	}
}

void Chip8::OP_6XNN()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;
	const uint8_t lValue = (mOpcode & 0x00FFu);

	mRegisters[lRegisterX] = lValue;
}

void Chip8::OP_7XNN()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;
	const uint8_t lValue = mOpcode & 0x00FFu;

	mRegisters[lRegisterX] += lValue;
}

void Chip8::OP_8XY0()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;
	const uint8_t lRegisterY = (mOpcode & 0x00F0u) >> 4u;

	mRegisters[lRegisterX] = mRegisters[lRegisterY];
}

void Chip8::OP_8XY1()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;
	const uint8_t lRegisterY = (mOpcode & 0x00F0u) >> 4u;

	mRegisters[lRegisterX] |= mRegisters[lRegisterY];
	mRegisters[0xFu] = 0;
}

void Chip8::OP_8XY2()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;
	const uint8_t lRegisterY = (mOpcode & 0x00F0u) >> 4u;

	mRegisters[lRegisterX] &= mRegisters[lRegisterY];
	mRegisters[0xFu] = 0;
}

void Chip8::OP_8XY3()
{

	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;
	const uint8_t lRegisterY = (mOpcode & 0x00F0u) >> 4u;

	mRegisters[lRegisterX] ^= mRegisters[lRegisterY];
	mRegisters[0xFu] = 0;
}

void Chip8::OP_8XY4()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;
	const uint8_t lRegisterY = (mOpcode & 0x00F0u) >> 4u;

	uint8_t lFlag{};

	uint16_t lSum = mRegisters[lRegisterX] + mRegisters[lRegisterY];

	if (lSum > 255u)
	{
		lFlag = 1;
	}
	else {
		lFlag = 0;
	}

	mRegisters[lRegisterX] = lSum & 0xFFu;
	mRegisters[0xFu] = lFlag;
}

void Chip8::OP_8XY5()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;
	const uint8_t lRegisterY = (mOpcode & 0x00F0u) >> 4u;

	uint8_t lFlag{};

	if (mRegisters[lRegisterX] >= mRegisters[lRegisterY])
	{
		lFlag = 1;
	}
	else {
		lFlag = 0;
	}

	mRegisters[lRegisterX] -= mRegisters[lRegisterY];
	mRegisters[0xFu] = lFlag;
}

void Chip8::OP_8XY6()
{
	const uint8_t lRegisterY = (mOpcode & 0x00F0u) >> 4u;
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;
	uint8_t lFlag{};

	mRegisters[lRegisterX] = mRegisters[lRegisterY];

	if (mRegisters[lRegisterX] & 0X001U)
	{
		lFlag = 1;
	}
	else {
		lFlag = 0;
	}

	mRegisters[lRegisterX] >>= 1;
	mRegisters[0xFu] = lFlag;
}

void Chip8::OP_8XY7()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;
	const uint8_t lRegisterY = (mOpcode & 0x00F0u) >> 4u;

	uint8_t lFlag{};

	if (mRegisters[lRegisterY] >= mRegisters[lRegisterX])
	{
		lFlag = 1;
	}
	else {
		lFlag = 0;
	}

	mRegisters[lRegisterX] = mRegisters[lRegisterY] - mRegisters[lRegisterX];
	mRegisters[0xFu] = lFlag;
}

void Chip8::OP_8XYE()
{
	const uint8_t lRegisterY = (mOpcode & 0x00F0u) >> 4u;
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;
	uint8_t lFlag{};

	mRegisters[lRegisterX] = mRegisters[lRegisterY];

	lFlag = (mRegisters[lRegisterX] & 0x80u) >> 7u;

	mRegisters[lRegisterX] <<= 1;
	mRegisters[0xFu] = lFlag;

}

void Chip8::OP_9XY0()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;
	const uint8_t lRegisterY = (mOpcode & 0x00F0u) >> 4u;

	if (mRegisters[lRegisterX] != mRegisters[lRegisterY])
	{
		mProgramCounter += 2;
	}
}

void Chip8::OP_ANNN()
{
	const uint16_t lValue = mOpcode & 0x0FFFu;

	mIndexRegister = lValue;
}

void Chip8::OP_BNNN()
{
	const uint16_t lAddress = mOpcode & 0x0FFFu;

	mProgramCounter = lAddress + mRegisters[0];
}

void Chip8::OP_CXKK()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;
	const uint8_t lValue = mOpcode & 0x00FFu;
	const uint8_t lRandom = static_cast<uint8_t>(distrib(gen));

	mRegisters[lRegisterX] = lRandom & lValue;
}

void Chip8::OP_DXYN()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;
	const uint8_t lRegisterY = (mOpcode & 0x00F0u) >> 4u;

	const uint8_t lHeight = mOpcode & 0x000Fu;
	const uint8_t lWidth = 8u;

	const uint8_t lX = mRegisters[lRegisterX] % 64u;
	const uint8_t lY = mRegisters[lRegisterY] % 32u;

	mRegisters[0xF] = 0;

	for (uint8_t lByte{}; lByte != lHeight; ++lByte)
	{
		const uint8_t lSpriteByte = mMemory[mIndexRegister + lByte];

		for (uint8_t lBit{}; lBit != lWidth; ++lBit)
		{
			const uint8_t lSpritePixel = lSpriteByte & (0x80u >> lBit);
			uint8_t* lScreenPixel = &mVideo[(lY + lByte) * 64u + (lX + lBit)];

			if (lX + lBit < 64 && lY + lByte < 32)
			{
				if (lSpritePixel)
				{
					if (*lScreenPixel == 0xFFu)
					{
						mRegisters[0xFu] = 1;
					}

					*lScreenPixel ^= 0xFFU;
				}
			}
		}
	}
}

void Chip8::OP_EX9E()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;

	if (mKeypad[mRegisters[lRegisterX]])
	{
		mProgramCounter += 2;
	}
}

void Chip8::OP_EXA1()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;

	if (!mKeypad[mRegisters[lRegisterX]])
	{
		mProgramCounter += 2;
	}
}

void Chip8::OP_Fx07()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;

	mRegisters[lRegisterX] = mDelayTimer;
}

void Chip8::OP_FX0A()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;

	if (mKeypad[0])
	{
		mRegisters[lRegisterX] = 0;
	}
	else if (mKeypad[1])
	{
		mRegisters[lRegisterX] = 1;
	}
	else if (mKeypad[2])
	{
		mRegisters[lRegisterX] = 2;
	}
	else if (mKeypad[3])
	{
		mRegisters[lRegisterX] = 3;
	}
	else if (mKeypad[4])
	{
		mRegisters[lRegisterX] = 4;
	}
	else if (mKeypad[5])
	{
		mRegisters[lRegisterX] = 5;
	}
	else if (mKeypad[6])
	{
		mRegisters[lRegisterX] = 6;
	}
	else if (mKeypad[7])
	{
		mRegisters[lRegisterX] = 7;
	}
	else if (mKeypad[8])
	{
		mRegisters[lRegisterX] = 8;
	}
	else if (mKeypad[9])
	{
		mRegisters[lRegisterX] = 9;
	}
	else if (mKeypad[10])
	{
		mRegisters[lRegisterX] = 10;
	}
	else if (mKeypad[11])
	{
		mRegisters[lRegisterX] = 11;
	}
	else if (mKeypad[12])
	{
		mRegisters[lRegisterX] = 12;
	}
	else if (mKeypad[13])
	{
		mRegisters[lRegisterX] = 13;
	}
	else if (mKeypad[14])
	{
		mRegisters[lRegisterX] = 14;
	}
	else if (mKeypad[15])
	{
		mRegisters[lRegisterX] = 15;
	}
	else
	{
		mProgramCounter -= 2;
	}
}

void Chip8::OP_FX15()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;

	mDelayTimer = mRegisters[lRegisterX];
}

void Chip8::OP_FX1E()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;

	mIndexRegister += mRegisters[lRegisterX];
}

void Chip8::OP_FX29()
{
	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };
	const uint8_t lDigit = mRegisters[lRegisterX];

	mIndexRegister = 0x0050u + (5 * lDigit);
}

void Chip8::OP_FX33()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;
	uint8_t lDigit = mRegisters[lRegisterX];

	mMemory[mIndexRegister + 2] = lDigit % 10;
	lDigit /= 10;

	mMemory[mIndexRegister + 1] = lDigit % 10;
	lDigit /= 10;

	mMemory[mIndexRegister] = lDigit % 10;
	lDigit /= 10;
}

void Chip8::OP_FX55()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;

	for (uint8_t a{}; a != (lRegisterX + 1); ++a)
	{
		mMemory[mIndexRegister + a] = mRegisters[a];
	}

	mIndexRegister++;
}

void Chip8::OP_FX65()
{
	const uint8_t lRegisterX = (mOpcode & 0x0F00u) >> 8u;

	for (uint8_t a{}; a != (lRegisterX + 1); ++a)
	{
		mRegisters[a] = mMemory[mIndexRegister + a];
	}

	mIndexRegister++;
}