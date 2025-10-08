#include <bits/stdc++.h>
#include "Material.h"
#include "Inventory.h"
#include "OrderItem.h"
#include "Invoice.h"
#include "InputHelper.h"
#include "Flows.h"
using namespace std;

bool login(const string& role, string& usernameOut) {
    string file = (role == "admin") ? "taikhoanadmin.txt" : "taikhoannhanvien.txt";
    ifstream fin(file);
    if (!fin.is_open()) {
        cout << "Khong mo duoc file tai khoan: " << file << "\n";
        return false;
    }

    string user, pass;
    getline(fin, user);
    getline(fin, pass);
    fin.close();

    string inputUser, inputPass;
    cout << "Ten dang nhap: ";
    getline(cin, inputUser);
    cout << "Mat khau: ";
    getline(cin, inputPass);

    if (inputUser == user && inputPass == pass) {
        usernameOut = inputUser;
        cout << "Dang nhap thanh cong voi quyen " << role << "!\n";
        return true;
    } else {
        cout << "Sai ten dang nhap hoac mat khau.\n";
        return false;
    }
}

// === HÀM ĐỔI MẬT KHẨU ===
void changePassword(const string& role) {
    string file = (role == "admin") ? "taikhoanadmin.txt" : "taikhoannhanvien.txt";
    ifstream fin(file);
    if (!fin.is_open()) {
        cout << "Khong mo duoc file: " << file << "\n";
        return;
    }

    string user, oldPass;
    getline(fin, user);
    getline(fin, oldPass);
    fin.close();

    string curPass;
    cout << "Nhap mat khau hien tai: ";
    getline(cin, curPass);
    if (curPass != oldPass) {
        cout << "Mat khau hien tai sai!\n";
        return;
    }

    string newPass;
    cout << "Nhap mat khau moi: ";
    getline(cin, newPass);

    ofstream fout(file);
    fout << user << "\n" << newPass << "\n";
    fout.close();
    cout << "Doi mat khau thanh cong!\n";
}
