/*	proj2.c - Iteracni vypocty
 *	Ludek Burda
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define EPS 0.00000001
#define MaxIter 99999

double taylor_log(double x, unsigned int n){
	double y;
	double citatel = 1;
	double out = 0;
	char nan[10] = "";
	if (x == 0) return -INFINITY;
	if (x < 0) return NAN;
	if (isinf(x)) return INFINITY;
	if (isnan(x)){
		sprintf(nan,"%f",x);
		if (!strcmp(nan,"nan")) return NAN;
		else return -NAN;
	}
	if (x < 1 && x > 0){
		y = 1;
		for (int i = 1; i <= (int)n; i++){
			citatel *= 1 - x;
			y = citatel/i;
			out -= y;
		}
	}
	if (x >= 1){
		citatel = ((x-1)/x);
		for (int i = 1; i <= (int)n; i++){
			out += citatel/i;
			citatel *= ((x-1)/x);
		}
	}
	return out;
}

double cfrac_log(double x, unsigned int n){
	double cf = 0;
	double z = (x-1)/(x+1);
	double out = 1;
	char nan[10] = "";
	if (x == 0) return -INFINITY;
	if (x < 0) return NAN;
	if (isinf(x)) return INFINITY;
	if (isnan(x)){
		sprintf(nan,"%f",x);
		if (!strcmp(nan,"nan")) return NAN;
		else return -NAN;
	}
	for (; n > 1; n--){
		cf = (z*z*(n-1)*(n-1))/((n*2-1)-cf);
	}
	out = 2*z/(1-cf);
	return out;
}

double taylor_pow(double x, double y, unsigned int n){
	double out = 1;
	double out0 = 1;
	if (x == 0 || isnan(x)) return x;
	if (y == 0 || n == 1) return 1;
	if (y == 1) return x;
	for (int i = 1; i < (int)n; i++){
		out0 *= y*taylor_log(x,n)/i;
		out += out0;
	}
	return out;
}

double taylorcf_pow(double x, double y, unsigned int n){
	double out = 1;
	double out0 = 1;
	if (x == 0 || isnan(x)) return x;
	if (y == 0 || n == 1) return 1;
	if (y == 1) return x;
	for (int i = 1; i < (int)n; i++){
		out0 *= y*cfrac_log(x,n)/i;
		out += out0;
	}
	return out;
}

double mylog(double x){
	double taylor = taylor_log(x,1);
	double cfrac = cfrac_log(x,1);
	double TaylorDiff = 1;
	double CfracDiff = 1;
	for (int n = 2; n < MaxIter; n++){ // cyklus porovna rozdily variant zretezenych zlomku/taylorova polynomu
		TaylorDiff = fabs(taylor - taylor_log(x,n));
		CfracDiff = fabs(cfrac - cfrac_log(x,n));
		if (TaylorDiff < EPS || CfracDiff < EPS){
			if (TaylorDiff == CfracDiff){
				printf("Vysledek funkce mylog(%g) = %.7e s pouzitim libovolne metody po %d iteracich\n",x,cfrac_log(x,n),n); return 0;
			}
			else if(TaylorDiff < EPS){
				printf("Vysledek funkce mylog(%g) = %.7e s pouzitim metody Taylorova Polynomu po %d iteracich\n",x,taylor_log(x,n),n); return 0;
			}
			else if(CfracDiff < EPS){
				printf("Vysledek funkce mylog(%g) = %.7e s pouzitim metody Zretezenych zlomku po %d iteracich\n",x,cfrac_log(x,n),n); return 0;
			}
		}
		taylor = taylor_log(x,n);
		cfrac = cfrac_log(x,n);
	}
	printf("%ld nebylo dostatecne mnozstvi iteraci, vysledek neznamy\n",(long int)MaxIter);
	return 0;
}

double mypow(double x, double y){
	double taylor = taylor_pow(x,y,1);
	double cfrac = taylorcf_pow(x,y,1);
	double TaylorDiff = 1;
	double CfracDiff = 1;
	for (int n = 2; n < MaxIter; n++){ // cyklus porovna rozdily varianty zretezenych zlomku/taylorova polynomu
		TaylorDiff = fabs(taylor - taylor_pow(x,y,n));
		CfracDiff = fabs(cfrac - taylorcf_pow(x,y,n));
		if (TaylorDiff < EPS || CfracDiff < EPS){
			if (TaylorDiff == CfracDiff){
				printf("Vysledek funkce mypow(%g,%g) = %.7e s pouzitim libovolne metody po %d iteracich\n",x,y,taylor_pow(x,y,n),n); return 0;
			}
			else if(TaylorDiff < EPS){
				printf("Vysledek funkce mypow(%g,%g) = %.7e s pouzitim metody Taylorova Polynomu po %d iteracich\n",x,y,taylor_pow(x,y,n),n); return 0;
			}
			else if(CfracDiff < EPS){
				printf("Vysledek funkce mypow(%g,%g) = %.7e s pouzitim metody Zretezenych zlomku po %d iteracich\n",x,y,taylorcf_pow(x,y,n),n); return 0;
			}
		}
		taylor = taylor_pow(x,y,n);
		cfrac = taylorcf_pow(x,y,n);
	}
	return 0;
}

int argumenty(int argc, char *argv[]){ // kontrola spravneho formatu argumentu
	if (argc > 2 && argc < 6){
		if (!strcmp(argv[1],"--log") || !strcmp(argv[1],"--pow") || !strcmp(argv[1],"mylog") || !strcmp(argv[1],"mypow")){ 
			for (int i = 2; i < argc; i++){ // cyklus kontroluje zda jsou zadana cisla
				int delka = strlen(argv[i]);
				for (int j = 0; j < delka; j++){
					if (!isdigit(argv[i][j]) && argv[i][j] != '.' && argv[i][j] != '-'){
						if (isinf(atof(argv[i])) || isnan(atof(argv[i])));
						else return 0;
					}
				}
			}
		}
		if (!strcmp(argv[1],"--log") && argc == 4){ // varianta log
			if (atoi(argv[3]) < 1) return 0; // chyba kdyz je zaporny pocet iteraci (nebo 0)
			return 1;
		}
		if (!strcmp(argv[1],"--pow") && argc == 5){ // varianta pow
			if (atoi(argv[4]) < 1 /*|| atof(argv[2]) != fabs(atof(argv[2]))*/) return 0; // -||-
			return 2;
		}
		if (!strcmp(argv[1],"mylog") && argc == 3){ // premie mylog
			if(atof(argv[2]) != fabs(atof(argv[2]))) return 0;
			return 3;
		}
		if (!strcmp(argv[1],"mypow") && argc == 4){ // premie mypow
			if(atof(argv[2]) != fabs(atof(argv[2]))) return 0;
			return 4;
		}
	}
	return 0;
}

void PrintLog(double x, unsigned int n){ // funkce pro tisk vysledku logaritmu
	printf("       log(%g) = %.12g\n",x,log(x));
	printf(" cfrac_log(%g) = %.12g\n",x,cfrac_log(x,n));
	printf("taylor_log(%g) = %.12g\n",x,taylor_log(x,n));
	return;
}

void PrintPow(double x, double y, unsigned int n){ // funkce pro tisk vysledku mocniny
	printf("         pow(%g,%g) = %.12g\n",x,y,pow(x,y));
	printf("  taylor_pow(%g,%g) = %.12g\n",x,y,taylor_pow(x,y,n));
	printf("taylorcf_pow(%g,%g) = %.12g\n",x,y,taylorcf_pow(x,y,n));
	return;
}

int main(int argc, char *argv[]){
	double x;
	double y;
	unsigned int n;
	switch (argumenty(argc,argv)){//funkce pro overeni spravneho formatu arg..
		case 0: // nespravne zadane argumenty
			fprintf(stderr,"Nespravne zadane argumenty.\n");
			return 0;
		case 1: // log
			if (!strcmp(argv[2], "-nan")) x = -NAN;
			else x = strtod(argv[2],NULL);
			n = atoi(argv[3]);
			PrintLog(x,n);
			break;
		case 2:	// pow
			if (!strcmp(argv[2], "-nan")) x = -NAN;
			else x = strtod(argv[2],NULL);
			y = strtod(argv[3],NULL);
			n = atoi(argv[4]);
			PrintPow(x,y,n);
			break;
		case 3: // mylog
			x = strtod(argv[2],NULL);
			mylog(x);
			break;
		case 4: // mypow
			x = strtod(argv[2],NULL);
			y = strtod(argv[3],NULL);
			mypow(x,y);
			break;
	}
	return 0;
}
