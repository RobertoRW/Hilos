struct nodo_control_struct {
	char[2] ID;
	int inicio;
	int longitud;
	char[3] ultimo_escritor;
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
