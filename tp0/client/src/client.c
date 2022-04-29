#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

//	t_log* logger;
//	t_config* config;

	/* ---------------- LOGGING ---------------- */


	t_log* logger = iniciar_logger();

	log_info(logger, "Hola! soy un log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	t_config* config = iniciar_config();
//	config = config_create("/home/utnso/TP0/tp0/client/tp0.config");

		 // esta funcion obtiene un valor de tipo string de un .config
		 // config_get_string_value(puntero al config creado, nombre del campo a obtener)
	valor = config_get_string_value(config, "CLAVE");
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// Loggeamos el valor de config

	log_info(logger, valor);
	log_info(logger, ip);
	log_info(logger, puerto);


	/* ---------------- LEER DE CONSOLA ---------------- */
	leer_consola(logger);



	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	// conexion = socket_cliente
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	// ACA SE ENVIA SOLAMENTE EL CAMPO "CLAVE" DE tp0.config
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	// ACA SE ENVIA VARIOS STRINGS, LEIDOS POR CONSOLA
	paquete(conexion);

	terminar_programa(conexion, logger, config);

}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = malloc(sizeof(t_log));

	//	           log_create(direccion del log, nombre que se usara al logear, si debe mostrarse en consola, nivel de detalle minimo a loguear)
	nuevo_logger = log_create("/home/utnso/TP0/tp0/client/logs/tp0.log", "log_tp0", true, LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = malloc(sizeof(t_config));

	nuevo_config = config_create("/home/utnso/TP0/tp0/client/tp0.config");

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;
	leido = readline("Guardar en el log: > ");

	while( strcmp(leido, "\0") ){
		log_info(logger, leido);
		free(leido);
		leido = readline("Guardar en el log: > ");
	}

	free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();
	leido = readline("Enviar por socket: > ");

	while( strcmp(leido, "") ) {
		agregar_a_paquete(paquete, leido, strlen(leido) + 1 );
		free(leido);
		leido = readline("Enviar por socket: > ");
	}

	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_destroy(logger);
	close(conexion);
	config_destroy(config);
}
