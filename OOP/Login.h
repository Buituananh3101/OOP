#ifndef LOGIN_H
#define LOGIN_H
#include <bits/stdc++.h>
#include "Material.h"
#include "Inventory.h"
#include "OrderItem.h"
#include "Invoice.h"
#include "InputHelper.h"
#include "Flows.h"
using namespace std;

bool login(const string& role, string& usernameOut);

void changePassword(const string& role);
#endif

