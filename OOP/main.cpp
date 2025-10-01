// sales_manager.cpp
#include <bits/stdc++.h>
#include "Material.h"
#include "Inventory.h"
#include "OrderItem.h"
#include "Invoice.h"
#include "InputHelper.h"
#include "Flows.h"
using namespace std;


/* -------------------- Main -------------------- */
int main() {
    Inventory inventory;
    inventory.loadFromFile("materials.txt");

    while (true) {
        cout << "\n====== QUAN LY BAN VAT LIEU ======\n";
        cout << "1. Them vat lieu\n";
        cout << "2. Cap nhat vat lieu\n";
        cout << "3. Xoa vat lieu\n";
        cout << "4. Danh sach vat lieu\n";
        cout << "5. Tim vat lieu theo ten\n";
        cout << "6. Nhap kho\n";
        cout << "7. Ban (tao hoa don)\n";
        cout << "8. Luu va thoat\n";
        cout << "0. Thoat (khong luu)\n";
        cout << "Chon (0-8): ";
        int choice;
        if (!(cin >> choice)) {
            cout << "Loi nhap, thoat.\n";
            break;
        }
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
                if (inventory.saveToFile("materials.txt")) cout << "Da luu materials.txt\n";
                else cout << "Luu that bai!\n";
                cout << "Thoat.\n";
                return 0;
            case 0:
                cout << "Thoat (khong luu).\n";
                return 0;
            default:
                cout << "Lua chon khong hop le.\n";
        }
    }
    return 0;
}
