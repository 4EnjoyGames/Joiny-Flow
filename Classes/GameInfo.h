#ifndef GAMEINFO_H
#define GAMEINFO_H
#include "vector"
//#include "RWGeneric.h"
#include "Logic/JoinyCollection.h"

class GameInfo
{
   public:
       static GameInfo* getInstance( );

       const std::vector<FlowPoint>& getFlowPoints();
       const std::vector<std::string>& getCollectionNames();
       const std::vector<ccColor3B>& getCollectionColors();

       const ccColor3B& getCloseColor();
   private:
       GameInfo( );
       void setInfo();

       GameInfo(GameInfo const&);              // don't Implement
       void operator=(GameInfo const&); // don't implement
       static GameInfo* instance;

       std::vector<FlowPoint> _flow_points;
       std::vector<std::string> _coll_names;
       std::vector<ccColor3B> _colors;
       unsigned int _level_number;

       ccColor3B _close_color;
};
#endif // GAMEINFO_H
