#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>

constexpr int WIDTH = 600;
constexpr int HEIGHT = 480;
int SPEED = 4;
bool field[WIDTH][HEIGHT] = {false};

int random() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis{};
    return dis(gen);
}


struct player {
    int x, y, direction;
    sf::Color color;

    explicit player(const sf::Color c) {
        x = random() % WIDTH;
        y = random() % HEIGHT;
        color = c;
        direction = random() % 4;
    }

    void tick() {
        if (direction == 0) { y += 1; }
        if (direction == 1) { x -= 1; }
        if (direction == 2) { x += 1; }
        if (direction == 3) { y -= 1; }

        if (x >= WIDTH) { x = 0; }
        if (x < 0) { x = WIDTH - 1; }

        if (y >= HEIGHT) { y = 0; }
        if (y < 0) { y = HEIGHT - 1; }
    }

    [[nodiscard]] sf::Vector3f getColor() const {
        return {static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b)};
    }
};



int main() {
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Tron");
    window.setFramerateLimit(60);


    return 0;
}