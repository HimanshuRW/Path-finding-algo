#include <conio.h>
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <string>
using namespace std;

#define ESP 27
#define START_POINT_KEY 9 // tab key
#define END_POINT_KEY 13  // ENter key
#define START_SEARCH 32   // space bar

// arrow keys to move
#define UP_ARROW 72
#define DOWN_ARROW 80
#define LEFT_ARROW 75
#define RIGHT_ARROW 77

// wall keys wasd
#define WALL_UP 119    // w
#define WALL_DOWN 115  // s
#define WALL_LEFT 97   // a
#define WALL_RIGHT 100 // d

// board
#define BOX_X_START 40
#define BOX_X_END 110
#define BOX_Y_START 7
#define BOX_Y_END 32

// design
// #define VERTICAL_LINE 186
// #define HORIZONTAL_LINE 205
// #define LD_CORNER 200
// #define LT_CORNER 201
// #define RT_CORNER 187
// #define RD_CORNER 188
#define WALL 219 // █
// #define VISITED 176  // ░
#define VISITED 177 // ▒
#define CURSOR 254  // ■

// colors
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15

typedef struct Coordinate
{
    int x;
    int y;
} Coordinate;

void game_engine();
void draw_board();
void GotoXY(int x, int y);
void initialize();
void print_wall();
void print_visited();
void print_cursor();
void print_starting_point();
void print_ending_point();
void move_cursor(string directtion);
void make_walls(string direction);
Coordinate pixel_to_index(int pixel_x, int pixel_y);
void update_cursor(string direction);
void set_starting_point();
void set_ending_point();
void start_search();

class Point
{
public:
    int x, y;
    bool wall = false, visited = false, end = false, start = false, explore = false;
    Point()
    {
        x = 0;
        y = 0;
    }
    Point(int a, int b)
    {
        x = a;
        y = b;
    }

    Point *get_neighbours()
    {
        Point *arr = new Point[8];
        int my_x = x - 1;
        int my_y = y - 1;
        int index = 0;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (!(i == 1 && j == 1))
                {
                    int pt_x = my_x + i;
                    int pt_y = my_y + j;
                    Point pt(pt_x, pt_y);
                    arr[index] = pt;
                    index++;
                }
            }
        }
        return arr;
    }

    void show()
    {
        cout << "x : " << x << ",   y : " << y << endl;
    }

    void print()
    {
        GotoXY(x, y);
        if (wall)
            print_wall();
        else if (visited)
            print_visited();
        else if (start)
            print_starting_point();
        else if (end)
            print_ending_point();
        // else { cout << "A"; }
    }
};

const int box_width = BOX_X_END - BOX_X_START + 1;
const int box_height = BOX_Y_END - BOX_Y_START + 1;

Point board[box_width][box_width];

int start_pt_x = 0;
int start_pt_y = 0;
int end_pt_x = 0;
int end_pt_y = 0;

int cursor_x = (BOX_X_START + BOX_X_END) / 2;
int cursor_y = (BOX_Y_START + BOX_Y_END) / 2;

int main()
{
    initialize();
    draw_board();
    game_engine();
    return 0;
}
void print_visited()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, GREEN); // 12 for red color
    char a = VISITED;
    cout << a;
    // SetConsoleTextAttribute(hConsole, 15); // Reset color to default (white)
}
void print_wall()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, LIGHTRED);
    char a = WALL;
    cout << a;
    // SetConsoleTextAttribute(hConsole, 15); // Reset color to default (white)
}
void print_cursor()
{
    GotoXY(cursor_x, cursor_y);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, CYAN);
    char a = CURSOR;
    cout << a;
}
void print_starting_point()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, YELLOW);
    char a = CURSOR;
    cout << a;
}
void print_ending_point()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, MAGENTA);
    char a = CURSOR;
    cout << a;
}

void initialize()
{
    for (int i = 0; i < box_width; i++)
    {
        for (int j = 0; j < box_height; j++)
        {
            int pt_x = i + BOX_X_START;
            int pt_y = j + BOX_Y_START;
            Point pt(pt_x, pt_y);
            if (i == 0 || i == box_width - 1 || j == 0 || j == box_height - 1)
                pt.wall = true;
            // if(i==j && i!=0) pt.visited = true;
            // if(i==55 && j==9) pt.start = true;
            // if(i==60 && j==10) pt.end = true;
            board[i][j] = pt;
        }
    }
}

void GotoXY(int x, int y)
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cursorPosition;
    cursorPosition.X = x;
    cursorPosition.Y = y;
    SetConsoleCursorPosition(consoleHandle, cursorPosition);
}

// clear the screen and make borders
void draw_board()
{
    system("cls");

    for (int i = 0; i < box_width; i++)
    {
        for (int j = 0; j < box_height; j++)
        {
            board[i][j].print();
        }
    }
    print_cursor();
}

void move_cursor(string direction)
{
    bool clear_background = true;

    // if curson is over starting point
    if (cursor_x == start_pt_x && cursor_y == start_pt_y)
    {
        clear_background = false;
        GotoXY(cursor_x, cursor_y);
        print_starting_point();
    }

    // if cursor is over ending point
    if (cursor_x == end_pt_x && cursor_y == end_pt_y)
    {
        clear_background = false;
        GotoXY(cursor_x, cursor_y);
        print_ending_point();
    }

    // if curson is over any wall
    Coordinate index = pixel_to_index(cursor_x, cursor_y);
    Point *point = &(board[index.x][index.y]);
    if (point->wall)
    {
        clear_background = false;
        GotoXY(cursor_x, cursor_y);
        print_wall();
    }
    if (clear_background)
    {
        GotoXY(cursor_x, cursor_y);
        printf(" ");
    }
    update_cursor(direction);
}

void update_cursor(string direction)
{
    if (direction == "up" && cursor_y > BOX_Y_START + 1)
        cursor_y--;
    if (direction == "down" && cursor_y < BOX_Y_END - 1)
        cursor_y++;
    if (direction == "left" && cursor_x > BOX_X_START + 1)
        cursor_x--;
    if (direction == "right" && cursor_x < BOX_X_END - 1)
        cursor_x++;
    print_cursor();
}

void make_walls(string direction)
{
    Coordinate index = pixel_to_index(cursor_x, cursor_y);
    board[index.x][index.y].wall = true;
    GotoXY(cursor_x, cursor_y);
    print_wall();
    update_cursor(direction);
}

Coordinate pixel_to_index(int pixel_x, int pixel_y)
{
    Coordinate index;
    index.x = pixel_x - BOX_X_START;
    index.y = pixel_y - BOX_Y_START;
    return index;
}

void set_starting_point()
{
    if (start_pt_x > 0)
    { // setting by erasing
        GotoXY(start_pt_x, start_pt_y);
        printf(" ");
    }
    start_pt_x = cursor_x;
    start_pt_y = cursor_y;
    GotoXY(start_pt_x, start_pt_y);
    print_starting_point();
}
void set_ending_point()
{
    if (end_pt_x > 0)
    { // setting by erasing
        GotoXY(end_pt_x, end_pt_y);
        printf(" ");
    }
    end_pt_x = cursor_x;
    end_pt_y = cursor_y;
    GotoXY(end_pt_x, end_pt_y);
    print_ending_point();
}

void game_engine()
{
    while (true)
    {
        if (kbhit())
        {
            char key = getch();
            if (key == ESP)
                exit(1);
            else if (key == UP_ARROW)
                move_cursor("up");
            else if (key == DOWN_ARROW)
                move_cursor("down");
            else if (key == LEFT_ARROW)
                move_cursor("left");
            else if (key == RIGHT_ARROW)
                move_cursor("right");
            else if (key == WALL_UP)
                make_walls("up");
            else if (key == WALL_DOWN)
                make_walls("down");
            else if (key == WALL_LEFT)
                make_walls("left");
            else if (key == WALL_RIGHT)
                make_walls("right");
            else if (key == START_POINT_KEY)
                set_starting_point();
            else if (key == END_POINT_KEY)
                set_ending_point();
            else if (key == START_SEARCH)
            {
                // clear cursor
                GotoXY(cursor_x, cursor_y);
                printf(" ");
                start_search();
            }
            GotoXY(0, 0);
            Sleep(5);
        }
        Sleep(5);
    }
}

void start_search()
{
    // set start end blocks.. visited.. explaore 
    // and start algo
}