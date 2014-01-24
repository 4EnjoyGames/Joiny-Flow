#ifndef JOINY_H
#define JOINY_H
#include "FGenerator.h"
#include "Binary.h"

#include "Flow/JoinyPair.h"
const unsigned int getColor(std::map<unsigned int,unsigned int>& mymap);
JoinyTask flowToJoiny(const FlowTask& task, const unsigned int colors);
JoinyTask flowToJoinyStaightforward(const FlowTask& task);
JoinyInfo solveJoiny(const JoinyTask& task, const unsigned int width, const unsigned int height);


void JoinyColorSchemeInit();
std::map<unsigned int,unsigned int> getColorScheme(unsigned int color_num);
static std::map<unsigned int,unsigned int> _map5_1;

static std::map<unsigned int,unsigned int> _map4_1;
static std::map<unsigned int,unsigned int> _map4_2;

static std::map<unsigned int,unsigned int> _map3_1;
static std::map<unsigned int,unsigned int> _map3_2;

static std::map<unsigned int, unsigned int> _map2_1;



#endif // JOINY_H
