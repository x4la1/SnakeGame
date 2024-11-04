#include <iostream>
#include <vector>
#include <stdio.h>
#include <conio.h>
#include <deque>
#include <random>
#include <time.h>
#include <iterator>
#include <Windows.h>
using namespace std;

const char APPLE_SYMBOL = '$';
const char BORDER_SYMBOL = '#';
const char SNAKE_BODY_SYMBOL = '@';
const char SNAKE_HEAD_SYMBOL = '+';
const int COLUMN_COUNT = 16;
const int ROW_COUNT = 8;
const int SNAKE_SLEEP_TIME = 500;
const string SNAKE_LEFT_DIRECTION = "Left";
const string SNAKE_RIGHT_DIRECTION = "Right";
const string SNAKE_UP_DIRECTION = "Up";
const string SNAKE_DOWN_DIRECTION = "Down";
const int UP_DIRECTION_KEY = 72;
const int DOWN_DIRECTION_KEY = 80;
const int LEFT_DIRECTION_KEY = 75;
const int RIGHT_DIRECTION_KEY = 77;
const int FIELD_CELLS_COUNT = (ROW_COUNT - 2) * (COLUMN_COUNT - 2);

struct Apple
{
    int x;
    int y;
};

struct SnakeCell
{
    int x;
    int y;
};

struct Snake
{
    deque<SnakeCell> body;
    string direction;
};

Snake CreateSnake(Snake &snake)
{
    SnakeCell snakeHead;
    snakeHead.x = rand() % (COLUMN_COUNT - 3) + (COLUMN_COUNT - 15);
    snakeHead.y = rand() % (ROW_COUNT - 2) + (ROW_COUNT - 7);
    snake.body.push_back(snakeHead);

    SnakeCell snakeBody;
    snakeBody.x = snakeHead.x + 1;
    snakeBody.y = snakeHead.y;
    snake.body.push_back(snakeBody);

    return snake;
}

void AddSnakeToField(vector<vector<char>> &field, Snake &snake)
{
    SnakeCell snakeHead = snake.body.at(0);
    field[snakeHead.y][snakeHead.x] = SNAKE_HEAD_SYMBOL;

    for (size_t i = 1; i < snake.body.size(); i++)
    {
        field[snake.body[i].y][snake.body[i].x] = SNAKE_BODY_SYMBOL;
    }
}

Apple CreateApple(Apple &apple, Snake &snake, vector<vector<char>> &field)
{
    apple.x = rand() % (COLUMN_COUNT - 3) + 1;
    apple.y = rand() % (ROW_COUNT - 3) + 1;

    for (auto i = 0; i < snake.body.size(); i++)
    {
        if (snake.body[i].x == apple.x && snake.body[i].y == apple.y)
        {
            apple.x = rand() % (COLUMN_COUNT - 3) + 1;
            apple.y = rand() % (ROW_COUNT - 3) + 1;

            i = 0;
        }
    }
    field[apple.y][apple.x] = APPLE_SYMBOL;

    return apple;
}

void PrintField(vector<vector<char>> &field)
{

    for (const auto vec : field)
    {
        std::copy(vec.begin(), vec.end(), std::ostream_iterator<char>(cout, ""));
        cout << "\n";
    }
}

vector<vector<char>> DeleteSnakeFromField(vector<vector<char>> &field, Snake &snake)
{
    for (auto i = 0; i < snake.body.size(); i++)
    {
        field[snake.body[i].y][snake.body[i].x] = ' ';
    }

    return field;
}

Snake SnakeMove(Snake &snake, bool &hasEatenApple)
{
    SnakeCell newSnakeCell = snake.body.at(snake.body.size() - 1);
    SnakeCell snakeHead = snake.body.at(0);

    newSnakeCell.x = snakeHead.x;
    newSnakeCell.y = snakeHead.y;

    if (snake.direction == SNAKE_LEFT_DIRECTION)
    {
        newSnakeCell.x = newSnakeCell.x - 1;
    }
    else if (snake.direction == SNAKE_RIGHT_DIRECTION)
    {
        newSnakeCell.x = newSnakeCell.x + 1;
    }
    else if (snake.direction == SNAKE_DOWN_DIRECTION)
    {
        newSnakeCell.y = newSnakeCell.y + 1;
    }
    else if (snake.direction == SNAKE_UP_DIRECTION)
    {
        newSnakeCell.y = newSnakeCell.y - 1;
    }

    if (hasEatenApple)
    {
        snake.body.push_front(newSnakeCell);
        hasEatenApple = false;
    }
    else
    {
        snake.body.pop_back();
        snake.body.push_front(newSnakeCell);
    }

    return snake;
}

bool checkWall(Snake &snake)
{
    if (snake.body[0].x == 0 || snake.body[0].x == COLUMN_COUNT - 1)
    {
        return true;
    }
    else if (snake.body[0].y == 0 || snake.body[0].y == ROW_COUNT - 1)
    {
        return true;
    }

    return false;
}

bool CheckCrashItself(Snake &snake)
{
    SnakeCell snakeHead = snake.body.at(0);

    for (auto i = 1; i < snake.body.size(); i++)
    {
        if (snakeHead.x == snake.body[i].x && snakeHead.y == snake.body[i].y)
        {
            return true;
        }
    }

    return false;
}

Snake ChangeSnakeDirection(Snake &snake)
{
    int k1, k2;
    string newDirection;

    k1 = _getch();

    if (k1 == 0)
    {
        k2 = _getch();

        switch (k2)
        {
        case UP_DIRECTION_KEY:
            if (snake.direction != SNAKE_DOWN_DIRECTION)
            {
                snake.direction = SNAKE_UP_DIRECTION;
            }
            break;

        case RIGHT_DIRECTION_KEY:
            if (snake.direction != SNAKE_LEFT_DIRECTION)
            {
                snake.direction = SNAKE_RIGHT_DIRECTION;
            }
            break;

        case DOWN_DIRECTION_KEY:
            if (snake.direction != SNAKE_UP_DIRECTION)
            {
                snake.direction = SNAKE_DOWN_DIRECTION;
            }
            break;

        case LEFT_DIRECTION_KEY:
            if (snake.direction != SNAKE_RIGHT_DIRECTION)
            {
                snake.direction = SNAKE_LEFT_DIRECTION;
            }
            break;

        default:
            break;
        }
    }

    return snake;
}

void printGameOver(vector<vector<char>> &field)
{
    field[4][4] = 'G';
    field[4][5] = 'a';
    field[4][6] = 'm';
    field[4][7] = 'e';
    field[4][8] = 'O';
    field[4][9] = 'v';
    field[4][10] = 'e';
    field[4][11] = 'r';
    field[4][12] = '!';

    PrintField(field);
}

vector<vector<char>> CreateField()
{
    vector<vector<char>> field(ROW_COUNT, vector<char>(COLUMN_COUNT, ' '));

    for (int i = 0; i < COLUMN_COUNT; i++)
    {
        field[0][i] = BORDER_SYMBOL;
        field[ROW_COUNT - 1][i] = BORDER_SYMBOL;
    }

    for (int i = 1; i < ROW_COUNT - 1; i++)
    {
        field[i][0] = BORDER_SYMBOL;
        field[i][COLUMN_COUNT - 1] = BORDER_SYMBOL;
    }

    return field;
}

bool CheckApple(Snake &snake, Apple &apple, bool &hasEatenApple)
{
    SnakeCell snakeHead = snake.body.at(0);

    if (snakeHead.x == apple.x && snakeHead.y == apple.y)
    {
        hasEatenApple = true;
        return true;
    }

    return false;
}

bool CheckWin(Snake &snake)
{
    if (snake.body.size() == FIELD_CELLS_COUNT)
    {
        return true;
    }

    return false;
}

void PrintWin(vector<vector<char>> &field)
{
    field[4][6] = 'W';
    field[4][7] = 'I';
    field[4][8] = 'N';
    field[4][9] = '!';
}

int main()
{
    srand(time(NULL));

    bool hasEatenApple = false;
    Snake snake;
    Apple apple;
    vector<vector<char>> field = CreateField();

    snake.direction = SNAKE_LEFT_DIRECTION;

    snake = CreateSnake(snake);
    AddSnakeToField(field, snake);

    apple = CreateApple(apple, snake, field);

    PrintField(field);

    while (true)
    {
        Sleep(SNAKE_SLEEP_TIME);

        if (_kbhit())
        {
            snake = ChangeSnakeDirection(snake);
        }

        field = DeleteSnakeFromField(field, snake);
        snake = SnakeMove(snake, hasEatenApple);

        if (checkWall(snake))
        {
            field = CreateField();
            printGameOver(field);
            break;
        }

        if (CheckCrashItself(snake))
        {
            field = CreateField();
            printGameOver(field);
            break;
        }

        if (CheckWin(snake))
        {
            field = CreateField();
            PrintWin(field);
            break;
        }

        if (CheckApple(snake, apple, hasEatenApple))
        {
            apple = CreateApple(apple, snake, field);
        }
        
        AddSnakeToField(field, snake);
        PrintField(field);
    }

    return 0;
}