#include <SFML/Graphics.hpp>
#include <fstream>
#include<cstdlib>
#include<ctime>
using namespace std;

const unsigned int WINDOW_WIDTH = 600;
const unsigned int WINDOW_HEIGHT = 700;

const int PADDLE_WIDTH=100;
const int PADDLE_HEIGHT=14;
const int PADDLE_SPEED =8;

int paddleX= 250;
int paddleY=650;

const int BALL_SIZE =12;
int ballX = paddleX +(PADDLE_WIDTH-BALL_SIZE)/2;
int ballY=paddleY - BALL_SIZE;

bool launched =false;
int dx=0;
int dy =0;

const int ROWS = 8;
const int COLS = 10;
const float BRICK_W = 60.f;
const float BRICK_H = 25.f;
const float BRICKS_Y = 100.f;

int bricks[ROWS][COLS];
bool levelMissing = false;
sf::Font font;

void movement(bool left,bool right){
    if(left){
        paddleX -=PADDLE_SPEED;
    }
    if(right){
        paddleX +=PADDLE_SPEED;
    }
    if(paddleX <0){
        paddleX =0; //left side window limit
    }
    if(paddleX>WINDOW_WIDTH-PADDLE_WIDTH){
        paddleX =WINDOW_WIDTH -PADDLE_WIDTH;  //Right side Limit 
    }
}
//To follow ball the paddle before launching
void updatewaitball(){
    if(!launched){
        ballX =paddleX+(PADDLE_WIDTH -BALL_SIZE);
        ballY =paddleY -BALL_SIZE;
    }
}
void launchball(){
    if(!launched){
        launched =true;
        dx =(rand()%2 ==0) ? -3 :3 ; // it will assign randomly -3 or 3 so that the ball move randomly left or right;
        dy =-5;
    }
}
// to Draw the the shape of paddle;
void drawpaddle(sf::Renderwindow& window)
{
    sf::RectangleShape paddle(sf:: Vectore2f(PADDLE_WIDTH,PADDLE_HEIGHT));
    paddle.setPosition(sf::Vector2f(static_cast<float>(paddleX),static_cast<float>paddleY));
    paddle.setFillColor(sf::Color::white);
    window.draw(paddle);

}
// To draw the shape of a ball
void drawBall(sf::Renderwindow& window){
    sf::CircleShape ball(6.f);
    ball.setPosition(sf::Vector2f(static_cast<float>(ballX),static_cast<float>(ballY)));
    ball.setFillColor(sf::color::white);
    window.draw(ball);
}
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
    }
}
//
void updateGame()
{
    bool left =sf::Keyboard::isKeyPressed(sf::Keyboard::Key::left)|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    bool right =sf::Keyboard::isKeyPressed(sf::Keyboard::Key::right)|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
    //Left or Right movement of the paddle
    movement(left,right);
     
    //if space is pressed to Launch the ball
    updatewaitball();
    if(sf::Keyboard::isKeyPressed(sf::Key::Space)){
        launchball();
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
    }

    window.display();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Block Buster");
    window.setFramerateLimit(60);

    font.openFromFile("C:/Windows/Fonts/arial.ttf");

    loadLevel();
    srand(staric_cast<unsigned int>(time(nullptr)));

    while (window.isOpen())
    {
        handleInput(window);
        updateGame();
        drawGame(window);
    }
 return -1;
    return 0;
}