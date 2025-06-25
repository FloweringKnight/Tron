#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/RenderStates.hpp>
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
        if (direction == 0) { y += 1; }     //  向下
        if (direction == 1) { x -= 1; }     //  向左
        if (direction == 2) { x += 1; }     //  向右
        if (direction == 3) { y -= 1; }     //  向上

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

    const sf::Texture texture("../bg.png");
    sf::Sprite sBackground(texture);

    player p1(sf::Color::Red);
    player p2(sf::Color::Green);

    sf::RenderTexture renderTexture({WIDTH,HEIGHT});
    renderTexture.setSmooth(true);      //  平滑的
    sf::Sprite sprite(renderTexture.getTexture());
    renderTexture.clear();
    renderTexture.draw(sBackground);

    sf::Font font("../arial.ttf");
    const char *text("YOU WIN!!!");
    sf::Text text1(font , text , 35);
    text1.setPosition(sf::Vector2f(WIDTH / 2 - 80, 20));

    auto* shader = new sf::Shader;
    bool b = shader->loadFromFile("../shader.frag",sf::Shader::Type::Fragment);
    if (!b) { std::cout << "fail to load shader!!!" << std::endl; }
    shader->setUniform("frag_ScreenResolution", sf::Vector2f(WIDTH, HEIGHT));
    shader->setUniform("frag_LightAttenuation" , 100.f);        //  setUniform的参数不支持int，仅支持float
    sf::RenderStates state(shader);
    // state.shader = shader;

    bool game = true;

    while (window.isOpen()) {
        while (std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            else if (event->is<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) window.close();

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  if (p1.direction != 2) p1.direction = 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) if (p1.direction != 1) p1.direction = 2;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))    if (p1.direction != 0) p1.direction = 3;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))  if (p1.direction != 3) p1.direction = 0;

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) if (p2.direction != 2) p2.direction = 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) if (p2.direction != 1) p2.direction = 2;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) if (p2.direction != 0) p2.direction = 3;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) if (p2.direction != 3) p2.direction = 0;
            }
        }

        if (!game) {
            window.draw(text1);
            window.display();
            continue;
        }

        for (int i = 0 ; i < SPEED ; i++) {
            p1.tick();
            p2.tick();
            if (field[p1.x][p1.y] == 1) {game = false; text1.setFillColor(p2.color);}
            if (field[p2.x][p2.y] == 1) {game = false; text1.setFillColor(p1.color);}
            field[p1.x][p1.y] = true;
            field[p2.x][p2.y] = true;

            // sf::CircleShape c(3);
            // c.setPosition({static_cast<float>(p1.x),static_cast<float>(p1.y)});
            // c.setFillColor(p1.color);
            // renderTexture.draw(c);
            // c.setPosition({static_cast<float>(p2.x),static_cast<float>(p2.y)});
            // c.setFillColor(p2.color);
            // renderTexture.draw(c);
            renderTexture.display();

            shader->setUniform("frag_LightOrigin", sf::Vector2f(static_cast<float>(p1.x), static_cast<float>(p1.y)));
            shader->setUniform("frag_LightColor", p1.getColor());
            renderTexture.draw(sprite, state);
            shader->setUniform("frag_LightOrigin", sf::Vector2f(static_cast<float>(p2.x), static_cast<float>(p2.y)));
            shader->setUniform("frag_LightColor", p2.getColor());
            renderTexture.draw(sprite, state);
        }

        window.clear();
        window.draw(sprite,state);
        window.display();


    }

    delete shader;
    shader = nullptr;
    return 0;
}