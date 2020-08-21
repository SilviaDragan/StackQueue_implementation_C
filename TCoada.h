/*DRAGAN Silvia 311CB*/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>


#ifndef _ELEMENTQ_
#define _ELEMENTQ_

/* -> T/F daca Stiva/Coada nu contine elemente */

/* tipul functiei de comparare folosita pentru sortari */
typedef int (*TFCmp)(void*, void*);

typedef struct cel
{
	struct cel *urm;
	void *info;
}TCel, *ACel;

typedef struct coada 
{
	size_t dime;
	ACel ic, sc;
}TCoada, *AQ;

#define IC(a) (((AQ)(a))->ic)
#define SC(a) (((AQ)(a))->sc)
#define DIMEQ(a) (((AQ)(a))->dime)
#define VidaQ(a) (IC(a) == NULL && SC(a)==NULL)

#endif /* _ELEMENTQ_ */


#ifndef _COADA_H_
#define _COADA_H_

/* FUNCTII COADA */

void *InitQ(size_t d);
void IntrQ(void *c, void *x);
void ExtrQ(void *a, void *ae);
void DistrugeLGQ(ACel *L, void(*freec)(void *));
void ResetQ(void *c, void(*dis)(ACel *, void(*freec)(void *) ), void(*freec)(void *));
void DistrQ(void *c, void(*dis)(ACel *, void(*freec)(void *) ), void(*freec)(void *));
void AfisQ(void *c, void (*afiEl)(void *, FILE *), FILE *out);
void ConcatQ(void *queueDestination, void *queueSource);

#endif /* _COADA_H_ */
