#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>
#include<windows.h>
#include <iostream>
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
using namespace std;


//PROTOTIPO DE LAS FUNCIONES
void CursorOn(bool visible, DWORD size); //tamaño del cursor y parpadie 
void CursorOff();  //el cursor estara apagado 
void gotoxy(int x,int y); // mueve el cursor a la pocision indicada
void dibujarCuadro(int x1,int y1,int x2,int y2);
void limpia(); //limpia el area de la pantalla
void menu();
void ingreso();
void actualizar();
void comprar();


//Estructuras para almacenar en el archivo
struct estudiante{
	char nombre[35];
	char cedula[15];
	char facultad[20];
	char carrera[40];
	char fechadenac[10];
	char sexo[15];
	char estadocivil[15];
	char telefono[15];
	char direccion[25];
};

struct compra{
	char cedula[15];
	char nombre[35];
	float valorinicial;
	float transacc;
};


//FUNCION PRINCIPAL MAIN: EN ESTA FUNCION PRINCIPAL AQUI SE PUEDEN LLAMAR A LAS DEMAS FUNCIONES
int main(){
	system("mode con: cols=80 lines=25");
	char menu[3][15]= {"ESTUDIANTES","PEDIDOS","CERRAR"};
	char submenu[4][22]= {"1. REGISTRAR","2. ACTUALIZAR","1. COMPRAR","2. ATRAS"};
	int tecla, col=4, i, indice=0, fila=8, op,opc, opc2, opc1; //ociones de filas
	int a,b;
	system("COLOR C0");
	dibujarCuadro(1,1,78,24);//Cuadro grande
	dibujarCuadro(2,2,77,4);//Cuadro titulo
	gotoxy(16,3);	//cordenadas de la pantalla
	cout << "     C O M E D O R      U N I V E R S I T A R I O";

	//Imprimir menu en pantalla
	for (i=0; i<3;i++){
		gotoxy(col,6);cout << menu[i];
		col+=30;}

	col=4; i=1;

	do{
	CursorOn(1,10);
	gotoxy(4,9); cout << "MENU PRINCIPAL";
	gotoxy(4,11); cout << "(1) Estudiantes";
	gotoxy(4,13); cout << "(2) Pedidos";
	gotoxy(4,15); cout << "(3) Cerrar";
	gotoxy(4,17); cout << "Ingrese opcion: ";
	scanf("%i",&op);
					
				if (op==1){
				limpia();
				for (i=0; i<2;i++){
					gotoxy(4,fila);cout <<submenu[i];
					fila+=2; }
				fila=8;
				CursorOn(1,10);
				gotoxy(4,13); cout << "Ingrese opcion: ";
				scanf("%i",&opc1);

				if (opc1==1) ingreso();
				if (opc1==2) actualizar();
				if (opc1< 1 || opc1>2) limpia(); }

				if(op==2){
					limpia();
					for (i=2; i<4;i++){
					gotoxy(34,fila);cout << submenu[i];
					fila+=2; }
				fila=8;

				CursorOn(1,10);
				gotoxy(34,13); cout << "Ingrese opcion: ";
				scanf("%i",&opc2);

				if (opc2==1) comprar();

				if (opc2< 1 || opc2>1) limpia();
				} 
				
				if (op == 3) exit(0); 
				if (op < 1 || op >2){
					gotoxy(4,20); cout << "Opcion no valida. ";
					gotoxy(4,17); cout << "                                           ";}
				
	}while(i); //bucle que permite regresar
	return 0;
}


// FUNCION INGRESO DE DATOS Y ALMACENAMIENTO EN ARCHIVO
void ingreso(){
	FILE *pa;
	char r;
	estudiante nuevo, lista[50];
	limpia();
	if ((pa = fopen("E2","a+b"))==NULL)
	{ cout << "No se puede abrir el archivo.\n";
		return ;
		}

	int encontro=1,i,ultimo,res;

	rewind(pa); i=0; //rewind es equivalente a void
	while(!feof(pa))
	{ if(fread(&nuevo, sizeof(estudiante), 1, pa)){
		lista[i]=nuevo;
		i++;}
	}

	fflush(stdin); //limpia el bufer 
	gotoxy(4,8);cout << "REGISTRO DE ESTUDIANTE";
	gotoxy(4,10);cout << "Nombres y apellidos:  "; gets(nuevo.nombre);

	gotoxy(4,11);cout << "Codigo:                  ";
		res=i;
		do{
			gotoxy(26,11); gets(nuevo.cedula);
			//proceso de busqueda de la informacion
			ultimo=res-1;
			i=0;
			encontro=1;
			while (i<=ultimo && encontro){
				if (strcmp(lista[i].cedula,nuevo.cedula)==0) encontro=0;
				else i++;
			}
			if (i<=ultimo){
				gotoxy(26,11);cout << "                              ";
				gotoxy(4,22);cout << "Codigo ya registrado. Ingrese nuevamente.";}
			else {gotoxy(4,22); cout << "                                               ";}
		}
		while(i<=ultimo);

	fflush(stdin);
	gotoxy(4,12);cout << "Nivel:                "; gets(nuevo.facultad);
	gotoxy(4,13);cout << "Carrera:              "; gets(nuevo.carrera);
	gotoxy(4,14);cout << "Edad:                 "; gets(nuevo.fechadenac);
	gotoxy(4,15);cout << "Sexo:                 "; gets(nuevo.sexo);
	gotoxy(4,16);cout << "Estado Civil:         "; gets(nuevo.estadocivil);
	gotoxy(4,17);cout << "Telefono:             "; gets(nuevo.telefono);
	gotoxy(4,18);cout << "Direccion:            "; gets(nuevo.direccion);
	gotoxy(4,20);cout << "¨Registrar Nuevo Estudiante? Si[s] / No [n]: "; r=getch();
	if (r=='s' || r=='S') {
		fwrite(&nuevo, sizeof(estudiante),1,pa); //fwrite direccion de memoria de datos
		gotoxy(4,22);cout << "Estudiante Registrado. Presione una tecla para volver..";}
	else {gotoxy(4,22);cout << "Estudiante NO Registrado. Presione una tecla para volver..";}
	CursorOff();
	getch();
	limpia();
	fclose(pa); } //FCLOSE ES PARA CERRAR LOS DATOS DE LA MEMORIA
	
	
// FUNCION MODIFICAR DATOS ALMACENADOS
void actualizar()
{ estudiante nuevo, lista[50];
  char cedula[15];
  int encontro=1,linea,i,ultimo;
  char r;
  FILE *pa;
  limpia();
  if ((pa = fopen("E2", "a+b"))== NULL)
      { cout << "No se puede abrir el archivo.\n";
	return;
      }
  rewind(pa); i=0;
  while(!feof(pa))
  { if(fread(&nuevo, sizeof(estudiante), 1, pa)){
	 lista[i]=nuevo;
	 i++;
	 }
  }
  fclose(pa);
 

  //proceso de busqueda de la informaci¢n
  gotoxy(4,8);cout << "ACTUALIZAR DATOS";
  gotoxy(4,10);
  fflush(stdin);
  cout << "Ingrese Numero De Codigo: ";gets(cedula);
  ultimo=i-1;
  i=0;
  while (i<=ultimo && encontro)
  { if (strcmp(lista[i].cedula,cedula)==0) encontro=0;
    else i++;
  }
  if (i<=ultimo)
  { do {
    limpia();
    gotoxy(4,8);cout << "ACTUALIZAR DATOS";
    gotoxy(4,10);cout << "1. Nombre:                 " << lista[i].nombre;
    gotoxy(4,11);cout << "2. Codigo:                 " << lista[i].cedula;
    gotoxy(4,12);cout << "3. Nivel:                  " << lista[i].facultad;
    gotoxy(4,13);cout << "4. Carrera:                " << lista[i].carrera;
    gotoxy(4,14);cout << "5. Edad:                   " << lista[i].fechadenac;
    gotoxy(4,15);cout << "6. Sexo:                   " << lista[i].sexo;
    gotoxy(4,16);cout << "7. Estado Civil:           " << lista[i].estadocivil;
    gotoxy(4,17);cout << "8. Telefono:               " << lista[i].telefono;
    gotoxy(4,18);cout << "9. Direccion:              " << lista[i].direccion;
    
    gotoxy(4,20);cout << "Escriba la linea a modificar: ";
    do{
       gotoxy(34,20);scanf("%d",&linea);
       if(linea<1 || linea>9) {
       	gotoxy(34,20);cout << "          ";
       	gotoxy(4,22);cout << "Opcion no valida. Ingrese nuevamente..";
	   }
    }while(linea<1 || linea>9);
    fflush(stdin);
    switch(linea){
    case 1: gotoxy(4,10 );cout << "                                                       ";
	    gotoxy(4,22);cout << "                                           ";
		gotoxy(4,10 );
	    cout << "1. Nombre:                 ";
	    gets(lista[i].nombre);
	    break;
    case 2: gotoxy(4,11 );cout << "                                            ";
	    gotoxy(4,22);cout << "                                           ";
		gotoxy(4,11 );
	    cout << "2. Codigo:                 ";
	    gets(lista[i].cedula);
	    break;
    case 3: gotoxy(4,12);cout << "                                                ";
	    gotoxy(4,22);cout << "                                           ";
		gotoxy(4,12);
	    cout << "3. Nivel:                  ";
	    gets(lista[i].facultad);
	    break;
    case 4: gotoxy(4,13);cout << "                                                       ";
	    gotoxy(4,22);cout << "                                           ";
		gotoxy(4,13);
	    cout << "4. Carrera:                ";
	    gets(lista[i].carrera);
	    break;
	case 5: gotoxy(4,14);cout << "                                               ";
	    gotoxy(4,22);cout << "                                           ";
		gotoxy(4,14);
	    cout << "5. Edad:                   ";
	    gets(lista[i].fechadenac);
	    break;
	case 6: gotoxy(4,15);cout << "                                              ";
	    gotoxy(4,22);cout << "                                           ";
		gotoxy(4,15);
	    cout << "6. Sexo:                   ";
	    gets(lista[i].sexo);
	    break;
	case 7: gotoxy(4,16);cout << "                                                 ";
	    gotoxy(4,22);cout << "                                           ";
		gotoxy(4,16);
	    cout << "7. Estado Civil:           ";
	    gets(lista[i].estadocivil);
	    break;
	case 8: gotoxy(4,17);cout << "                                                  ";
	    gotoxy(4,22);cout << "                                           ";
		gotoxy(4,17);
	    cout << "8. Telefono:               ";
	    gets(lista[i].telefono);
	    break;
	case 9: gotoxy(4,18);cout << "                                                     ";
	    gotoxy(4,22);cout << "                                           ";
		gotoxy(4,18);
	    cout << "9. Direccion:              ";
	    gets(lista[i].direccion);
    }
    gotoxy(4,20);cout << "Actualizacion Correcta.                  ";
    gotoxy(4,21);cout << "¨Desea Seguir Actualizando? Si[s] / No[n]: ";r=getch();
    }while (r!='n' && r!='N');
    // Abrir el archivo para Sobrescribir
    if ((pa = fopen("E2", "wb"))== NULL)
      { cout << "No se puede abrir el archivo.\n";
	return;
      }
    i=0;
    while(i<=ultimo)
    { fwrite(&lista[i], sizeof(estudiante), 1, pa);
      i++;
    }
    gotoxy(4,23);cout << "Presione una tecla para volver...                   ";
    CursorOff();
    getch();limpia();
  }
  else
  { gotoxy(4,21);cout << "Usuario NO registrado.";
    gotoxy(4,23);cout << "Presione una tecla para volver... ";
    CursorOff();
    getch();limpia();
  }
fclose(pa);

}


//FUNCION PERMITE REALIZAR LAS COMPRAS EN EL BAR
void comprar(){
  limpia();
  gotoxy(4,8);cout << "COMPRA >>";

  estudiante nuevo, lista[50];
  char cedula[15];
  int encontro=1,i,ultimo,op,r;
  float total=0, total2=0,t3,cant,tc,sumacred;
  FILE *pa;
  if ((pa = fopen("E2", "a+b"))== NULL)
      { cout << "No se puede abrir el archivo.\n";
	return;
      }
  rewind(pa); i=0;
  while(!feof(pa))
  { if(fread(&nuevo, sizeof(estudiante), 1, pa)){
	 lista[i]=nuevo;
	 i++;
	 }
  }
  fclose(pa);

  //proceso de busqueda de la informaci¢n
  gotoxy(4,10);
  fflush(stdin);
  cout << "Ingrese Numero De Codigo: ";gets(cedula);
  ultimo=i-1;
  i=0;
  while (i<=ultimo && encontro)
  { if (strcmp(lista[i].cedula,cedula)==0) encontro=0;
    else i++;
  }
  if (i<=ultimo)
  {
    gotoxy(4,10);cout << "                                              ";
    gotoxy(13,8);cout << " Estudiante: " << lista[i].nombre;

  //Almacenar datos en estructura 2
  char cedula2[15], nombre[35];
  strcpy(cedula2,cedula);
  strcpy(nombre, lista[i].nombre);

  FILE *da;
  compra nuevot, listat[50];
  if ((da = fopen("E3","a+b"))==NULL)
  { cout << "No se puede abrir el archivo.";
  return ;
  }
  rewind(da); i=0;
  while(!feof(da))
  { if(fread(&nuevot, sizeof(compra), 1, da)){
	 listat[i]=nuevot;
	 i++;
	 }
  }
  encontro=1;
  ultimo=i-1;
  i=0;
  while (i<=ultimo && encontro)
  { if (strcmp(listat[i].cedula,cedula2)==0) encontro=0;
    else i++;
  }
  if (i<=ultimo){
	gotoxy(50,8); printf("   Credito: $ %.3f",listat[i].valorinicial);

	do{
	
	//MENU
	gotoxy(4,11);cout << "MENU DISPONIBLE";
	gotoxy(4,13);cout << "1) Almuerzo del dia  $ 8.000";
	gotoxy(4,14);cout << "2) Sancocho          $ 6.000";
	gotoxy(4,15);cout << "3) Medio Pollo Asado $ 9.000";
	gotoxy(4,16);cout << "4) Ensalada de fruta $ 3.500";
	gotoxy(4,17);cout << "5) Sanduche          $ 1.000";
	gotoxy(4,18);cout << "6) Jugo              $ 1.000";
	gotoxy(4,19);cout << "7) Yogurt            $ 1.500";
	gotoxy(4,20);cout << "8) Cafe              $ 700";
	gotoxy(4,21);cout << "9) Agua              $ 0";

	gotoxy(39,11);cout << "Ingrese opcion del menu: "; 
	
	do{ gotoxy(65,11); scanf("%i",&op);
	}while (op <1 || op > 9);
	fflush(stdin);
	switch(op){
		case 1: 
		gotoxy(39,13);cout << "Ingrese cantidad: ";
		scanf("%f",&cant);
		tc = cant * 8.000; 
		total += tc;
		break;
		case 2: 
		gotoxy(39,13);cout << "Ingrese cantidad: ";
		scanf("%f",&cant);
		tc = cant * 6.000;
		total += tc;
		break;
		case 3: 
		gotoxy(39,13);cout << "Ingrese cantidad: ";
		scanf("%f",&cant);
		tc = cant * 9.000;
		total += tc;
		break;
		case 4: 
		gotoxy(39,13);cout << "Ingrese cantidad: ";
		scanf("%f",&cant);
		tc = cant * 3.500;
		total += tc;
		break;
		case 5: 
		gotoxy(39,13);cout << "Ingrese cantidad: ";
		scanf("%f",&cant);
		tc = cant * 1.000;
		total += tc;
		break;
		case 6: 
		gotoxy(39,13);cout << "Ingrese cantidad: ";
		scanf("%f",&cant);
		tc = cant * 1.000;
		total += tc;
		break;
		case 7: 
		gotoxy(39,13);cout << "Ingrese cantidad: ";
		scanf("%f",&cant);
		tc = cant * 1.500;
		total += tc;
		break;	
		case 8: 
		gotoxy(39,13);cout << "Ingrese cantidad: ";
		scanf("%f",&cant);
		tc = cant * 700;
		total += tc;
		break;	
		case 9: 
		gotoxy(39,13);cout << "Ingrese cantidad: ";
		scanf("%f",&cant);
		tc = cant * 0;
		total += tc;
		break;			
	}
	
	
	
	gotoxy(39,15);cout << "Desea Seguir Pidiendo? Si[s] / No[n]: ";r=getch();
	if(r== 'S' || r=='s') { 
	gotoxy(65,11); cout << "        ";
	gotoxy(39,15);cout << "                                      ";}
    }while (r!='n' && r!='N');
	
	if (total > listat[i].valorinicial){
		gotoxy(4,23); cout << "Credito insuficiente para realizar el pedido";
		CursorOff();
		getch();}
	else{
	gotoxy(39,11);cout << "                                       ";
	gotoxy(39,13);cout << "                                       ";
	gotoxy(39,15);cout << "                                       ";
	gotoxy(39,11); printf("Total del pedido: $%6.3f",total);
	gotoxy(39,13); printf("Ingresar valor a pagar: "); scanf("%f",&listat[i].transacc);

	if (listat[i].transacc <= total){
	total2 = total - listat[i].transacc;
	listat[i].valorinicial -= total2;
	t3 = listat[i].transacc - total;}
	
	else{
		t3 = listat[i].transacc - total;
		sumacred = t3 + listat[i].valorinicial;
		
	if (sumacred < 30)
		listat[i].valorinicial = sumacred;	
	else{
	
		gotoxy(39,15); printf("Vuelto: $%6.3f",t3);
		gotoxy(39,17); cout << "No se puede abonar mas";
		gotoxy(39,19); cout << "Maximo $30.000 de credito";
		getch();} }	
   
	
	
	
	
	fflush(stdin);
	
	limpia();
	
	if (listat[i].transacc > total && sumacred <30){
	
	dibujarCuadro(13,8,63,19);
	gotoxy(20,10); cout << "        C O M P R O B A N T E  ";
	gotoxy(20,12);cout << "Codigo:           " <<  listat[i].cedula;
	gotoxy(20,13);cout << "Nombre:           " << listat[i].nombre;
	gotoxy(20,14);printf("Valor de Pedido:  $ %6.3f",total);
	gotoxy(20,15);printf("Su cambio es:     $ %6.3f",t3);
	gotoxy(20,16);printf("Valor Pagado:     $ %6.3f",listat[i].transacc);
	gotoxy(20,17);printf("Credito total:    $ %6.3f",listat[i].valorinicial);	

	fwrite(&listat[i], sizeof(compra),1,da);
	fclose(da); }
	
	else{
	dibujarCuadro(13,8,63,19);
	gotoxy(20,10); cout << "        C O M P R O B A N T E  ";
	gotoxy(20,12);cout << "Codigo:           " << listat[i].cedula;
	gotoxy(20,13);cout << "Nombre:           " << listat[i].nombre;
	gotoxy(20,14);printf("Valor de Pedido:  $ %6.3f",total);
	gotoxy(20,15);printf("Valor Pagado:     $ %6.3f",listat[i].transacc);
	gotoxy(20,16);printf("Credito total:    $ %6.3f",listat[i].valorinicial);	

	fwrite(&listat[i], sizeof(compra),1,da);
	fclose(da); 
	}


	//Abrir el archivo para sobreescribir
	if ((da = fopen("E3", "wb"))== NULL){
		cout << "No se puede abrir el archivo.";
		return;}
	i=0;
	while(i<=ultimo){
		fwrite(&listat[i], sizeof(compra), 1, da);
		i++;}
	fclose(da);
}}

  else{
	strcpy(nuevot.cedula,cedula2);
	strcpy(nuevot.nombre,nombre);

	nuevot.valorinicial = 30;
	gotoxy(50,8);printf("   Credito: $ %.3f",nuevot.valorinicial);
	
	do{
	
	//MENU
	gotoxy(4,11);cout << "MENU DISPONIBLE";
	gotoxy(4,13);cout << "1) Almuerzo del dia  $ 8.000";
	gotoxy(4,14);cout << "2) Sancocho          $ 6.000";
	gotoxy(4,15);cout << "3) Medio Pollo Asado $ 9.000";
	gotoxy(4,16);cout << "4) Ensalada de fruta $ 3.500";
	gotoxy(4,17);cout << "5) Sanduche          $ 1.000";
	gotoxy(4,18);cout << "6) Jugo              $ 1.000";
	gotoxy(4,19);cout << "7) Yogurt            $ 1.500";
	gotoxy(4,20);cout << "8) Cafe              $ 700";
	gotoxy(4,21);cout << "9) Agua              $ 0";
	

	gotoxy(39,11);cout << "Ingrese opcion del menu: "; 
	
	do{ gotoxy(65,11); scanf("%i",&op);
	}while (op <1 || op > 9);
	
	
	fflush(stdin);
	switch(op){
		case 1: 
		gotoxy(39,13);cout << "Ingrese cantidad: ";
		scanf("%f",&cant);
		tc = cant * 8.000; 
		total += tc;
		break;
		case 2: 
		gotoxy(39,13);cout << "Ingrese cantidad: ";
		scanf("%f",&cant);
		tc = cant * 6.000;
		total += tc;
		break;
		case 3: 
		gotoxy(39,13);cout << "Ingrese cantidad: ";
		scanf("%f",&cant);
		tc = cant * 9.000;
		total += tc;
		break;
		case 4: 
		gotoxy(39,13);cout << "Ingrese cantidad: ";
		scanf("%f",&cant);
		tc = cant * 3.500;
		total += tc;
		break;
		case 5: 
		gotoxy(39,13);cout << "Ingrese cantidad: ";
		scanf("%f",&cant);
		tc = cant * 1.000;
		total += tc;
		break;
		case 6: 
		gotoxy(39,13);cout << "Ingrese cantidad: ";
		scanf("%f",&cant);
		tc = cant * 1.000;
		total += tc;
		break;
		case 7: 
		gotoxy(39,13);cout << "Ingrese cantidad: ";
		scanf("%f",&cant);
		tc = cant * 1.500;
		total += tc;
		break;	
		case 8: 
		gotoxy(39,13);cout << "Ingrese cantidad: ";
		scanf("%f",&cant);
		tc = cant * 700;
		total += tc;
		break;	
		case 9: 
		gotoxy(39,13);cout << "Ingrese cantidad: ";
		scanf("%f",&cant);
		tc = cant * 0;
		total += tc;
		break;			
	}
	
	
	gotoxy(39,15);cout << "Desea Seguir Pidiendo? Si[s] / No[n]: ";r=getch();
	if(r== 'S' || r=='s') { 
	gotoxy(65,11); cout << "        ";
	gotoxy(39,15);cout << "                                      ";}
    }while (r!='n' && r!='N');
	
	if (total > nuevot.valorinicial){
		gotoxy(4,23); cout << "Credito insuficiente para realizar el pedido";
		CursorOff();
		getch();}
	else{
	gotoxy(39,11);cout << "                                       ";
	gotoxy(39,13);cout << "                                       ";
	gotoxy(39,15);cout << "                                       ";
	gotoxy(39,11); printf("Total del pedido: $%6.2f",total);
	gotoxy(39,13); cout << "Ingresar valor a pagar: "; scanf("%f",&nuevot.transacc);

	if (nuevot.transacc <= total){
	total2 = total - nuevot.transacc;
	nuevot.valorinicial -= total2;
	t3 = nuevot.transacc - total;}
	
	else{
		t3 = nuevot.transacc - total;
		sumacred = t3 + nuevot.valorinicial;
		
	if (sumacred < 30)
		nuevot.valorinicial = sumacred;	
	else	
		gotoxy(39,15); printf("Vuelto: $%6.2f",t3);
		gotoxy(39,17); cout << "No se puede abonar mas";
		gotoxy(39,19); cout << "Maximo $30.000 de credito";
		getch(); }	
   
	
	
	
	
	fflush(stdin);
	
	limpia();
	
	if (nuevot.transacc > total && sumacred <30){
	
	dibujarCuadro(13,8,63,19);
	gotoxy(20,10); cout << "        C O M P R O B A N T E  ";
	gotoxy(20,12);cout << "Codigo:           " << nuevot.cedula;
	gotoxy(20,13);cout << "Nombre:           " << nuevot.nombre;
	gotoxy(20,14);printf("Valor de Pedido:  $ %6.2f",total);
	gotoxy(20,15);printf("Su cambio es:     $ %6.2f",t3);
	gotoxy(20,16);printf("Valor Pagado:     $ %6.2f",nuevot.transacc);
	gotoxy(20,17);printf("Credito total:    $ %6.2f",nuevot.valorinicial);	

	fwrite(&nuevot, sizeof(compra),1,da);
	fclose(da); }
	
	else{
	dibujarCuadro(13,8,63,19);
	gotoxy(20,10); cout << "        C O M P R O B A N T E  ";
	gotoxy(20,12);cout << "Codigo:           " << nuevot.cedula;
	gotoxy(20,13);cout << "Nombre:           " << nuevot.nombre;
	gotoxy(20,14);printf("Valor de Pedido:  $ %6.2f",total);
	gotoxy(20,15);printf("Valor Pagado:     $ %6.2f",nuevot.transacc);
	gotoxy(20,16);printf("Credito total:    $ %6.2f",nuevot.valorinicial);	

	fwrite(&nuevot, sizeof(compra),1,da);
	fclose(da); 
	}

}}

  gotoxy(4,23);cout << "                                                           ";
  gotoxy(4,23);cout << "Presione una tecla para volver... ";
  CursorOff();
  getch();limpia();}
  else{
      gotoxy(4,20);cout << "Estudiante NO registrado en el sistema.";
      gotoxy(4,22);cout << "Presione una tecla para volver... ";
      CursorOff;
      getch();limpia();}

}


// Funcion para ocultar el cursor
void CursorOff()
{
  CONSOLE_CURSOR_INFO cursor = {1, FALSE};
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

//Funcion para aparecer cursor
void CursorOn(bool visible, DWORD size) // set bool visible = 0 - invisible, bool visible = 1 - visible
{
	if(size == 0)
	{
		size = 20;	
// tamaño predeterminado del cursor Al cambiar a números del 1 al 20, disminuye el ancho del cursor
	}
	CONSOLE_CURSOR_INFO lpCursor;	
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console,&lpCursor);
}


//Funcion Gotoxy
void gotoxy(int x,int y){  
      HANDLE hcon;  
      hcon = GetStdHandle(STD_OUTPUT_HANDLE);  
      COORD dwPos;  
      dwPos.X = x;  
      dwPos.Y= y;  
      SetConsoleCursorPosition(hcon,dwPos);  
 }




//DIBUJAR CUADRO
void dibujarCuadro(int x1,int y1,int x2,int y2)
{

    int i;
    for (i=x1;i<x2;i++)
    {
	gotoxy(i,y1);cout << "Ä";//linea horizontal superior
	gotoxy(i,y2);cout << "Ä";//linea horizontal inferior
    }

    for (i=y1;i<y2;i++)
    {
	gotoxy(x1,i);cout << "³";//linea vertical izquierda
	gotoxy(x2,i);cout << "³";//linea vertical derecha
    }
    gotoxy(x1,y1);cout << "Ú";
    gotoxy(x1,y2);cout << "À";
    gotoxy(x2,y1);cout << "¿";
    gotoxy(x2,y2);cout << "Ù";
}


//FUNCION LIMPIAR AREA
void limpia(){
	int i,j;
	for(i=8;i<=23;i++){
		for(j=4;j<=74;j++){
			gotoxy(j,i); cout << " ";}}
}
