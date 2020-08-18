//
// Created by 31951 on 2020/8/18.
//

#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <queue>
#include "common.h"

using namespace std;

const char* helper =\
R"(head ������ļ����ݵĺ��У�Ĭ��Ϊ 10 �У�
���磺
tail C:\1.txt
tail -n 20 C:\1.txt
cat C:\1.txt | tail -n 20)";

class Queue;

class Queue{
    // ���޶��У���ഢ�� _size ��Ԫ��
    size_t _size;
    queue<string> que;
public:
    explicit Queue(size_t size) : _size(size){}
    void push(const string str){
        for (; que.size() >= _size - 1 ; --_size) {
            que.pop();
        }
        que.push(str);
    }
    string pop(){
        string str = que.front();
        que.pop();
        return str;
    }
    size_t size(){
        return que.size();
    }
};

Queue& getInstance(size_t size = -1){
    static bool hasInit = false;
    static Queue* que;
    if( !hasInit ) que = new Queue(size);
    return *que;
}

void readStdin(int line){
    string buf;
    auto que = getInstance();
    for (int i = 0; i < line && cin.good(); ++i) {
        getline(cin, buf);
        que.push(buf);
    }
}

template <typename T>
inline void disPlay(T que_func){
    Queue que = que_func();
    while (que.size()){
        cout<<que.pop()<<endl;
    }
}

int main(int argc, char** argv) {
//    setlocale(LC_ALL, "");
    auto para = Parse(argc, argv, helper);
    getInstance(para.first); // ��ʼ�����޶����ݻ�
    string buf;
    if( !para.second || (para.second.get() == "-") ){
        // ���ڶ�������Ϊ��
        disPlay([&]()->Queue{
            auto que = getInstance(); // �� lambda ����Ҫ�ֶ���ȡ����
            for (int i = 0; i < para.first && cin.good(); ++i) {
                getline(cin, buf);
                que.push(buf);
            }
            return que;
        });
        return 0;
    }

    disPlay([&]() mutable ->Queue{
        auto que = getInstance();
        ifstream inf(para.second.get());
        for (int i = 0; i < para.first && inf.good(); ++i) {
            getline(inf, buf);
            que.push(buf);
        }
        return que;
    });

    return 0;
}