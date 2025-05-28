#include "miniShell.h"

int	status = 0;

// Tableau des commandes internes avec leurs gestionnaires
t_builtin	g_builtin[] = 
{
	{.builtin_name = "echo", .foo=an_echo},  
	{.builtin_name = "env", .foo=an_env},    
	{.builtin_name = "exit", .foo=an_exit},  
	{.builtin_name = NULL},                   
};


/**
 * an_launch - Crée un processus enfant pour exécuter des commandes externes
 * @args: Tableau des arguments de la commande
 * Return: 1 en cas de succès, 0 en cas d’échec
 */
void	an_launch(char **args){
	if(Fork() == 0)
		Execvp(args[0], args);
	else
		Wait(&status);
}

/**
 * an_execute - Exécute des commandes internes ou externes
 * @args: Tableau des arguments de la commande
 * Return: 1 pour continuer le shell, 0 pour le quitter
 */
void	an_execute(char **args)
{
	int i;
	const char *curr_builtin;

	if (!args || !args[0])
		return ;
	i = 0;

	// Vérifier si une commande interne est demandée
	while((curr_builtin = g_builtin[i].builtin_name)){
		if(!strcmp(args[0], curr_builtin)){
		    status = (g_builtin[i].foo)(args);
		    return; 
		}
		i++;
	}
	an_launch(args);
}

/**
 * an_read_line - Lit une ligne depuis l’entrée standard
 * Return: Pointeur vers la ligne lue ou NULL en cas d’échec
 */
char *get_username() {
	struct passwd *pw = getpwuid(getuid());
	return pw ? pw->pw_name : "unknown";
}

int is_root() {
	return getuid() == 0;
}

char *an_read_line() {
	char *line = NULL;
	size_t bufsize = 0;
	char cwd[BUFSIZ];

	if (!getcwd(cwd, sizeof(cwd)))
		strcpy(cwd, "?");

	char *user = get_username();

	if (is_root()) {
		printf(RED "[%s" NC "👿" BLUE "%s" RED "] ># " NC, user, cwd);
	} else {
		printf(GREEN "[%s" NC "😃" CYAN "%s" GREEN "] >$ " NC, user, cwd);
	}

	if (getline(&line, &bufsize, stdin) == -1) {
		perror("Erreur de lecture");
		free(line);
		return NULL;
	}

	return line;
}


/**
 * an_split_line - Découpe une ligne en tokens
 * @line: Ligne d’entrée à découper
 * Return: Tableau de tokens ou NULL en cas d’échec
 */
char	**an_split_line(char *line)
{
	size_t bufsize;
	unsigned long position;
	char **tokens;

	bufsize = BUFSIZ;
	position = 0;
	tokens = Malloc(bufsize * sizeof *tokens);
	char *token;

	for(token = strtok(line, DL); token; token = strtok(NULL, DL)){
		tokens[position++] = token;	
		if(position >= bufsize){
			bufsize *= 2;
			tokens = Realloc(tokens, bufsize * sizeof *tokens);
		}
	}
	tokens[position] = NULL;
	return tokens;
}


int main(){
	printbanner();
	
	char	*line;
	char	**args;

	while((line = an_read_line())){

		args = an_split_line(line);	
		int i;
		if (args[0] && !strcmp(args[0], "cd")){
			if(args[1])
				 i = Chdir(args[1]);
			else
				chdir(getenv("HOME"));
		} else {
			an_execute(args);
		}

		free(line);
		free(args);
	}

	return 0;
}
