// Snake Game using SFML
// Features: graphics, sound effects, increasing difficulty

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <deque>
#include <cstdlib>
#include <ctime>

const int WINDOW_SIZE = 600;
const int GRID_SIZE = 20;
const int CELL = WINDOW_SIZE / GRID_SIZE;

struct Point {
    int x, y;
};

int main() {
    srand(time(0));

    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Snake Game");
    window.setFramerateLimit(60);

    sf::RectangleShape snakeShape(sf::Vector2f(CELL - 2, CELL - 2));
    snakeShape.setFillColor(sf::Color::Green);

    sf::RectangleShape foodShape(sf::Vector2f(CELL - 2, CELL - 2));
    foodShape.setFillColor(sf::Color::Red);

    // Load sound
    sf::SoundBuffer eatBuffer, overBuffer;
    eatBuffer.loadFromFile("eat.wav");
    overBuffer.loadFromFile("gameover.wav");
    sf::Sound eatSound(eatBuffer);
    sf::Sound overSound(overBuffer);

    std::deque<Point> snake;
    snake.push_back({10, 10});

    Point food = {rand() % GRID_SIZE, rand() % GRID_SIZE};

    int dx = 1, dy = 0;
    float speed = 0.15f;
    sf::Clock clock;

    bool gameOver = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && dy == 0) { dx = 0; dy = -1; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && dy == 0) { dx = 0; dy = 1; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && dx == 0) { dx = -1; dy = 0; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && dx == 0) { dx = 1; dy = 0; }

        if (clock.getElapsedTime().asSeconds() > speed && !gameOver) {
            clock.restart();

            Point newHead = {snake.front().x + dx, snake.front().y + dy};

            // Wall collision
            if (newHead.x < 0 || newHead.y < 0 ||
                newHead.x >= GRID_SIZE || newHead.y >= GRID_SIZE) {
                gameOver = true;
                overSound.play();
            }

            // Self collision
            for (auto &s : snake) {
                if (s.x == newHead.x && s.y == newHead.y) {
                    gameOver = true;
                    overSound.play();
                }
            }

            snake.push_front(newHead);

            // Eat food
            if (newHead.x == food.x && newHead.y == food.y) {
                eatSound.play();
                food = {rand() % GRID_SIZE, rand() % GRID_SIZE};

                // Increase difficulty
                if (speed > 0.05f)
                    speed -= 0.01f;
            } else {
                snake.pop_back();
            }
        }

        window.clear(sf::Color::Black);

        // Draw food
        foodShape.setPosition(food.x * CELL, food.y * CELL);
        window.draw(foodShape);

        // Draw snake
        for (auto &s : snake) {
            snakeShape.setPosition(s.x * CELL, s.y * CELL);
            window.draw(snakeShape);
        }

        window.display();
    }

    return 0;
}


