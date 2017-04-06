#include "sorting.h"

sortBy getSortingByChar(char c)
{
    static char sortingPreserver = 'c';
    if (c == 'p' || c == 'c' || c == 'm')
    {
        sortingPreserver = c;
    }
    switch (sortingPreserver)
    {
        case 'p':
            return sortBy::PID;
        case 'c':
            return sortBy::CPU;
        case 'm':
            return sortBy::MEM;
    }

    return sortBy::NONE;
}