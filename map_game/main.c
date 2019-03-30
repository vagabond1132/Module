#include <curses.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>


#define GRASS ' '
#define EMPTY '.'
#define WATER '~'
#define MOUNTAIN  '$'
#define PLAYER_UP '^'
#define PLAYER_DOWN ''
#define PLAYER_LEFT '>'
#define PLAYER_RIGHT '<'
#define BULLET '*'


int is_move_okay(int y, int x);

void draw_map(void);

int main(void)
{
    int y, x;
    int ch;
    /* 初始化curses */
    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();
    clear();
    /* 初始化探索地图 */
    draw_map();
    /* 在左下角初始化玩家 */
    y = LINES - 1;
    x = 0;
    do
    {
        /* 默认获得一个闪烁的光标--表示玩家字符 */
        mvaddch(y, x, PLAYER_UP);
        move(y, x);
        refresh();
        ch = getch();
        /* 测试输入的键并获取方向 */
        switch (ch)
        {
        case KEY_UP:
        case 'w':
        case 'W':
            if ((y > 0) && is_move_okay(y - 1, x))
            {
                //mvaddch(y, x, EMPTY);
                mvaddch(y, x, GRASS);
                y = y - 1;
            }
            break;
        case KEY_DOWN:
        case 's':
        case 'S':
            if ((y < LINES - 1) && is_move_okay(y + 1, x))
            {
                //mvaddch(y, x, EMPTY);
                mvaddch(y, x, GRASS);
                y = y + 1;
            }
            break;
        case KEY_LEFT:
        case 'a':
        case 'A':
            if ((x > 0) && is_move_okay(y, x - 1))
            {
                //mvaddch(y, x, EMPTY);
                mvaddch(y, x, GRASS);
                x = x - 1;
            }
            break;
        case KEY_RIGHT:
        case 'd':
        case 'D':
            if ((x < COLS - 1) && is_move_okay(y, x + 1))
            {
                //mvaddch(y, x, EMPTY);
                mvaddch(y, x, GRASS);
                x = x + 1;
            }
            break;
        case 32: //绘制子弹;;


            break;
        }
    }
    while ((ch != 'q') && (ch != 'Q'));
    endwin();

    return 0;
}

int is_move_okay(int y, int x)
{
    int testch;
    /* 当空间可以进入时返回true */
    testch = mvinch(y, x);
    return ((testch == GRASS) || (testch == EMPTY));
}

void draw_map(void)
{
    int y, x;
    /* 绘制探索地图 */
    /* 背景 */
    for (y = 0; y < LINES; y++)
    {
        mvhline(y, 0, GRASS, COLS);
    }
    /* 山脉和山道 */
    for (x = COLS / 2; x < COLS * 3 / 4; x++)
    {
        mvvline(0, x, MOUNTAIN, LINES);
    }
    mvhline(LINES / 4, 0, GRASS, COLS);
    /* 湖 */
    for (y = 1; y < LINES / 2; y++)
    {
        mvhline(y, 1, WATER, COLS / 3);
    }
}
