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
    ofstream infile;
};

class Yaml
{
public:
    Yaml(){
    }
    ~Yaml(){
    }
    //写
    bool file_write()
    {
        ofstream fout(filename);
        fout << config;
        fout.close();
    }

    YAML::Node load_file(string name){
        filename = name;
        config = YAML::LoadFile(filename);
        return config;
    }
public:
    YAML::Node config;

private:
    string filename;
    // ofstream fout;
    // ifstream infile;
};

#endif //FILE_H
