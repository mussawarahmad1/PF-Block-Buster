#include <SFML/Graphics.hpp>
#include <fstream>
#include<cstdlib>
#include<ctime>
using namespace std;

const unsigned int WINDOW_WIDTH = 600;
const unsigned int WINDOW_HEIGHT = 700;

const int ROWS = 8;
const int COLS = 10;
const float BRICK_W = 60.f;
const float BRICK_H = 25.f;
const float BRICKS_Y = 100.f;

int bricks[ROWS][COLS];
bool levelMissing = false;
sf::Font font;

// Paddle and ball variables
const float PADDLE_W = 100.f;
const float PADDLE_H = 14.f;
const float PADDLE_Y = 650.f;
const int PADDLE_SPEED = 8;
const float BALL_RADIUS = 6.f;

float paddleX = 250.f;
float ballX = paddleX + 44.f;
float ballY = 638.f;
int dx = 0;
int dy = 0;
bool ballLaunched = false;

void loadLevel()
{
    ifstream fin("level.txt");

    if (!fin)
    {
        levelMissing = true;
        return;
    }

    int rows, cols;

    fin >> rows >> cols;

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            fin >> bricks[i][j];
        }
    }

    fin.close();
}


void drawBricks(sf::RenderWindow& window)
{
    sf::Color colors[4] = { sf::Color(25, 25, 45),
                            sf::Color::Green,
                            sf::Color::Yellow,
                            sf::Color::Red };

    sf::RectangleShape brick({ BRICK_W - 2, BRICK_H - 2 });

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            if (bricks[r][c] != 0)
            {
                brick.setFillColor(colors[bricks[r][c]]);
                brick.setPosition({ c * BRICK_W + 1, BRICKS_Y + r * BRICK_H + 1 });
                window.draw(brick);
            }
        }
    }
}

void handleInput(sf::RenderWindow& window)
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();
        // launch ball on Space key
        if (const auto* key = event->getIf<sf::Event::KeyPressed>())
        {
            if (key->code == sf::Keyboard::Key::Space && !ballLaunched)
            {
                dy = -5;
                dx = (rand() % 2 == 0) ? 3 : -3;
                ballLaunched = true;
            }
        }
    }

    // move paddle and keep inside window
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        paddleX -= PADDLE_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        paddleX += PADDLE_SPEED;
    }

    if (paddleX < 0.f)
        paddleX = 0.f;
    if (paddleX > WINDOW_WIDTH - PADDLE_W)
        paddleX = WINDOW_WIDTH - PADDLE_W;
    }


void updateGame()
{
    // keep ball on paddle before launch
    if (!ballLaunched)
    {
        ballX = paddleX + 44.f;
        ballY = 638.f;
    }
}

void drawGame(sf::RenderWindow& window)
{
    window.clear(sf::Color(15, 15, 40));

    if (levelMissing)
    {
        sf::Text message(font, "Error: level.txt not found", 24);
        message.setPosition({ 100.f, 300.f });
        window.draw(message);
    }
    else
    {
        drawBricks(window);

        // draw paddle and ball
        sf::RectangleShape paddle({ PADDLE_W, PADDLE_H });
        paddle.setPosition({ paddleX, PADDLE_Y });
        paddle.setFillColor(sf::Color::Cyan);

        sf::CircleShape ball(BALL_RADIUS);
        ball.setPosition({ ballX, ballY });
        ball.setFillColor(sf::Color::White);

        window.draw(paddle);
        window.draw(ball);
    }

    window.display();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Block Buster");
    window.setFramerateLimit(60);

    // seed random generator
    srand(time(0));

    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        // Continue even if font file is not found
    }

    loadLevel();

    while (window.isOpen())
    {
        handleInput(window);
        updateGame();
        drawGame(window);
    }

    return 0;
}