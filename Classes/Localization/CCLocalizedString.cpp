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

std::string Localization (const char* key)
{
    //get info about main settings for collection
    std::string file_name = "Translation.en.po";
    std::ifstream infile(file_name);
    std::string line="";

    //first 13 lines is setting for proEdit
    for(unsigned int i=0; i<13; ++i)
    {
        getline(infile,line);
    }
    while(getline(infile,line))
    {
        getline(infile,line);

        std::string curr_line = line;
        if(curr_line.size()>0 && curr_line[0] != '#')
        {

            std::string msg="";
            std::string key="";

            istringstream is(line);
            is >> msg;
            is >> key;

            if(msg=="msgid")
            {
                //cut first and last symbol
                key = key.substr(1,key.size()-1);

                if(getline(infile,line))
                {
                    std::string msgstr="";
                    std::string value="";

                    istringstream is(line);
                    is >> msgstr;
                    is >> value;

                    if(msgstr=="msgstr")
                    {
                        //cut first and last symbol
                        value = value.substr(1,value.size()-1);

                        localizedStrings.insert(std::pair<std::string, std::string>(key,value));
                    }
                }
            }

        }
    }

    return "1";
}

std::string CCLocalizedString(const char * mKey)
{ 
    Localization("1");


    if(localizedStrings.empty())
    {
        ccLanguageType curLanguage = CCApplication::sharedApplication()->getCurrentLanguage();
        const char * fileName;
        switch (curLanguage)
        {
            case kLanguageEnglish:
                fileName = "Localized_en";
                break;
    //        case kLanguageUkrainian:
    //            fileName = "Localized_ua";
    //            break;
            /**
             case for more localize
             */
            default:
                fileName = "Localized_en";
                break;
        }

        std::string resultStr="";
        unsigned char * fileContents = NULL;
        string line="";
        string contents="";
        unsigned long fileSize = 0;
        fileContents = CCFileUtils::sharedFileUtils()->getFileData( fileName,
                                                                    "r",
                                                                    &fileSize);


        if(fileSize != 0)
        {

            contents.append( ( char * ) fileContents );
            istringstream fileStringStream( contents );
            while ( std::getline( fileStringStream, line ) )
            {
                if (line.find("/*",0) == string::npos &&
                    line.find("//",0) == string::npos &&
                    line.find("*/",0) == string::npos )
                {
                    std::string::size_type validPos= line.find('=',0);
                    if ( validPos != string::npos)
                    {
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

                        if (strcmp(keyChars,mKey) == 0)
                        {
                            resultStr = subStr;
                        }
                    }
                }
            }
            //must delete fileContents
            if (fileContents!= NULL)
            {
                delete [] fileContents;
                fileContents = NULL;
            }

            //return valid value
            if (resultStr.length() > 0)
            {
                return resultStr;
            }
            //return mKey;
            return resultStr;
        }
//        else
//        {
//            std::map<std::string,std::string>::iterator itr = localizedStrings.find(std::string(mKey));
//            if (itr != localizedStrings.end())
//            {
//                return (itr->second).c_str();
//            }
//        }
        //return mKey;
        return resultStr;
    }
    else
    {
        return localizedStrings.find(mKey)->second;
    }
}
