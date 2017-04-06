#pragma once

#include <iostream>
#include <string>
#include "../proc/processInfo.h"
#include "../proc/memoryInfo.h"
#include <iomanip>
#include <algorithm>
#include "sorting.h"

std::string formatHeader();

std::string formatProcessTable(std::vector<ProcessInfo> &info, long totalMemory, int y, sortBy sorting);