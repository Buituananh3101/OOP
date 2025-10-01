#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

class Material {
private:
    string id;
    string name;
    string unit;
    double price;
    int stock;
public:
    Material();
    Material(const string& _id, const string& _name, const string& _unit, double _price, int _stock);

    // getters
    string getId() const;
    string getName() const;
    string getUnit() const;
    double getPrice() const;
    int getStock() const;

    // setters
    void setName(const string& s);
    void setUnit(const string& s);
    void setPrice(double p);
    void setStock(int s);

    string serialize() const;
    static bool deserialize(const string& line, Material& out);
    void printTableRow() const;
};

#endif
