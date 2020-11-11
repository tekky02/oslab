/*
 * @Author: tekky 
 * @Date: 2020-11-11 13:15:45 
 * @Last Modified by: tekky
 * @Last Modified time: 2020-11-11 15:38:03
 */

#include "fileprinter.hh"

int main(int argc, char const *argv[])
{
    std::string path = ".";
    if (argc == 2) {
        path = argv[1];
    }
    FilePrinter demo(path);
    demo.run();
    return 0;
}
