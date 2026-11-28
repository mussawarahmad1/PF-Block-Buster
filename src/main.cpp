
#include <SFML/Graphics.hpp>
#include <fstream>
#include <cstdlib>
#include <ctime>
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
//scroing variable
int score = 0;
int lives = 3;

// Game states
const int MENU = 0;
const int PLAYING = 1;
const int GAME_OVER = 2;

int gameState = MENU;

// Paddle variables
const int PADDLE_WIDTH = 100;
const int PADDLE_HEIGHT = 14;
const int PADDLE_SPEED = 8;

int paddleX = 250;
int paddleY = 650;

// Ball variables
const int BALL_SIZE = 12;

int ballX = paddleX + (PADDLE_WIDTH - BALL_SIZE) / 2;
int ballY = paddleY - BALL_SIZE;

bool launched = false;

int dx = 0;
int dy = 0;

void loadLevel();

// Paddle movement
void movement(bool left, bool right)
{
    if (left)
        paddleX -= PADDLE_SPEED;

    if (right)
        paddleX += PADDLE_SPEED;

    if (paddleX < 0)
        paddleX = 0;

    if (paddleX > WINDOW_WIDTH - PADDLE_WIDTH)
        paddleX = WINDOW_WIDTH - PADDLE_WIDTH;
}


// Keep ball on paddle before launching
void updatewaitball()
{
    if (!launched)
    {
        ballX = paddleX + (PADDLE_WIDTH - BALL_SIZE) / 2;
        ballY = paddleY - BALL_SIZE;
    }
}
//ResetBall after falling below
void resetBall() {
    launched = false;
    
    dx = 0;
    dy = 0;
    ballX = paddleX + (PADDLE_WIDTH - BALL_SIZE) / 2;
    ballY = paddleY - BALL_SIZE;

}

// reset everything for a new game
void resetGame()
{
    score = 0;
    lives = 3;
    levelMissing = false;
    paddleX = (WINDOW_WIDTH - PADDLE_WIDTH) / 2;
    paddleY = 650;
    resetBall();
    loadLevel();
}

// Launch ball
void launchball()
{
    launched = true;
        if (rand() % 2 == 0) {
            dx = 3;
        }
        else {
            dx = -3;
        }
            
        dy = -5;
    
}


// Draw paddle
void drawPaddle(sf::RenderWindow& window)
{
    sf::RectangleShape paddle(
        sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT)
    );

    paddle.setPosition(
        sf::Vector2f(
            static_cast<float>(paddleX),
            static_cast<float>(paddleY)
        )
    );

    paddle.setFillColor(sf::Color::White);
    window.draw(paddle);
}


// Draw ball
void drawBall(sf::RenderWindow& window)
{
    sf::CircleShape ball(6.f);

    ball.setPosition(
        sf::Vector2f(
            static_cast<float>(ballX),
            static_cast<float>(ballY)));

    ball.setFillColor(sf::Color::White);
    window.draw(ball);
}


// Load level from file
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


// Draw bricks
void drawBricks(sf::RenderWindow& window)
{
    sf::Color colors[4] = {
        sf::Color(25, 25, 45),
        sf::Color::Green,
        sf::Color::Yellow,
        sf::Color::Red
    };

    sf::RectangleShape brick({ BRICK_W - 2, BRICK_H - 2 });

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            if (bricks[r][c] != 0)
            {
                brick.setFillColor(colors[bricks[r][c]]);

                brick.setPosition({ c * BRICK_W + 1,BRICKS_Y + r * BRICK_H + 1});

                window.draw(brick);
            }
        }
    }
}


// Brick position
float getBrickX(int col)
{
    return col * BRICK_W;
}

float getBrickY(int row)
{
    return BRICKS_Y + row * BRICK_H;
}
//to Check how Much brick are present
int countBricks() {
    int total = 0;
    for (int row = 0;row < ROWS;row++) {
        for (int col = 0;col < COLS;col++) {
            if(bricks[row][col]>0){
                total++;
            }
        }
    }
    return total;
}


// Check brick collision

bool checkBrickCollision(bool alreadyHit[ROWS][COLS])
{
    // Four corners of the ball
    int cornerX[4] = {ballX,ballX + BALL_SIZE - 1,ballX,ballX + BALL_SIZE - 1};
    int cornerY[4] = { ballY,ballY, ballY + BALL_SIZE - 1,ballY + BALL_SIZE - 1 };
    bool touchedBrick = false;
    for (int i = 0; i < 4; i++)
    {
        int cx = cornerX[i];
        int cy = cornerY[i];

        // Make sure corner is inside the brick area
        if (cy >= BRICKS_Y &&cy < BRICKS_Y + ROWS * BRICK_H &&cx >= 0 &&cx < COLS * BRICK_W)
        {
            // Calculatee bricks Positiion in the array
            int row = (cy - (int)BRICKS_Y) / (int)BRICK_H;
            int col = cx / (int)BRICK_W;
            if (bricks[row][col] > 0 && !alreadyHit[row][col])
            {
                alreadyHit[row][col] = true;
                bricks[row][col]--;// Reduce brick durabilit
                if (bricks[row][col] == 0)// Increase score
                    score += 50;
                else
                    score += 10;

                touchedBrick = true;
            }
        }
    }

    return touchedBrick;
}




// Handle window events
void handleInput(sf::RenderWindow& window)
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
        // keys for current state
        switch (gameState)
        {
        case MENU:
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::Num1 ||
                    keyPressed->code == sf::Keyboard::Key::Enter)
                {
                    resetGame();
                    gameState = PLAYING;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num2)
                {
                    gameState = PLAYING;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num3 ||
                    keyPressed->code == sf::Keyboard::Key::Escape)
                {
                    window.close();
                }
            }
            break;

        case PLAYING:
            break;

        case GAME_OVER:
            break;
        }
    }
}


// Update game
void updateGame()
{
    // run logic for active state
    switch (gameState)
    {
    case MENU:
        break;

    case PLAYING:
    {
        // 1. Paddle movement
        bool left =
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);

        bool right =
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

        movement(left, right);

        // 2. Keep ball on paddle
        updatewaitball();

        // 3Launch ball
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !launched)
        {
            launchball();
        }

        // 4. Move ball after launching
        if (launched)

        {
            bool alreadyHit[ROWS][COLS] = {};
            //mOVE hORIZONTALLY 
            int oldX = ballX;
            ballX += dx;
            if (checkBrickCollision(alreadyHit)) {
                ballX = oldX;
                dx = -dx;
            }
            ///move vertically;
            int oldY = ballY;
            ballY += dy;
            if (checkBrickCollision(alreadyHit)) {
                ballY = oldY;
                dy = -dy;
            }

            // Left and right wall collision
            if (ballX < 0) {
                ballX = 0;
                dx = -dx;

            }
            if (ballX + BALL_SIZE > WINDOW_WIDTH) {
                ballX = WINDOW_WIDTH - BALL_SIZE;
                dx = -dx;
            }
            // Top wall collision
            if (ballY < 50)
            {
                dy = -dy;
            }

            // Paddle collision
            bool overlapX =
                (ballX < paddleX + PADDLE_WIDTH) &&
                (ballX + BALL_SIZE > paddleX);

            bool overlapY =
                (ballY < paddleY + PADDLE_HEIGHT) &&
                (ballY + BALL_SIZE > paddleY);

            if (dy > 0 && overlapX && overlapY)
            {
                ballY = paddleY - BALL_SIZE;
                dy = -5;

                int ballCenterX = ballX + BALL_SIZE / 2;
                int partWidth = PADDLE_WIDTH / 4;
                int part = (ballCenterX - paddleX) / partWidth;

                if (part < 0) part = 0;
                if (part > 3) part = 3;

                if (part == 0)
                    dx = -6;
                else if (part == 1)
                    dx = -3;
                else if (part == 2)
                    dx = 3;
                else dx = 6;
            }

            //if Ball is missed
            if (ballY > WINDOW_HEIGHT)
            {
                lives--;
                if (lives > 0)
                {
                    resetBall();
                }
            }
        }
        break;
    }
    case GAME_OVER:
        break;
    }
}
   
    // draw complete game
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
            // draw current state
            switch (gameState)
            {
            case MENU:
            {
                sf::Text title(font, "BLOCK BUSTER", 36);
                title.setPosition({ 160.f, 160.f });
                window.draw(title);

                sf::Text opt1(font, "1. New Game", 24);
                opt1.setPosition({ 200.f, 270.f });
                window.draw(opt1);

                sf::Text opt2(font, "2. Continue", 24);
                opt2.setPosition({ 200.f, 320.f });
                window.draw(opt2);

                sf::Text opt3(font, "3. Exit", 24);
                opt3.setPosition({ 200.f, 370.f });
                window.draw(opt3);
            }
                break;

            case PLAYING:
                drawBricks(window);
                drawPaddle(window);
                drawBall(window);
                break;

            case GAME_OVER:
                break;
            }
        }

        window.display();
    }


// Main function
int main()
{
    sf::RenderWindow window(
        sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }),
        "Block Buster"
    );

    window.setFramerateLimit(60);

    srand(static_cast<unsigned int>(time(nullptr)));

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
