#pragma once

#include "../utility/readFile.h"
#include "unistd.h"
#include <string>
#include <iostream>

unsigned long pagesToKB(unsigned long n);

long getTotalMemory();