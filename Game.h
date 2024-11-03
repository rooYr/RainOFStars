#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

class Game
{
public:
	Game();
	void Run();

private:
    sf::VideoMode windowSize{ 1200, 718 };
    sf::String titleName{ "SFML Game" };
    sf::RenderWindow window;
    sf::Image image;
    sf::Texture textureBG;
    sf::Texture playerTexture;
    sf::Sprite spriteBG;
    sf::Sprite playerSprite;
    sf::Texture goodEmojiTexture;
    sf::Texture badEmojiTexture;
    sf::Texture bombTexture;
    std::vector<sf::Sprite> emojis;
    sf::Clock clock;
    sf::Clock clock2;
    sf::Text scoreTxt;
    sf::Text GameOverTxt;
    sf::Font font;
    sf::Color colorTxt{255, 0, 0};
    sf::SoundBuffer soundOfSmileEmoji;
    sf::Sound smileSound;
    sf::SoundBuffer soundOfBadEmoji;
    sf::Sound badSound;
    sf::SoundBuffer soundOfGO;
    sf::Sound GOSound;
    sf::Music mainMusic;

    float dt = 0.0f;
    float bombScaleX = 0.0f;
    int score = 0;
    bool gameOver = false;

    bool isOverlapping(sf::Sprite newEmoji);
    void HandleInput();
    void Update();
    void Draw();
};
