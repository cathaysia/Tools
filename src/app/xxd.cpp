#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <sstream>
#include <cstring>

using namespace std;

/*
00000000: 8fff 7cf9 0100 0000 0100 0000 1000 0000  ..|.............
00000010: 8dff 7cf9 0f00 0000 5345 204c 696e 7578  ..|.....SE Linux
00000020: 204d 6f64 756c 6502 0000 0014 0000 0001   Module.........
00000030: 0000 0008 0000 0000 0000 0008 0000 006d  ...............m
00000040: 792d 6e67 696e 7803 0000 0031 2e30 4000  y-nginx....1.0@.
00000050: 0000 0000 0000 0000 0000 0000 0000 0000  ...............

一行一共是 8*4 个16进制数, 一共是 4*8*4 = 128 位
字节数一共是 128/8 = 16 个字节
 在中间部分中，每 2 个字节组成一组，一共8组

 */

const size_t LENGTH = 16; // 每行输出 16 个字节
const unsigned char FALLBACK = '.'; // 若字符不是 ASCII 字符，输出此字符

const char *helper = \
R"(xxd：将文件以 16 进制打印出来
用法：xxd -h [filename]

例如：
xxd -h
xxd 1.txt
cat 1.txt | xxd)";

void left();

void center(const char *);

void right(const char *);

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
    // 获取输入流
    istream *is = nullptr;
    if (filename.empty()) is = &cin;
    else is = new ifstream(filename, ios::in | ios::binary);
    // 打开异常
    is->exceptions(is->exceptions() | ios::failbit | ios::badbit);
    // 现在一行一行地进行解析
    char buf[LENGTH];
    try {
        while (!is->eof()) {
            for (int i = 0; i < 8 && !is->eof(); ++i){
                buf[i] = is->get();
            }
            left();
            center(buf);
            right(buf);
        }
    } catch (exception &e) {
        cout << endl << "error:" << e.what() << endl;
    }
    auto hand = dynamic_cast<ifstream *>(is);
    if (hand) hand->close();
    return 0;
}

void left() {
    static size_t start = 0;
    printf("%08zx: ", start);
    start += LENGTH;
}

void center(const char *str) {
    uint16_t buf = 0;;
    uint8_t *buf_mul = (uint8_t *) &buf;
    // 一次输出两个字节，一共16个字节，需要输出8次
    for (int i = 0; i < 16; i += 2) {
        buf_mul[0] = str[i];
        buf_mul[1] = str[i + 1];
        printf("%04x ", buf);
    }
}

void right(const char *str) {
    static char buf[LENGTH + 1];
    buf[LENGTH] = '\0';
    memcpy_s(buf, LENGTH + 1, str, LENGTH);
    for (int i = 0; i < LENGTH; ++i)
        if (buf[i] > 126 || str[i] < 32)
            buf[i] = FALLBACK;
    printf("%s\n", buf);
}