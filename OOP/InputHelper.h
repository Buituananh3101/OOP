#ifndef INPUTHELPER_H
#define INPUTHELPER_H

#include <string>
using namespace std;

// Hàm bỏ qua dòng còn sót trong input buffer
void ignoreLine();

// Nhập 1 dòng string
string inputLine(const string& prompt);

// Nhập 1 số nguyên với kiểm tra hợp lệ
int inputInt(const string& prompt);

// Nhập 1 số thực với kiểm tra hợp lệ
double inputDouble(const string& prompt);

#endif
