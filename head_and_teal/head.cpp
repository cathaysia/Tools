#include <iostream>
#include <thread>
#include <fstream>
#include <windows.graphics.h>
#include "common.h"
using namespace std;

const char* helper =\
R"(head ：输出文件内容的前几行（默认为 10 行）
例如：
head C:\1.txt
head -n 20 C:\1.txt
cat C:\1.txt | head -n 20)";

void readStdin(int line){
    string buf;
    for (int i = 0; i < line && cin.good(); ++i) {
        getline(cin, buf);
        cout<<buf<<endl;
    }
}

int main(int argc, char** argv) {
//    setlocale(LC_ALL, "");
    auto para = Parse(argc, argv, helper);
    if( !para.second || (para.second.get() == "-") ){
        // 若第二个参数为空
        readStdin(para.first);
        return 0;
    }
    ifstream inf(para.second.get());
    string buf;
    for (int i = 0; i < para.first && inf.good(); ++i) {
        getline(inf, buf);
        cout<<buf<<endl;
    }


    return 0;
}