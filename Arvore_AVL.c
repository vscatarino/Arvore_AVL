
#include<stdio.h>
#include<stdlib.h>

struct folha{
	int num;
	int equilibrio;//indica se a árvore está desequilibrada
	struct folha* direcao[2];	
};

typedef struct folha Folha;

struct arvore{
	Folha *f;
};

typedef struct arvore Arv;

Folha* inicializa(){
	return NULL;
}

int ehVazia(Folha* f){
	return f == NULL;		
}

Folha* cria(int num){	
	Folha* f= (Folha*)malloc(sizeof(Folha));
	
		f->num = num;
		f->equilibrio = 0;
		f->direcao[0] = f->direcao[1] = inicializa();
			
	
	return f;
}

Folha* rotacao_simples(Folha* f, int dir){

	Folha* aux= f->direcao[!dir];

	f->direcao[!dir] = aux->direcao[dir];
	aux->direcao[dir] = f;

	return aux;
}

Folha *rotacao_dupla(Folha *f, int dir){

	Folha* aux= f->direcao[!dir]->direcao[dir];

	f->direcao[!dir]->direcao[dir] = aux->direcao[!dir];
	aux->direcao[!dir] = f->direcao[!dir];
	f->direcao[!dir] = aux;

	aux = f->direcao[!dir];
	f->direcao[!dir] = aux->direcao[dir];
	aux->direcao[dir] = f;

	return aux;
}

void ajuste_equilibrio(Folha *f, int dir, int eq){

	Folha *aux1 = f->direcao[dir];
	Folha *aux2 = aux1->direcao[!dir];

	if(aux2->equilibrio == 0){
		f->equilibrio = aux1->equilibrio = 0;

	}else if(aux2->equilibrio == eq){
		f->equilibrio = -eq;
		aux1->equilibrio = 0;

	}else{
		f->equilibrio = 0;
		aux1->equilibrio = eq;
	}

	aux2->equilibrio = 0;
}

Folha *insere_equilibrado(Folha *f, int dir){
	Folha *aux =  f->direcao[dir];
	int eq = dir == 0 ? -1 : 1;

	if(aux->equilibrio == eq){
		printf("o no :%d - rs e eq = %d  - equilibrio: %d\n", aux->num, eq, aux->equilibrio);		
		f->equilibrio = aux->equilibrio = 0;
		f = rotacao_simples(f, !dir);

	}else{
		printf("o no :%d - rd e eq = %d \n", aux->num, eq);
		ajuste_equilibrio(f, dir, eq);
		f = rotacao_dupla(f, !dir);
	}

	return f;
}

Folha *insere_r(Folha *f, int num, int *done){

	if(f == NULL){
		f = cria(num);
	
	}else{
		int dir = f->num < num;
		
		f->direcao[dir] = insere_r(f->direcao[dir], num, done);

		if(!*done){
			f->equilibrio += dir == 0 ? -1 : +1;

			if(f->equilibrio == 0){
				*done = 1;
			}else if(abs(f->equilibrio) > 1){
				printf("Equilibrio %d do no %d - dir: %d\n", f->equilibrio, f->num, dir);
				f = insere_equilibrado(f, dir);
				*done = 1;
			}
			printf("fora do if2: Equilibrio %d do no %d - dir: %d\n", f->equilibrio, f->num, dir);
		}
	}

	return f;
}



void imprime(Folha* f){
	if(f != NULL){		
		printf("%d - %d \n", f->num, f->equilibrio);
		imprime(f->direcao[0]);		
		imprime(f->direcao[1]);	
			
	}
}

Folha *insere(Folha *f, int num){
	int done = 0;

	f = insere_r(f, num, &done);
	return f;
}

Folha *remove_equilibrado(Folha *f, int dir, int *done){

	Folha *aux = f->direcao[!dir];
	int eq = dir == 0 ? -1 : 1;

	if(aux->equilibrio == -eq){
		f->equilibrio = aux->equilibrio = 0;
		f = rotacao_simples(f, dir);
	
	}else if(aux->equilibrio == eq){
		ajuste_equilibrio(f, !dir, -eq);
		f = rotacao_dupla(f, dir);
	
	}else{
		f->equilibrio = - eq;
		aux->equilibrio = eq;
		f = rotacao_simples(f, dir);
		*done = 1;
	}

	return f;
}

Folha *remove_r(Folha *f, int num, int *done){

	if(f != NULL){
		int dir;

		if(f->num == num){
			if(f->direcao[0] == NULL || f->direcao[1] == NULL){
				Folha *aux;

				dir = f->direcao[0] == NULL;
				aux = f->direcao[dir];
				free(f);

				return f;
			
			}else{
				Folha *filho = f->direcao[0];

				while(filho->direcao[1] != NULL){
					filho = filho->direcao[1];
				}

				f->num = filho->num;
				num = filho->num;
			}
		}

		dir = f->num < num;
		f->direcao[dir] = remove_r(f->direcao[dir], num, done);

		if(!*done){
			f->equilibrio += dir != 0 ? -1 : 1;

			if(abs(f->equilibrio) == 1){
				*done = 1;
			
			}else if(abs(f->equilibrio) > 1){
				f = remove_equilibrado(f, dir, done);
			}
		}
	}

	return f;
}

Folha *retira(Folha *f, int num){
	int done = 0;

	f = remove_r(f, num, &done);
	
	return f;
}

void destruir(Folha* f){
	if(f != NULL){
		destruir(f->direcao[0]);
		destruir(f->direcao[1]);
		free(f);
	}
}

int main(int argc, char const *argv[])
{
	Folha *f;
	int i = 0;

	for(i; i < 9; i++){
		f = insere(f, i);
		imprime(f);
		printf("\n\n");
	}

	f = retira(f, 5);
	printf("5 foi retirado:");
	imprime(f);
		
	
}


