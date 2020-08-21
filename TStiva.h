#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#ifndef _ELEMENT_
#define _ELEMENT_


typedef int (*TFCmp)(void*, void*);

typedef struct celst
{
	struct celst *urm;
	void* info;
} TcelSt, *ACelSt;

typedef struct stiva
{
	size_t dime;
	ACelSt vf;
} TStiva, *ASt;

#define VF(a) (((ASt)(a))->vf)
#define DIMES(a) (((ASt)(a))->dime)
#define Vida(s) (VF(s) == NULL)


#endif /* _ELEMENT_ */

#ifndef _STIVA_H_
#define _STIVA_H_

/* FUNCTII STIVA */

void *InitS(size_t d);
void Push(void *s, void *x);
int Pop(void *s, void *ae);
void DistrugeLG(ACelSt *L, void(*freeEl)(void *));
void DistrS(void *s, void(*freeEl)(void *));
void ResetS(void *s, void(*freeEl)(void *));
void AfisareS(void *s, void (*afiEl)(void *));
void afiINT(void *el);
void Rastoarna(void *sursa, void *dest);


#endif /* _STIVA_H_ */
