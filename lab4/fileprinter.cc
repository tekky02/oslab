/*
 * @Author: tekky
 * @Date: 2020-11-11 13:22:42
 * @Last Modified by: tekky
 * @Last Modified time: 2020-11-11 15:34:34
 */

#include "fileprinter.hh"
#include <ctime> // localtime...
#include <dirent.h>
#include <grp.h>   // get group name from gid
#include <iomanip> // put_time
#include <iostream>
#include <pwd.h>   // get username from uid
#include <sstream> // for stringstream class.
#include <sys/stat.h>
#include <unistd.h>

FilePrinter::FilePrinter(std::string const &path) : directory(path) {}

FilePrinter::~FilePrinter() {}

void FilePrinter::run() const { print(directory); }

void FilePrinter::print(std::string const &path) const {
  struct dirent *entry = nullptr;
  DIR *dir = opendir(path.c_str());
  while ((entry = readdir(dir))) {
    if (entry->d_name[0] == '.')
      continue;
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
  std::cout << std::endl;
  closedir(dir);
}

void FilePrinter::fileInfo(std::string const &dir) const {
  struct stat buf;
  lstat(dir.c_str(), &buf);
  std::cout << parseMode(buf.st_mode) << " " << std::setw(2) << std::right
            << buf.st_nlink << " " << getpwuid(buf.st_uid)->pw_name << " "
            << getgrgid(buf.st_gid)->gr_name << " " << std::setw(8)
            << std::right << buf.st_size * 1.0 / 1024 << "KB "
#if __macos__
            << std::put_time(std::localtime(&buf.st_mtimespec.tv_sec),
                             "%b %d %R")
            << " "
#elif __linux__
            << std::put_time(std::localtime(&buf.st_ctim.tv_sec), "%b %d %R")
            << " "
#endif
            << dir.substr(dir.find_last_of('/') + 1) << "\n";
}

std::string FilePrinter::parseMode(mode_t mode) const {
  std::stringstream modstr;

  modstr << (S_ISDIR(mode) ? 'd' : '-') << (mode & S_IRUSR ? 'r' : '-')
         << (mode & S_IWUSR ? 'w' : '-') << (mode & S_IXUSR ? 'x' : '-')
         << (mode & S_IRGRP ? 'r' : '-') << (mode & S_IWGRP ? 'w' : '-')
         << (mode & S_IXGRP ? 'x' : '-') << (mode & S_IROTH ? 'r' : '-')
         << (mode & S_IWOTH ? 'w' : '-') << (mode & S_IXOTH ? 'x' : '-');
  return modstr.str();
}