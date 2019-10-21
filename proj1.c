/*  proj1.c - Práce s textem  *
 *														*
 *       Ludek Burda					*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int napoveda(){
	puts("Napoveda...");
	return 0;
}

int main(int argc, char *argv[]){

	FILE *soubor = fopen(argv[1],"r"); // soubor s prikazy
	char* ch = "aa";	//!!!promenna pro kontrolu obsahu stdin
	char tmp[1001] = {};	// promenna pro nacitani radku
	char PText[1001] = {}; // nacitani dodatecneho obsahu souboru s prikazy
	int prikaz; // promenna pro nacitani prikazu
	int kk;
	int bol = 1; // rozhodnuti zda nacist dalsi radek ze stdin
	int r = 0; // pomocna promenna pro prikaz 'r'
	long int i;
	char delice[2] = " "; // pomocna promenna pro prikaz 's' 
	char patrep[1001];	// pomocna promenna pro prikaz 's'
	char help[1001];
	char *ptr;
	char *pattern;
	char *replacement;
	if (argc != 2) exit(napoveda());
	if (soubor != NULL){
		while ((prikaz=fgetc(soubor)) != EOF && ch != NULL){ // nacitani z prikazu a stdin
			if (bol){	// nacteni noveho radku
				if ((ch=fgets(tmp,1001,stdin)) == NULL){break;}
				else{
					tmp[strlen(tmp)-1] = '\0'; //odstraneni znaku '\n' z nacteneho radku
				}
			}
			bol = 0;
			switch(prikaz){
				case 'i':
					fgets(PText,1001,soubor);
					if((kk = fgetc(soubor))!='\n'){ 
						fprintf(stderr,"Prilis dlouhy radek\n");
						fclose(soubor);
						return 0;
					}
					if(!strcmp(PText,"\n"))break;
					printf("%s",PText);
					break;
				case 'b':
					fgets(PText,1001,soubor);
					PText[strlen(PText)-1] = '\0';
					strcat(PText,tmp);
					strcpy(tmp,PText);
					break;
				case 'd':
					fgets(PText,1001,soubor);
					if((i = strtol(PText,&ch,35)) != 0){
						for(int j = 1; j<i;j++){
						if((ch=fgets(tmp,1001,stdin))==NULL) break;
						}
					}
					bol = 1;
					break;
				case 'r':
					r = 1;
					break;
				case 'a':
					fgets(PText,1001,soubor);
					PText[strlen(PText)-1] = '\0';
					strcat(tmp,PText);
					break;
				case 's':
					delice[1] = fgetc(soubor);
					fgets(patrep,1001,soubor);
					pattern = strtok(patrep,delice);
					if((replacement = strtok(NULL,"\n"))==NULL){
					 fprintf(stderr, "Chyba!! Nebyl zadan text pro nahrazeni\n");
					 fclose(soubor);
					 return 0;
					}
					if((ptr = strstr(tmp,pattern)) == NULL){
						 fprintf(stderr,"Vzor pro nahrazeni nenalezen\n");
							fclose(soubor);
							return 0;
					}
					strncpy(help,ptr+strlen(pattern),strlen(ptr)-strlen(pattern));
					ptr[strlen(replacement)] = '\0';
					strncpy(ptr,replacement,strlen(replacement));
					strcat(ptr,help);
					break;
				case 'S':
					delice[1] = fgetc(soubor);
					fgets(patrep,1001,soubor);
					pattern = strtok(patrep,delice);
					if((replacement = strtok(NULL,"\n"))==NULL){
						fprintf(stderr, "Chyba!! Nebyl zadan text pro nahrazeni\n");
						fclose(soubor);
						return 0;
					}
					if((ptr = strstr(tmp,pattern)) == NULL){
						fprintf(stderr,"Vzor pro nahrazeni nenalezen\n");
						fclose(soubor);
						return 0;
					}
					while (ptr != NULL){
						char heelp[1001] = {};
						strncpy(heelp,ptr+strlen(pattern),strlen(ptr)-strlen(pattern));
						ptr[strlen(replacement)] = '\0';
						strncpy(ptr,replacement,strlen(replacement));
						strcat(ptr,heelp);
						ptr = strstr(tmp,pattern);
					}
					break;
				case 'n':
					if(!r)strcat(tmp,"\n");
					r = 0;
					printf("%s",tmp);
					bol = 1;
					fgets(PText,1001,soubor);
					if((i = strtol(PText,&ch,35)) != 0){
						for(int j = 1; j<i;j++){
							if((ch=fgets(tmp,1001,stdin))==NULL) break;
							printf("%s",tmp);
						}
					}
					break;
				case 'q':
					return 0;
				case 'g':
					fgets(PText,1001,soubor);
					i = strtol(PText,&ch,35);
					rewind(soubor);
					for (int j = 1; j<i; j++){
						while ((prikaz=fgetc(soubor) != '\n')){}
						if ((prikaz=fgetc(soubor)) == EOF){
							fprintf(stderr,"Nespravne zadany prikaz 'g'\n");
							fclose(soubor);
							return 0;
						}
					}
					break;
				default: 
					if(prikaz != '\n' || '\0'){
						fprintf(stderr,"Chyba!! Nespravne zadany prikaz\n");
						fclose(soubor);
						return 0;
					}
			}
		}
	}
	if(prikaz == EOF && ch != NULL){
		if(!bol) puts(tmp);
		while ((ch = fgets(tmp,1001,stdin)) != NULL) printf("%s",tmp);
	}
	fclose(soubor);
	return 0;
}
