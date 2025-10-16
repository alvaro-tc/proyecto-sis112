#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;

class Product {
private:
    int id;
    string name;
    string description;
    int quantity;
    double unit_price;

public:
    Product() : id(0), quantity(0), unit_price(0.0) {}
    Product(int id, const string& name, const string& description, int quantity, double unit_price)
        : id(id), name(name), description(description), quantity(quantity), unit_price(unit_price) {}

    int getId() const { return id; }
    string getName() const { return name; }
    string getDescription() const { return description; }
    int getQuantity() const { return quantity; }
    double getUnitPrice() const { return unit_price; }

    void setId(int newId) { id = newId; }
    void setName(const string& newName) { name = newName; }
    void setDescription(const string& newDescription) { description = newDescription; }
    void setQuantity(int newQuantity) { quantity = newQuantity; }
    void setUnitPrice(double newPrice) { unit_price = newPrice; }
};


void listProducts(const vector<Product>& products) {
    if (products.empty()) {
        cout << "No hay productos registrados.\n";
        return;
    }

    cout << left
         << setw(4) << "ID"
         << setw(25) << "Nombre"
         << setw(10) << "Cantidad"
         << setw(10) << "Precio"
         << "Descripcion\n";
    cout << string(70, '-') << "\n";

    int n = (products.size());
    for (int i = 0; i < n; ++i) {
        const Product& p = products[i];
        string name = p.getName();
        if (name.size() > 24) name = name.substr(0, 21) + "...";

        cout << left
             << setw(4) << p.getId()
             << setw(25) << name
             << setw(10) << p.getQuantity()
             << setw(10) << fixed << setprecision(2) << p.getUnitPrice()
             << p.getDescription() << "\n";
    }
}

void addProduct(vector<Product>& products, const Product& p) {
    products.push_back(p);
}

bool updateProduct(vector<Product>& products, int id, const Product& p) {
    int n = (products.size());
    for (int i = 0; i < n; ++i) {
        if (products[i].getId() == id) {
            products[i] = p;
            return true;
        }
    }
    return false;
}

bool deleteProduct(vector<Product>& products, int id) {
    for (int i = 0; i < (products.size()); ++i) {
        if (products[i].getId() == id) {
            products.erase(products.begin() + i);
            return true;
        }
    }
    return false;
}
