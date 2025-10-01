#include <bits/stdc++.h>
#include "Material.h"
#include "Inventory.h"
#include "OrderItem.h"
#include "Invoice.h"
#include "InputHelper.h"
#include "Flows.h"
using namespace std;

void addMaterialFlow(Inventory& inv) {
    cout << "\n--- Them vat lieu moi ---\n";
    string id = inputLine("ID: ");
    if (id.empty()) { cout << "ID khong duoc bo trong.\n"; return; }
    if (inv.findIndexById(id) != -1) { cout << "ID da ton tai.\n"; return; }
    string name = inputLine("Ten: ");
    string unit = inputLine("Don vi (vd: kg, m,...): ");
    double price = inputDouble("Gia: ");
    int stock = inputInt("So luong ton kho ban dau: ");
    Material m(id, name, unit, price, stock);
    if (inv.addMaterial(m)) cout << "Them thanh cong.\n";
    else cout << "Them that bai.\n";
}

void updateMaterialFlow(Inventory& inv) {
    cout << "\n--- Cap nhat vat lieu ---\n";
    string id = inputLine("ID can cap nhat: ");
    Material* p = inv.getMaterialPtr(id);
    if (!p) { cout << "Khong tim thay ID.\n"; return; }
    cout << "Thong tin hien tai:\n";
    p->printTableRow();
    string name = inputLine("Ten moi (enter de giu): ");
    if (name.empty()) name = p->getName();
    string unit = inputLine("Don vi moi (enter de giu): ");
    if (unit.empty()) unit = p->getUnit();
    string tmp = inputLine("Gia moi (enter de giu): ");
    double price = p->getPrice();
    if (!tmp.empty()) price = stod(tmp);
    tmp = inputLine("Ton moi (enter de giu): ");
    int stock = p->getStock();
    if (!tmp.empty()) stock = stoi(tmp);
    inv.updateMaterial(id, name, unit, price, stock);
    cout << "Cap nhat xong.\n";
}

void removeMaterialFlow(Inventory& inv) {
    cout << "\n--- Xoa vat lieu ---\n";
    string id = inputLine("ID can xoa: ");
    if (inv.removeMaterial(id)) cout << "Xoa thanh cong.\n"; 
    else cout << "Khong tim thay ID.\n";
}

void restockFlow(Inventory& inv) {
    cout << "\n--- Nhap kho ---\n";
    string id = inputLine("ID: ");
    Material* p = inv.getMaterialPtr(id);
    if (!p) { cout << "Khong tim thay ID.\n"; return; }
    int qty = inputInt("So luong nhap them: ");
    if (qty <= 0) { cout << "So luong > 0.\n"; return; }
    inv.restock(id, qty);
    cout << "Nhap kho thanh cong. Ton moi: " << p->getStock() << "\n";
}

void saleFlow(Inventory& inv) {
    cout << "\n--- Tao hoa don ---\n";
    vector<OrderItem> cart;
    while (true) {
        string id = inputLine("Nhap ID (enter de ket thuc): ");
        if (id.empty()) break;
        Material* p = inv.getMaterialPtr(id);
        if (!p) { cout << "Khong tim thay ID.\n"; continue; }
        cout << "Ten: " << p->getName() << ", Ton: " << p->getStock() << ", Gia: " 
             << fixed << setprecision(2) << p->getPrice() << "\n";
        int qty = inputInt("So luong ban: ");
        if (qty <= 0) { cout << "So luong > 0.\n"; continue; }
        if (p->getStock() < qty) { cout << "Ton khong du. Hien co " << p->getStock() << "\n"; continue; }
        inv.reduceStock(id, qty);
        OrderItem it{id, p->getName(), p->getUnit(), p->getPrice(), qty};
        cart.push_back(it);
        cout << "Da them vao gio.\n";
    }
    if (cart.empty()) { cout << "Khong co hang, huy hoa don.\n"; return; }
    Invoice invc(cart);
    invc.print();
    if (invc.saveToFile("invoices.txt")) cout << "Hoa don da luu (invoices.txt)\n";
    else cout << "Khong the luu hoa don!\n";
}

void searchFlow(Inventory& inv) {
    cout << "\n--- Tim vat lieu theo ten ---\n";
    string key = inputLine("Tu khoa: ");
    auto res = inv.searchByName(key);
    if (res.empty()) { cout << "Khong tim thay.\n"; return; }
    cout << left << setw(10) << "ID" << left << setw(25) << "Ten" << left << setw(8) << "Don vi"
         << right << setw(10) << "Gia" << right << setw(8) << "Ton" << "\n";
    cout << string(61, '-') << "\n";
    for (const auto& m : res) m.printTableRow();
}
