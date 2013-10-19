#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <string>
#include <sstream>
class FileUtils
{
public:
    typedef std::string FileName;
    static bool openPackageFile(const std::string& name, std::stringstream &out);

    static const FileName getPackageFilePath(const std::string& name);
    static const FileName getStorageFilePath(const std::string& name);
    static bool isFileExists(const FileName& name);
private:
    FileUtils();
};

#endif // FILEUTILS_H
