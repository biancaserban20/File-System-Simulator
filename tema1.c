#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir{
	char *name;   // numele directorului
	struct Dir* parent;  // pointer catre parintele directorului(null pentru radacina)
	struct File* head_children_files;  // pointer catre primul element de tip File din interiorul directorului
	struct Dir* head_children_dirs; // pointer catre primul element de tip Dir din interiorul directorului
	struct Dir* next; // pointer catre urmatorul element din lista in care se afla directorul
}Dir; // structura de tip director

typedef struct File {
	char *name;  // numele fisierului
	struct Dir* parent; // pointer catre directorul parinte
	struct File* next; // pointer catre urmatorul element din lista de fisiere
}File; // structura de tip fisier

// initializarea directorului
Dir* init_dir(Dir* parent, char* name)
{
	Dir *dir = (Dir *)malloc(sizeof(Dir));
	dir->name = (char *)malloc((strlen(name) + 1) * sizeof(char));
	strcpy(dir->name, name);
    dir->parent = parent;
	dir->head_children_dirs = NULL;
	dir->head_children_files = NULL;
	dir->next = NULL;

	return dir;
}

//initializarea fisierului
File* init_file(Dir* parent, char* name)
{
	File *file = (File *)malloc(sizeof(File));
	file->name = (char *)malloc((strlen(name) + 1) * sizeof(char));
	strcpy(file->name, name);
    file->parent = parent;
	file->next = NULL;

	return file;
}

void touch (Dir* parent, char* name)
{
	/* daca directorul parent contine directoare,
	 se verifica daca exista un director cu numele name */
	if(parent->head_children_dirs != NULL)
	{ 
		Dir *dir;
		dir = parent->head_children_dirs;
		while(dir->next != NULL && strcmp(dir->name, name) != 0)
			dir = dir->next;
				
		if(strcmp(dir->name, name) == 0)
		{
			printf("File already exists\n");
			return;
		}
	}

	/* daca directorul parent nu contine fisiere,
	se creeaza un fisier */
    if(parent->head_children_files == NULL)
	{
		File *new_file = init_file(parent, name);
        parent->head_children_files = new_file;
		return;
	}

	/* se parcurge lista de fisiere 
	si se adauga noul fisier la finalul listei */
    else {
		File *file;
		file = parent->head_children_files;
		while(file->next != NULL && strcmp(file->name, name) != 0)
        	file = file->next;

		// fisierul cu numele name deja exista
		if(strcmp(file->name, name) == 0)
		{
			printf("File already exists\n");
			return;
		}
		File *new_file = init_file(parent, name);
		file->next = new_file;
			
	}
}


void mkdir (Dir* parent, char* name)
{
	
	/* daca directorul parent contine fisiere,
	 se verifica daca exista un fisier cu numele name */
	if(parent->head_children_files != NULL)
	{
		File *file;
		file = parent->head_children_files;
		while(file->next != NULL && strcmp(file->name, name) != 0)
			file = file->next;

		if(strcmp(file->name, name) == 0)
		{
			printf("Directory already exists\n");
			return;
		}
	}

	/* daca directorul parent nu contine directoare,
	se creeaza un director */
	if(parent->head_children_dirs == NULL)
	{
		Dir *new_dir = init_dir(parent, name);
		parent->head_children_dirs = new_dir;
		return;
	}

	/* se parcurge lista de directoare 
	si se adauga noul director la finalul listei */	
	else
	{
		Dir *dir;
		dir = parent->head_children_dirs;
		while(dir->next != NULL && strcmp(dir->name, name) != 0)
			dir = dir->next;
		
		// directorul cu numele name deja exista
		if(strcmp(dir->name, name) == 0)
			printf("Directory already exists\n");
		else
		{
			Dir *new_dir = init_dir(parent, name);
			dir->next = new_dir;

		}	
	}
}

void ls (Dir* parent) 
{
	Dir *dir = parent->head_children_dirs;

	// se afiseaza fiecare director din lista
	while(dir != NULL)
	{
		printf("%s\n", dir->name);
		dir = dir->next;
	}

	// se afiseaza fiecare fisier din lista 
	File *file = parent->head_children_files;
	while(file != NULL)
	{
		printf("%s\n", file->name);
		file = file->next;
	}

}

void rm(Dir* parent, char* name)
{
    File *p;
	File *file = parent->head_children_files;

	if (file == NULL)
		printf("Could not find the file\n"); 
	else 
	{	
		// ok va arata daca s-a gasit sau nu fisierul de sters
		int ok = 0;
		// daca fisierul de sters este primul din lista
		if (strcmp(file->name, name) == 0) 
		{
			// s-a gasit fisierul
			ok = 1;
			p = file;
			file = file->next;
			parent->head_children_files = file;
			free(p->name);
			free(p);
		} 
		// daca fisierul de sters este altul decat primul
		else 
		{
			/* se parcurge lista de fisiere
			 pana se gaseste fisierul de sters */
			File* tmp;
			tmp = parent->head_children_files;
			while(tmp->next != NULL){
				if(strcmp(tmp->next->name, name) == 0){
					ok = 1;
					p = tmp->next;
					tmp->next = tmp->next->next;
					free(p->name);
					free(p);
					break;
				}
				tmp = tmp->next;
			}
		}

		// nu s-a gasit fisierul de sters	
		if(ok == 0)
			printf("Could not find the file\n");
	}
}

// sterge toate fisierele dintr-un director
void rm_allfiles(Dir* parent)
{
	File* file;
	file = parent->head_children_files;
	if(file == NULL)
		return;
	while(file != NULL){
			File* p;
			p = file;
			file = file->next;
			free(p->name);
			free(p);
		}
	parent->head_children_files = NULL;
}

// sterge toate directoarele si fisierele dintr-un director
void rm_all(Dir* parent)
{
	Dir* dir;
	dir = parent->head_children_dirs;

	// daca nu exista directoare, sterge doar fisierele
	if(dir == NULL){
		rm_allfiles(parent);
		return;
	}

	// sterge fiecare director din lista si continutul lui
	while(dir != NULL)
	{
		Dir* p;
		p = dir;
		rm_allfiles(p);
		rm_all(p);
		dir = dir->next;
		free(p->name);
		free(p);
	}	
	parent->head_children_dirs = NULL;
}

void rmdir (Dir* parent, char* name)
{
	Dir *p;
	Dir *dir = parent->head_children_dirs;

	if (dir == NULL)
		printf("Could not find the dir\n");
	else
	{
		// ok va arata daca s-a gasit sau nu directorul de sters
		int ok = 0;
		// daca directorul de sters este primul din lista
		if (strcmp(dir->name, name) == 0) 
		{ 
			// s-a gasit directorul de sters
			ok = 1;
			rm_allfiles(dir);
			rm_all(dir);
			p = dir;
			dir = dir->next;
			parent->head_children_dirs = dir;
			free(p->name);
			free(p);
		} 
		// daca directorul de sters este altul decat primul
		else 
		{
			 /* se parcurge lista de directoare
			 pana se gaseste directorul de sters */
			Dir* tmp;
			tmp = parent->head_children_dirs;
			while(tmp->next != NULL)
			{
				if(strcmp(tmp->next->name, name) == 0)
				{
					ok = 1;
					rm_allfiles(tmp->next);
					rm_all(tmp->next);
					p = tmp->next;
					tmp->next = tmp->next->next;
					free(p->name);
					free(p);
					break;
				}
				tmp = tmp->next;
			}
		}
		
		// nu s-a gasit directorul
		if(ok == 0)
			printf("Could not find the dir\n");
	}
}

void cd(Dir** target, char* name)
{
	// daca se doreste intoarcerea in directorul parent
	if(strcmp("..", name) == 0)
	{
		if((*target)->parent != NULL)
			(*target) = (*target)->parent;
		return;
	}
	// daca nu exista directoare
	else if ((*target)->head_children_dirs == NULL)
		printf("No directories found!\n"); 
	else 
	{
		// ok va arata daca s-a gasit sau nu directorul dorit
		int ok = 0;
		Dir* dir = (*target)->head_children_dirs;
		// daca directorul in care vrem sa ajungem este primul din lista 
		if (strcmp(dir->name, name) == 0)
		{
			ok = 1;
			(*target) = dir;
		}
		// daca directorul in care vrem sa ajungem este altul decat primul
		else
		{
			// se parcurge lista de directoare pana se gaseste directorul 
			Dir* tmp;
			tmp = dir;
			while(tmp->next != NULL)
			{
				if(strcmp(tmp->next->name, name) == 0)
				{
					ok = 1;
					(*target) = tmp->next;
					break;
				}
				tmp = tmp->next;
			}
		
		// nu s-a gasit directorul
		if(ok == 0)
			printf("No directories found!\n");
		}
	}	
}

void tree(Dir* target, int level) {
	
	// daca target nu contine directoare si fisiere
	if(target->head_children_dirs == NULL && target->head_children_files == NULL)
		return;
	
	// daca contine directoare sau/si fisiere
	else 
	{
		// se parcurge lista de directoare
		Dir* dir = target->head_children_dirs;
		while(dir != NULL)
		{
			// se afiseaza spatii in functie de level
			for(int i = 0; i < level; i++)
				printf("    ");
			printf("%s\n", dir->name);
			tree(dir, level + 1);

			// daca directorul contine fisiere
			if(dir->head_children_files != NULL)
			{
				// se parcurge si se afiseaza lista de fisiere
				File* file = dir->head_children_files;
				while(file != NULL)
				{
					for(int i = 0; i <= level ; i++)
						printf("    ");
					printf("%s\n", file->name);
					file = file->next;
				}
			}
			dir = dir->next;
		}

		// daca target este directorul home si acesta contine fisiere
		if(level == 0 && target->head_children_files != NULL){
					File* home_files = target->head_children_files;
					while(home_files != NULL){
						printf("%s\n", home_files->name);
						home_files = home_files->next;
					}
				}
	}
}

// sterge n caractere de la adresa “d”
char* strdel ( char *d, int n) 
{
	char* aux = (char*)malloc(200 * sizeof(char));
	if (n < strlen(d))
	{
		strcpy(aux,d+n);
		strcpy(d,aux); 
	}
	else
	 *d = '\0'; 
	free(aux);

	return d;
}

char* pwd (Dir* target)
{
	Dir* tmp = target;
	int n = 0;

	// calculeaza lungimea sirului path
	while(tmp != NULL)
	{
		n = n + strlen(tmp->name) + 1;
		tmp = tmp->parent;
	}
	
	char* path = (char *)malloc((n + 1) * sizeof(char));
	char* buffer =  (char *)malloc((n + 1) * sizeof(char));
	// sterg caracterele in cazul in care exista reziduuri in memorie
	strdel(path, strlen(path));
	strdel(buffer, strlen(path));

	Dir* dir = target;
	// se construieste calea (path) 
	while(dir != NULL)
	{
		strcpy(buffer, path);
		strdel(path, strlen(path));
		strcat(path, "/");
		strcat(path, dir->name);
		strcat(path, buffer);
		strdel(buffer, strlen(buffer));
		dir = dir->parent;

	}

	free(buffer);
	return path;
	
}

void stop (Dir* target)
{
	// sterge tot ce se afla in target
	rm_allfiles(target);
	rm_all(target);
	
}

void mv(Dir* parent, char* oldname, char* newname)
{
	// daca parent nu contine nici directoare, nici fisiere
	if(parent->head_children_dirs == NULL && parent->head_children_files == NULL)
	{
		printf("File/Director not found\n");
		return;
	}

	Dir* dir = parent->head_children_dirs;
	File* file = parent->head_children_files;
	Dir* dir_pointer;

	// type este 0 cand se gaseste un director de inlocuit
	// si 1 cand se gaseste un fisier 
	int type = 2;
	while(dir != NULL && strcmp(dir->name, newname) != 0)
	{
		if(strcmp(dir->name, oldname) == 0)
		{
			type = 0; // mv de director
			dir_pointer = dir;
		}

		dir = dir->next;
	}

	// daca nu am gasit un director de inlocuit, caut in fisiere
	if(type != 0)
		while(file != NULL && strcmp(file->name, newname) != 0){
			if(strcmp(file->name, oldname) == 0)
			{
				type = 1; // mv de file
			}
			file = file->next;
		}

	// exista un fisier care are deja numele newname
	if(file != NULL)
	{
		printf("File/Director already exists\n");
		return;
	}

	// exista un director care are deja numele newname
	if(dir != NULL )
	{
		printf("File/Director already exists\n");
		return;
	}

	// daca s-a gasit un director cu numele oldname
	if(type == 0) {
		// redenumesc directorul
		strcpy(dir_pointer->name, newname);
		// daca directorul este primul din lista de directoare
		if(dir_pointer == parent->head_children_dirs){
			// daca este singurul din lista
			if(dir_pointer->next == NULL) //un singur dir
				{
					return;
				}
			// daca exista mai multe directoare in lista
			else
			{
				Dir* director = parent->head_children_dirs; 
				while(director->next != NULL){
					director = director->next;
				}
				parent->head_children_dirs = dir_pointer->next;
				director->next = dir_pointer;
				dir_pointer->next = NULL;
				return;
			}	
		}
		// daca directorul nu este primul din lista
		else
		{
			Dir* directories = parent->head_children_dirs;
			Dir* prev; // retine directorul dinaintea dir_pointer
			while(directories->next !=  NULL)
			{

				if(directories->next == dir_pointer)
					prev = directories;
				directories = directories->next;
			}
			// daca este ultimul din lista
			if(dir_pointer->next == NULL)
				return;
			// directories este ultimul director din lista
			prev->next = dir_pointer->next;
			directories->next = dir_pointer;
			dir_pointer->next = NULL;
			return;
		}
	}

	// daca s-a gasit un fisier cu numele oldname
	if(type == 1)
	{
		rm(parent, oldname);
		touch(parent, newname);
		return;
	}
	// daca nu s-a gasit niciun fisier sau director
	if(type == 2)
	{
		printf("File/Director not found\n");
		return;
	}

	


}


int main()
{
	// se creeaza si se initializeaza directorul home
	Dir* home = (Dir *)malloc(sizeof(Dir));
	home->name = (char *)malloc(5 * sizeof(char));
	strcpy(home->name, "home");
    home->parent = NULL;
	home->head_children_dirs = NULL;
	home->head_children_files = NULL;
	home->next = NULL;
	Dir* curr_dir = home;
	char *command = (char *)malloc(MAX_INPUT_LINE_SIZE * sizeof(char));
	char *arg1 = (char *)malloc(MAX_INPUT_LINE_SIZE * sizeof(char));
	char *arg2 = (char *)malloc(MAX_INPUT_LINE_SIZE * sizeof(char));

	
	do{
		fscanf(stdin, "%s", command);
		if(strcmp(command, "touch") == 0)
		{
			fscanf(stdin, "%s", arg1);
			touch(curr_dir, arg1);
		}
		else if(strcmp(command, "mkdir") == 0)
		{
			fscanf(stdin, "%s", arg1);
			mkdir(curr_dir, arg1);
		}
		else if(strcmp(command, "ls") == 0)
			ls(curr_dir);
		else if(strcmp(command, "rm") == 0)
		{
			fscanf(stdin, "%s", arg1);
			rm(curr_dir, arg1);
		}
		else if(strcmp(command, "rmdir") == 0)
		{
			fscanf(stdin, "%s", arg1);
			rmdir(curr_dir, arg1);
		}
		else if(strcmp(command, "cd") == 0)
		{
			fscanf(stdin, "%s", arg1);
			cd(&curr_dir, arg1);
		}
		else if(strcmp(command, "tree") == 0)
		{
			tree(curr_dir, 0);
		}
		else if(strcmp(command, "pwd") == 0)
		{
			char* path = pwd(curr_dir);
			fprintf(stdout, "%s\n", path);
			free(path);
		}
		else if(strcmp(command, "mv") == 0){
			fscanf(stdin, "%s", arg1);
			fscanf(stdin, "%s", arg2);
			mv(curr_dir, arg1, arg2);
		}
		else if(strcmp(command, "stop") == 0)
		{
			stop(home);
			// sterge directorul home
			free(home->name);
			free(home);
		}

	}while (strcmp(command, "stop") != 0);

	free(command);
	free(arg1);
	free(arg2);
	



	return 0;

}