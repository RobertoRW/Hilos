#define MAX_TAM_MSG		(4096-16)

#define COLOR_ERR "\x1B[36m"
#define TAM_MSG_HEADER	(sizeof(Mesg)-MAX_TAM_MSG)

//En mesg_type se define el tipo de mensaje: 0 es mensaje normal y 1 es error.
typedef struct {
	bool terminar;
	int long_msg;
	int	mesg_type;
	char	mesg_data[MAX_TAM_MSG];
} Msg;


