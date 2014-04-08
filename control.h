typedef struct{
	char ID;
	int inicio;
	int longitud;
	int ultimo_escritor;
	bool escritor_adentro;
	bool lector_adentro;
	bool destructor_adentro;
	struct Nodo_control_struct *siguiente;
} Nodo_control;
