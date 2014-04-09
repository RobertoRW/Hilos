struct nodo_control_struct {
    char *ID;
    int inicio;
    int longitud;
    char *ultimo_escritor;
    int escritor_adentro;
    int lector_adentro;
    int destructor_adentro;
    int escritores_esperando;
    int lectores_esperando;
    int compresor_esperando;
    int destructor_esperando;
    struct nodo_control_struct *siguiente;
};

typedef struct nodo_control_struct nodo;

void agregar(char *nombre, int ini, int lon);
nodo *obtener(char *nombre);
void eliminar(char *nombre);
