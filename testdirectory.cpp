#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>      // std::istringstream
#include <math.h>
#include <ctime>
#include "HashTable.h"
#include <filesystem>
#include <stdio.h>
#include <dirent.h>


int main () {
    if (__cplusplus == 201703L) std::cout << "C++17\n";
    else if (__cplusplus == 201402L) std::cout << "C++14\n";
    else if (__cplusplus == 201103L) std::cout << "C++11\n";
    else if (__cplusplus == 199711L) std::cout << "C++98\n";
    else std::cout << "pre-standard C++\n";
    
    struct dirent *entry = nullptr;
    DIR *dp = nullptr;

    dp = opendir("c:\\programs\\CS410_Pres_raw");
    if (dp != nullptr) {
        while ((entry = readdir(dp)))
            printf ("%s\n", entry->d_name);
    }

    closedir(dp);

    return 0;
}