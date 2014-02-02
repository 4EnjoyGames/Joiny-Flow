#include "cocos2d-A.h"
#include "CCApplication.h"
#include "Core/FileUtils.h"
#include "Logic/RWGeneric.h"
#include "Logic/LevelManager.h"
#include "CCLocalizedString.h"
#include "ADLib/Device/ADLanguage.h"
using namespace std;

USING_NS_CC;

#define LE_MAGIC 0x950412de
#define BE_MAGIC 0xde120495


static map<std::string,std::string> localizedStrings;

uint32_t swap_le(const char *buf, size_t ofs)
{
    return *((uint32_t *)(buf + ofs));
}

uint32_t swap_be(const char *buf, size_t ofs)
{
    return *((uint32_t *)(buf + ofs));
}

void parse_file(const char *filename)
{
    unsigned char * fileContents = NULL;
    unsigned long fileSize = 0;
    fileContents = CCFileUtils::sharedFileUtils()->getFileData( filename,
                                                                "r",
                                                                &fileSize);
    if(fileContents != nullptr)
    {
        parse_data(reinterpret_cast<const char*>(fileContents),fileSize);
        delete[] fileContents;
    }
}



void parse_data(const char *data, size_t length)
{

    uint32_t (*swap)(const char *, size_t);
    if (((uint32_t *)data)[0] == LE_MAGIC)
    {
        swap = &swap_le;
    }
    else if (((uint32_t *)data)[0] == BE_MAGIC)
    {
        swap = &swap_be;
    }
    else
        CCLog("File reading error");

    uint32_t number_of_strings = swap(data, 8);
    uint32_t ofs_original = swap(data, 12);
    uint32_t ofs_translated = swap(data, 16);

    for (unsigned i = 0; i < number_of_strings; i++)
    {
        uint32_t original_len = swap(data, ofs_original + i * 8);
        uint32_t original_ofs = swap(data, ofs_original + 4 + i * 8);
        uint32_t translated_len = swap(data, ofs_translated + i * 8);
        uint32_t translated_ofs = swap(data, ofs_translated + 4 + i * 8);

        std::string original(&(data[original_ofs]), original_len);
        std::string translated(&(data[translated_ofs]), translated_len);
        localizedStrings[original] = translated;
    }
}

const char* CCLocalizedString(const string& key)
{
    static bool loaded = false;
    if(!loaded)
    {
        loaded = true;
        parse_file(ADLanguage::localizeFileName("Translation.mo").c_str());
    }
    auto it = localizedStrings.find(key);
    if(it == localizedStrings.end())
        return key.c_str();
    else
        return (it->second).c_str();
}
