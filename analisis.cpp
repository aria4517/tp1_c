#include <iostream>
#include <cstdio>
using namespace std;

//1-- Determinar **el vendedor que más dinero generó** (mostrar su nombre y monto total).
//2- Determinar **la sucursal que más dinero generó** (sumando las ventas de todos los vendedores de esa sucursal).
//3-- Mostrar **un ranking de los productos más vendidos** (por cantidad de veces que se vendió cada código de producto).

struct Venta {
    int fecha;
    int codigoVendedor;
    int codigoProducto;
    float monto;
};

struct Vendedor {
    int codigo;
    char nombre[50];
    char sucursal [50];
};







int main(){
    Vendedor vendedores[15]; // máx. 15 vendedores
    int cantVendedores = 0;

    Venta ventas[1000]; //más. 1000 ventas x dia
    int cantVentas=0;


    //abre archivo para leer
    FILE*archivoVendedores = fopen("vendedores.dat", "rb"); // modo lectura binaria
    if (archivoVendedores == NULL) {
        cout<<"No se pudo abrir vendedores.dat"<<endl;
        return 1; // sale si no existe
    }

    // Lee todos los vendedores en el arreglo
    while (fread(&vendedores[cantVendedores], sizeof(Vendedor), 1, archivoVendedores) == 1) {
        cantVendedores++;
    }

    fclose(archivoVendedores);


    //para leer todas las ventas por dia
    FILE*archivoVentas= fopen("ventas_diarias.dat", "rb");
    if (archivoVentas == NULL){
        cout<<"No se pudo abrir ventas_diarias.dat"<<endl;
        return 1;
    }

    while (fread(&ventas[cantVentas], sizeof(Venta), 1, archivoVentas)==1){
        cantVentas++; //incrementa cantVentas en cada lectura
    }
    fclose(archivoVentas);


    //Acumular ventas por vendedor
    float totalVentasXVendedor[15]={0}; //son 15 vendedores y en principio ninguno hizo ninguna venta

    //Recorrer todas las ventas
    for (int i = 0; i < cantVentas; i++) {
        int codigo = ventas[i].codigoVendedor;

        //Buscar en el arreglo de vendedores el índice que corresponde a este código
        for (int j = 0; j < cantVendedores; j++) {
            if (vendedores[j].codigo == codigo) {
                totalVentasXVendedor[j] += ventas[i].monto; // sumar el monto al vendedor correspondiente
                break; // cuando lo encuentro salgo del for
            }
        }
    }

    //Acumular ventas por sucursal
    float totalVentasXSucursal[3]={0}; //son 3 sucursales y en principio en ninguna se hizo ninguna venta
    int cantSucursales=0;
    char sucursales[3][50]; 

    //Recorrer todas las sucursales
    for (int i=0; i<cantVentas; i++){
        int codigoVen=ventas[i].codigoVendedor;
        float monto = ventas[i].monto;

        //busco indice del vendedor
        int idxVend = -1;
        for (int j = 0; j < cantVendedores; j++) {
            if (vendedores[j].codigo == codigoVen) {
                idxVend = j;
                break;
            }
        }
        if (idxVend == -1) continue; // vendedor no encontrado (por si acaso)

        // Buscar la sucursal del vendedor
        char* sucursalVend = vendedores[idxVend].sucursal;

        // Buscar si ya está esta sucursal en el arreglo sucursales[]
        int idxSuc = -1;
        for (int k = 0; k < cantSucursales; k++) {
            // Comparar cadenas manualmente (función iguales)
            bool igual = true;
            for (int c = 0; c < 50; c++) {
                if (sucursales[k][c] != sucursalVend[c]) {
                    igual = false;
                    break;
                }
                if (sucursales[k][c] == '\0' && sucursalVend[c] == '\0') break;
            }
            if (igual) {
                idxSuc = k;
                break;
            }
        }

        // Si no está, agregarla
        if (idxSuc == -1) {
            // Copiar la sucursal nueva al arreglo
            for (int c = 0; c < 50; c++) {
                sucursales[cantSucursales][c] = sucursalVend[c];
                if (sucursalVend[c] == '\0') break;
            }
            idxSuc = cantSucursales;
            cantSucursales++;
        }

        // Acumular monto en la sucursal
        totalVentasXSucursal[idxSuc] += monto;
    }

    // Mostrar sucursal que más vendió
    int idxMejorSucursal = 0;
    for (int i = 1; i < cantSucursales; i++) {
        if (totalVentasXSucursal[i] > totalVentasXSucursal[idxMejorSucursal]) {
            idxMejorSucursal = i;
        }
    }

    cout << "La sucursal que mayor cantidad de ventas hizo fue: " << sucursales[idxMejorSucursal] << " con $" << totalVentasXSucursal[idxMejorSucursal] << endl;

    //ranking de productos más vendidos
    int codigosProd[1000]; //codigos de productos distintos
    int cantProd[1000]; //cant. vendida de c/ código
    int cantProductos = 0; //cuántos productos disntintos se encuentran

    for (int i = 0; i < cantVentas; i++) {
        int codigoProducto = ventas[i].codigoProducto;

        //ver si el producto ya está registrado
        int idxProd = -1;
        for (int j = 0; j < cantProductos; j++) {
            if (codigosProd[j] == codigoProducto) {
                idxProd = j;
                break;
            }
        }

        if (idxProd == -1) {
            //si hay un nuevo producto lo agrega al arreglo
            codigosProd[cantProductos] = codigoProducto;
            cantProd[cantProductos] = 1;
            cantProductos++;
        } else {
            //si el producto ya está registrado, lo suma
            cantProd[idxProd]++;
        }
    }

    // Ordenar el ranking de mayor a menor cantidad vendida
    for (int i = 0; i < cantProductos - 1; i++) {
        for (int j = i + 1; j < cantProductos; j++) {
            if (cantProd[j] > cantProd[i]) {
                //intercambiar cantidades
                int auxCant = cantProd[i];
                cantProd[i] = cantProd[j];
                cantProd[j] = auxCant;

                //intercambiar código
                int auxCod = codigosProd[i];
                codigosProd[i] = codigosProd[j];
                codigosProd[j] = auxCod;
            }
        }
    }

    // Mostrar ranking
    cout << "\n📦 Ranking de productos más vendidos:\n";
    for (int i = 0; i < cantProductos; i++) {
        cout << "Producto " << codigosProd[i]
             << " → " << cantProd[i] << " ventas" << endl;
    }

    return 0;



        

    
}
