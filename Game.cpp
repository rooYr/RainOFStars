#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <SFML/Audio.hpp>

Game::Game()
	: window(windowSize, titleName, sf::Style::Default)
{
	std::string windowIconFileName = "Resourсes/Pictures/main_icon.png";
	std::string windowBGIconFileName = "Resourсes/Pictures/main_fon.jpg";
	std::string windowPLIconFileName = "Resourсes/Pictures/player.png";
	std::string windowSmileEmogiFileName = "Resourсes/Pictures/smile_emoji.png";
	std::string windowAngryEmojiFileName = "Resourсes/Pictures/angry_emoji.png";
	std::string windowBombEmojiFileName = "Resourсes/Pictures/bomb_emoji.png";
	std::string fontFileName = "Font/Calibri-Font/Calibri/Calibri.ttf";
	std::string soundFileNameSmile = "Resourсes/SoundAndMusic/smile_sound.mp3";
	std::string soundFileNameBad = "Resourсes/SoundAndMusic/negative_sound.wav";
	std::string soundFileNameGO = "Resourсes/SoundAndMusic/game-lost.mp3";
	std::string fileNameMainMusic = "Resourсes/SoundAndMusic/main_music.mp3";

	if (!mainMusic.openFromFile(fileNameMainMusic))
		throw std::exception(("Could`t load " + fileNameMainMusic).c_str());
	if (!soundOfGO.loadFromFile(soundFileNameGO))
		throw std::exception(("Could`t load " + soundFileNameGO).c_str());
	if (!soundOfBadEmoji.loadFromFile(soundFileNameBad))
		throw std::exception(("Could`t load " + soundFileNameBad).c_str());
	if (!soundOfSmileEmoji.loadFromFile(soundFileNameSmile))
		throw std::exception(("Couldn't load " + soundFileNameSmile).c_str());
	if (!goodEmojiTexture.loadFromFile(windowSmileEmogiFileName))
		throw std::exception(("Couldn't load " + windowSmileEmogiFileName).c_str());
	if (!badEmojiTexture.loadFromFile(windowAngryEmojiFileName))
		throw std::exception(("Couldn't load " + windowAngryEmojiFileName).c_str());
	if (!bombTexture.loadFromFile(windowBombEmojiFileName))
		throw std::exception(("Couldn't load " + windowBombEmojiFileName).c_str());
	if (!playerTexture.loadFromFile(windowPLIconFileName))
		throw std::exception(("Could`t load file " + windowPLIconFileName).c_str());
	if (!textureBG.loadFromFile(windowBGIconFileName))
		throw std::exception(("Could`t load file" + windowBGIconFileName).c_str());

	textureBG.setSmooth(true);
	playerTexture.setSmooth(true);
	goodEmojiTexture.setSmooth(true);
	badEmojiTexture.setSmooth(true);
	bombTexture.setSmooth(true);


	if (!image.loadFromFile(windowIconFileName))
		throw std::exception(("Could`t load file" + windowIconFileName).c_str());
	if (!font.loadFromFile(fontFileName))
		throw std::exception(("Couldn't load font " + fontFileName).c_str());

	scoreTxt.setFont(font);
	scoreTxt.setCharacterSize(50);
	scoreTxt.setFillColor(sf::Color::White);
	scoreTxt.setOutlineColor(sf::Color::Black);
	scoreTxt.setOutlineThickness(5);
	scoreTxt.setPosition(10, 10);
	scoreTxt.setString("Score: 0");

	GameOverTxt.setFont(font);
	GameOverTxt.setCharacterSize(190);
	GameOverTxt.setFillColor(colorTxt);
	GameOverTxt.setStyle(sf::Text::Italic);
	GameOverTxt.setOutlineColor(sf::Color::Black);
	GameOverTxt.setOutlineThickness(15);

	spriteBG.setTexture(textureBG);
	playerSprite.setTexture(playerTexture);
	playerSprite.setScale(0.3f, 0.3f);
	window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());

	smileSound.setBuffer(soundOfSmileEmoji);
	smileSound.setVolume(50);
	badSound.setBuffer(soundOfBadEmoji);
	badSound.setVolume(50);
	GOSound.setBuffer(soundOfGO);
	GOSound.setVolume(50);
	mainMusic.setVolume(25);
	mainMusic.setLoop(true);
	mainMusic.play();
}

void Game::Run()
{
	playerSprite.setPosition((window.getSize().x - playerSprite.getGlobalBounds().width) / 2, window.getSize().y - playerSprite.getGlobalBounds().height);
	bombScaleX = (window.getSize().y / 3.0f) / bombTexture.getSize().y;

	while (window.isOpen()) {
		dt = clock.restart().asSeconds();
		HandleInput();
		if (!gameOver) {
			Update();
		}
		Draw();
	}
}

void Game::HandleInput()
{
	float speed = 200.0f;

	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}
	}

	if (gameOver == true) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
			emojis.clear();
			score = 0;
			gameOver = false;
			scoreTxt.setString("Score: " + std::to_string(score));
			GameOverTxt.setString("");
			mainMusic.play();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			window.close();
		}
	}
	else {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			playerSprite.move(-speed * dt, 0);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			playerSprite.move(speed * dt, 0);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			window.close();
		}
	}
}

bool Game::isOverlapping(sf::Sprite newEmoji) {
	for (const auto& emoji : emojis) {
		if (newEmoji.getGlobalBounds().intersects(emoji.getGlobalBounds())) {
			return true;
		}
	}
	return false;
}

void Game::Update() {
	for (auto& emoji : emojis) {
		emoji.move(0, 220.0f * dt);
		if (playerSprite.getGlobalBounds().intersects(emoji.getGlobalBounds())) {
			if (emoji.getTexture() == &goodEmojiTexture) {
				score++;
				smileSound.play();
			}
			else if (emoji.getTexture() == &badEmojiTexture) {
				score--;
				badSound.play();
			}
			else if (emoji.getTexture() == &bombTexture) {
				GameOverTxt.setString("Game Over!");
				sf::FloatRect textRect = GameOverTxt.getLocalBounds();
				GameOverTxt.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				GameOverTxt.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));
				GOSound.play();
				gameOver = true;
				mainMusic.pause();
			}
			emoji.setPosition(-100, -100);
			scoreTxt.setString("Score: " + std::to_string(score));
		}
	}

	// Генерація імодзі кожні 1.5 секунди в співвідношенні 2:1.5:0.5
	if (clock2.getElapsedTime().asSeconds() >= 1.5) {
		clock2.restart();

		float yOffset = -50.0f; // Початкова позиція за межами вікна

		// Генерація 2 goodEmoji
		for (int i = 0; i < 2; ++i) {
			sf::Sprite newEmoji;
			newEmoji.setTexture(goodEmojiTexture);
			newEmoji.setScale(0.3f, 0.3f);
			int xPos = rand() % static_cast<int>(window.getSize().x - newEmoji.getGlobalBounds().width);
			newEmoji.setPosition(static_cast<float>(xPos), yOffset);
			emojis.push_back(newEmoji);
			yOffset -= 50.0f; // Зміщуємо кожен імодзі по Y
		}

		// Генерація 1 badEmoji
		for (int i = 0; i < 1; ++i) {
			sf::Sprite newEmoji;
			newEmoji.setTexture(badEmojiTexture);
			newEmoji.setScale(0.15f, 0.15f);
			int xPos = rand() % static_cast<int>(window.getSize().x - newEmoji.getGlobalBounds().width);
			newEmoji.setPosition(static_cast<float>(xPos), yOffset);
			emojis.push_back(newEmoji);
			yOffset -= 50.0f; // Зміщуємо кожен імодзі по Y
		}

		// Генерація 1 bomb кожні 2 секунди
		static bool toggleBomb = false;
		if (toggleBomb) {
			sf::Sprite newEmoji;
			newEmoji.setTexture(bombTexture);
			newEmoji.setScale(1.2f, 1.1f);
			int xPos = rand() % static_cast<int>(window.getSize().x - newEmoji.getGlobalBounds().width);
			newEmoji.setPosition(static_cast<float>(xPos), yOffset);
			emojis.push_back(newEmoji);
		}
		toggleBomb = !toggleBomb;

		if (score < 0) {
			mainMusic.pause();
			scoreTxt.setString("Score: 0");
			GameOverTxt.setString("Game Over!");
			sf::FloatRect textRect = GameOverTxt.getLocalBounds();
			GameOverTxt.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
			GameOverTxt.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));
			GOSound.play();
			gameOver = true;
		}
	}
}
		
void Game::Draw() {
	window.clear();
	window.draw(spriteBG);
	window.draw(playerSprite);

	for (const auto& emoji : emojis) {
		window.draw(emoji);
	}
	window.draw(scoreTxt);
	window.draw(GameOverTxt);

	window.display();
}
