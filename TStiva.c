#include "TStiva.h"
#include "TCoada.h"

/* FUNCTII STIVA */


/* initiaza o stiva goala*/
void *InitS(size_t d) 
{
	ASt s;
	s = (ASt)malloc(sizeof(TStiva));
	if (!s) return NULL;
	s->dime = d;
	s->vf = NULL;
	return (void *)s;
}

/* adauga elemtul x in varful stivei */
void Push(void *s, void *x) 
{
	ACelSt new = (ACelSt)malloc(sizeof(TcelSt));
	if(!new) return;

	new->info = (void *)malloc(DIMES(s));
	if(!(new->info)) return; 
	memcpy(new->info, x, DIMES(s));
	new->urm = VF(s);
	VF(s) = new;

}

/* scoate un element din varful stivei si il pune la adresa ae*/
int Pop(void *s, void *ae) 
{
	if(Vida(s)) return 0;
	memcpy(ae, VF(s)->info, DIMES(s));
	ACelSt aux = VF(s);
	VF(s) = aux->urm;
	free(aux->info);
	free(aux);
	return 1;
}

void AfisareS(void *s, void (*afiEl)(void *))
{
	void *aux = InitS(DIMES(s));
	void *el=malloc(DIMES(s));
	while(!Vida(s)) 
	{
		Pop(s, el);
		afiEl(el);
		Push(aux, el);
	}
	Rastoarna(aux, s);
	printf("\n");
	free(el);
	//DistrS(aux, void(*freeEl)(void *));
}

/* elibereaza tot spatiul ocupat de stiva, inclusiv referinta catre varf */
/* functie de elberare a spatiului ocupat de lista*/
void DistrugeLG(ACelSt *L, void(*freeEl)(void *))
{
	ACelSt aux, p;
	p=*L;
	while(p)
	{
		aux=p;
		p=p->urm;
		freeEl(aux->info);
		free(aux);
	}
	*L=NULL;
}

void DistrS(void *s, void(*freeEl)(void *))
{
	ACelSt l=(ACelSt)VF(s);
	DistrugeLG(&l, freeEl);
	((ASt)s)->vf=NULL;
	free(s);
}
void ResetS(void *s, void(*freeEl)(void *))
{
	ACelSt l=(ACelSt)VF(s);
	DistrugeLG(&l, freeEl);
	((ASt)s)->vf=NULL;
}
void Rastoarna(void *sursa, void *dest)
{
	if(Vida(sursa)) return;
	if(DIMES(sursa)!=DIMES(dest)) return;
	ACelSt ld=(ACelSt)VF(dest);
	ACelSt ls=(ACelSt)VF(sursa);
	ACelSt aux=NULL;
	while(ls)
	{
		aux=ls;
		ls=ls->urm;
		aux->urm=ld;
		ld=aux;
	}
	((ASt)sursa)->vf=NULL;
	((ASt)dest)->vf=ld;
} 


