#include <iostream>
#include "StepsGenerator.h"
#include <Windows.h>
using namespace std;

int main()
{
    if(!SetCurrentDirectoryA("../../JoinyGenerator/levels"))
        return 1;
    GenerateLevels(5,120,2,4);
    SaveCollection("collection_5x5_1.txt","puzzle_5x5_1.ad");
    SaveCollection("collection_5x5_2.txt","puzzle_5x5_2.ad");

    return 0;
}

