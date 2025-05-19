#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <vector>

namespace fs = std::filesystem;

namespace my_random {
    void clear();
    void feed(std::string input);
    void resize();
    void run(std::string input);
    std::string tema();
}
