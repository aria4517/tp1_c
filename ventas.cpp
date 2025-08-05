  #include<iostream>
  #include<cstdio>
  #include<cstring>
  using namespace std;

//Estructura para una venta
  struct Venta{
  	int fecha;
  	int codigoVendedor;
  	int codigoProducto;
  	float monto;
  };
  
  struct Vendedor{
  	int codigo;
  	char nombre[50];
  	char sucursal[50];
  };
  
  //Función que verifica si existe un vendedor con el código dado en el archivo "vendedores.dat".
  bool vendedorExiste(int codigo){
  	FILE* archivo = fopen("vendedores.dat", "rb");
  	if(archivo==NULL) return false;
  	
  	Vendedor v;
  	while(fread(&v, sizeof(Vendedor), 1, archivo) == 1){
  		if(v.codigo==codigo){
  			fclose(archivo);
  			return true;
		  }
	  }
	  fclose(archivo);
	  return false;
  }
  
  int main(){
  	Venta ventas[1000];
  	int cantidad = 0;
  	
  	cout<<"---REGISTRO DE VENTAS DIARIAS---"<<endl;
  	
  	int opcion;
  	do{
		//Si se alcanzó el máximo de 1000 ventas, se detiene
  		if(cantidad>=1000){
  			cout<<"Se alcanzó el máximo de ventas (1000)."<<endl;
  			break;
		  }
		Venta v;
		cout<<"\nVenta "<<(cantidad+1)<<":"<<endl;
		cout<<"Fecha (AAAAMMDD): "; cin>>v.fecha;
		
		cout<<"Código de vendedor: "; cin>>v.codigoVendedor;
		while(!vendedorExiste(v.codigoVendedor)){
			cout<<"Ese vendedor no existe. Ingrese otro código: "; cin>>v.codigoVendedor;
		}
		cout<<"Código de producto: "; cin>>v.codigoProducto;
		cout<<"Monto de la venta: $"; cin>>v.monto;
		
		ventas[cantidad++] = v;
		cout<<"\n¿Desea registrar otra venta? (1: si, 0: no): "; cin>>opcion;
	  }while(opcion!=0);
	  
	  FILE* archivo = fopen("ventas_diarias.dat", "ab");
	  if(archivo!=NULL){
	  	fwrite(ventas, sizeof(Venta), cantidad, archivo);
	  	fclose(archivo);
	  	cout<<"\nVentas guardadas correctamente."<<endl;
	  }else{
	  	cout<<"\nNo se pudo abrir el archivo para guardar ventas."<<endl;
	  }
	  return 0;
  }

