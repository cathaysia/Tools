//
// Created by 31951 on 2020/8/19.
//

#include <windows.h>
#include <iostream>
#include <optional>
#include <string>

using namespace std;

const char *helper = \
R"(xsel �����ݸ��Ƶ�ϵͳ����������Ƕ�����
-h \t ��ʾ��������
-o \t ����ϵͳ����������
���磺
cat 1.txt | xsel
xsel -o)";

// �������������а�
bool Copy2Clipboard(const char* data, const size_t len){
    if(OpenClipboard(nullptr)){
        EmptyClipboard();
        auto clipBuffer = GlobalAlloc(GMEM_DDESHARE, len + 1);
        auto buf = (char*)GlobalLock(clipBuffer); // ����������
        strcpy_s(buf, len + 1, data);
        GlobalUnlock(clipBuffer); // ����������
        SetClipboardData(CF_TEXT, clipBuffer);
        CloseClipboard();
        return true;
    }
    return false;
}

// �Ӽ��а���ȡ������
optional<char*> getTextFromClipboard(){
    if(OpenClipboard(nullptr)){
        auto hMem = GetClipboardData(CF_TEXT);
        char* lpStr;
        if(hMem){
            lpStr = (char*)GlobalLock(hMem);
            if(lpStr) GlobalUnlock(hMem);
        }
        CloseClipboard();
        return lpStr;
    }
    return nullopt;
}

template<typename T>
void setClipboard(T t){ // t ��һ����������Ϊ string �� callable ����
    string data = t();
    auto bak = getTextFromClipboard();
    bool success = Copy2Clipboard(data.c_str(), data.length());
    if(!success){
        if( !bak) return;
        Copy2Clipboard(bak.value(), strlen(bak.value()));
    }
}

int main(int argc, char **argv) {
    for (int i = 1; i <argc ; ++i) {
        if( !strcmp(argv[i], "-h") || !strcmp(argv[i], "--help") ){
            cout<<helper<<endl;
            return 0;
        }else if( !strcmp(argv[i], "-o")){
            auto opt_string = getTextFromClipboard();
            if( !opt_string ) return 0;
            cout<<opt_string.value();
            return 0;
        }
    }
    // �����ݸ��Ƶ�������
    setClipboard([]()->string{
       string str;
       string buf;
       while(cin.good()){
           getline(cin, buf);
           str.append(buf);
           str.append("\n");
       }
       return str;
    });
}