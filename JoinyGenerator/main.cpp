#include <iostream>
#include "StepsGenerator.h"
#include <Windows.h>
using namespace std;

int main()
{
    if(!SetCurrentDirectoryA("../../JoinyGenerator/levels"))
        return 1;
    GenerateLevels(10,50,2,4);
    //SaveCollection("collection_9x9_1.txt","puzzle_9x9_1.ad");
    SaveCollection("collection_10x10_1.txt","puzzle_10x10_1.ad");

    return 0;
}

