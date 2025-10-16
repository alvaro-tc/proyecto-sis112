#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "lib/productos.h"

using namespace std;

int main(){
    vector<Product> products;

    while (true) {
        cout << "\n--- Menu Productos ---\n";
        cout << "1) Listar productos\n2) Agregar producto\n3) Editar producto\n4) Eliminar producto\n0) Salir\n";
        int opt;
        if (!(cin >> opt)) {
            cout << "Opcion invalida.\n";
            return 0;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (opt == 0) break;

        if (opt == 1) {
            listProducts(products);
        }
        else if (opt == 2) {
            Product p;
            int maxid = 0;
            for (int i = 0; i < (int)products.size(); ++i)
                if (products[i].getId() > maxid) maxid = products[i].getId();
            p.setId(maxid + 1);

            string name, desc;
            int qty;
            double price;

            cout << "Nombre: ";
            getline(cin, name);
            p.setName(name);

            cout << "Descripcion: ";
            getline(cin, desc);
            p.setDescription(desc);

            cout << "Cantidad: ";
            cin >> qty;
            p.setQuantity(qty);

            cout << "Precio unitario: ";
            cin >> price;
            p.setUnitPrice(price);
            


            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            addProduct(products, p);
            cout << "Producto agregado con ID " << p.getId() << "\n";
        }
        else if (opt == 3) {
            cout << "ID a editar: ";
            int id;
            if (!(cin >> id)) { cout << "ID invalido.\n"; return 0; }


            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            bool found = false;
            for (int i = 0; i < (int)products.size(); ++i)
                if (products[i].getId() == id) { found = true; break; }
            if (!found) { cout << "Producto no encontrado.\n"; continue; }

            Product p;
            p.setId(id);

            string name, desc;
            int qty;
            double price;

            cout << "Nuevo nombre: ";
            getline(cin, name);
            p.setName(name);

            cout << "Nueva descripcion: ";
            getline(cin, desc);
            p.setDescription(desc);

            cout << "Nueva cantidad: ";
            cin >> qty;
            p.setQuantity(qty);

            cout << "Nuevo precio unitario: ";
            cin >> price;
            p.setUnitPrice(price);


            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (updateProduct(products, id, p)) cout << "Actualizado.\n";
            else cout << "Error al actualizar.\n";
        }
        else if (opt == 4) {
            cout << "ID a eliminar: ";
            int id;
            cin >> id;


            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (deleteProduct(products, id)) cout << "Eliminado.\n";
            else cout << "No se encontro el ID.\n";
        }
        else {
            cout << "Opcion invalida.\n";
        }
    }

    return 0;
}