#include <curses.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include "hit_mouse.h"

#define MAX_LEN  128


static unsigned int pos_x = 0;
static unsigned int count = 0;
static char grade[MAX_LEN]; 
static char rand_ch = 0;


void PrintTitle()
{
    mvaddstr(4, 20, GAME_NAME);
    mvaddstr(6, 30, VERSION);
    mvaddstr(8, 7, KEY_INFO_1);

}

void PrintRanking()
{
    char *info = "Rank:";
    char *info_1 = "1    EveryOne   1024";
    char *info_2 = "2    ep         424";
    char *info_3 = "3    qwer       234";
    char *info_4 = "4    zazzz      14";
    int i ;


    for (i = 0 ; *info != '\0'; info++, i++)
        if ( mvaddch(7, i + 7, *info) == ERR)
            return ;

    for (i = 0 ; *info_1 != '\0'; info_1++, i++)
        if ( mvaddch(8, i + 10, *info_1) == ERR)
            return ;

    for (i = 0 ; *info_2 != '\0'; info_2++, i++)
        if ( mvaddch(9, i + 10, *info_2) == ERR)
            return ;


    for (i = 0 ; *info_3 != '\0'; info_3++, i++)
        if ( mvaddch(10, i + 10, *info_3) == ERR)
            return ;

    for (i = 0 ; *info_4 != '\0'; info_4++, i++)
        if ( mvaddch(11, i + 10, *info_4) == ERR)
            return ;

}


void PrintControl()
{
    char *ctl = "Start";
    int i = 0;
    for ( ; *ctl != '\0' ; ctl++, i++)
        if ( mvaddch(20, i + 48, *ctl) == ERR)
            return ;

}

void PrintHole()
{
    int i = 0;
    for ( ; i < 100; i = i + 2)
    {
         mvaddch(40, i, '-');
         mvaddch(40, i, '-');
    }

}


void* PrintMouse(void *arg)
{
    for ( ; ; )
    {

        srand( (unsigned)time( NULL ) );
        pos_x = rand() % 40;
        rand_ch = rand() % 26 + 97;

        mvaddch(39, pos_x, '^');
        mvaddch(40, pos_x, rand_ch);
        mvaddch(41, pos_x, 'Q');

        count++;

        refresh();
        usleep(1500000);
    }

    return 0;
}


int main(int argc, char *argv[])
{

    char ch = 0;
    int num = 0;
    pthread_t tid = 0;
    initscr(); //初始化屏幕;;
    signal(SIGINT, SIG_IGN);
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    leaveok(stdscr, TRUE);
    scrollok(stdscr, FALSE);


    //Parall Control ;
    PrintTitle(GAME_NAME);
    
//    PrintRanking();
//
//    PrintHole();
//    PrintControl();
    
    refresh();

    if ( pthread_create(&tid, NULL, &PrintMouse, NULL) )
    {
        endwin();
        return 1;
    }


    for ( ; ; )
    {
        ch = getch();


        if ( 27 == ch) 
            break;

        if ( rand_ch == ch )
        {
            sprintf(grade, "%u/%u", num, count);
            mvaddstr(12, 1,  grade); //使用;

            mvaddch(39, pos_x, ' ');
            mvaddch(40, pos_x, ' ' );
            mvaddch(41, pos_x, ' ');


            num++;

            refresh();

        }

        refresh();

    }


    mvcur(0, COLS - 1, LINES - 1, 0);
    endwin();

    return 0;
}
