#ifndef FLOWS_H
#define FLOWS_H

#include "Inventory.h"
#include "InputHelper.h"
#include "Invoice.h"
#include <vector>
using namespace std;

// Các flow chức năng
void addMaterialFlow(Inventory& inv);
void updateMaterialFlow(Inventory& inv);
void removeMaterialFlow(Inventory& inv);
void restockFlow(Inventory& inv);
void saleFlow(Inventory& inv);
void searchFlow(Inventory& inv);

#endif
