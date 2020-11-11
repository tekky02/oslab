/*
 * @Author: tekky 
 * @Date: 2020-11-11 13:22:42 
 * @Last Modified by: tekky
 * @Last Modified time: 2020-11-11 15:34:34
 */

#include <iostream>
#include <iomanip> // put_time
#include <ctime>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "fileprinter.hh"

FilePrinter::FilePrinter(std::string const& path) 
    : directory(path)
{
}

FilePrinter::~FilePrinter() 
{
}

void FilePrinter::run() const {
    print(directory);
}

void FilePrinter::print(std::string const& path) const {
    struct dirent * entry = nullptr;
    DIR * dir = opendir(path.c_str());
    while ((entry = readdir(dir))) {
        if (entry->d_name[0] == '.') continue;
        std::string tempdir = path + "/" + entry->d_name;
        if (entry->d_type == DT_DIR) {
            // recursively print directories.
            std::cout << tempdir << ":\n";
            print(tempdir);
        } else if (entry->d_type == DT_REG) {
            // print detail information for regular files.
            fileInfo(tempdir);
        }
    }
    closedir(dir);
}

void FilePrinter::fileInfo(std::string const& dir) const {
    struct stat buf;
    lstat(dir.c_str(), &buf);
    std::cout << buf.st_mode << "\t" << buf.st_uid << " " << buf.st_gid << " "
              << buf.st_size << "\t" 
              << std::put_time(std::localtime(&buf.st_birthtimespec.tv_sec), "%b %d %R") 
              << " "
              << dir.substr(dir.find_last_of('/') + 1) << "\n";
}