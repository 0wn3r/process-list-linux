#pragma once

enum class sortBy
{
    PID,
    CPU,
    MEM,
    NONE
};

sortBy getSortingByChar(char c);