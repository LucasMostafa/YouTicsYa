#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

const char ESC=27;
const char usuario[]="admi";
const char contrasenia[]="admi1";

///Estructuras
typedef struct
{
    int idCliente;
    unsigned long movil;
    char mail[30];
    char apellido[30];
    char nombre[30];
    char domicilio[30];
    char usuario[30];
    char contrasenia[30];
    int bajaCliente; ///1 o 0
} stCliente;

typedef struct
{
    int idPedido;
    int idCliente;
    char fechaPedido[20];
    float costoPedido;
    char descripcionPedido[50];
    int pedidoAnulado;///1 o 0
} stPedido;

///Propotipado
void cargarUnCliente(char archCliente[]);
int comparacion(int a, char archCliente[]);
int comparacionUsuario(char usuario[], char archCliente[]);
int comparacionMail(char mail[], char archCliente[]);
void mostrarUnCliente(stCliente cliente);
void imprimirUsuarioYContrasenia(stCliente cliente);
void mostrarClientes(char archCliente[]);
void mostrarClientesActivos(char archCliente[]);
void mostrarClientesInactivos(char archCliente[]);
stCliente MenuDeModificacion (stCliente A);
stCliente BajaCliente (stCliente A);
stCliente SubMenuDeModificacion (stCliente A);
void ModificarCliente (char archCliente[], int pos);
int posClientePorId(char archCliente[],int id);
int posClientePorUsuario(char archCliente[],char usuario[]);
int BusquedaPorId(char archCliente[]);
int BusquedaPorUsuario(char archCliente[]);
int ingresarOpcion();
char ingresarOpcionChar();
int ingresarUsuarioYContrasenia(char archCliente[]);
int validarUsuarioYContrasenia(char archCliente[], char usuario[], char contrasenia[]);
int posUsuarioYContrasenia(char archCliente[],char mail[]);
int recuperarUsuarioYContrasenia(char archCliente[]);
void mostrarUsuarioYContrasenia (char archCliente[], int pos);
void mostrarClienteEspecifico (char archCliente[], int pos);
void mostrarNombreUsuario (char archCliente[], int pos);
int buscarUltimoIdCargado(char archCliente[]);
void buscarUltimoClienteCargado(char archCliente[]);
int validarId(char archCliente[], int pos);
int posicionIdCliente(char archCliente[], int id);
int contarPedidos(char archPedido[]);
void cargarPedido(char archPedido[], char archCliente[], int pos);
void mostrarUnPedido(stPedido pedido);
void mostrarPedidosDeUnCliente(char archPedido[], int id);
stPedido BajaPedido(stPedido A);
int posicionIdPedido(char archPedido[], int id);
void modificarPedido (char archPedido[], int pos);
stPedido MenuDeModificacionPedido (stPedido A);
stPedido SubMenuDeModificacionPedido (stPedido A);
void RecuperarClienteAdmin (char archCliente[], int pos);
stCliente AltaClienteDadoDeBaja (stCliente A);
void menuInicial(char archCliente[], char archPedido[]);
void menuUsuario(char archCliente[], char archPedido[]);
void menuIniciarSesion(char archCliente[], char archPedido[], int pos);
void menuAdmin(char archCliente[], char archPedido[]);
void imprimirCabecera(char cabecera[]);
int mejorCliente(char archCliente[], char archPedido[]);

int clienteDadoDeBaja (char archCliente[], int pos);
void clienteDadoDeAltaPorUsuario(char archCliente[], int pos);

int main()
{
    char archCliente[]="Cliente.bin";
    char archPedido[]="Pedidos.bin";

    SetConsoleTitle("PEDIDOS: YA TE LO LLEVO!");

    menuInicial(archCliente, archPedido);



    return 0;
}

///CARGAR CLIENTES
void cargarUnCliente(char archCliente[])
{
    stCliente cliente;
    int flag=0;
    int x=1;

    do
    {
        cliente.idCliente= (rand()%x)+1000;
        flag=comparacion(cliente.idCliente, archCliente);
        if (flag==1)
        {
            x=x*10;
        }
    }
    while(flag!=0);

    FILE *puntero;
    puntero= fopen(archCliente,"ab");

    if(puntero!=NULL)
    {
        printf("Carga de nuevo cliente:\n");
        printf("Nombre: ");
        fflush(stdin);
        gets(cliente.nombre);
        printf("Apellido: ");
        fflush(stdin);
        gets(cliente.apellido);
        do
        {
            printf("Mail: ");
            fflush(stdin);
            gets(cliente.mail);
            flag=comparacionMail(cliente.mail, archCliente);
            if(flag==1)
            {
                printf("\nEl mail ingresado ya existe. Debera ingresar uno nuevo.\n");
            }
        }
        while(flag==1);
        do
        {
            printf("M%cvil: ",162);
            scanf("%u", &cliente.movil);
            if(cliente.movil>4294967295)
            {
                printf("\nEl numero ingresado no es valido. Debera ingresar uno nuevo.\n");
            }
        }
        while(cliente.movil>4294967295);
        printf("Domicilio: ");
        fflush(stdin);
        gets(cliente.domicilio);
        cliente.bajaCliente=1;
        do
        {
            printf("Ingrese nombre de usuario: ");
            fflush(stdin);
            gets(cliente.usuario);
            flag=comparacionUsuario(cliente.usuario, archCliente);
            if(flag==1)
            {
                printf("\nEl nombre de usuario ya existe. Debera ingresar uno nuevo.\n");
            }
        }
        while(flag==1);
        printf("Ingrese su contrase%ca: ",164);
        fflush(stdin);
        gets(cliente.contrasenia);
        printf("Su Id de Cliente es: ");
        printf("%d", cliente.idCliente);

        printf("\n\nUsuario creado correctamente.\n\n");
        system("pause");

        fwrite(&cliente, sizeof(cliente), 1, puntero);
    }
    else
    {
        printf("\nNo se pudo abrir el archivo.\n");
    }
    fclose(puntero);
}

///VALIDAR CLIENTES
int comparacion(int a, char archCliente[])
{
    stCliente cliente;
    int flag=0;

    FILE *puntero;
    puntero= fopen(archCliente, "rb");

    if(puntero!=NULL)
    {
        while(flag==0 && fread(&cliente, sizeof(stCliente), 1, puntero)>0)
        {
            if(a==cliente.idCliente)
            {
                flag=1;
            }
        }
    }
    fclose(puntero);
    return flag;
}

int comparacionUsuario(char usuario[], char archCliente[])
{
    stCliente cliente;
    int flag=0;

    FILE *puntero;
    puntero= fopen(archCliente, "rb");

    if(puntero!=NULL)
    {
        while(flag==0 && fread(&cliente, sizeof(stCliente), 1, puntero)>0)
        {
            if(strcmp(usuario, cliente.usuario)==0)
            {
                flag=1;
            }
        }
    }
    fclose(puntero);
    return flag;
}

int comparacionMail(char mail[], char archCliente[])
{
    stCliente cliente;
    int flag=0;

    FILE *puntero;
    puntero= fopen(archCliente, "rb");

    if(puntero!=NULL)
    {
        while(flag==0 && fread(&cliente, sizeof(stCliente), 1, puntero)>0)
        {
            if(strcmp(mail, cliente.mail)==0)
            {
                flag=1;
            }
        }
    }
    fclose(puntero);
    return flag;
}

///Mostrar Clientes

void mostrarUnCliente(stCliente cliente)
{
    printf("\n\n");
    printf("-------------------------------------------------");
    printf("\nId cliente...........: %d\n", cliente.idCliente);
    printf("\nNombre...............: %s\n", cliente.nombre);
    printf("\nApellido.............: %s\n", cliente.apellido);
    printf("\nMail.................: %s\n", cliente.mail);
    printf("\nM%cvil................: %u\n",162, cliente.movil);
    printf("\nDirecci%cn............: %s\n",162, cliente.domicilio);
    printf("\nUsuario..............: %s\n", cliente.usuario);
    printf("\nContrase%ca...........: ********\n",164);
    printf("-------------------------------------------------");
    printf("\n");
}

void imprimirUsuarioYContrasenia(stCliente cliente)
{
    printf("\nUsuario..............: %s\n", cliente.usuario);
    printf("\nContrase%ca...........: %s\n\n",164, cliente.contrasenia);
}

void mostrarClientes(char archCliente[])
{
    stCliente cliente;

    FILE *puntero;
    puntero= fopen(archCliente, "rb");

    if(puntero!=NULL)
    {
        printf("Listado de Clientes:");
        while(fread(&cliente, sizeof(stCliente), 1, puntero)>0)
        {
            mostrarUnCliente(cliente);
        }
    }
    else
    {
        printf("\nNo se pudo abrir el archivo.\n");
    }
    fclose(puntero);
}

///DIFERENCIAR ENTRE MOSTRAR CLIENTES ACTIVOS Y DADOS DE BAJA
void mostrarClientesActivos(char archCliente[])
{
    stCliente cliente;

    FILE *puntero;
    puntero= fopen(archCliente, "rb");

    if(puntero!=NULL)
    {
        printf("\nListado de Clientes Activos:\n");
        while(fread(&cliente, sizeof(stCliente), 1, puntero)>0)
        {
            if (cliente.bajaCliente==1)
            {

                mostrarUnCliente(cliente);
            }
        }
    }
    else
    {
        printf("\nNo se pudo abrir el archivo.\n");
    }
    fclose(puntero);
}

void mostrarClientesInactivos(char archCliente[])
{
    stCliente cliente;

    FILE *puntero;
    puntero= fopen(archCliente, "rb");

    if(puntero!=NULL)
    {
        printf("\nListado de Clientes que dieron la baja:\n");
        while(fread(&cliente, sizeof(stCliente), 1, puntero)>0)
        {
            if (cliente.bajaCliente==0)
            {
                mostrarUnCliente(cliente);
            }
        }
    }
    else
    {
        printf("\nNo se pudo abrir el archivo.\n");
    }
    fclose(puntero);
}

///BUSQUEDA DE CLIENTE
stCliente MenuDeModificacion (stCliente A)
{
    char op;

    do
    {
        system("cls");
        printf("Men%c de modificaciones.\n",163);
        printf("1-Modificar datos\n");
        printf("2-Dar de baja\n");
        printf("0-Salir.\n\n");
        op= ingresarOpcionChar();

        switch(op)
        {
        case '0':
            printf("\n");
            break;
        case '1':
            A=SubMenuDeModificacion(A);
            break;

        case '2':
            A=BajaCliente(A);
            break;
        default:
            printf("El n%cmero ingresado no se encuentra entre las opciones, por favor ingr%cselo nuevamente.\n",163,130);
            break;

        }
    }
    while(op!='0');

    return A;
}

stCliente BajaCliente (stCliente A)
{
    char opcion;

    printf("Desea usted dar de baja el usuario de nuestra plataforma? (s/n) ");
    fflush(stdin);
    scanf("%c", &opcion);
    if (opcion=='s'|| opcion=='S')
    {
        A.bajaCliente=0;
        printf("El usuario fue dado de baja con %cxito.\n",130);
        system("pause");
    }
    return A;
}

stCliente SubMenuDeModificacion (stCliente A)
{
    char op;
    char control='s';

    do
    {
        system("cls");
        printf("Selecci%cn el campo que desea modificar:\n",162);
        printf("1-Nombre.\n");
        printf("2-Apellido.\n");
        printf("3-Mail.\n");
        printf("4-Domicilio.\n");
        printf("5-M%cvil.\n",162);
        printf("6-Usuario.\n");
        printf("7-Contrase%ca.\n",164);
        printf("0-Salir.\n\n");
        op= ingresarOpcionChar();

        switch (op)
        {
        case '0':
            printf("\n");
            break;
        case '1':
            printf("Ingrese nuevo nombre: ");
            fflush(stdin);
            gets(A.nombre);
            break;
        case '2':
            printf("Ingrese nuevo apellido: ");
            fflush(stdin);
            gets(A.apellido);
            break;
        case '3':
            printf("Ingrese nuevo mail: ");
            fflush(stdin);
            gets(A.mail);
            break;
        case '4':
            printf("Ingrese nuevo domicilio: ");
            fflush(stdin);
            gets(A.domicilio);
            break;
        case '5':
            printf("Ingrese nuevo m%cvil: ",162);
            scanf("%u", &A.movil);
            break;
        case '6':
            printf("Ingrese nuevo nombre de usuario: ");
            fflush(stdin);
            gets(A.usuario);
            break;
        case '7':
            printf("Ingrese nueva contrase%ca: ",164);
            fflush(stdin);
            gets(A.contrasenia);
            break;
        default:
            printf("El n%cmero ingresado no se encuentra entre las opciones por favor ingr%cselo nuevamente.\n",163,130);
            break;
        }
        if(op!='0')
        {
            printf("\nDesea modificar un nuevo dato? s/n\n");
            fflush(stdin);
            scanf("%c", &control);
        }
    }
    while (op!='0' && control=='s'|| control=='S');

    return A;
}

void ModificarCliente (char archCliente[], int pos)
{
    int i;
    int flag=0;

    stCliente cliente;

    FILE *puntero;
    puntero= fopen(archCliente,"r+b");

    if (puntero!=NULL)
    {
        fseek(puntero,(pos-1)*sizeof(stCliente),SEEK_SET);
        fread(&cliente,sizeof(stCliente),1,puntero);
        cliente=MenuDeModificacion(cliente);
        fseek(puntero,(-1)*sizeof(stCliente),SEEK_CUR);
        fwrite(&cliente,sizeof(stCliente),1,puntero);
    }
    else
    {
        printf("El archivo no existe o no pudo abrise");
    }

    fclose(puntero);
}

int posClientePorId(char archCliente[],int id)
{
    int u=0;
    int flag=0;

    stCliente cliente;
    FILE *puntero;

    puntero= fopen(archCliente,"rb");

    if (puntero!=NULL)
    {
        while (!feof(puntero) && flag==0)
        {
            u++;
            if(fread(&cliente,sizeof(stCliente),1,puntero)>0)
            {
                if (cliente.idCliente==id)
                {
                    flag=1;
                }
            }
        }
    }
    else
    {
        printf("El archivo no existe o no pudo abrise");
    }
    if(flag==0)
    {
        u=-1;
    }

    fclose(puntero);
    return u;
}

int posClientePorUsuario(char archCliente[],char usuario[])
{
    int u=0;
    int flag=0;

    stCliente cliente;
    FILE *puntero;

    puntero= fopen(archCliente,"rb");

    if (puntero!=NULL)
    {
        while (!feof(puntero) && flag==0)
        {
            u++;
            if(fread(&cliente,sizeof(stCliente),1,puntero)>0)
            {
                if (strcmpi(usuario, cliente.usuario)==0)
                {
                    flag=1;
                }
            }
        }
    }
    else
    {
        printf("El archivo no existe o no pudo abrise");
    }
    if(flag==0)
    {
        u=-1;
    }

    fclose(puntero);
    return u;
}

int BusquedaPorId(char archCliente[])
{
    int id=0;
    int pos=0;

    do
    {
        printf("\n\nIngrese id del usuario: ");
        scanf("%d",&id);
        pos=posClientePorId(archCliente, id);
        if(pos==-1)
        {
            printf("\nEl id ingresado no es v%clido.\n",160);
        }
    }
    while(pos==-1);
    return pos;
}

int BusquedaPorUsuario(char archCliente[])
{
    char nombre[30];
    int pos=0;

    do
    {
        printf("\n\nIngrese nombre de usuario: ");
        fflush(stdin);
        gets(nombre);
        pos=posClientePorUsuario(archCliente, nombre);
        if(pos==-1)
        {
            printf("\nEl nombre de usuario ingresado no es v%clido.\n",160);
        }
    }
    while(pos==-1);
    return pos;
}

///Funciones que nos permite capturar una opcion que seleccione el usuario por comando
int ingresarOpcion()
{
    int opcion;
    printf("Ingrese opci%cn: ",162);
    scanf("%d", &opcion);
    return opcion;
}

char ingresarOpcionChar()
{
    char opcion;
    printf("");
    fflush(stdin);
    opcion=getch();
    return opcion;
}
///FUNCIONES REFERIDAS A USUARIOS Y CONTRASEÑAS
int ingresarUsuarioYContrasenia(char archCliente[])
{
    char usuario[30];
    char contrasenia[30];
    int pos=0;
    char opcion;

    do
    {
        system("cls");
        printf("INICIO DE SESI%cN:\n\n",224);
        printf("Nombre de usuario: ");
        fflush(stdin);
        gets(usuario);
        printf("Contrase%ca: ",164);
        fflush(stdin);
        gets(contrasenia);
        pos= validarUsuarioYContrasenia(archCliente, usuario, contrasenia);
        if(pos==-1)
        {
            printf("\nUsuario o contrase%ca inexistente.\n",164);
            printf("ESC para salir de este men%c (cualquier otra letra para volver a intentarlo).\n",163);
            fflush(stdin);
            opcion= getch();
        }
    }
    while(pos==-1 && opcion!=ESC);

    if(pos==-1)
    {
        printf("\nVolviendo al men%c de usuario...\n",163);
        system("pause");
    }
    return pos;
}

int validarUsuarioYContrasenia(char archCliente[], char usuario[], char contrasenia[])
{
    int flag=0;
    int pos=0;

    stCliente cliente;
    FILE *puntero;

    puntero= fopen(archCliente,"rb");

    if (puntero!=NULL)
    {
        while (!feof(puntero) && flag==0)
        {
            pos++;
            if(fread(&cliente,sizeof(stCliente),1,puntero)>0)
            {
                if (strcmpi(usuario, cliente.usuario)==0 && strcmpi(contrasenia, cliente.contrasenia)==0)
                {
                    flag=1;
                }
            }
        }
    }
    else
    {
        printf("El archivo no existe o no pudo abrise");
    }

    fclose(puntero);
    if(flag==0)
    {
        pos=-1;
    }
    return pos;
}

int posUsuarioYContrasenia(char archCliente[],char mail[])
{
    int flag=0;
    int pos=0;
    stCliente cliente;
    FILE *puntero;

    puntero= fopen(archCliente,"rb");

    if (puntero!=NULL)
    {
        while (!feof(puntero) && flag==0)
        {
            pos++;
            if(fread(&cliente,sizeof(stCliente),1,puntero)>0)
            {
                if (strcmpi(mail, cliente.mail)==0)
                {
                    flag=1;
                }
            }

        }
    }
    else
    {
        printf("El archivo no existe o no pudo abrise");
    }
    if(flag==0)
    {
        pos=-1;
    }

    fclose(puntero);
    return pos;
}
int recuperarUsuarioYContrasenia(char archCliente[])
{
    char mail[30];
    int pos=0;
    char opcion;

    do
    {
        system("cls");
        printf("RECUPERAR USUARIO Y CONTRASE%cA:\n", 165);
        printf("\nIngrese mail de usuario: ");
        fflush(stdin);
        gets(mail);
        pos=posUsuarioYContrasenia(archCliente, mail);
        if(pos==-1)
        {
            printf("\nEl mail de usuario ingresado no se encuentra en nuestra base de datos.\n");
            if(pos==-1)
            {
                printf("ESC para salir de este men%c (cualquier otra letra para volver a intentarlo).\n",163);
                fflush(stdin);
                opcion= getch();
            }
        }
    }
    while(pos==-1 && opcion!=ESC);
    return pos;
}

void mostrarUsuarioYContrasenia (char archCliente[], int pos)
{
    int i;
    int flag=0;

    if(pos!=-1)
    {
        stCliente cliente;

        FILE *puntero;
        puntero= fopen(archCliente,"rb");

        if (puntero!=NULL)
        {
            fseek(puntero,(pos-1)*sizeof(stCliente),SEEK_SET);
            fread(&cliente,sizeof(stCliente),1,puntero);
            imprimirUsuarioYContrasenia(cliente);
        }
        else
        {
            printf("El archivo no existe o no pudo abrise");
        }

        fclose(puntero);
    }
}

void mostrarClienteEspecifico (char archCliente[], int pos)
{
    int i;
    int flag=0;

    stCliente cliente;

    FILE *puntero;
    puntero= fopen(archCliente,"rb");

    if (puntero!=NULL)
    {
        fseek(puntero,(pos-1)*sizeof(stCliente),SEEK_SET);
        fread(&cliente,sizeof(stCliente),1,puntero);
        mostrarUnCliente(cliente);
    }
    else
    {
        printf("El archivo no existe o no pudo abrise");
    }

    fclose(puntero);
}

void mostrarNombreUsuario (char archCliente[], int pos)
{
    int i;
    int flag=0;

    stCliente cliente;

    FILE *puntero;
    puntero= fopen(archCliente,"rb");

    if (puntero!=NULL)
    {
        fseek(puntero,(pos-1)*sizeof(stCliente),SEEK_SET);
        fread(&cliente,sizeof(stCliente),1,puntero);
        printf("%s", cliente.usuario);
    }
    else
    {
        printf("El archivo no existe o no pudo abrise");
    }

    fclose(puntero);
}

///Buscar ultimo usuario cargado
int buscarUltimoIdCargado(char archCliente[])
{
    int id=0;
    stCliente cliente;

    FILE *puntero;
    puntero= fopen(archCliente,"rb");

    if(puntero!=NULL)
    {
        fseek(puntero, (-1)*sizeof(stCliente),SEEK_END);
        if(fread(&cliente,sizeof(stCliente), 1, puntero)>0)
        {
            id=cliente.idCliente;
        }
    }
    else
    {
        printf("El archivo no existe o no pudo abrise");
    }
    return id;
}

void buscarUltimoClienteCargado(char archCliente[])
{
    stCliente cliente;

    FILE *puntero;
    puntero= fopen(archCliente,"rb");

    if(puntero!=NULL)
    {
        fseek(puntero, (-1)*sizeof(stCliente),SEEK_END);
        if(fread(&cliente,sizeof(stCliente), 1, puntero)>0)
        {
            mostrarUnCliente(cliente);
        }
    }
    else
    {
        printf("El archivo no existe o no pudo abrise");
    }
}

int validarId(char archCliente[], int pos)
{
    int id=0;
    int u=0;

    stCliente cliente;
    FILE *puntero;

    puntero= fopen(archCliente,"rb");

    if (puntero!=NULL)
    {
        while (!feof(puntero) && id==0)
        {
            u++;
            if(fread(&cliente,sizeof(stCliente),1,puntero)>0)
            {
                if (u==pos)
                {
                    id= cliente.idCliente;
                }
            }
        }
    }
    else
    {
        printf("El archivo no existe o no pudo abrise");
    }

    fclose(puntero);
    return id;
}

int posicionIdCliente(char archCliente[], int id)
{
    stCliente cliente;
    int pos=0;
    int flag=0;

    FILE *puntero;
    puntero= fopen(archCliente, "rb");

    if(puntero!=NULL)
    {
        while(flag==0 && fread(&cliente, sizeof(stCliente), 1, puntero)>0)
        {
            pos++;
            if (cliente.idCliente==id)
            {
                mostrarUnCliente(cliente);
                flag=1;
            }
        }
    }
    else
    {
        printf("\nNo se pudo abrir el archivo.\n");
    }
    fclose(puntero);

    if(flag==0)
    {
        printf("No hay ning%cn cliente con ese id.\n", 163);
        pos=-1;
    }
    return pos;
}

///CARGAR PEDIDOS
void cargarPedido(char archPedido[], char archCliente[], int pos)
{
    stPedido pedido;
    int flag=0;
    time_t t;
    struct tm *tm;

    FILE *punteroPed;
    punteroPed= fopen(archPedido,"ab");

    if(punteroPed!=NULL)
    {
        printf("Cargar su pedido:\n");
        printf("Descripci%cn del pedido: ",162);
        fflush(stdin);
        gets(pedido.descripcionPedido);
        printf("Costo del pedido: $");
        scanf("%f", &pedido.costoPedido);
        pedido.idCliente= validarId(archCliente, pos);
        printf("Id del cliente: %d", pedido.idCliente);
        pedido.idPedido=contarPedidos(archPedido);
        printf("\nId del pedido: %d", pedido.idPedido);

        t=time(NULL);
        tm=localtime(&t);
        strftime(pedido.fechaPedido, 100, "%d/%m/%Y %H:%M:%S", tm);
        printf("\nFecha del pedido: %s", pedido.fechaPedido);
        pedido.pedidoAnulado=1;
        fwrite(&pedido, sizeof(stPedido), 1, punteroPed);
    }
    else
    {
        printf("\nNo se pudo abrir el archivo.\n");
    }
    fclose(punteroPed);
}

int contarPedidos(char archPedido[])
{
    stPedido pedido;
    int contador=1;

    FILE *punteroPed;
    punteroPed= fopen(archPedido,"rb");

    if(punteroPed!=NULL)
    {
        while(fread(&pedido, sizeof(stPedido), 1, punteroPed)>0)
        {
            contador++;
        }
    }
    else
    {
        printf("\nNo se pudo abrir el archivo.\n");
    }
    fclose(punteroPed);
    return contador;
}
/// MOSTRAR PEDIDOS
void mostrarUnPedido(stPedido pedido)
{
    printf("\n\n");
    printf("-------------------------------------------------");
    printf("\nDescripci%cn del pedido...................: %s\n",162, pedido.descripcionPedido);
    printf("\nCosto....................................: $%.2f\n", pedido.costoPedido);
    printf("\nId del cliente...........................: %d\n", pedido.idCliente);
    printf("\nId del pedido............................: %d\n", pedido.idPedido);
    printf("\nFecha en la que se realiz%c el pedido.....: %s\n",162, pedido.fechaPedido);
    printf("-------------------------------------------------");
    printf("\n");
}

void mostrarPedidosDeUnCliente(char archPedido[], int id)
{
    stPedido pedido;

    FILE *punteroPed;
    punteroPed= fopen(archPedido, "rb");

    if(punteroPed!=NULL)
    {
        printf("Listado de sus pedidos realizados:\n");
        while(fread(&pedido, sizeof(stPedido), 1, punteroPed)>0)
        {
            if (pedido.idCliente==id && pedido.pedidoAnulado==1)
            {
                mostrarUnPedido(pedido);
            }
        }
    }
    else
    {
        printf("\nNo se pudo abrir el archivo.\n");
    }
    fclose(punteroPed);
}
/// FUNCIONES DE MODIFICACION Y BAJA DE PEDIDOS
stPedido BajaPedido(stPedido A)
{
    char opcion;

    printf("\nDesea usted dar de baja el pedido? (s/n) ");
    fflush(stdin);
    scanf("%c", &opcion);
    if (opcion=='s'|| opcion=='S')
    {
        A.pedidoAnulado=0;
        printf("\nEl pedido fue dado de baja con %cxito.",130);
    }
    return A;
}

int posicionIdPedido(char archPedido[], int id)
{
    stPedido pedido;
    int pos=0;
    int flag=0;

    FILE *punteroPed;
    punteroPed= fopen(archPedido, "rb");

    if(punteroPed!=NULL)
    {
        while(flag==0 && fread(&pedido, sizeof(stPedido), 1, punteroPed)>0)
        {
            pos++;
            if (pedido.idPedido==id && pedido.pedidoAnulado==1)
            {
                mostrarUnPedido(pedido);
                flag=1;
            }
        }
    }
    else
    {
        printf("\nNo se pudo abrir el archivo.\n");
    }
    fclose(punteroPed);

    if(flag==0)
    {
        printf("No hay ning%cn pedido con ese id.\n",163);
        pos=-1;
    }
    return pos;
}

void modificarPedido (char archPedido[], int pos)
{
    int i;
    int flag=0;

    stPedido pedido;

    FILE *punteroPed;
    punteroPed= fopen(archPedido,"r+b");

    if (punteroPed!=NULL)
    {
        fseek(punteroPed,(pos-1)*sizeof(stPedido),SEEK_SET);
        fread(&pedido,sizeof(stPedido),1,punteroPed);
        pedido=MenuDeModificacionPedido(pedido);
        fseek(punteroPed,(-1)*sizeof(stPedido),SEEK_CUR);
        fwrite(&pedido,sizeof(stPedido),1,punteroPed);
    }
    else
    {
        printf("El archivo no existe o no pudo abrise");
    }

    fclose(punteroPed);
}

stPedido MenuDeModificacionPedido (stPedido A)
{
    char op;

    do
    {
        system("cls");
        printf("Men%c de modificaci%cn de pedidos:\n",163,162);
        printf("1-Modificar pedido.\n");
        printf("2-Dar de baja.\n");
        printf("0-Salir.\n\n");
        op=ingresarOpcionChar();

        switch(op)
        {
        case '0':
            printf("\n");
            break;
        case '1':
            A=SubMenuDeModificacionPedido(A);
            break;

        case '2':
            A=BajaPedido(A);
            break;
        default:
            printf("El n%cmero ingresado no se encuentra entre las opciones, por favor ingr%cselo nuevamente.\n",163,130);
            break;

        }
    }
    while(op!='0');

    return A;
}

stPedido SubMenuDeModificacionPedido (stPedido A)
{
    char op;
    char control='s';

    do
    {
        system("cls");
        printf("Seleccione el campo que desea modificar:\n");
        printf("1-Descripci%cn.\n",162);
        printf("2-Valor.\n");
        printf("0-Salir.\n\n");
        op= ingresarOpcionChar();

        switch (op)
        {
        case '0':
            printf("\n");
            break;
        case '1':
            printf("Nueva descripci%cn del pedido: ",162);
            fflush(stdin);
            gets(A.descripcionPedido);
            break;
        case '2':
            printf("Ingrese el nuevo precio: ");
            scanf("%f", &A.costoPedido);
            break;

        default:
            printf("El n%cmero ingresado no se encuentra entre las opciones por favor ingr%cselo nuevamente.\n",163,130);
            break;
        }
        if(op!='0')
        {
            printf("\nDesea modificar un nuevo dato? s/n\n");
            fflush(stdin);
            scanf("%c", &control);
        }
    }
    while (op!='0' && control=='s'|| control=='S');

    return A;
}
///ALTA DE CLIENTES DADOS DE BAJA
void RecuperarClienteAdmin (char archCliente[], int pos)
{
    int i;
    int flag=0;

    stCliente cliente;

    FILE *puntero;
    puntero= fopen(archCliente,"r+b");

    if (puntero!=NULL)
    {
        fseek(puntero,(pos-1)*sizeof(stCliente),SEEK_SET);
        fread(&cliente,sizeof(stCliente),1,puntero);
        cliente=AltaClienteDadoDeBaja(cliente);
        fseek(puntero,(-1)*sizeof(stCliente),SEEK_CUR);
        fwrite(&cliente,sizeof(stCliente),1,puntero);
    }
    else
    {
        printf("El archivo no existe o no pudo abrise");
    }

    fclose(puntero);
}


stCliente AltaClienteDadoDeBaja (stCliente A)
{
    char opcion;

    printf("Desea usted dar de alta nuevamente el usuario en nuestra plataforma? (s/n) ");
    fflush(stdin);
    scanf("%c", &opcion);
    if (opcion=='s'|| opcion=='S')
    {
        A.bajaCliente=1;
        printf("El usuario fue dado de alta con %cxito.",130);
    }
    return A;
}

int clienteDadoDeBaja (char archCliente[], int pos)
{
    int activo=0;

    stCliente cliente;

    FILE *puntero;
    puntero= fopen(archCliente,"rb");

    if (puntero!=NULL)
    {
        fseek(puntero,(pos-1)*sizeof(stCliente),SEEK_SET);
        fread(&cliente,sizeof(stCliente),1,puntero);
        activo=cliente.bajaCliente;
    }
    else
    {
        printf("El archivo no existe o no pudo abrise");
    }

    fclose(puntero);
    return activo;
}

void clienteDadoDeAltaPorUsuario(char archCliente[], int pos)
{
    int activo=0;

    stCliente cliente;

    FILE *puntero;
    puntero= fopen(archCliente,"r+b");

    if (puntero!=NULL)
    {
        fseek(puntero,(pos-1)*sizeof(stCliente),SEEK_SET);
        fread(&cliente,sizeof(stCliente),1,puntero);
        cliente.bajaCliente=1;
        fseek(puntero,(pos-1)*sizeof(stCliente),SEEK_SET);
        fwrite(&cliente,sizeof(stCliente),1,puntero);
    }
    else
    {
        printf("El archivo no existe o no pudo abrise");
    }

    fclose(puntero);
}
///CLIENTE DESTACADO
int mejorCliente(char archCliente[], char archPedido[])
{
    stCliente cliente;
    stPedido pedido;
    int contador=0;
    int mayor=0;
    int pos=0;
    int posMejorCliente=0;

    FILE *puntero;
    puntero= fopen(archCliente, "rb");

    FILE *punteroPed;
    punteroPed= fopen(archPedido, "rb");

    if(puntero!=NULL)
    {
        while(fread(&cliente, sizeof(stCliente), 1, puntero)>0)
        {
            pos++;
            contador=0;

            while(fread(&pedido, sizeof(stPedido), 1, punteroPed)>0)
            {
                if(cliente.idCliente==pedido.idCliente)
                {
                    contador++;
                }
            }
            rewind(punteroPed);
            if(mayor<contador)
            {
                mayor= contador;
                posMejorCliente=pos;
            }
        }
    }
    else
    {
        printf("\nNo se pudo abrir el archivo.\n");
    }
    fclose(puntero);
    fclose(punteroPed);

    return posMejorCliente;
}

///MENU INICIAL
void menuInicial(char archCliente[], char archPedido[])
{
    char opcion;

    do
    {
        system("cls");
        imprimirCabecera("\"Ya te lo llevo!\"");
        printf("\nINICIO:\n");
        printf("1)Ingresar como cliente.\n");
        printf("2)Ingresar como administrador.\n");
        printf("0)Salir.\n\n");
        opcion= ingresarOpcionChar();

        switch(opcion)
        {
        case '0':
            printf("\n\nSaliendo de \"Ya te lo llevo!\"...\n");
            break;
        case '1':
            system("cls");
            menuUsuario(archCliente, archPedido);
            break;

        case '2':
            system("cls");
            menuAdmin(archCliente, archPedido);
            break;

        default:
            printf("\n\nLa opci%cn ingresada es inv%clida.\n",162,160);
            system("pause");
            break;
        }
    }
    while(opcion!='0');
}

///Menu al ingresar como usuario
void menuUsuario(char archCliente[], char archPedido[])
{
    char opcion;
    char darAlta;
    int flag;
    int pos;
    int activo=0;

    do
    {
        pos=-1;
        system("cls");
        printf("Men%c de usuario:\n",163);
        printf("1)Iniciar sesi%cn.\n",162);
        printf("2)Registrarse.\n");
        printf("3)Recuperar usuario y contrase%ca.\n",164);
        printf("0)Regresar al men%c principal.\n\n",163);
        opcion= ingresarOpcionChar();

        switch(opcion)
        {
        case '0':
            printf("\n");
            break;
        case '1':
            system("cls");
            pos=ingresarUsuarioYContrasenia(archCliente);
            ///Hay que poner si pos==-1 que no se ejecute nada
            activo=clienteDadoDeBaja(archCliente, pos);
            if(activo==0){
                printf("\nEl usuario que intenta abrir se encuentra dado de baja. Desea darle el alta? s/n: ");
                fflush(stdin);
                scanf("%c", &darAlta);
                if(darAlta=='s' || darAlta=='S'){
                    clienteDadoDeAltaPorUsuario(archCliente, pos);
                    activo=1;
                    printf("\n\nEl usuario fue dado de alta.\n\n");
                    system("pause");
                }
            }
            if(pos!=-1 && activo==1)
                menuIniciarSesion(archCliente, archPedido, pos);
            break;
        case '2':
            system("cls");
            printf("REGISTRARSE.\n\n");
            cargarUnCliente(archCliente);
            break;
        case '3':
            system("cls");
            pos= recuperarUsuarioYContrasenia(archCliente);
            mostrarUsuarioYContrasenia(archCliente, pos);
            system("pause");
            break;
        default:
            printf("\nLa opci%cn ingresada es inv%clida.\n",162,160);
            system("pause");
            break;
        }
    }
    while(opcion>'3' || opcion<'0' || pos!=-1);
}

///Menu al iniciar sesion
void menuIniciarSesion(char archCliente[], char archPedido[], int pos)
{
    char opcion;
    int id;
    char salir;
    int posPedido;

    do
    {
        system("cls");
        printf("USUARIO: ");
        mostrarNombreUsuario(archCliente, pos);
        printf("\n\n1)Realizar pedido.\n");
        printf("2)Ver pedidos realizados.\n");
        printf("3)Modificar pedidos.\n");
        printf("4)Mi cuenta.\n");
        printf("0)Cerrar sesi%cn.\n\n",162);
        opcion= ingresarOpcionChar();

        switch(opcion)
        {
        case '0':
            printf("\n");
            break;
        case '1':
            do
            {
                system("cls");
                cargarPedido(archPedido, archCliente, pos);
                printf("\nPresione cualquier tecla para realizar otro pedido (ESC para salir).\n");
                fflush(stdin);
                salir= getch();
            }
            while(salir!=ESC);
            break;
        case '2':
            system("cls");
            id=validarId(archCliente, pos);
            mostrarPedidosDeUnCliente(archPedido, id);
            system("pause");
            break;
        case '3':
            system("cls");
            id=validarId(archCliente, pos);
            mostrarPedidosDeUnCliente(archPedido, id);
            do
            {
                printf("\n\nIngrese el id del pedido que desea modificar: ");
                scanf("%d", &id);
                posPedido=posicionIdPedido(archPedido, id);
                if(posPedido==-1)
                {
                    printf("\nPresione cualquier tecla para ingresar un nuevo id(ESC para salir).\n");
                    fflush(stdin);
                    salir=getch();
                }
            }
            while(posPedido==-1 && salir!=ESC);

            if(posPedido!=-1)
                modificarPedido(archPedido, posPedido);
            break;
        case '4':
            system("cls");
            printf("MIS DATOS:");
            mostrarClienteEspecifico(archCliente, pos);
            printf("\n");
            system("pause");
            ModificarCliente(archCliente, pos);
            break;
        default:
            printf("\nLa opci%cn ingresada es inv%clida.\n",162,160);
            system("pause");
            break;
        }
    }
    while(opcion!='0');
}

///Menu al ingresar como administrador
void menuAdmin(char archCliente[], char archPedido[])
{
    char opcion;
    int posPedido;
    int pos;
    int id;
    char salir, control;
    char ingresarUsuario[5];
    char ingresarContra[6];

    do
    {
        system("cls");
        printf("ADMINISTRADOR.\n");
        printf("\nUsuario: ");
        fflush(stdin);
        gets(ingresarUsuario);
        printf("Contrase%ca: ",164);
        fflush(stdin);
        gets(ingresarContra);

        if(strcmp(ingresarUsuario, usuario)==0 && strcmp(ingresarContra, contrasenia)==0)
            do
            {
                system("cls");
                printf("Men%c de administrador:\n",163);
                printf("1)Usuarios activos.\n");
                printf("2)Usuarios inactivos.\n");
                printf("3)Modificar usuarios.\n");
                printf("4)Listado de pedidos.\n");
                printf("5)Modificar pedidos.\n");
                printf("6)Dar de alta cliente.\n");
                printf("7)Usuario con mayor cantidad de pedidos.\n");
                printf("0)Cerrar sesi%cn de administrador.\n\n",162);
                opcion= ingresarOpcionChar();

                switch(opcion)
                {
                case '0':
                    printf("\n");
                    break;
                case '1':
                    system("cls");
                    mostrarClientesActivos(archCliente);
                    printf("\n");
                    system("pause");
                    break;
                case '2':
                    system("cls");
                    mostrarClientesInactivos(archCliente);
                    printf("\n");
                    system("pause");
                    break;
                case '3':
                    system("cls");
                    mostrarClientes(archCliente);
                    do
                    {
                        printf("\nId del cliente a modificar: ");
                        scanf("%d", &id);
                        pos=posicionIdCliente(archCliente, id);
                        if(pos==-1)
                        {
                            printf("\nPresione cualquier tecla para ingresar otro id (ESC para salir).\n");
                            fflush(stdin);
                            salir= getch();
                        }
                    }
                    while(pos==-1 && salir!=ESC);

                    if(pos!=-1)
                    {
                        ModificarCliente(archCliente, pos);
                        printf("\n");
                        system("pause");
                    }
                    break;
                case '4':
                    system("cls");
                    mostrarClientes(archCliente);
                    do
                    {
                        printf("\nDesea ingresar buscar los pedidos de un usuario en particular? (s/n)");
                        fflush(stdin);
                        scanf("%c",&control);
                        if (control=='s')
                        {
                        printf("\nId del cliente para ver sus pedidos: ");
                        scanf("%d", &id);
                        pos=posicionIdCliente(archCliente, id);
                        }

                        if(pos==-1 && control=='s')
                        {
                            printf("\nPresione cualquier tecla para ingresar otro id (ESC para salir).\n");
                            fflush(stdin);
                            salir= getch();
                        }
                    }
                    while(pos==-1 && salir!=ESC && control =='s');

                    if(pos!=-1 && control=='s')
                    {
                        system("cls");
                        mostrarPedidosDeUnCliente(archPedido, id);
                        printf("\n");
                        system("pause");
                    }
                    break;
                case '5':
                    system("cls");
                    mostrarClientes(archCliente);
                    do
                    {
                        printf("\nId del cliente para ver sus pedidos y modificarlos: ");
                        scanf("%d", &id);
                        pos=posicionIdCliente(archCliente, id);
                        if(pos==-1)
                        {
                            printf("\nPresione cualquier tecla para ingresar otro id (ESC para salir).\n");
                            fflush(stdin);
                            salir= getch();
                        }
                    }
                    while(pos==-1 && salir!=ESC);

                    if(pos!=-1)
                    {
                        system("cls");
                        mostrarPedidosDeUnCliente(archPedido, id);
                        printf("\n\nDesea modificar el pedido? s/n: ");
                        fflush(stdin);
                        scanf("%c", &salir);

                        if(salir=='s' || salir=='S')
                        {
                            do
                            {
                                printf("\n\nIngrese el id del pedido que desea modificar: ");
                                scanf("%d", &id);
                                posPedido=posicionIdPedido(archPedido, id);
                                if(posPedido==-1)
                                {
                                    printf("\nPresione cualquier tecla para ingresar un nuevo id(ESC para salir).\n");
                                    fflush(stdin);
                                    salir=getch();
                                }
                            }
                            while(posPedido==-1 && salir!=ESC);

                            if(posPedido!=-1)
                                modificarPedido(archPedido, posPedido);
                        }
                    }
                    break;
                case '6':
                    system("cls");
                    mostrarClientesInactivos(archCliente);
                    do
                    {
                        printf("\nIngrese el id del cliente que desea dar de alta: ");
                        scanf("%d", &id);
                        pos=posicionIdCliente(archCliente, id);
                        if(pos==-1)
                        {
                            printf("\nPresione cualquier tecla para ingresar otro id (ESC para salir).\n");
                            fflush(stdin);
                            salir= getch();
                        }
                    }
                    while(pos==-1 && salir!=ESC);

                    if(pos!=-1)
                    {
                        RecuperarClienteAdmin(archCliente, pos);
                        printf("\n");
                        system("pause");
                    }
                    break;
                case '7':
                    system("cls");
                    printf("Cliente con mayor cantidad de pedidos.\n");
                    pos=mejorCliente(archCliente, archPedido);
                    mostrarClienteEspecifico(archCliente, pos);
                    printf("\n");
                    system("pause");
                    break;
                default:
                    printf("\nLa opci%cn ingresada es inv%clida.\n",162,160);
                    system("pause");
                    break;
                }
            }
            while(opcion!='0');
        else
        {
            printf("\nSu usuario o contrase%ca son incorrectas. Presione cualquier tecla para volver a intentarlo(ESC para salir).\n",164);
            fflush(stdin);
            salir=getch();
        }
    }
    while(salir!=ESC && opcion!='0');
}

///Recuadro para el nombre del programa
void imprimirCabecera(char cabecera[])
{
    int i;

    printf("%c", 201);

    for(i=0; i<50; i++)
    {
        printf("%c",205);
    }
    printf("%c\n", 187);
    printf("%c%32s%19c\n", 186,cabecera,186);
    printf("%c", 200);

    for(i=0; i<50; i++)
    {
        printf("%c",205);
    }
    printf("%c", 188);
}

