//#include <iostream>
#include "proc/processInfo.h"
#include "proc/memoryInfo.h"
#include "utility/outputFormatting.h"
#include "utility/sorting.h"
#include "curses.h"
#include <exception>

const char *BASE= "/proc";

int main()
{
    initscr();
    start_color();
    timeout(10000);
    std::vector<ProcessInfo> info = getProcessesInfo();
    long totalMemory = getTotalMemory();

    char c{0};
    sortBy sorting{sortBy::NONE};

    init_pair(1, COLOR_BLACK, COLOR_CYAN);
    while(c != 'q')
    {
        info = getProcessesInfo();

        wclear(stdscr);
        printw("Sorting by - PID: \'p\' - CPU: \'c\' - MEM: \'m\'\nTo exit press - \'q\'\n");
        attron(COLOR_PAIR(1));
        printw("%s", formatHeader().c_str());
        attroff(COLOR_PAIR(1));

        int y = getmaxy(stdscr);

        printw("%s", formatProcessTable(info, totalMemory, y, sorting).c_str());
        
        refresh();

        c = getch();
        sorting = getSortingByChar(c);
    }

    endwin();
    return 0;
}