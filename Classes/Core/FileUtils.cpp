#include "FileUtils.h"
#include "cocos2d.h"
#include <sstream>
FileUtils::FileUtils()
{
}

bool FileUtils::openPackageFile(const std::string& name, std::stringstream& out)
{
    FileName fullPath = getPackageFilePath(name);
    unsigned long size = 0;
    unsigned char* file_cont = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullPath.c_str(), "r", &size);

    if(size == 0)
        return false;

//    std::ios_base::fmtflags mode = std::ios::binary;
//    out.setf(mode);
    out.write((char*)file_cont, size);
    return true;
}

const FileUtils::FileName FileUtils::getPackageFilePath(const std::string& name)
{
    return cocos2d::CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(name.c_str());
}

const FileUtils::FileName FileUtils::getStorageFilePath(const std::string &name)
{
    return cocos2d::CCFileUtils::sharedFileUtils()->getWriteablePath() + name;
}

bool FileUtils::isFileExists(const FileName& name)
{
    FILE *fp = fopen(name.c_str(), "r");
    bool bRet = false;

    if (fp)
    {
        bRet = true;
        fclose(fp);
    }

    return bRet;
}
