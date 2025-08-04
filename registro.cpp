#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

// Estructura para un vendedor
struct Vendedor {
    int codigo;
    char nombre[50];
    char sucursal[50];
};

// Función para verificar si un código ya existe en el archivo
bool codigoExiste(int codigoBuscado) {
    FILE* archivo = fopen("vendedores.dat", "rb");
    if (archivo == NULL) return false;

    Vendedor v;
    while (fread(&v, sizeof(Vendedor), 1, archivo)) {
        if (v.codigo == codigoBuscado) {
            fclose(archivo);
            return true;
        }
    }
    fclose(archivo);
    return false;
}

int main() {
    FILE* archivo = fopen("vendedores.dat", "ab"); // Append binario
    if (archivo == NULL) {
        cout << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    int cantidad = 0;
    char opcion;

    do {
        if (cantidad >= 15) {
            cout << "Se alcanzó el máximo de 15 vendedores." << endl;
            break;
        }

        Vendedor nuevo;

        cout << "Ingrese código del vendedor: ";
        cin >> nuevo.codigo;

        if (codigoExiste(nuevo.codigo)) {
            cout << "Error: ese código ya está registrado." << endl;
            continue;
        }

        cout << "Ingrese nombre del vendedor: ";
        cin.ignore(); // Limpiar buffer
        cin.getline(nuevo.nombre, 50);

        cout << "Ingrese nombre de la sucursal: ";
        cin.getline(nuevo.sucursal, 50);

        fwrite(&nuevo, sizeof(Vendedor), 1, archivo);
        cout << "Vendedor registrado correctamente.\n" << endl;

        cantidad++;

        cout << "¿Desea ingresar otro vendedor? (s/n): ";
        cin >> opcion;
        cout << endl;

    } while (opcion == 's' || opcion == 'S');

    fclose(archivo);
    return 0;
}
