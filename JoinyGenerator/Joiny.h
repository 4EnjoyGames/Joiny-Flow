#ifndef JOINY_H
#define JOINY_H
#include "FGenerator.h"
#include "Binary.h"

#include "Flow/JoinyPair.h"
const unsigned int getColor(std::map<unsigned int,unsigned int>& mymap);
JoinyTask flowToJoiny(const FlowTask& task, const unsigned int colors);
JoinyTask flowToJoinyStaightforward(const FlowTask& task);
JoinyInfo solveJoiny(const JoinyTask& task, const unsigned int width, const unsigned int height);

typedef std::vector<unsigned int> Palete;
typedef std::map<unsigned int, std::vector<Palete> > AllPaletes;
static Palete _curr_palete;
bool operator==(const Palete& p1, const Palete& p2);
const Palete& getCurrPalete();

void JoinyColorSchemeInit();
const Palete& getColorScheme(unsigned int color_num);



#endif // JOINY_H
