#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGHT 60
#define LEADER_BOARD_TOP 60
#define PLAYERS_MAX 100
#define MAX_LENGHT 60
#define SPACE_BAR 32
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ESC 27
#define BOX_X_START 10
#define BOX_X_END 70
#define BOX_Y_START 10
#define BOX_Y_END 30
#define DOWN_ARROW "v"
#define UP_ARROW "^"
#define LEFT_ARROW "<"
#define RIGHT_ARROW ">"
#define FOOD 149
#define VERTICAL_LINE 186
#define HORIZONTAL_LINE 205
#define LD_CORNER 200
#define LT_CORNER 201
#define RT_CORNER 187
#define RD_CORNER 188

struct coordinate
{
    int x;
    int y;
};

struct people
{
    char name[40];
    int last_score;
    int high_score;
};

typedef struct coordinate coordinate;
typedef struct people people;

int length = 57;
int direction = RIGHT;
int score = 0;
char player_name[40];
char player_game_name[40];
coordinate body[MAX_LENGHT];
coordinate head;
coordinate tail = {30, 20};
coordinate tailold = {29, 20};
coordinate food = {0, 0};
coordinate score_coordinates = {40, 5};
char msgs_arr[6][200] = {"", "", "", "", "", ""};
int total_msgs = 0;

void initializeValues();
void makeSnake();
void moveSnake();
void Print();
void load();
void gotoxy(int x, int y);
void Boarder();
void GotoXY(int x, int y);
void delay();
void runGame();
void printHead();
int checkStrike();
void makeFood();
int is_Present(struct coordinate apnaFood);
coordinate getFoodCoordinates();
void end();
void foodEaten();
void print_score();
void input_name();
int check_LeaderBoard();
void save_leader_board(people *leader_board_array, int n);
void convert_to_game_style(char str[]);
int check_player();
void already_present(people *arr, int i, int total_players);
void new_player(people *arr, int i);
void convert_to_game_style(char *str);
int get_last_score_of_leader_board();
void write_in_players(people *arr, int n);
void add_msg(char msg[]);
void print_people_arr(people *leader_board_array, int n);
int already_present_people(people *arr, int total, char *given_name);
void display_msgs();
void reset();

int main()
{
    // Print();
    // input_name();
    load();
    initializeValues();
    Boarder();
    makeFood();
    runGame();
    return 0;
}

// game engine
void runGame()
{
    while (!kbhit())
    {
        delay();
        moveSnake();
        makeSnake();
    }
    char a = getch();
    if (a == UP || a == DOWN || a == LEFT || a == RIGHT)
    {
        direction = a;
        runGame();
    }
    else if (a == ESC)
        exit(0);
    else
        runGame();
}

// move the snake according to body and direction
void moveSnake()
{
    // checkStrike - return 1 for game end
    //               - 2 for food eaten
    //               - 0 for nothing;
    if (checkStrike() == 1)
        end();
    else if (checkStrike() == 2)
        foodEaten();
    if (direction == UP)
        head.y--;
    else if (direction == DOWN)
        head.y++;
    else if (direction == LEFT)
        head.x--;
    else if (direction == RIGHT)
        head.x++;
    tailold = body[0];
    for (int i = 0; i < length - 1; i++)
    {
        body[i] = body[i + 1];
    }
    body[length - 1] = head;
}

// reset all values like tail, body, lenght,score,total_msgs
void reset()
{
    length = 7;
    tail.x = 30;
    tail.y = 20;
    tailold.x = 29;
    tailold.y = 20;
    score = 0;
    direction = RIGHT;
    total_msgs = 0;
    initializeValues();
}

// set head , tail and body of the snake
void initializeValues()
{
    body[0] = tail;
    for (int i = 1; i < length; i++)
    {
        body[i].x = tail.x + i;
        body[i].y = tail.y;
    }
    head = body[length - 1];
}

// draw the snake on the board
void makeSnake()
{
    for (int i = 0; i < length; i++)
    {
        coordinate currentBodyPart = body[i];
        GotoXY(currentBodyPart.x, currentBodyPart.y);
        printf("*");
    }
    printHead();
    GotoXY(tailold.x, tailold.y);
    printf(" ");
    gotoxy(0, 0);
}

// draw the head
void printHead()
{
    GotoXY(head.x, head.y);
    if (direction == RIGHT)
        printf(">");
    if (direction == LEFT)
        printf("<");
    if (direction == UP)
        printf("^");
    if (direction == DOWN)
        printf("v");
}

// welcome msg
void Print()
{
    system("cls");
    GotoXY(10, 12);
    printf("\tWelcome to the mini Snake game.(press any key to continue)\n");
    getch();
    system("cls");
    printf("\tGame instructions:\n");
    printf("\n-> Use arrow keys to move the snake.\n\n-> You will be provided foods at the several coordinates of the screen which you have to eat. Everytime you eat a food the length of the snake will be increased by 1 element and thus the score.\n\n-> Here you are provided with three lives. Your life will decrease as you hit the wall or snake's body.\n\n-> YOu can pause the game in its middle by pressing any key. To continue the paused game press any other key once again\n\n-> If you want to exit press esc. \n");
    printf("\n\nPress any key to play game...");
    if (getch() == ESC)
        exit(0);
}

// set cursor to x and y
void gotoxy(int x, int y)
{

    COORD c;
    c.X = x;
    c.Y = y;

    SetConsoleCursorPosition(
        GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// print the score
void print_score()
{
    GotoXY(score_coordinates.x, score_coordinates.y);
    printf("%d", score);
}

// clear the screen and make borders
void Boarder()
{
    system("cls");
    GotoXY(29, 3);
    printf("%s", player_name);
    GotoXY(30, 5);
    printf("SCORE : ");
    print_score();
    int i;
    for (i = BOX_X_START; i <= BOX_X_END; i++)
    {
        // --- top line ---
        GotoXY(i, 10);
        if (i == BOX_X_START)
            printf("%c", LT_CORNER);
        else if (i == BOX_X_END)
            printf("%c", RT_CORNER);
        else
            printf("%c", HORIZONTAL_LINE);

        // --- bottom line ---
        GotoXY(i, 30);
        if (i == BOX_X_START)
            printf("%c", LD_CORNER);
        else if (i == BOX_X_END)
            printf("%c", RD_CORNER);
        else
            printf("%c", HORIZONTAL_LINE);
    }
    for (i = BOX_Y_START + 1; i < BOX_Y_END; i++)
    {
        GotoXY(10, i);
        printf("%c", VERTICAL_LINE);
        GotoXY(70, i);
        printf("%c", VERTICAL_LINE);
    }
}

// loading animation
void load()
{
    system("cls");
    int row, col, r, c, q;
    gotoxy(36, 14);
    printf("loading...");
    gotoxy(30, 15);
    for (r = 1; r <= 20; r++)
    {
        delay();

        printf("%c", 177); // ascii 177 is ▒
    }
    getch();
}

// gotoxy + fast output
void GotoXY(int x, int y)
{
    HANDLE a;
    COORD b;
    fflush(stdout);
    b.X = x;
    b.Y = y;
    a = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(a, b);
}

void delay()
{
    Sleep(80);
}

int checkStrike()
{
    // return 1 for game end
    // 2 for food eaten
    // 0 for nothing
    if (head.x <= BOX_X_START || head.x >= BOX_X_END || head.y >= BOX_Y_END || head.y <= BOX_Y_START)
        return 1;
    for (int i = 0; i < length - 1; i++)
        if (body[i].x == head.x && body[i].y == head.y)
            return 1;
    if (head.x == food.x && head.y == food.y)
        return 2;
    return 0;
}

void makeFood()
{
    food = getFoodCoordinates();
    GotoXY(food.x, food.y);
    printf("%c", FOOD);
}

coordinate getFoodCoordinates()
{
    coordinate myFood;
    do
    {
        srand(time(NULL));
        myFood.x = rand() % (BOX_X_END - BOX_X_START - 1) + BOX_X_START + 1;
        myFood.y = rand() % (BOX_Y_END - BOX_Y_START - 1) + BOX_Y_START + 1;
    } while (is_Present(myFood));
    return myFood;
}

int is_Present(struct coordinate apnaFood)
{
    for (int i = 0; i < length; i++)
    {
        if (body[i].x == apnaFood.x && body[i].y == apnaFood.y)
            return 1;
    }
    return 0;
}

// end the game
void end()
{
    char myMsg[70];
    sprintf(myMsg, "U died.... Your Score was %d", score);
    add_msg(myMsg);

    check_player();

    strcpy(myMsg, "Press any key to exit .... or SPACE to play again.......... ");
    add_msg(myMsg);
    
    display_msgs();

    int a = getch();
    if (a == SPACE_BAR)
    {
        reset();
        Boarder();
        makeFood();
        runGame();
    }
    else
        exit(0);
};

// when food is eaten
void foodEaten()
{
    score++;
    print_score();
    makeFood();
    body[length].x = head.x;
    body[length].y = head.y;
    length++;
};

// the name input and set the player_game_name
void input_name()
{
    system("cls");
    gotoxy(30, 15);
    printf("Enter your Name : ");
    gets(player_name);
    strcpy(player_game_name, player_name);
    convert_to_game_style(player_game_name);
}

// check if player cracked the leader board or not
// if s/he did .. update the leader board file
int check_LeaderBoard()
{
    FILE *f = fopen("leader_board.txt", "r");
    people arr[LEADER_BOARD_TOP];
    int total = 0;
    // ------- READ ENTRIES ------------
    for (; fscanf(f, "%s %d",
                  arr[total].name,
                  &(arr[total].high_score)) > 0;
         total++)
        ;

    fclose(f);

    int myIndex = already_present_people(arr, total, player_game_name);
    int i;
    for (i = 0; i < total; i++)
    {
        people current = arr[i];
        if (current.high_score < score)
            break;
    }
    if (i == 10)
        return 0; // nahi hua in leader board
    if (total == 10)
        total--;
    if (myIndex < 0)   // new player in the leader board
    {
        for (int j = total; j >= i; j--)
        {
            arr[j] = arr[j - 1];
        }
        strcpy(arr[i].name, player_game_name);
    }
    else   // player already present in leader board
    {
        people myPlayer = arr[myIndex];

        // move it to from myIndex to ith index
        printf("myIndex : %d, name : %s\n", myIndex, arr[myIndex].name);
        printf("i : %d, name : %s\n", i, arr[i].name);
        for (int j = myIndex; j > i; j--)
        {
            arr[j] = arr[j - 1];
        }
        arr[i] = myPlayer;
    }
    arr[i].high_score = score;
    save_leader_board(arr, total);
    return i+1; // u got ith position in leader board
}

// take an array.. clean the leader board and write it according to array it takes
void save_leader_board(people *leader_board_array, int n)
{
    FILE *f = fopen("leader_board.txt", "w");

    // if there was an error
    if (f == NULL)
        perror("Error opening file"); // print error
    // if there was no error
    else
        for (int i = 0; i <= n; i++)
            fprintf(f, "%s %d\n", leader_board_array[i].name, leader_board_array[i].high_score);

    fclose(f);
}

// only for development porpuse.. no use in production
void print_people_arr(people *leader_board_array, int n)
{
    printf("\n------------Inside print People , n: %d----------------------\n", n);
    for (int i = 0; i <= n; i++)
    {
        printf("Name : %s\n", leader_board_array[i].name);
        printf("last_Score : %d\n", leader_board_array[i].last_score);
        printf("high_Score : %d\n", leader_board_array[i].high_score);
        printf("--------------------------\n");
    }
    printf("\n");
    getch();
}

// check if player is present in players.txt or not
// and deal it accoridningly
// life if its old player.. check if its break high score.. if yes.. check leader board breaking
// check the last played game of old player .. set it current score then
// if player is new ... set its new entries.. check leader board also
int check_player()
{
    FILE *f = fopen("players.txt", "r");
    people arr[PLAYERS_MAX];
    int total_players;
    int chalte_raho = 1;
    // ------- READ ENTRIES ------------
    for (total_players = 0; chalte_raho > 0; total_players++)
    {
        chalte_raho = fscanf(f, "%s %d %d", arr[total_players].name, &(arr[total_players].last_score), &(arr[total_players].high_score));
        // printf("name : %s\n",arr[total_players].name);
        // printf("chalte raho : %d\n",chalte_raho);
        // printf("-------------------\n");
    }
    fclose(f);
    int i;
    for (i = 0; i < total_players; i++)
    {
        if (strcmp(player_game_name, arr[i].name) == 0)
        {
            // printf("already present with i: %d, name : %s\n", i, arr[i].name);
            total_players = total_players - 2;
            already_present(arr, i, total_players);
            return 0;
        }
    }
    i--;
    new_player(arr, i);
    return 0;
}

// lower case all name characters and fill all  spaces with '_'
void convert_to_game_style(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        if (isspace(str[i]))
            str[i] = '_';
        str[i] = tolower(str[i]);
    }
}

// player already present in the players.txt
void already_present(people *arr, int i, int total_players)
{
    // ---- to remove -----
    // printf("in the top of fuction already present and total players are : %d", total_players);

    char myMsg[50];

    // set new high score
    if (arr[i].high_score < score)
    {
        // ---- to remove -----
        // printf("\n---u breaked yr high score -------\n");

        int old_high_score = arr[i].high_score;
        sprintf(myMsg, "Congrats !!! You breaked your old high score of %d with %d points...\n", old_high_score, score - old_high_score);
        add_msg(myMsg);

        // check leader board
        int leader_position = check_LeaderBoard();

        // ---- to remove -----
        // printf("\n leader board scene , position : %d\n", leader_position);

        if (leader_position != 0)
        {
            sprintf(myMsg, "Congrats !!! You breaked the leader Board with position of %d...\n", leader_position);
            add_msg(myMsg);

            // ---- to remove -----
            // printf("\nhigh score breaked the leader board \n");
            getch();
        }
        arr[i].high_score = score;
    }
    if (arr[i].last_score < score)
    {
        // ---- to remove -----
        // printf("\nBetter the last score \n");

        int old_score = arr[i].last_score;
        sprintf(myMsg, "Congrats !!! You Played better then before.. making %d points extra...\n", score - old_score);
        add_msg(myMsg);
    }
    arr[i].last_score = score;
    write_in_players(arr, total_players);
};

// new comer
void new_player(people *arr, int i)
{
    int leader_position = check_LeaderBoard();
    if (leader_position != 0)
    {
        char myMsg[50];
        sprintf(myMsg, "Congrats !!! You break the Leader Board with position of %d\n", leader_position);
        add_msg(myMsg);
    }

    strcpy(arr[i].name, player_game_name);
    arr[i].last_score = score;
    arr[i].high_score = score;
    write_in_players(arr, i);
};

void write_in_players(people *arr, int n)
{
    FILE *f = fopen("players.txt", "w");

    // if there was an error
    if (f == NULL)
        perror("Error opening file"); // print error
    // if there was no error
    else
        for (int i = 0; i <= n; i++)
            fprintf(f, "%s %d %d\n", arr[i].name, arr[i].last_score, arr[i].high_score);

    fclose(f);
}

int get_last_score_of_leader_board()
{
    FILE *f = fopen("leader_board.txt", "r");
    people arr[LEADER_BOARD_TOP];
    int total = 0;
    // ------- READ ENTRIES ------------
    for (; fscanf(f, "%s %d",
                  arr[total].name,
                  &(arr[total].high_score)) > 0;
         total++)
        ;

    fclose(f);
    int last_high_score = arr[total - 1].high_score;
    return last_high_score;
}

void add_msg(char msg[])
{
    strcpy(msgs_arr[total_msgs], msg);
    total_msgs++;
}

int already_present_people(people *arr, int total, char *given_name)
{
    for (int i = 0; i < total; i++)
    {
        if (strcmp(arr[i].name, given_name) == 0)
            return i;
    }
    return -1;
}

// display all msgs stored in arr_msgs
void display_msgs()
{
    // printf("display all the msgs..........................");
    // getch();
    for (int i = 0; i < total_msgs; i++)
    {
        GotoXY(BOX_X_START + 10, BOX_Y_END + 3 + (i * 2));
        printf("%s", msgs_arr[i]);
    }
}

