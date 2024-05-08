#include <SFML/Graphics.hpp>

#include "../lib/Chip8.hpp"

int main() {
	sf::RenderWindow lWindow(sf::VideoMode(1280, 640), "Chip8 Emulator",
		sf::Style::Default);

	Chip8 Chip8{};
	Chip8.LoadRom("");

	sf::Texture lTexture;
	lTexture.create(64, 32);

	sf::Sprite lSprite{ lTexture };
	lSprite.setScale(20, 20);

	sf::Clock lClock;

	while (lWindow.isOpen()) {

		if (Chip8.getDelayTimer() > 0) {
			sf::Time lElapsed{ lClock.getElapsedTime() };

			if (lElapsed.asMilliseconds() >= 16) {
				(Chip8.setDelayTimer(Chip8.getDelayTimer() - 1));
				lClock.restart();
			}
		}

		sf::Event lEvent;
		while (lWindow.pollEvent(lEvent)) {

			switch (lEvent.type) {

			case sf::Event::Closed: {
				lWindow.close();
			} break;

			case sf::Event::KeyPressed: {
				switch (lEvent.key.code) {
				case sf::Keyboard::X: {
					Chip8.getKeypad()[0] = 1;
				} break;

				case sf::Keyboard::Num1: {
					Chip8.getKeypad()[1] = 1;
				} break;

				case sf::Keyboard::Num2: {
					Chip8.getKeypad()[2] = 1;
				} break;

				case sf::Keyboard::Num3: {
					Chip8.getKeypad()[3] = 1;
				} break;

				case sf::Keyboard::Q: {
					Chip8.getKeypad()[4] = 1;
				} break;

				case sf::Keyboard::W: {
					Chip8.getKeypad()[5] = 1;
				} break;

				case sf::Keyboard::E: {
					Chip8.getKeypad()[6] = 1;
				} break;

				case sf::Keyboard::A: {
					Chip8.getKeypad()[7] = 1;
				} break;

				case sf::Keyboard::S: {
					Chip8.getKeypad()[8] = 1;
				} break;

				case sf::Keyboard::D: {
					Chip8.getKeypad()[9] = 1;
				} break;

				case sf::Keyboard::Z: {
					Chip8.getKeypad()[10] = 1;
				} break;

				case sf::Keyboard::C: {
					Chip8.getKeypad()[11] = 1;
				} break;

				case sf::Keyboard::Num4: {
					Chip8.getKeypad()[12] = 1;
				} break;

				case sf::Keyboard::R: {
					Chip8.getKeypad()[13] = 1;
				} break;

				case sf::Keyboard::F: {
					Chip8.getKeypad()[14] = 1;
				} break;

				case sf::Keyboard::V: {
					Chip8.getKeypad()[15] = 1;
				} break;
				}

			} break;

			case sf::Event::KeyReleased: {
				switch (lEvent.key.code) {
				case sf::Keyboard::X: {
					Chip8.getKeypad()[0] = 0;
				} break;

				case sf::Keyboard::Num1: {
					Chip8.getKeypad()[1] = 0;
				} break;

				case sf::Keyboard::Num2: {
					Chip8.getKeypad()[2] = 0;
				} break;

				case sf::Keyboard::Num3: {
					Chip8.getKeypad()[3] = 0;
				} break;

				case sf::Keyboard::Q: {
					Chip8.getKeypad()[4] = 0;
				} break;

				case sf::Keyboard::W: {
					Chip8.getKeypad()[5] = 0;
				} break;

				case sf::Keyboard::E: {
					Chip8.getKeypad()[6] = 0;
				} break;

				case sf::Keyboard::A: {
					Chip8.getKeypad()[7] = 0;
				} break;

				case sf::Keyboard::S: {
					Chip8.getKeypad()[8] = 0;
				} break;

				case sf::Keyboard::D: {
					Chip8.getKeypad()[9] = 0;
				} break;

				case sf::Keyboard::Z: {
					Chip8.getKeypad()[10] = 0;
				} break;

				case sf::Keyboard::C: {
					Chip8.getKeypad()[11] = 0;
				} break;

				case sf::Keyboard::Num4: {
					Chip8.getKeypad()[12] = 0;
				} break;

				case sf::Keyboard::R: {
					Chip8.getKeypad()[13] = 0;
				} break;

				case sf::Keyboard::F: {
					Chip8.getKeypad()[14] = 0;
				} break;

				case sf::Keyboard::V: {
					Chip8.getKeypad()[15] = 0;
				} break;
				}
			} break;

			default:
				break;
			}
		}

		Chip8.EmulateCycle();

		sf::Uint8 Pixels[32 * 64 * 4]{};

		for (int a{}, b{}; a != (32 * 64) && b != ((32 * 64 * 4) - 4);
			++a, b += 4) {
			if (Chip8.getVideo()[a]) {
				Pixels[b] = Pixels[b + 1] = Pixels[b + 2] = 255;
			}

			Pixels[b + 3] = 255;
		}

		lTexture.update(Pixels);
		lWindow.clear();
		lWindow.draw(lSprite);
		lWindow.display();

		sf::sleep(sf::milliseconds(1));
	}

	return 0;
}
