#include "miniShell.h"

int	an_echo(char **args){
	int start = 1;
	bool newline = true;

	if(!args || !args[0])
		return (1);

	// vérifier l'existence de l'option -n
	if(args[1] && !strcmp(args[1], "-n")){
		newline = false;
		start = 2;
	}

	int i;

	for(i = start; args[i]; i++){
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
	}

	if (newline)
		printf("\n");

	return (0);
}

int	an_env(char **args){
	extern char **environ;

	(void)args;

	if (!environ)
		return (1);

	int i;
	for (i = 0; environ[i]; i++)
		printf("%s\n", environ[i]);
	return (0);
}

int	an_exit(char **args)
{
	(void)args;

	exitLoading();
	exit(EX_OK);
}
