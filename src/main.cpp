
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Block Buster");

    sf::RectangleShape paddle(sf::Vector2f(120, 20));
    paddle.setFillColor(sf::Color::White);
    paddle.setPosition({ 340, 540 });

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            paddle.move({ -2, 0 });
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            paddle.move({ 2, 0 });
        }
        window.clear(sf::Color::Blue);
        window.draw(paddle);
        window.display();
    }

    return 0;
}
