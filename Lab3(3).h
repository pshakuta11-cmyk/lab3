#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <array>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cctype>

using namespace std;

class Service {
protected:
    int id;
    string name;
    double price;
    int duration;
    string type;

public:
    Service() : id(0), price(0), duration(0) {}
    Service(int i, string n, double p, int d, string t) : id(i), name(n), price(p), duration(d), type(t) {}
    virtual ~Service() {}

    int getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getDuration() const { return duration; }
    string getType() const { return type; }

    void setId(int v) { id = v; }
    void setName(string v) { name = v; }
    void setPrice(double v) { price = v; }
    void setDuration(int v) { duration = v; }
    void setType(string v) { type = v; }

    virtual void input();
    virtual void printHeader() const;
    virtual void printRow() const;
};

class CosmeticService : public Service {
protected:
    int sessions;
public:
    CosmeticService() : Service(), sessions(0) { type = "Косметология"; }
    CosmeticService(int i, string n, double p, int d, string t, int s) : Service(i, n, p, d, t), sessions(s) {}
    void input() override;
    void printRow() const override;
    int getSessions() const { return sessions; }
};

class ServiceVector {
    vector<Service> data;
public:
    void add();
    void show();
    void edit();
    void remove();
    void search();
    void sortData();
    void saveToFile();
};

class ServiceList {
    list<CosmeticService> data;
public:
    void add();
    void show();
    void edit();
    void remove();
    void search();
    void sortData();
    void saveToFile();
};

class ServiceArray {
    array<Service, 100> data;
    int count = 0;
public:
    void add();
    void show();
    void edit();
    void remove();
    void search();
    void sortData();
    void saveToFile();
};

class Salon {
public:
    ServiceVector v;
    ServiceList l;
    ServiceArray a;
    void run();
};

int inputInt(string prompt);
double inputDouble(string prompt);
string toLower(string s);
string padRight(string str, int width);
void editService(Service& s);
void runLab3();