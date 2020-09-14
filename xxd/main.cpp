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

һ��һ���� 8*4 ��16������, һ���� 4*8*4 = 128 λ
�ֽ���һ���� 128/8 = 16 ���ֽ�
 ���м䲿���У�ÿ 2 ���ֽ����һ�飬һ��8��

 */

const size_t LENGTH = 16; // ÿ����� 16 ���ֽ�
const unsigned char FALLBACK = '.'; // ���ַ����� ASCII �ַ���������ַ�

const char *helper = \
R"(xxd�����ļ��� 16 ���ƴ�ӡ����
�÷���xxd -h [filename]

���磺
xxd -h
xxd 1.txt
cat 1.txt | xxd)";

void left();

void center(const char *);

void right(const char *);

int main(int argc, char **argv) {
    string filename;
    for (int i = 1; i < argc; ++i) {
        // ��������
        if (!strcmp(argv[i], "-h") | !strcmp(argv[i], "--help")) {
            cout << helper << endl;
            return 0;
        }
        filename = argv[i];
    }
    // ��ȡ������
    istream *is = nullptr;
    if (filename.empty()) is = &cin;
    else is = new ifstream(filename, ios::in | ios::binary);
    // ���쳣
    is->exceptions(is->exceptions() | ios::failbit | ios::badbit);
    // ����һ��һ�еؽ��н���
    char buf[LENGTH];
    try {
        while (is->get(buf, LENGTH)) {
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
    // һ����������ֽڣ�һ��16���ֽڣ���Ҫ���8��
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