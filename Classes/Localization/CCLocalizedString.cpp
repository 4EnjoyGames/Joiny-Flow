#include "cocos2d-A.h"
#include "CCApplication.h"
#include "Core/FileUtils.h"
#include <fstream>
#include "Logic/RWGeneric.h"
//#include "CCCommon.h"
//#include "CCFileUtils.h"
//#include "CCDirector.h"
//#include "CCString.h"
USING_NS_CC;

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <memory>
#include "Logic/LevelManager.h"
using namespace std;

#include "CCLocalizedString.h"

static map<std::string,std::string> localizedStrings;

const char * CCLocalizedString(const char * mKey,const char * mComment)
{ 
    ccLanguageType curLanguage = CCApplication::sharedApplication()->getCurrentLanguage();
    const char * fileName;
    switch (curLanguage)
    {
        case kLanguageEnglish:
            fileName = "Localized_en";
            break;
        case kLanguageChinese:
            fileName = "Localized_zh";
            break;
        /**
         case for more localize
         */
        default:
            fileName = "Localized_en";
            break;
    }

    std::string resultStr="";
    //unsigned long fileSize = 0;
    unsigned char * fileContents = NULL;
    string line="";
    //string fullPath="";
    string contents="";
    unsigned long fileSize = 0;

    std::string fullPath = FileUtils::getStorageFilePath(fileName);
    if(FileUtils::isFileExists(fullPath))
    {

        fileContents = CCFileUtils::sharedFileUtils()->getFileData( fullPath.c_str( ) , "r", &fileSize );
        contents.append( ( char * ) fileContents );

        istringstream fileStringStream( contents );
        while ( std::getline( fileStringStream, line ) )
        {
//            debug_Printf("line :%s",line.c_str());
            if (line.find("/*",0) == string::npos &&
                line.find("//",0) == string::npos &&
                line.find("*/",0) == string::npos )
                //filter the valid string of one line
            {
//                debug_Printf("find the line not start with \" /* \"");
                std::string::size_type validPos= line.find('=',0);
                if ( validPos != string::npos)
                {
//                    debug_Printf("fimd the line contain \" = \"");
                    std::string keyStr = line.substr(0,validPos-1);
                    // get valid string
                    std::string subStr = line.substr(validPos+1,line.size()-1);

                    //trim space
                    keyStr.erase(0, keyStr.find_first_not_of(" \t"));
                    keyStr.erase(keyStr.find_last_not_of(" \t") + 1);

                    subStr.erase(0, subStr.find_first_not_of(" \t"));
                    subStr.erase(subStr.find_last_not_of(" \t") + 1);

                    //trim \"
                    keyStr.erase(0, keyStr.find_first_not_of("\""));
                    keyStr.erase(keyStr.find_last_not_of("\"") + 1);

                    subStr.erase(0, subStr.find_first_not_of("\""));


                    //trim ; character and last \" character
                    subStr.erase(subStr.find_last_not_of(";") + 1);
                    subStr.erase(subStr.find_last_not_of("\"") + 1);


                    //replace line feed with \n
                    string::size_type pos(0);
                    string old_value("\\n");
                    if((pos=subStr.find(old_value))!=string::npos)
                    {
                        for(; pos!=string::npos; pos+=1)
                        {
                            if((pos=subStr.find(old_value,pos))!=string::npos)
                            {
                                subStr.erase(pos, 2);
                                subStr.insert(pos, 1, '\n');
                            }
                            else
                                break;
                        }
                    }

                    localizedStrings.insert(std::pair<std::string, std::string>(keyStr,subStr));

                    const char * keyChars = keyStr.c_str();

                    if (strcmp(keyChars,mKey) == 0) {
                        resultStr = subStr;
                    }
                }
            }
        }
        //must delete fileContents
        if (fileContents!= NULL) {
            delete [] fileContents;
            fileContents = NULL;
        }

        //return valid value
        if (resultStr.length() > 0){
            char* res = ((char*)resultStr.c_str());
            return res;
        }
        return mKey;
    }
    else
    {
        std::map<std::string,std::string>::iterator itr = localizedStrings.find(std::string(mKey));
        if (itr != localizedStrings.end()) {
            return (itr->second).c_str();
        }
    }
    return mKey;
}
