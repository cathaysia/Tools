//
// Created by 31951 on 2020/8/17.
//

#ifndef HEAD_COMMON_H
#define HEAD_COMMON_H
#include <iostream>
#include <string>
#include <memory>
#include <utility>
using namespace std;

pair<int, shared_ptr<char> > Parse(int argc, char **argv, const char *helper) {
    int line = 10;
    char *path = nullptr;
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            cout << helper << endl;
            exit(EXIT_SUCCESS);
        } else if (!strcmp(argv[i], "-n")) {
            if (i < argc - 1) {
                line = stoi(argv[++i]);
            }
        } else {
            size_t len = strlen(argv[i]) + 1;
            path = new char[len];
            strcpy_s(path, len, argv[i]);
        }
    }
    return std::make_pair(line, shared_ptr<char>(path));
}

#endif //HEAD_COMMON_H
