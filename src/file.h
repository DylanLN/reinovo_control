#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
//cpp读写yaml
#include <yaml-cpp/yaml.h>

using namespace std;


class File
{
public:
    File();
    ~File();
    bool file_write(string text);
    bool file_open(string name);
    bool file_close();

private:
    string filename;
    ifstream infile;
};

class Yaml
{
public:
    Yaml();
    ~Yaml();
    bool file_write(string text);
    string file_read();
    bool file_open(string name);
    bool file_close();

private:
    string filename;
    ifstream infile;
};

#endif //FILE_H
