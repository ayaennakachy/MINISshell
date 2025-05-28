#include "miniShell.h"

/**
 * Chdir - Change le répertoire de travail courant avec gestion des erreurs

 * @path : Chemin du répertoire vers lequel se déplacer
 *
 * Cas particuliers :
 * - Chemin NULL : affiche un message d'erreur
 * - Répertoire inexistant : affiche un message d'erreur
 * - Accès refusé (droits insuffisants) : affiche un message d'erreur
 */
int Chdir(const char *path){
	if (!path)
	{
		fprintf(stderr, RED"cd: path argument required\n"NC);
		return ;
	}
	if (chdir(path) == -1)
		perror(RED"cd failed"NC);

return 0;
}

/**
 * Fork - Crée un nouveau processus avec gestion des erreurs
 *
 * Retour : 
 * - PID du processus fils en cas de succès
 * - -1 en cas d'échec (le programme quitte avec un message d'erreur)
 *
 * Cas particuliers gérés :
 * - Limites de ressources système atteintes : affiche une erreur et quitte
 * - Table des processus pleine : affiche une erreur et quitte
 */
pid_t	Fork(void)
{
	pid_t pid;

	pid = fork();
	if(pid < 0){
		perror(RED"Fork failed"NC);
		exit(-1);
	}
	return (pid);
}

/**
 * Execvp - Exécute un programme avec gestion des erreurs
 *
 * @file : Nom du programme à exécuter
 * @argv : Tableau d'arguments (se terminant par NULL)
 *
 * Cas particuliers gérés :
 * - file ou argv est NULL : affiche une erreur et quitte
 * - Commande introuvable : affiche une erreur et quitte
 * - Permission refusée : affiche une erreur et quitte
 */
void Execvp(const char *file, char *const argv[]){
	if(!file || !argv){
		fprintf(stderr, RED"Execvp: invalid arguments\n"RED);
		exit(EXIT_FAILURE);
	}

	if(execvp(file, argv) == -1){
		perror(RED"failed"RED);
		exit(EX_UNAVAILABLE);
	}
}

/**
 * Wait - Attend la fin de n'importe quel processus fils avec gestion des erreurs
 *
 * @status : Adresse où stocker les informations de statut de terminaison
 *
 * Retour :
 * - PID du processus fils terminé en cas de succès
 * - -1 en cas d’échec
 *
 * Cas particuliers gérés :
 * - status est NULL : affiche une erreur et retourne -1
 * - Aucun processus fils : affiche une erreur
 */
pid_t Wait(int *status){
	pid_t	result;

	if (!status)
	{
		fprintf(stderr, RED"Wait: status argument required\n"NC);
		return (-1);
	}
	result = wait(status);
	if (result == -1)
		perror(RED"Wait failed"NC);
	if (WIFEXITED(*status))
		*status = WEXITSTATUS(*status); 
	return (result);
}


void *Malloc(size_t size){
	void	*ptr;

	if (size == 0)
		return (NULL);

	ptr = malloc(size);
	if (!ptr){
		perror(RED"Malloc failed"NC);
		exit(EXIT_FAILURE);
	}

	return (ptr);
}


/**
 * Realloc - Reallocates memory with error handling
 * @ptr: Pointer to previously allocated memory
 * @size: New size in bytes
 * Return: Pointer to reallocated memory
 * Corner cases:
 * - NULL ptr: acts as malloc
 * - Zero size: acts as free
 * - Allocation failure: prints error and exits
 */
void	*Realloc(void *ptr, size_t size){
	void	*new_ptr;

	new_ptr = realloc(ptr, size);
	if(!new_ptr && size != 0){
		perror(RED"Realloc failed"NC);
		exit(EXIT_FAILURE);
	}

	return (new_ptr);
}


char	*Getcwd(char *buf, size_t size){
	char	*result;

	result = getcwd(buf, size);
	if (!result)
		perror(RED"Getcwd failed"NC);

	return result;
}


void	Getline(char **lineptr, size_t *n, FILE *stream){
	if (!lineptr || !stream){
		fprintf(stderr, RED"Getline: invalid arguments\n"NC);
		return ;
	}

	if (getline(lineptr, n, stream) == -1){
		free(*lineptr);
		*lineptr = NULL;
		if (feof(stream))
			printf(RED"[EOF]"NC"\n");
		else
			perror(RED"Getline failed"NC);
	}
}


void exitLoading() {
    // Les positions du "cercle" tournant (roue)
    const char *circle_frames[] = {
        "◐",  // position 1
        "◓",  // position 2
        "◑",  // position 3
        "◒"   // position 4
    };
    const int frames_count = sizeof(circle_frames) / sizeof(circle_frames[0]);

    printf(RED "Arrêt en cours " NC);

    int i;
    for (i = 0; i < 40; i++) { // tourne 40 fois (~4 secondes)
        printf("\r" RED "Arrêt en cours %s" NC, circle_frames[i % frames_count]);
        fflush(stdout);
        usleep(100000); // 100 ms
    }

    printf("\r" CYAN "✅ Arrêt terminé !     \n" NC);
    exit(EX_OK);
}


void printbanner(){
printf(GREEN);
printf("      █████╗ ███████╗ \n");
printf("     ██╔══██╗██╔════╝ \n");
printf("     ███████║█████╗   \n");
printf("     ██╔══██║██╔══╝   \n");
printf("     ██║  ██║███████╗ \n");
printf("     ╚═╝  ╚═╝╚══════╝ \n");

	

    printf("%s", CYAN);
printf("          ========================================\n");
printf("%s            🛠️  BIENVENUE DANS MON PREMIER SHELL   \n", BLUE);
printf("%s          ========================================\n", CYAN);
printf("%s           Réalisé par     : Mohammed El janati\n", YELLOW);
printf("           Version         : 1.0\n");
printf("           Date            : 28 mai 2025\n");
printf("%s          ========================================%s\n", CYAN, NC);
}



