struct creador_struct{
    char[2] id;
    char nombre;
    int inicio;
    int longitud;
    int espera;
    char* tira;
};

typedef struct creador_struct creador;

struct escritor_struct{
    int id;
    char nombre;
    int espera;
    char* tira;
};

typedef struct escritor_struct escritor;

struct lector_struct{
    char[2] id;
    char nombre;
    int espera;
};

typedef struct lector_struct lector;

struct destructor_struct{
    char[2] id;
    char nombre;
    int espera;
    char* tira;
};

typedef struct destructor_struct destructor;

struct compresor_struct{
    int tipo;
};

typedef struct compresor_struct compresor;