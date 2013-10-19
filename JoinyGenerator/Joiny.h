#ifndef JOINY_H
#define JOINY_H
#include "FGenerator.h"
#include "Binary.h"

#include "Flow/JoinyPair.h"
JoinyTask flowToJoiny(const FlowTask& task, const unsigned int colors);
JoinyTask flowToJoinyStaightforward(const FlowTask& task);
JoinyInfo solveJoiny(const JoinyTask& task, const unsigned int width, const unsigned int height);





#endif // JOINY_H
