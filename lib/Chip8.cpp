#include "../lib/Chip8.hpp"

#include <fstream>
#include <iostream>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> distrib(0, 255);

uint8_t lFontset[80]{
	0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
	0x20, 0x60, 0x20, 0x20, 0x70,  // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
	0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
	0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
	0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
	0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
	0xF0, 0x80, 0xF0, 0x80, 0x80   // F
};

void OpSuccess(const std::string& lOpcodeName)
{
	std::cout << "(\x1B[32mSuccess\033[0m) (" << lOpcodeName << ") Executed" << std::endl;
}

void OpError()
{
	std::cout << "(\x1B[31mError\033[0m) Opcode Not Executed" << std::endl;
}


Chip8::Chip8() : mProgramCounter{ 0x200 } {

	for (uint8_t a{}; a != 80; ++a) {
		mMemory[0x50 + a] = lFontset[a];
	}
}

Chip8::Chip8(const char* const iSource) : Chip8() { LoadRom(iSource); }

bool Chip8::LoadRom(const char* const iSource) {

	std::ifstream lSource(iSource, std::ios::binary | std::ios::ate);

	if (lSource.is_open()) {

		std::cout << "(\x1B[32mSuccess\033[0m) File Opened" << std::endl;

		std::streampos lSize{ lSource.tellg() };

		if (lSize < 3584) {

			lSource.seekg(0, std::ios::beg);
			lSource.read(reinterpret_cast<char*>(&mMemory[0x200]), lSize);
			std::cout << "(\x1B[32mSuccess\033[0m) Source Code Loaded" << std::endl;

			lSource.close();
			std::cout << "(\x1B[32mSuccess\033[0m) File Closed" << std::endl;

			return true;
		}
		else {
			std::cout << "(\x1B[31mError\033[0m) File Not Opened" << std::endl;
			return false;
		}
	}
}

void Chip8::EmulateCycle() {

	mOpcode = uint16_t((mMemory[mProgramCounter] << 8u) | mMemory[mProgramCounter + 1u]);
	mProgramCounter += 2u;

	switch (mOpcode & 0xF000u) {

	case 0x0000u: {

		switch (mOpcode & 0x0FFFu) {

		case 0x00E0u:
			OP_00E0();
			break;

		case 0x00EEu:
			OP_00EE();
			break;

		default:
			OpError();
			break;
		}
	} break;

	case 0x1000u:
		OP_1NNN();
		break;

	case 0x2000u:
		OP_2NNN();
		break;

	case 0x3000u:
		OP_3XNN();
		break;

	case 0x4000u:
		OP_4XNN();
		break;

	case 0x5000u: {

		switch (mOpcode & 0x000Fu) {

		case 0x0000u:
			OP_5XY0();
			break;

		default:
			OpError();
			break;
		}
	}break;

	case 0x6000u:
		OP_6XNN();
		break;

	case 0x7000u:
		OP_7XNN();
		break;

	case 0x8000u: {

		switch (mOpcode & 0x000Fu) {

		case 0x0000u:
			OP_8XY0();
			break;

		case 0x0001u:
			OP_8XY1();
			break;

		case 0x0002u:
			OP_8XY2();
			break;

		case 0x0003u:
			OP_8XY3();
			break;

		case 0x0004u:
			OP_8XY4();
			break;

		case 0x0005u:
			OP_8XY5();
			break;

		case 0x0006u:
			OP_8XY6();
			break;

		case 0x0007u:
			OP_8XY7();
			break;

		case 0x000Eu:
			OP_8XYE();
			break;

		default:
			OpError();
			break;
		}
	} break;

	case 0x9000u: {

		switch (mOpcode & 0x000Fu)
		{
		case 0x0000u:
			OP_9XY0();
			break;

		default:
			OpError();
			break;
		}
	}break;

	case 0xA000u:
		OP_ANNN();
		break;

	case 0xB000u:
		OP_BNNN();
		break;

	case 0xC000u:
		OP_CXKK();
		break;

	case 0xD000u:
		OP_DXYN();
		break;

	case 0xE000u: {

		switch (mOpcode & 0x00FFu) {

		case 0x00A1u:
			OP_EXA1();
			break;

		case 0x009Eu:
			OP_EX9E();
			break;

		default:
			OpError();
			break;
		}
	} break;

	case 0xF000u: {

		switch (mOpcode & 0x00FFu) {

		case 0x0007u:
			OP_FX07();
			break;

		case 0x000Au:
			OP_FX0A();
			break;

		case 0x0015u:
			OP_FX15();
			break;

		case 0x001Eu:
			OP_FX1E();
			break;

		case 0x0029u:
			OP_FX29();
			break;

		case 0x0033u:
			OP_FX33();
			break;

		case 0x0055u:
			OP_FX55();
			break;

		case 0x0065u:
			OP_FX65();
			break;

		default:
			OpError();
			break;
		}

	} break;


	default:
		OpError();
		break;
	}
}

uint8_t* const Chip8::getVideo() { return mVideo; }
uint8_t* const Chip8::getKeypad() { return mKeypad; }
uint8_t Chip8::getDelayTimer() { return mDelayTimer; }

void Chip8::setDelayTimer(uint8_t iDelayTimer)
{
	mDelayTimer = iDelayTimer;
}

void Chip8::OP_00E0() {

	for (int a{}; a != 32 * 64; ++a) {
		mVideo[a] = 0;
	}

	OpSuccess("00E0");
}

void Chip8::OP_00EE() {

	mStackPointer--;
	mProgramCounter = mStack[mStackPointer];

	OpSuccess("00EE");
}

void Chip8::OP_1NNN() {

	const uint16_t lAddress{ uint16_t(mOpcode & 0x0FFFu) };

	mProgramCounter = lAddress;

	OpSuccess("1NNN");
}

void Chip8::OP_2NNN() {

	const uint16_t lAddress{ uint16_t(mOpcode & 0x0FFFu) };

	mStack[mStackPointer] = mProgramCounter;
	mStackPointer++;

	mProgramCounter = lAddress;

	OpSuccess("2NNN");
}

void Chip8::OP_3XNN() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };
	const uint8_t lValue{ uint8_t((mOpcode & 0x00FFu)) };

	if (mRegisters[lRegisterX] == lValue) {
		mProgramCounter += 2;
	}

	OpSuccess("3XNN");
}

void Chip8::OP_4XNN() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };
	const uint8_t lValue{ uint8_t((mOpcode & 0x00FFu)) };

	if (mRegisters[lRegisterX] != lValue) {
		mProgramCounter += 2;
	}

	OpSuccess("4XNN");
}

void Chip8::OP_5XY0() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };
	const uint8_t lRegisterY{ uint8_t((mOpcode & 0x00F0u) >> 4u) };

	if (mRegisters[lRegisterX] == mRegisters[lRegisterY]) {
		mProgramCounter += 2;
	}

	OpSuccess("5XY0");
}

void Chip8::OP_6XNN() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };
	const uint8_t lValue{ uint8_t((mOpcode & 0x00FFu)) };

	mRegisters[lRegisterX] = lValue;

	OpSuccess("6XNN");
}

void Chip8::OP_7XNN() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };
	const uint8_t lValue{ uint8_t(mOpcode & 0x00FFu) };

	mRegisters[lRegisterX] += lValue;

	OpSuccess("7XNN");
}

void Chip8::OP_8XY0() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };
	const uint8_t lRegisterY{ uint8_t((mOpcode & 0x00F0u) >> 4u) };

	mRegisters[lRegisterX] = mRegisters[lRegisterY];

	OpSuccess("8XY0");
}

void Chip8::OP_8XY1() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };
	const uint8_t lRegisterY{ uint8_t((mOpcode & 0x00F0u) >> 4u) };

	mRegisters[lRegisterX] |= mRegisters[lRegisterY];
	mRegisters[0xFu] = 0;

	OpSuccess("8XY1");
}

void Chip8::OP_8XY2() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };
	const uint8_t lRegisterY{ uint8_t((mOpcode & 0x00F0u) >> 4u) };

	mRegisters[lRegisterX] &= mRegisters[lRegisterY];
	mRegisters[0xFu] = 0;

	OpSuccess("8XY2");
}

void Chip8::OP_8XY3() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };
	const uint8_t lRegisterY{ uint8_t((mOpcode & 0x00F0u) >> 4u) };

	mRegisters[lRegisterX] ^= mRegisters[lRegisterY];
	mRegisters[0xFu] = 0;

	OpSuccess("8XY3");
}

void Chip8::OP_8XY4() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };
	const uint8_t lRegisterY{ uint8_t((mOpcode & 0x00F0u) >> 4u) };

	uint8_t lFlag{};
	uint16_t lSum{ uint16_t(mRegisters[lRegisterX] + mRegisters[lRegisterY]) };

	if (lSum > 255u) {
		lFlag = 1;
	}
	else {
		lFlag = 0;
	}

	mRegisters[lRegisterX] = lSum & 0xFFu;
	mRegisters[0xFu] = lFlag;

	OpSuccess("8XY4");
}

void Chip8::OP_8XY5() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };
	const uint8_t lRegisterY{ uint8_t((mOpcode & 0x00F0u) >> 4u) };

	uint8_t lFlag{};

	if (mRegisters[lRegisterX] >= mRegisters[lRegisterY]) {
		lFlag = 1;
	}
	else {
		lFlag = 0;
	}

	mRegisters[lRegisterX] -= mRegisters[lRegisterY];
	mRegisters[0xFu] = lFlag;

	OpSuccess("8XY5");
}

void Chip8::OP_8XY6() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };
	const uint8_t lRegisterY{ uint8_t((mOpcode & 0x00F0u) >> 4u) };
	uint8_t lFlag{};

	mRegisters[lRegisterX] = mRegisters[lRegisterY];

	if (mRegisters[lRegisterX] & 0X001U) {
		lFlag = 1;
	}
	else {
		lFlag = 0;
	}

	mRegisters[lRegisterX] >>= 1;
	mRegisters[0xFu] = lFlag;

	OpSuccess("8XY6");
}

void Chip8::OP_8XY7() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };
	const uint8_t lRegisterY{ uint8_t((mOpcode & 0x00F0u) >> 4u) };

	uint8_t lFlag{};

	if (mRegisters[lRegisterY] >= mRegisters[lRegisterX]) {
		lFlag = 1;
	}
	else {
		lFlag = 0;
	}

	mRegisters[lRegisterX] = mRegisters[lRegisterY] - mRegisters[lRegisterX];
	mRegisters[0xFu] = lFlag;

	OpSuccess("8XY7");
}

void Chip8::OP_8XYE() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };
	const uint8_t lRegisterY{ uint8_t((mOpcode & 0x00F0u) >> 4u) };
	uint8_t lFlag{};

	mRegisters[lRegisterX] = mRegisters[lRegisterY];

	lFlag = (mRegisters[lRegisterX] & 0x80u) >> 7u;

	mRegisters[lRegisterX] <<= 1;
	mRegisters[0xFu] = lFlag;

	OpSuccess("8XYE");
}

void Chip8::OP_9XY0() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };
	const uint8_t lRegisterY{ uint8_t((mOpcode & 0x00F0u) >> 4u) };

	if (mRegisters[lRegisterX] != mRegisters[lRegisterY]) {
		mProgramCounter += 2;
	}

	OpSuccess("9XY0");
}

void Chip8::OP_ANNN() {

	const uint16_t lValue{ uint16_t(mOpcode & 0x0FFFu) };

	mIndexRegister = lValue;

	OpSuccess("ANNN");
}

void Chip8::OP_BNNN() {

	const uint16_t lAddress{ uint16_t(mOpcode & 0x0FFFu) };

	mProgramCounter = lAddress + mRegisters[0];

	OpSuccess("BNNN");
}

void Chip8::OP_CXKK() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };
	const uint8_t lValue{ uint8_t(mOpcode & 0x00FFu) };
	const uint8_t lRandom{ uint8_t(distrib(gen)) };

	mRegisters[lRegisterX] = lRandom & lValue;

	OpSuccess("CXKK");
}

void Chip8::OP_DXYN() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };
	const uint8_t lRegisterY{ uint8_t((mOpcode & 0x00F0u) >> 4u) };

	const uint8_t lHeight{ uint8_t(mOpcode & 0x000Fu) };
	const uint8_t lWidth{ 8u };

	const uint8_t lX{ uint8_t(mRegisters[lRegisterX] % 64u) };
	const uint8_t lY{ uint8_t(mRegisters[lRegisterY] % 32u) };

	mRegisters[0xF] = 0;

	for (uint8_t lByte{}; lByte != lHeight; ++lByte) {

		const uint8_t lSpriteByte{ uint8_t(mMemory[mIndexRegister + lByte]) };

		for (uint8_t lBit{}; lBit != lWidth; ++lBit) {

			const uint8_t lSpritePixel{ uint8_t(lSpriteByte & (0x80u >> lBit)) };
			uint8_t* lScreenPixel{ &mVideo[(lY + lByte) * 64u + (lX + lBit)] };

			if (lX + lBit < 64 && lY + lByte < 32) {
				if (lSpritePixel) {
					if (*lScreenPixel == 0xFFu) {
						mRegisters[0xFu] = 1;
					}

					*lScreenPixel ^= 0xFFU;
				}
			}
		}
	}

	OpSuccess("DXYN");
}

void Chip8::OP_EX9E() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };

	if (mKeypad[mRegisters[lRegisterX] & 0x0Fu]) {
		mProgramCounter += 2;
	}

	OpSuccess("EX9E");
}

void Chip8::OP_EXA1() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };

	if (!mKeypad[mRegisters[lRegisterX] & 0x0Fu]) {
		mProgramCounter += 2;
	}

	OpSuccess("EXA1");
}

void Chip8::OP_FX07() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };

	mRegisters[lRegisterX] = mDelayTimer;

	OpSuccess("FX07");
}

void Chip8::OP_FX0A() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };
	static int Pressed{};

	if (!Pressed)
	{
		for (int a{}; a != 16; ++a)
		{
			if (mKeypad[a]) {
				Pressed = (a + 1);
				break;
			}
		}

		mProgramCounter -= 2;
	}
	else {

		if (!mKeypad[Pressed - 1])
		{
			mRegisters[lRegisterX] = (Pressed - 1);
			Pressed = 0;
		}
		else {
			mProgramCounter -= 2;
		}
	}

	OpSuccess("FX0A");
}

void Chip8::OP_FX15() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };

	mDelayTimer = mRegisters[lRegisterX];

	OpSuccess("FX15");
}

void Chip8::OP_FX1E() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };

	mIndexRegister += mRegisters[lRegisterX];

	OpSuccess("FX1E");
}

void Chip8::OP_FX29() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };
	const uint8_t lDigit{ mRegisters[lRegisterX] & 0x0Fu };

	mIndexRegister = 0x0050u + (5u * lDigit);

	OpSuccess("FX29");
}

void Chip8::OP_FX33() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };
	uint8_t lDigit{ mRegisters[lRegisterX] };

	mMemory[mIndexRegister + 2] = lDigit % 10;
	lDigit /= 10;

	mMemory[mIndexRegister + 1] = lDigit % 10;
	lDigit /= 10;

	mMemory[mIndexRegister] = lDigit % 10;
	lDigit /= 10;

	OpSuccess("FX33");
}

void Chip8::OP_FX55() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };

	for (uint8_t a{}; a != (lRegisterX + 1); ++a) {
		mMemory[mIndexRegister + a] = mRegisters[a];
	}

	mIndexRegister++;

	OpSuccess("FX55");
}

void Chip8::OP_FX65() {

	const uint8_t lRegisterX{ uint8_t((mOpcode & 0x0F00u) >> 8u) };

	for (uint8_t a{}; a != (lRegisterX + 1); ++a) {
		mRegisters[a] = mMemory[mIndexRegister + a];
	}

	mIndexRegister++;

	OpSuccess("FX65");
}