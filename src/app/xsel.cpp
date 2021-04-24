//
// Created by 31951 on 2020/8/19.
// 代码大量参考：https://www.cnblogs.com/xuan52rock/p/6061155.html

#include <windows.h>
#include <iostream>
#include <optional>
#include <string>

using namespace std;

const char *helper = \
R"(xsel 将数据复制到系统剪贴板或者是读出：
-h \t 显示帮助内容
-o \t 读出系统剪贴板内容
例如：
cat 1.txt | xsel
xsel -o)";

// 复制数据至剪切板
bool Copy2Clipboard(const char* data, const size_t len){
    if(OpenClipboard(nullptr)){
        EmptyClipboard();
        auto clipBuffer = GlobalAlloc(GMEM_DDESHARE, len + 1);
        auto buf = (char*)GlobalLock(clipBuffer); // 锁定剪贴板
        strcpy_s(buf, len + 1, data);
        GlobalUnlock(clipBuffer); // 解锁剪贴板
        SetClipboardData(CF_TEXT, clipBuffer);
        CloseClipboard();
        return true;
    }
    return false;
}

// 从剪切板中取得数据
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
void setClipboard(T t){ // t 是一个返回类型为 string 的 callable 类型
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
    // 将数据复制到剪贴板
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