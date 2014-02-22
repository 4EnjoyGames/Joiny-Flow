#include <iostream>
#include "StepsGenerator.h"
#include <Windows.h>
using namespace std;

int main()
{
    if(!SetCurrentDirectoryA("../../JoinyGenerator/levels"))
        return 1;
    //GenerateLevels(8,100,2,4);
    SaveCollection("collection_8x8_1.txt","puzzle_8x8_1.ad");
    SaveCollection("collection_8x8_2.txt","puzzle_8x8_2.ad");

    return 0;
}

