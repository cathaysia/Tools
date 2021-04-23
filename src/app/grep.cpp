//
// Created by 31951 on 2020/10/22.
//
/**
 *  现在 grep 只考虑从 stdout 里读取数据
 *
 *  目前只实现 grep -i, grep -v, grep + regex
 *
 */
#include <iostream>
#include <cstring>
#include <regex>

using namespace std;

const char *helper = \
R"(grep ：过滤重定向的内容
例如：
cat 1.txt | grep s
cat 1.txt | grep -i s
使用 egrep 风格的正则表达式进行过滤
-i 大小写不敏感
-v 反向选择)";

int main(int argc, char **argv) {
    bool bit_exclude = false; // -v
    bool bit_sensitive = false; // -i 默认为大小写敏感
    string regex_str;

    // 判断参数
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help") || !strcmp(argv[i], "/h")) {
            cout << helper << endl;
            return 0;
        } else if (!bit_exclude && !strcmp(argv[i], "-v"))
            bit_exclude = true;
        else if (!bit_sensitive && !strcmp(argv[i], "-i"))
            bit_sensitive = true;
        else
            regex_str = argv[i];
    }
    // 构造正则表达式
    regex *pattern = nullptr;
    if (!bit_sensitive) pattern = new regex(regex_str, regex::egrep); // 大小写敏感
    else pattern = new regex(regex_str, regex::egrep | regex::icase);

    istream *fs = &cin;
    string tmp;
    smatch math_result;
    // 进行解析
    while (!fs->eof()) {
        getline(*fs, tmp);
        regex_search(tmp, math_result, *pattern);
        if (!(math_result.empty() ^ bit_exclude)) cout << tmp << endl; // empty 同或 exclude
    }

    return 0;
}
