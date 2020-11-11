/*
 * @Author: tekky 
 * @Date: 2020-11-11 12:59:34 
 * @Last Modified by: tekky
 * @Last Modified time: 2020-11-11 14:53:49
 */

#ifndef __FILEPRINTER_HH__
#define __FILEPRINTER_HH__

#include <string>
#include <dirent.h>

class FilePrinter {

public:
    explicit FilePrinter(std::string const& path=".");
    ~FilePrinter();
    void run() const;
private:
    void print(std::string const&) const; // print file info recursively
    void fileInfo(std::string const& dir) const;
    std::string parseMode(mode_t mode) const;

private:
    std::string directory;
};

#endif /* __FILEPRINTER_HH__ */