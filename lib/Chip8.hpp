#ifndef _CHIP8_HPP_
#define _CHIP8_HPP_

#include <cstdint>

class Chip8 {
private:
	uint8_t mRegisters[16]{};
	uint8_t mMemory[4096]{};
	uint8_t mVideo[32 * 64]{};
	uint8_t mKeypad[16]{};

	uint8_t mDelayTimer{};

	uint16_t mProgramCounter{};
	uint16_t mIndexRegister{};
	uint16_t mOpcode{};

	uint16_t mStack[16]{};
	uint8_t mStackPointer{};

public:
	Chip8();
	Chip8(const char* const);

	void LoadRom(const char* const);
	void EmulateCycle();

	
	uint8_t *const getVideo();
	uint8_t *const getKeypad();

	uint8_t getDelayTimer();
	void setDelayTimer(const uint8_t iDelayTimer);

	void OP_00E0();
	void OP_00EE();
	void OP_1NNN();
	void OP_2NNN();
	void OP_3XNN();
	void OP_4XNN();
	void OP_5XY0();
	void OP_6XNN();
	void OP_7XNN();
	void OP_8XY0();
	void OP_8XY1();
	void OP_8XY2();
	void OP_8XY3();
	void OP_8XY4();
	void OP_8XY5();
	void OP_8XY6();
	void OP_8XY7();
	void OP_8XYE();
	void OP_9XY0();
	void OP_ANNN();
	void OP_BNNN();
	void OP_CXKK();
	void OP_DXYN();
	void OP_EX9E();
	void OP_EXA1();
	void OP_Fx07();
	void OP_FX0A();
	void OP_FX15();
	void OP_FX1E();
	void OP_FX29();
	void OP_FX33();
	void OP_FX55();
	void OP_FX65();
};

#endif
