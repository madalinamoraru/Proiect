#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

struct InputState {
    bool up = false, down = false, left = false, right = false, fire = false;
};


class Projectile {
    sf::RectangleShape shape;
    sf::Vector2f velocity{0.f, 0.f};

    public:
        Projectile(const sf::Vector2f& pos, const sf::Vector2f& vel) : velocity(vel) {
            shape.setSize({12.f, 12.f});
            shape.setFillColor(sf::Color::Yellow);
            shape.setPosition(pos);
        }

        Projectile(const Projectile& other) {
                shape = other.shape;
                velocity = other.velocity;
        }

        Projectile& operator=(const Projectile& other) {
            if (this != &other) {
                shape = other.shape;
                velocity = other.velocity;
            }
            return *this;
        }
        ~Projectile() = default;

        void update(sf::Time dt) {
            shape.move(velocity * dt.asSeconds());
        }

        void draw(sf::RenderWindow& window) const {
            window.draw(shape);
        }

        sf::Vector2f position() const { return shape.getPosition(); }
        sf::FloatRect bounds()  const { return shape.getGlobalBounds(); }
        void setPosition(const sf::Vector2f& p) { shape.setPosition(p); }
};


class Player {
    sf::RectangleShape body;
    float speed = 260.f;
    std::vector<Projectile> bullets;
    float fireCooldown = 0.15f;
    float timeSinceShot = 0.f;

    enum class Facing { Left, Right };
    Facing lastDirection = Facing::Right;

    public:
        Player() {
            body.setSize({60.f, 40.f});
            body.setFillColor(sf::Color(100, 200, 255));
            body.setPosition(370.f, 520.f);
        }

        Player(const Player&) = default;
        Player& operator=(const Player&) = default;
        ~Player() = default;

        void handleInput(const InputState& input, sf::Time dt) {
            sf::Vector2f movement{0.f, 0.f};
            if (input.up)    movement.y -= speed;
            if (input.down)  movement.y += speed;
            if (input.left)  { movement.x -= speed; lastDirection = Facing::Left; }
            if (input.right) { movement.x += speed; lastDirection = Facing::Right; }


            body.move(movement * dt.asSeconds());

            timeSinceShot += dt.asSeconds();
            if (input.fire && timeSinceShot >= fireCooldown) {
                shoot();
                timeSinceShot = 0.f;
            }
        }

    void update(sf::Time dt, const sf::Vector2u& windowSize) {
        // keep inside window
        auto pos = body.getPosition();
        auto size = body.getSize();
        pos.x = std::max(0.f, std::min(pos.x, windowSize.x - size.x));
        pos.y = std::max(0.f, std::min(pos.y, windowSize.y - size.y));
        body.setPosition(pos);
        // update bullets & remove off-screen ones
        for (auto& b : bullets) b.update(dt);
        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                [&](const Projectile& p){
                    auto box = p.bounds();
                    return box.left + box.width < 0.f || box.left > windowSize.x;
                }),
            bullets.end()
        );
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(body);
        for (const auto& b : bullets) b.draw(window);
    }

    friend std::ostream& operator<<(std::ostream& os, const Player& player) {
            sf::Vector2f pos = player.body.getPosition();
            os << pos.x << ", " << pos.y << "\n";
            return os;
        }

    private:
        void shoot() {
            auto box = body.getGlobalBounds();
            const float bulletSize = 12.f;
            float y = box.top + box.height * 0.5f - bulletSize * 0.5f;

            if (lastDirection == Facing::Left) {
                float x = box.left - bulletSize; // from left side
                bullets.emplace_back(sf::Vector2f{x, y}, sf::Vector2f{-600.f, 0.f});
            } else {
                float x = box.left + box.width;  // from right side
                bullets.emplace_back(sf::Vector2f{x, y}, sf::Vector2f{+600.f, 0.f});
            }
        }
};

class Game {
    public:
    sf::RenderWindow window;
    sf::Color background{20, 20, 24};
    unsigned fps = 60;
    Player player;

    public:
        Game(unsigned w = 800, unsigned h = 600, const std::string& title = "oop"): window(sf::VideoMode(w, h), title) {
            window.setFramerateLimit(fps);
        }

        friend std::istream& operator>>(std::istream& in, Game& game) {
            int r, g, b;
            std::cout << "Enter background color (R G B): ";
            in >> r >> g >> b;

            if (!in) {
                std::cerr << "Invalid color input!\n";
                return in;
            }

            // clamp values between 0 and 255
            if (r < 0) r = 0; if (r > 255) r = 255;
            if (g < 0) g = 0; if (g > 255) g = 255;
            if (b < 0) b = 0; if (b > 255) b = 255;

            game.background = sf::Color(r, g, b);
            return in;
        }

        friend std::ostream& operator<<(std::ostream& os, const Game& game) {
            os << game.player;
            return os;
        }

        void run() {
            sf::Clock clock;
            while (window.isOpen()) {
                InputState input{};
                processEvents(input);
                sf::Time dt = clock.restart();
                player.handleInput(input, dt);
                player.update(dt, window.getSize());
                render();
            }
        }

    private:
        void processEvents(InputState& input) {
            sf::Event e{};
            while (window.pollEvent(e)) {
                if (e.type == sf::Event::Closed) window.close();
                if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
                    window.close();
            }
            input.up    = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
            input.down  = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
            input.left  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
            input.right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
            input.fire  = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
        }

        void render() {
            window.clear(background);
            player.draw(window);
            window.display();
        }
};


int main() {
    int r, g, b;
    Game game;

    // std::cin >> game;

    game.run();
    std::cout<<game;
    return 0;
}