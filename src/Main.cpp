#include <SFML/Graphics.hpp>
#include "../lib/Chip8.hpp"

#include <iostream>

int main() {
	sf::RenderWindow lWindow(sf::VideoMode(1280u, 640u), "Chip8 Emulator",
		sf::Style::Close | sf::Style::Titlebar);

	lWindow.setMouseCursorVisible(false);

	sf::Image lIcon{};
	if (!lIcon.loadFromFile(R"(../../../../img/Chip8OfficialLogo.png)"))
	{
		std::cout << "Failed to load Icon" << std::endl;
	}

	lWindow.setIcon(lIcon.getSize().x, lIcon.getSize().y, lIcon.getPixelsPtr());

	Chip8 Chip8{};

	if (Chip8.LoadRom(R"(C:\Users\VSnea\Downloads\Trip8 Demo (2008) [Revival Studios].ch8)")) {

		sf::Texture lTexture{};
		lTexture.create(64u, 32u);

		sf::Sprite lSprite{ lTexture };
		lSprite.setScale(20.f, 20.f);

		sf::Clock lClock{};

		while (lWindow.isOpen()) {

			if (Chip8.getDelayTimer() > 0u) {
				sf::Time lElapsed{ lClock.getElapsedTime() };

				if (lElapsed.asMilliseconds() >= 16) {
					(Chip8.setDelayTimer(Chip8.getDelayTimer() - 1));
					lClock.restart();
				}
			}

			sf::Event lEvent{};
			while (lWindow.pollEvent(lEvent)) {

				switch (lEvent.type) {

				case sf::Event::EventType::Closed: {
					lWindow.close();
				} break;

				case sf::Event::EventType::KeyPressed: {

					switch (lEvent.key.code) {

					case sf::Keyboard::Key::X: {
						Chip8.getKeypad()[0] = 1;
					} break;

					case sf::Keyboard::Key::Num1: {
						Chip8.getKeypad()[1] = 1;
					} break;

					case sf::Keyboard::Key::Num2: {
						Chip8.getKeypad()[2] = 1;
					} break;

					case sf::Keyboard::Key::Num3: {
						Chip8.getKeypad()[3] = 1;
					} break;

					case sf::Keyboard::Key::Q: {
						Chip8.getKeypad()[4] = 1;
					} break;

					case sf::Keyboard::Key::W: {
						Chip8.getKeypad()[5] = 1;
					} break;

					case sf::Keyboard::Key::E: {
						Chip8.getKeypad()[6] = 1;
					} break;

					case sf::Keyboard::Key::A: {
						Chip8.getKeypad()[7] = 1;
					} break;

					case sf::Keyboard::Key::S: {
						Chip8.getKeypad()[8] = 1;
					} break;

					case sf::Keyboard::Key::D: {
						Chip8.getKeypad()[9] = 1;
					} break;

					case sf::Keyboard::Key::Z: {
						Chip8.getKeypad()[10] = 1;
					} break;

					case sf::Keyboard::Key::C: {
						Chip8.getKeypad()[11] = 1;
					} break;

					case sf::Keyboard::Key::Num4: {
						Chip8.getKeypad()[12] = 1;
					} break;

					case sf::Keyboard::Key::R: {
						Chip8.getKeypad()[13] = 1;
					} break;

					case sf::Keyboard::Key::F: {
						Chip8.getKeypad()[14] = 1;
					} break;

					case sf::Keyboard::Key::V: {
						Chip8.getKeypad()[15] = 1;
					} break;

					default:
						break;
					}

				} break;

				case sf::Event::EventType::KeyReleased: {

					switch (lEvent.key.code) {

					case sf::Keyboard::Key::X: {
						Chip8.getKeypad()[0] = 0;
					} break;

					case sf::Keyboard::Key::Num1: {
						Chip8.getKeypad()[1] = 0;
					} break;

					case sf::Keyboard::Key::Num2: {
						Chip8.getKeypad()[2] = 0;
					} break;

					case sf::Keyboard::Key::Num3: {
						Chip8.getKeypad()[3] = 0;
					} break;

					case sf::Keyboard::Key::Q: {
						Chip8.getKeypad()[4] = 0;
					} break;

					case sf::Keyboard::Key::W: {
						Chip8.getKeypad()[5] = 0;
					} break;

					case sf::Keyboard::Key::E: {
						Chip8.getKeypad()[6] = 0;
					} break;

					case sf::Keyboard::Key::A: {
						Chip8.getKeypad()[7] = 0;
					} break;

					case sf::Keyboard::Key::S: {
						Chip8.getKeypad()[8] = 0;
					} break;

					case sf::Keyboard::Key::D: {
						Chip8.getKeypad()[9] = 0;
					} break;

					case sf::Keyboard::Key::Z: {
						Chip8.getKeypad()[10] = 0;
					} break;

					case sf::Keyboard::Key::C: {
						Chip8.getKeypad()[11] = 0;
					} break;

					case sf::Keyboard::Key::Num4: {
						Chip8.getKeypad()[12] = 0;
					} break;

					case sf::Keyboard::Key::R: {
						Chip8.getKeypad()[13] = 0;
					} break;

					case sf::Keyboard::Key::F: {
						Chip8.getKeypad()[14] = 0;
					} break;

					case sf::Keyboard::Key::V: {
						Chip8.getKeypad()[15] = 0;
					} break;

					default:
						break;
					}
				} break;

				default:
					break;
				}
			}

			Chip8.EmulateCycle();

			sf::Uint8 Pixels[32 * 64 * 4]{};

			for (int a{}, b{}; a != (32 * 64) && b != ((32 * 64 * 4) - 4); ++a, b += 4) {

				if (Chip8.getVideo()[a]) {
					Pixels[b] = Pixels[b + 1] = Pixels[b + 2] = 255;
				}

				Pixels[b + 3] = 255;
			}

			lTexture.update(Pixels);

			lWindow.clear(sf::Color::Black);
			lWindow.draw(lSprite);
			lWindow.display();

			sf::sleep(sf::milliseconds(1));
		}
	}
	else {

		// TODO

	}



	return 0;
}
