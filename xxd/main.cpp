/* Created by 31951 on 2020/8/19.
 * ASCII范围 http://ascii.911cha.com/
 */

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const size_t LENGTH = 16; // 每行输出 16 个字节
const size_t SLICE = 2; // 每两个字节分到一组
const unsigned char FALLBACK = '.'; // 若字符不是 ASCII 字符，输出此字符

const char *helper = \
R"(xxd：将文件以 16 进制打印出来
用法：xxd -h [filename]

例如：
xxd -h
xxd 1.txt
cat 1.txt | xxd)";

inline const char *left();

inline string cent(const char *begin, const char *end);

inline string right(const char *begin, const char *end);

inline void output(const char *data, size_t len);

inline void out(string &data);

int main(int argc, char **argv) {
    string filename;
    for (int i = 1; i < argc; ++i) {
        // 解析参数
        if (!strcmp(argv[i], "-h") | !strcmp(argv[i], "--help")) {
            cout << helper << endl;
            return 0;
        }
        filename = argv[i];
    }
    string buf;
    string data;
    if (filename.empty()) {
        while (getline(cin, buf)) data.append(buf);
        out(data);
    } else {
        fstream fs(filename);
        if (!fs.is_open()) return 0;
        while (getline(fs, buf)) data.append(buf);
        out(data);
    }
}


const char *left() {
    static unsigned *number = nullptr; // 用来保存状态
    char *buf = new char[LENGTH / 2 + 1];
    if (!number) {
        number = new unsigned(0);
        sprintf_s(buf, LENGTH / 2 + 1, "%08x", *number);
        return buf;
    }
    *number += LENGTH;
    sprintf_s(buf, LENGTH / 2 + 1, "%08x", *number);
    return buf;
}

string cent(const char *begin, const char *end) {
    static char buf[LENGTH * 2];
    for (int i = 0; i < LENGTH && (begin + i != end); ++i) {
        sprintf_s((buf + i * 2), SLICE + 1, "%x", *(begin + i));
    }
    static string str;
    str.clear();
    for (int i = 0; i < LENGTH * 2; ++i) {
        str += (buf[i]);
        if (!((i + 1) % (SLICE * 2))) str.append(" ");
    }
    return str;
}

string right(const char *begin, const char *end) {
    static string str;
    str.clear();
    for (int i = 0; i < LENGTH && (begin + i != end); ++i) {
        str += begin[i];
    }
    return str;
}

void output(const char *data, size_t len) {
    string str(data);
    cout << left() << ": ";
    cout << cent(data, data + min(LENGTH, len)) << "  ";
    cout << right(data, data + min(LENGTH, len)) << endl;
}

void out(string &data) {
    for (size_t i = 0; i < data.length(); ++i) { // 格式化字符串
        if (data[i] < 32 || data[i] > 126) data[i] = FALLBACK;
    }
    for (size_t i = 0; i < data.length(); i += LENGTH) {
        if (i + LENGTH >= data.length()) {
            output(data.c_str() + i, data.length() - i);
            continue;
        }
        output(data.c_str() + i, LENGTH);
    }
}
