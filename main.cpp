#include <bits/stdc++.h> 
using namespace std;

void clearScreen() { system("cls"); }
void pauseScreen() { cout<<endl; system("pause"); }

class Material {
private:
    string ma;
    string ten;
    string noiNhap;
    string nhom;
    string donVi;
    double gia;
    int tonKho;
    
public:
    Material() : gia(0.0), tonKho(0), noiNhap(""), nhom("") {}

    Material(const string& _id, const string& _name, const string& _unit, double _price, int _stock, const string& _noiNhap, const string& _nhom)
        : ma(_id), ten(_name), donVi(_unit), gia(_price), tonKho(_stock), noiNhap(_noiNhap), nhom(_nhom) {}

    string getId() const { return ma; }
    string getName() const { return ten; }
    string getUnit() const { return donVi; }
    double getPrice() const { return gia; }
    int getStock() const { return tonKho; }
    string getNoiNhap() const { return noiNhap; }
    string getNhom() const { return nhom; }

    void setName(const string& s) { ten = s; }
    void setUnit(const string& s) { donVi = s; }
    void setPrice(double p) { gia = p; }
    void setStock(int s) { tonKho = s; }
    void setNoiNhap(const string& s) { noiNhap = s; }
    void setNhom(const string& s) { nhom =s;}

    string serialize() const {
        ostringstream oss;
        oss << ma << "," << ten << "," << nhom << "," << noiNhap << "," << donVi << "," << fixed << setprecision(2) << gia << "," << tonKho;
        return oss.str();
    }

    static bool deserialize(const string& line, Material& out) {
        if (line.empty()) return false;
        if (line.rfind("ID,", 0) == 0 || line.rfind("Id,", 0) == 0) return false;

        stringstream ss(line);
        string id_, name_, unit_, priceStr, stockStr, noiNhap_, nhom_;
        if (!getline(ss, id_, ',')) return false;
        if (!getline(ss, name_, ',')) return false;
        if (!getline(ss, nhom_, ',')) return false;
        if (!getline(ss, noiNhap_, ',')) return false;
        if (!getline(ss, unit_, ',')) return false;
        if (!getline(ss, priceStr, ',')) return false;
        if (!getline(ss, stockStr)) return false;

        try {
            double p = stod(priceStr);
            int s = stoi(stockStr);
            out = Material(id_, name_, unit_, p, s, noiNhap_, nhom_);
            return true;
        } catch (...) {
            return false;
        }
    }

    void printTableRow() const {
        cout << left << setw(10) << ma
            << left << setw(25) << ten
            << left << setw(14) << nhom
            << left << setw(14) << noiNhap
            << left << setw(8) << donVi
            << right << setw(10) << fixed << setprecision(2) << gia
            << right << setw(10) << tonKho << "\n";
    }
};

class OrderItem {
private:
    string ma;
    string ten;
    string donVi;
    double donGia;
    int soLuong;

public:
    OrderItem(const string& _id, const string& _name, const string& _unit, double _unitPrice, int _qty)
        : ma(_id), ten(_name), donVi(_unit), donGia(_unitPrice), soLuong(_qty) {}

    string getId() const { return ma; }
    string getName() const { return ten; }
    string getUnit() const { return donVi; }
    double getUnitPrice() const { return donGia; }
    int getQty() const { return soLuong; }

    double subtotal() const { return donGia * soLuong; }
};

class Inventory {
private:
    vector<Material> danhSachNguyenLieu;

    string normalize(const string& s) const {
      string t = ""; 
      for (auto& c_in : s) {
          char c = (char)tolower((unsigned char)c_in);
          if (!isspace(c)) { // <-- Chỉ thêm nếu KHÔNG phải dấu cách
              t += c; 
          }
      }
      return t;
    }

public:
    bool addMaterial(const Material& m) {
        if (findIndexById(m.getId()) != -1) return false;
        danhSachNguyenLieu.push_back(m);
        return true;
    } 
    bool removeMaterial(const string& id) {
        int idx = findIndexById(id);
        if (idx == -1) return false;
        danhSachNguyenLieu.erase(danhSachNguyenLieu.begin() + idx);
        return true;
    }
    bool updateMaterial(const string& id, const string& name, const string& unit, double price, int stock, const string& noiNhap, const string& nhom) {
        Material* p = getMaterialPtr(id);
        if (!p) return false;
        p->setName(name);
        p->setUnit(unit);
        p->setPrice(price);
        p->setStock(stock);
        p->setNoiNhap(noiNhap);
        p->setNhom(nhom);
        return true;
    }
    bool restock(const string& id, int qty) {
        Material* p = getMaterialPtr(id);
        if (!p) return false;
        p->setStock(p->getStock() + qty);
        return true;
    }
    bool reduceStock(const string& id, int qty) {
        Material* p = getMaterialPtr(id);
        if (!p) return false;
        if (qty <= 0 || p->getStock() < qty) return false;
        p->setStock(p->getStock() - qty);
        return true;
    }
    int findIndexById(const string& id) const {
        for (size_t i = 0; i < danhSachNguyenLieu.size(); ++i)
            if (danhSachNguyenLieu[i].getId() == id) return (int)i;
        return -1;
    }
    Material* getMaterialPtr(const string& id) {
        int idx = findIndexById(id);
        if (idx == -1) return nullptr;
        return &danhSachNguyenLieu[idx];
    }

    vector<Material> searchByString(int field, const string& key) const {
      vector<Material> res;
      if (key.empty()) return res;

      double threshold;
      
      cout << "Nhap do chinh xac (0-10): ";
      while (!(cin >> threshold) || threshold < 0 || threshold > 10) {
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
          cout << "Nhap lai (0-10): ";
      }
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      
      threshold = threshold / 10.0; 

      string k = normalize(key); 
      
      auto levenshtein_dist = [](const string &s1, const string &s2) {
          int n = s1.length();
          int m = s2.length();
          if (n == 0) return m;
          if (m == 0) return n;

          vector<int> prev(m + 1);
          vector<int> cur(m + 1);

          for (int j = 0; j <= m; ++j) {
              prev[j] = j;
          }

          for (int i = 1; i <= n; ++i) {
              cur[0] = i; 
              for (int j = 1; j <= m; ++j) {
                  int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
                  cur[j] = std::min({
                      cur[j - 1] + 1,     // Thêm
                      prev[j] + 1,      // Xóa
                      prev[j - 1] + cost  // Thay thế / Khớp
                  });
              }
              prev = cur; // Cập nhật hàng trước đó
          }
          return prev[m];
      };

      for (const auto& mtr : danhSachNguyenLieu) {
          string value;
          switch (field) {
              case 1: value = mtr.getId(); break;
              case 2: value = mtr.getName(); break;
              case 3: value = mtr.getNhom(); break;
              case 4: value = mtr.getNoiNhap(); break;
              default: continue; 
          }
          
          string lowerValue = normalize(value); 

          int dist = levenshtein_dist(k, lowerValue);
          
          int max_len = std::max((int)k.length(), (int)lowerValue.length());
          
          double sim = (max_len == 0) ? 1.0 : (1.0 - (double)dist / (double)max_len);
          
          if (sim >= threshold) {
              res.push_back(mtr);
          }
      }
      return res;
    }

    vector<Material> searchByNumeric(int field, int compareType, double value) const {
        vector<Material> res;

        for (const auto& mtr : danhSachNguyenLieu) {
            if (field == 5) { 
                if (compareType == 1 && mtr.getPrice() <= value) {
                    res.push_back(mtr);
                } else if (compareType == 2 && mtr.getPrice() >= value) {
                    res.push_back(mtr);
                }
            } else if (field == 6) { 
                if (compareType == 1 && mtr.getStock() <= value) {
                    res.push_back(mtr);
                } else if (compareType == 2 && mtr.getStock() >= value) {
                    res.push_back(mtr);
                }
            }
        }
        return res;
    }

    void sortMaterials(int criteria, bool ascending) {
        sort(danhSachNguyenLieu.begin(), danhSachNguyenLieu.end(), [criteria, ascending](const Material& a, const Material& b) {
            switch (criteria) {
                case 1:
                    return ascending ? (a.getName() < b.getName()) : (a.getName() > b.getName());
                case 2:
                    return ascending ? (a.getPrice() < b.getPrice()) : (a.getPrice() > b.getPrice());
                case 3:
                    return ascending ? (a.getStock() < b.getStock()) : (a.getStock() > b.getStock());
                case 4:
                    return ascending ? (a.getNoiNhap() < b.getNoiNhap()) : (a.getNoiNhap() > b.getNoiNhap());
                default:
                    return false;
            }
        });
        cout << "Da sap xep xong.\n";
    }

    void listAll() const {
        clearScreen();
        cout << left << setw(10) << "ID"
            << left << setw(25) << "Ten"
            << left << setw(14) << "Nhom"
            << left << setw(14) << "Noi Nhap"
            << left << setw(8) << "Don vi"
            << right << setw(10) << "Gia"
            << right << setw(10) << "Ton" << "\n";

        cout << string(91, '-') << "\n";
        for (const auto& m : danhSachNguyenLieu) m.printTableRow();
        pauseScreen();
    }

    bool saveToFile(const string& filename) const {
        ofstream ofs(filename);
        if (!ofs) return false;
        ofs << "ID,Ten,Nhom,NoiNhap,DonVi,Gia,TonKho\n";
        for (const auto& m : danhSachNguyenLieu) {
            ofs << m.serialize() << "\n";
        }
        return true;
    }

    bool loadFromFile(const string& filename) {
        ifstream ifs(filename);
        if (!ifs) return false;

        danhSachNguyenLieu.clear();

        string line;
        bool firstLine = true;
        while (getline(ifs, line)) {
            if (firstLine) {
                firstLine = false;
                if (line.rfind("ID,", 0) == 0 || line.rfind("Id,", 0) == 0) continue;
            }
            Material m;
            if (Material::deserialize(line, m)) danhSachNguyenLieu.push_back(m);
        }
        return true;
    }

    bool importFromFile(const string& filename) {
        ifstream ifs(filename);
        if (!ifs.is_open()) {
            cout << "Khong the mo file: " << filename << "\n";
            pauseScreen();
            return false;
        }

        string line;
        bool firstLine = true;
        int added = 0, updated = 0;

        while (getline(ifs, line)) {
            if (firstLine) {
                firstLine = false;
                continue;
            }

            Material m;
            if (!Material::deserialize(line, m)) continue;

            int idx = findIndexById(m.getId());
            if (idx != -1) {
                danhSachNguyenLieu[idx].setStock(
                    danhSachNguyenLieu[idx].getStock() + m.getStock()
                );
                updated++;
            } else {
                danhSachNguyenLieu.push_back(m);
                added++;
            }
        }
        ifs.close();

        cout << "\n------------------------------\n";
        cout << "Da nhap tu file: " << filename << "\n";
        cout << " + Vat lieu moi: " << added << "\n";
        cout << " + Cap nhat so luong: " << updated << "\n";
        cout << "------------------------------\n";

        saveToFile("Inventory.csv");

        ofstream clearFile(filename);
        clearFile << "ID,Ten,Nhom,NoiNhap,DonVi,Gia,TonKho\n";
        clearFile.close();
        pauseScreen();
        return true;
    }
};

class Invoice {
private:
    string maHoaDon;
    string thoiGian;
    vector<OrderItem> danhSachMatHang;
public:
    Invoice() {}

    Invoice(const vector<OrderItem>& _items) : danhSachMatHang(_items) {
        time_t now = time(nullptr);
        maHoaDon = "INV" + to_string(now);
        char buf[64];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        thoiGian = buf;
    }

    double total() const {
        double s = 0;
        for (const auto& it : danhSachMatHang) s += it.subtotal();
        return s;
    }

    string toString() const {
        ostringstream oss;
        oss << "Hoa don ID: " << maHoaDon << "\nNgay: " << thoiGian << "\n";
        oss << left << setw(10) << "ID"
            << left << setw(25) << "Ten"
            << left << setw(8) << "Don vi"
            << right << setw(10) << "Gia"
            << right << setw(8) << "SL"
            << right << setw(12) << "Thanh tien" << "\n";

        oss << string(75, '-') << "\n";

        for (const auto& it : danhSachMatHang) {
            oss << left << setw(10) << it.getId()
                << left << setw(25) << it.getName()
                << left << setw(8) << it.getUnit()
                << right << setw(10) << fixed << setprecision(2) << it.getUnitPrice()
                << right << setw(8) << it.getQty()
                << right << setw(12) << fixed << setprecision(2) << it.subtotal() << "\n";
        }
        oss << string(75, '-') << "\n";
        oss << right << setw(60) << "TONG: " << setw(12) << fixed << setprecision(2) << total() << "\n";
        return oss.str();
    }

    void print() const {
        cout << toString();
    }

    bool saveToFile(const string& filename) const {
        ofstream ofs(filename, ios::app);
        if (!ofs) return false;
        ofs << toString() << "\n===\n";
        return true;
    }
};

void ignoreLine() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
string inputLine(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}
int inputInt(const string& prompt) {
    while (true) {
        string s = inputLine(prompt);
        try {
            int v = stoi(s);
            return v;
        } catch (...) {
            cout << "Gia tri khong hop le. Nhap lai.\n";
        }
    }
}
double inputDouble(const string& prompt) {
    while (true) {
        string s = inputLine(prompt);
        try {
            double v = stod(s);
            return v;
        } catch (...) {
            cout << "Gia tri khong hop le. Nhap lai.\n";
        }
    }
}
void addMaterialFlow(Inventory& inv) {
    cout << "\n--- Them vat lieu moi ---\n";
    string id = inputLine("ID: ");
    if (id.empty()) { cout << "ID khong duoc bo trong.\n"; pauseScreen(); return; }
    if (inv.findIndexById(id) != -1) { cout << "ID da ton tai.\n";pauseScreen(); return; }

    string name = inputLine("Ten: ");
    string unit = inputLine("Don vi (vd: kg, m,...): ");
    double price = inputDouble("Gia: ");
    int stock = inputInt("So luong ton kho ban dau: ");
    string noiNhap = inputLine("Noi Nhap: ");
    string nhom = inputLine("Nhom: ");

    Material m(id, name, unit, price, stock, noiNhap, nhom);

    if (inv.addMaterial(m)) cout << "Them thanh cong.\n";
    else cout << "Them that bai.\n";
    pauseScreen();
}
void updateMaterialFlow(Inventory& inv) {
    cout << "\n--- Cap nhat vat lieu (enter de giu)---\n";
    string id = inputLine("ID can cap nhat: ");
    Material* p = inv.getMaterialPtr(id);
    if (!p) { cout << "Khong tim thay ID.\n"; return; }

    cout << "Thong tin hien tai:\n"; p->printTableRow();

    string name = inputLine("Ten moi: ");
    if (name.empty()) name = p->getName();

    string unit = inputLine("Don vi moi: ");
    if (unit.empty()) unit = p->getUnit();

    string tmp = inputLine("Gia moi: ");
    double price = p->getPrice();
    if (!tmp.empty()) price = stod(tmp);

    tmp = inputLine("Ton moi: ");
    int stock = p->getStock();
    if (!tmp.empty()) stock = stoi(tmp);

    string noiNhap = inputLine("Noi nhap moi: ");
    if (noiNhap.empty()) noiNhap = p->getNoiNhap();

    string nhom = inputLine("Nhom moi: ");
    if(nhom.empty()) nhom = p->getNhom();

    inv.updateMaterial(id, name, unit, price, stock, noiNhap, nhom);
    cout << "Cap nhat xong.\n";
    pauseScreen();
}
void removeMaterialFlow(Inventory& inv) {
    cout << "\n--- Xoa vat lieu ---\n";
    string id = inputLine("ID can xoa: ");
    if (inv.removeMaterial(id)) cout << "Xoa thanh cong.\n";
    else cout << "Khong tim thay ID.\n";
    pauseScreen();
}
void restockFlow(Inventory& inv) {
    cout << "\n--- Nhap kho ---\n";
    string id = inputLine("ID: ");
    Material* p = inv.getMaterialPtr(id);
    if (!p) { cout << "Khong tim thay ID.\n";     pauseScreen(); return; }
    int qty = inputInt("So luong nhap them: ");
    if (qty <= 0) { cout << "So luong > 0.\n";     pauseScreen(); return; }
    inv.restock(id, qty);
    cout << "Nhap kho thanh cong. Ton moi: " << p->getStock() << "\n";
    pauseScreen();
}
void saleFlow(Inventory& inv) {
    cout << "\n--- Tao hoa don ---\n";
    vector<OrderItem> cart;
    while (true) {
        string id = inputLine("Nhap ID (enter de ket thuc): ");
        if (id.empty()) break;
        Material* p = inv.getMaterialPtr(id);
        if (!p) { cout << "Khong tim thay ID.\n"; continue; }
        cout << "Ten: " << p->getName() << ", Ton: " << p->getStock() << ", Gia: "<< fixed << setprecision(2) << p->getPrice() << "\n";
        int qty = inputInt("So luong ban: ");
        if (qty <= 0) { cout << "So luong > 0.\n"; continue; }
        if (p->getStock() < qty) { cout << "Ton khong du. Hien co " << p->getStock() << "\n"; continue; }
        inv.reduceStock(id, qty);
        OrderItem it(p->getId(), p->getName(), p->getUnit(), p->getPrice(), qty);
        cart.push_back(it);
        cout << "Da them vao gio.\n";
    }
    clearScreen();
    cout<<endl;
    if (cart.empty()) { cout << "Khong co hang, huy hoa don.\n"; pauseScreen(); return; }
    Invoice invc(cart);
    invc.print();
    if (invc.saveToFile("invoices.txt")) cout << "Hoa don da luu (invoices.txt)\n";
    else cout << "Khong the luu hoa don!\n";
    pauseScreen();
}

void searchFlow(Inventory& inv) {
    cout << "\n--- Tim vat lieu ---\n";
    cout << "\nChon tieu chi tim kiem:\n";
    cout<<"    1. ID\n";
    cout<<"    2. Ten\n";
    cout<<"    3. Nhom\n";
    cout<<"    4. Noi Nhap\n";
    cout<<"    5. Gia\n";
    cout<<"    6. Ton\n";
    cout<<"    0. Huy\n";

    int choice = inputInt("\nChon (0-6): ");
    if (choice < 1 || choice > 6) {
        cout << "Huy tim kiem.\n";
        pauseScreen();
        return;
    }

    vector<Material> res;

    string key="";

    if (choice >= 1 && choice <= 4) {
        key = inputLine("Nhap tu khoa: ");
        res = inv.searchByString(choice, key);

    } else if (choice == 5 || choice == 6) {
        cout << "\nChon kieu so sanh:\n";
        cout<< "    1. <=\n";
        cout<< "    2. >=\n";
        int compareType = inputInt("\nChon (1-2): ");

        if (compareType != 1 && compareType != 2) {
            cout << "Lua chon khong hop le. Huy tim kiem.\n";
            pauseScreen();
            return;
        }

        double value;
        if (choice == 5) { 
            value = inputDouble("Nhap gia tri (vd: 50000): ");
        } else { 
            value = (double)inputInt("Nhap so luong (vd: 100): "); 
        }
        
        res = inv.searchByNumeric(choice, compareType, value);
    }

    if (res.empty()) {
        clearScreen();
        cout << "\nKhong tim thay ket qua phu hop.\n";
        pauseScreen();
        return;
    }
    clearScreen();
    cout << "\n--- Ket qua tim kiem (" << key <<" : "<<res.size() << " vat lieu) ---\n";
    cout << left << setw(10) << "ID"
            << left << setw(25) << "Ten"
            << left << setw(14) << "Nhom"
            << left << setw(14) << "Noi Nhap"
            << left << setw(8) << "Don vi"
            << right << setw(10) << "Gia"
            << right << setw(10) << "Ton" << "\n";
            
    cout << string(91, '-') << "\n";
    for (const auto& m : res) m.printTableRow();
    pauseScreen();
}

void sortFlow(Inventory& inv) {
    cout << "\n--- Sap xep vat lieu ---\n";
    cout << "Sap xep theo:\n";
    cout << " 1. Ten\n";
    cout << " 2. Gia\n";
    cout << " 3. So luong ton kho\n";
    cout << " 4. Noi nhap\n";
    cout << " 0. Huy\n";

    int criteria = inputInt("Chon tieu chi (0-4): ");
    if (criteria < 1 || criteria > 4) {
        cout << "Huy sap xep.\n";
        return;
    }

    cout << "\nChon thu tu:\n";
    cout << " 1. Tang dan (A-Z, Nho-Lon)\n";
    cout << " 2. Giam dan (Z-A, Lon-Nho)\n";

    int order = inputInt("Chon thu tu (1-2): ");
    bool ascending = (order == 1);

    inv.sortMaterials(criteria, ascending);

    cout << "Danh sach sau khi sap xep:\n";
    inv.listAll();
}

class User {
protected:
    string tenDangNhap;
    string tepMatKhau;

public:
    User(string passFile) : tepMatKhau(passFile) {}
    virtual ~User() {}
    bool login() {
        ifstream fin(tepMatKhau);
        if (!fin.is_open()) {
            cout << "Khong mo duoc file tai khoan: " << tepMatKhau << "\n";
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
            this->tenDangNhap = inputUser;
            cout << "Dang nhap thanh cong!\n";
            return true;
        } else {
            cout << "Sai ten dang nhap hoac mat khau.\n";
            return false;
        }
    }

    void changePassword() {
        ifstream fin(tepMatKhau);
        if (!fin.is_open()) {
            cout << "Khong mo duoc file: " << tepMatKhau << "\n";
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
            pauseScreen();
            return;
        }
        string newPass;
        cout << "Nhap mat khau moi: ";
        getline(cin, newPass);

        ofstream fout(tepMatKhau);
        fout << user << "\n" << newPass << "\n";
        fout.close();
        cout << "Doi mat khau thanh cong!\n";
        pauseScreen();
    }

    virtual void showMenu(Inventory& inv) = 0;
};

class Admin : public User {
private:
    void adminResetPassword(const string& role) {
        string file;
        if (role == "admin") {
            file = "taikhoanadmin.txt";
        } else if (role == "nhanvien") {
            file = "taikhoannhanvien.txt";
        } else {
            cout << "Vai tro khong hop le.\n";
            pauseScreen();
            return;
        }
        ifstream fin(file);
        if (!fin.is_open()) {
            cout << "Khong mo duoc file: " << file << "\n";
            pauseScreen();
            return;
        }
        string user, oldPass;
        getline(fin, user);
        getline(fin, oldPass);
        fin.close();
        string newPass;
        cout << "Nhap mat khau moi cho " << role << ": ";
        getline(cin, newPass);
        ofstream fout(file);
        fout << user << "\n" << newPass << "\n";
        fout.close();
        cout << "Doi mat khau cho " << role << " thanh cong!\n";
        pauseScreen();
    }

public:
    Admin() : User("taikhoanadmin.txt") {}

    void showMenu(Inventory& inventory) override {
        while (true) {
            clearScreen();
            cout << "\n__________MENU ADMIN__________\n";
            cout << "\nQuan li vat lieu:\n";
            cout << "   1. Them vat lieu\n";
            cout << "   2. Cap nhat vat lieu\n";
            cout << "   3. Xoa vat lieu\n";
            cout << "   4. Kho\n";
            cout << "   5. Nhap kho\n";

            cout << "\nBan hang:\n";
            cout << "   6. Ban (tao hoa don)\n";

            cout << "\nBo loc:\n";
            cout << "   7. Sap xep\n";
            cout << "   8. Tim vat lieu\n";

            cout << "\nQuan li tai khoan:\n";
            cout << "   9. Doi mat khau (admin)\n";
            cout << "   10. Doi mat khau (nhan vien)\n";

            cout << "\nThao tac file:\n";
            cout << "   11. Nhap tu file themvatlieu.csv\n";

            cout << "\nThoat:\n";
            cout << "   12. Luu va thoat\n";
            cout << "   0. Thoat (khong luu)\n";
            cout << "\nChon (0-12): ";
            
            int choice;
            if (!(cin >> choice)) break;
            ignoreLine();

            switch (choice) {
                case 1: addMaterialFlow(inventory); break;
                case 2: updateMaterialFlow(inventory); break;
                case 3: removeMaterialFlow(inventory); break;
                case 4: inventory.listAll(); break;
                case 5: restockFlow(inventory); break;
                case 6: saleFlow(inventory); break;
                case 7: sortFlow(inventory); break;
                case 8: searchFlow(inventory); break;
                case 9: changePassword(); break;
                case 10: adminResetPassword("nhanvien"); break;
                case 11: inventory.importFromFile("themvatlieu.csv"); break;
                case 12:
                    if (inventory.saveToFile("Inventory.csv"))
                        cout << "Da luu Inventory.csv\n";
                    else
                        cout << "Luu that bai!\n";
                    cout << "Thoat.\n";
                    return;
                case 0:
                    cout << "Thoat (khong luu).\n";
                    return;
                default:
                    cout << "Lua chon khong hop le.\n";
            }
        }
    }
};

class Employee : public User {
public:
    Employee() : User("taikhoannhanvien.txt") {}

    void showMenu(Inventory& inventory) override {
        while (true) {
            clearScreen();
            cout << "\n__________MENU NHAN VIEN__________\n";
            cout << "\n1. Kho\n";
            cout << "2. Sap xep\n";
            cout << "3. Tim vat lieu (nang cao)\n";
            cout << "4. Ban (tao hoa don)\n";
            cout << "5. Luu va thoat\n";
            cout << "0. Thoat (khong luu)\n\n";
            cout << "Chon: ";

            int choice;
            if (!(cin >> choice)) break;
            ignoreLine();

            switch (choice) {
                case 1: 
                    inventory.listAll(); 
                    break;
                case 2: 
                    sortFlow(inventory); 
                    break;
                case 3: 
                    searchFlow(inventory); 
                    break;
                case 4: 
                    saleFlow(inventory); 
                    break;
                case 5: 
                    if (inventory.saveToFile("Inventory.csv"))
                        cout << "Da luu Inventory.csv\n";
                    else
                        cout << "Luu that bai!\n";
                    cout << "Thoat.\n";
                    return;
                case 0: 
                    cout << "Thoat (khong luu).\n";
                    return;
                default:
                    cout << "Lua chon khong hop le.\n";
            }
        }
    }
};

class Customer {
public:
    Customer() {} 

    void showMenu(Inventory& inventory) {
        while (true) {
            clearScreen();
            cout << "\n__________MENU KHACH HANG__________\n";
            cout << "\n1. Tim vat lieu (nang cao)\n";
            cout << "2. Sap xep theo tieu chi\n";
            cout << "0. Thoat\n\n";
            cout << "Chon: ";

            int choice;
            if (!(cin >> choice)) break;
            ignoreLine(); 

            switch (choice) {
                case 1:
                    searchFlow(inventory); 
                    break;
                case 2:
                    sortFlow(inventory); 
                    break;
                case 0:
                    cout << "Tam biet.\n";
                    return; 
                default:
                    cout << "Lua chon khong hop le.\n";
            }
        }
    }
};

void runApp() {
    Inventory inventory;
    inventory.loadFromFile("Inventory.csv");

    clearScreen();
    cout << "===== DANG NHAP HE THONG =====\n";
    cout << "1. Admin\n";
    cout << "2. Nhan vien\n";
    cout << "3. Khach hang (Khong can dang nhap)\n"; 
    cout << "Chon: ";
    
    int roleChoice;
    if (!(cin >> roleChoice)) return;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    if (roleChoice == 3) {
        Customer guest; 
        guest.showMenu(inventory); 
        cout << "Ket thuc phien khach hang.\n";
        return; 
    }

    User* currentUser = nullptr;

    if (roleChoice == 1) {
        currentUser = new Admin();
    } else if (roleChoice == 2) {
        currentUser = new Employee();
    } else {
        cout << "Lua chon khong hop le.\n";
        return;
    }

    if (currentUser && currentUser->login()) {
        currentUser->showMenu(inventory);
    }

    delete currentUser;
}

int main() {
    runApp();
    return 0;
}