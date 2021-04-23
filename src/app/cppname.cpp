#include <iostream>
#include <Utils.h>
#include <cstring>

using namespace std;

//enum STATUS {
//    SUCCESS = 0, // 成功
//    MEM_FAIL = -1, // 内存分配失败
//    VAILD = -2, // 无效名
//    ARGU_INVALID = -3, // 参数无效
//};

constexpr const char *msg = R"(=============================================================================================
显示可读的 C++ 类型名
使用方式: c++name [options] [mangled names]
options 可为:
    [-h]            显示帮助信息
    [--help]        显示帮助信息
例如:
    c++name i                        ==> int
    c++name P6Plugin                 ==> Plugin*
    c++name _ZNK4Json5ValueixEPKc    ==> Json::Value::operator[](char const*) const
=============================================================================================)";

int main(int argc, char **argv) {
    if (argc == 1) { // 若没有输入参数
        cout << msg << endl;
        exit(EXIT_SUCCESS);;
    }
    int status;
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            cout << msg << endl;
            continue;
        }
        const char* result = Utils::getFullName(argv[i]);
//        const char *result = abi::__cxa_demangle(argv[i], nullptr, nullptr, &status);
//        if (status) {
//            cout<<argv[i]<<": 输入的名称不合法!" << endl;
//            continue;
//        }
        cout<< result << endl;
    }
    return 0;
}
