#include "file.h"

File::File()
{
    cout << "file" << endl;
}
//打开文件
bool File::file_open(string name)
{
    infile.open(name,ios::out|ios::app);

    return infile.is_open();
}
//写入文件
bool File::file_write(string text)
{
    infile << text << endl;
    return true;
}

//关闭文件
bool File::file_close()
{
    infile.close();
    return true;
}


//析构函数
File::~File()
{
    infile.close();
}