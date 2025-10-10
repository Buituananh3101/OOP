#include <bits/stdc++.h>
#include "Material.h"
#include "Inventory.h"
#include "OrderItem.h"
#include "Invoice.h"
#include "InputHelper.h"
#include "Flows.h"
#include "Login.h"
using namespace std;


void runApp() {
    Inventory inventory;
    inventory.loadFromFile("materials.txt");

    cout << "===== DANG NHAP HE THONG =====\n";
    cout << "1. Admin\n";
    cout << "2. Nhan vien\n";
    cout << "Chon: ";
    int roleChoice;
    if (!(cin >> roleChoice)) return;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string username;
    bool isAdmin = (roleChoice == 1);

    if (!login(isAdmin ? "admin" : "nhanvien", username))
        return;

    // admin
    if (isAdmin) {
        while (true) {
            cout << "\n====== MENU ADMIN ======\n";
            cout << " 1. Them vat lieu\n";
            cout << " 2. Cap nhat vat lieu\n";
            cout << " 3. Xoa vat lieu\n";
            cout << " 4. Danh sach vat lieu\n";
            cout << " 5. Tim vat lieu theo ten\n";
            cout << " 6. Nhap kho\n";
            cout << " 7. Ban (tao hoa don)\n";
            cout << " 8. Luu va thoat\n";
            cout << " 9. Doi mat khau admin\n";
            cout << "10. Doi mat khau nhan vien\n";
            cout << " 0. Thoat (khong luu)\n";
            cout << "Chon (0-10): ";

            int choice;
            if (!(cin >> choice)) break;
            ignoreLine();

            switch (choice) {
                case 1: addMaterialFlow(inventory); break;
                case 2: updateMaterialFlow(inventory); break;
                case 3: removeMaterialFlow(inventory); break;
                case 4: inventory.listAll(); break;
                case 5: searchFlow(inventory); break;
                case 6: restockFlow(inventory); break;
                case 7: saleFlow(inventory); break;
                case 8:
                    if (inventory.saveToFile("materials.txt"))
                        cout << "Da luu materials.txt\n";
                    else
                        cout << "Luu that bai!\n";
                    cout << "Thoat.\n";
                    return;
                case 9:
                    changePassword("admin");
                    break;
                case 10:
                    changePassword("nhanvien");
                    break;
                case 0:
                    cout << "Thoat (khong luu).\n";
                    return;
                default:
                    cout << "Lua chon khong hop le.\n";
            }
        }
    }
    // nhan vien
    else {
        while (true) {
            cout << "\n====== MENU NHAN VIEN ======\n";
            cout << "1. Danh sach vat lieu\n";
            cout << "2. Tim vat lieu theo ten\n";
            cout << "3. Ban (tao hoa don)\n";
            cout << "4. Doi mat khau\n";
            cout << "0. Thoat\n";
            cout << "Chon: ";

            int choice;
            if (!(cin >> choice)) break;
            ignoreLine();

            switch (choice) {
                case 1: inventory.listAll(); break;
                case 2: searchFlow(inventory); break;
                case 3: saleFlow(inventory); break;
                case 4: changePassword("nhanvien"); break;
                case 0:
                    cout << "Thoat.\n";
                    return;
                default:
                    cout << "Lua chon khong hop le.\n";
            }
        }
    }
}
