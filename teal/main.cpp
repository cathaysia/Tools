//
// Created by 31951 on 2020/9/19.
//

#include <iostream>
#include <cstring>
#include <string>
#include <fstream>

using namespace std;

const char *helper = \
R"(head ：输出文件内容的后几行（默认为 10 行）
例如：
tail C:\1.txt
tail -n 20 C:\1.txt
cat C:\1.txt | tail -n 20)";


int main(int argc, char **argv) {
    int line = 10;
    const char *filename = nullptr;
    // 解析参数
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            cout << helper << endl;
            exit(EXIT_SUCCESS);

        } else if (!strcmp(argv[i], "-n") && i + 1 < argc) {
            line = stoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-n") != 0) {
            filename = argv[i];
        }
    }
    // 获取输入源
    istream *is = &cin;
    if (filename) {
        is = new ifstream(filename, ios::in);
        if (!is) exit(EXIT_FAILURE);
    };
    // 放置文件指针的位置
    is->seekg(-1, ios::end);
    for (int i = 0; i < line && (is->tellg() > 0); ++i) {
        while (is->peek() != '\n' && is->tellg() > 0) {
            is->seekg(-1, ios::cur);
        }
        if (is->tellg() > 0) is->seekg(-1, ios::cur);
    }
    // 打印数据
    string buf;

    for (int i = 0; i < line && getline(*is, buf); ++i) {
        cout << buf;
        if (i < line - 1) cout << endl;
    }
    // 资源清理
    if (is != &cin)
        dynamic_cast<ifstream *>(is)->close();

    return 0;
}