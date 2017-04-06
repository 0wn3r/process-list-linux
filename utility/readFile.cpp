#include "readFile.h"

#include <iostream>
#include <fstream>
#include <exception>

void readFile(std::string filename, std::stringstream &stream)
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
    {
        std::cerr << filename << " cannot load" << std::endl;
        throw std::exception();
    }

    stream << file.rdbuf();
    file.close();
}