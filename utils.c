/*DRAGAN Silvia 311CB*/
#include "utils.h"
#include "TStiva.h"
#include "TCoada.h"

Resource* get_page_resources(const char *URL_Name, int *n)
{
    int i, j, name_len, url_name_len = strlen(URL_Name);

    unsigned int resource_nr = 0;
    uint32_t size = 0;
    if(URL_Name == NULL){
        return NULL;
    }

    for(i = 0; i < url_name_len; i++){
        resource_nr += URL_Name[i];
    }

    resource_nr %= 13;
    Resource *result = (Resource *)calloc(resource_nr, sizeof(Resource));
    *n = resource_nr;
    for(i = 0; i < resource_nr; i++){
        sprintf(result[i].name, "%s-(%d)", URL_Name, i);
        size = 0;
	name_len = strlen(result[i].name);

        for(j = 0; j < name_len; j++){
            size += result[i].name[j];
        }
        /* Some randomness */
        size ^= size << 3;
        size += size >> 5;
        size ^= size << 4;
        size += size >> 17;
        size ^= size << 25;
        size += size >> 6;

        /* 100MB Maximum size */
        result[i].dimension = size % 104857601;
        result[i].currently_downloaded = 0;
    }

    return result;
}
//functie ce initializeaza browserul
Tab *InitBrowser()
{
    Tab *a=(Tab *)malloc(sizeof(Tab));
    a->current_page=NULL;
    a->back_stack=InitS(sizeof(WebPage));
    a->forward_stack=InitS(sizeof(WebPage));

    return a;
}
//functie cce aloca spatiu pentru un tab
Tab *AlocTab()
{
    Tab *t=(Tab *)malloc(sizeof(Tab));
    if(!t) return NULL;

    //initializez current page, bs si fws cu null
    t->current_page=NULL;
    t->back_stack=InitS(sizeof(WebPage));
    t->forward_stack=InitS(sizeof(WebPage));

    return t;
}
//functie care aloca o celula de lista de taburi
//primind ca parametru un tab alocat cu functia AlocTab sub forma void*
TabList AlocCelula(void *x)
{
    //aloc spatiu pentru o celula de lista
    TabList new;
    new = (TabList)malloc(sizeof(TCell));
    if(!new) return NULL;
    new->info=x;
    new->urm=NULL;
    return new;
}

//4.2
//deschid un nou tab si il pun in lista de tab uri
//acesta fiind setat ca tab ul curent
void OpenNewTab(TabList *L, void *x, TabList *current)
{
    TabList aux, p;
    //parcurg lista pentru a obtine ultimul tab
    for(p=*L; (p->urm)!=NULL; p=p->urm);
    aux=AlocCelula(x);
    if(!aux) return;
    //leg noul tab in lista si setez tab ul curent
    aux->urm=p->urm;
    p->urm=aux;
    *current=aux;

}

//functie ce elibereaza memoria ocupata de un tab
void DistrWP(void *w)
{
    free( ((WebPage *)w)->resources);
    free(w);
}

void DistrTab(void *t)
{
    if( ((Tab *)t)->current_page!=NULL)
    {
        free(  ((Tab *)t)->current_page->resources );
        free( ((Tab *)t)->current_page );
    }
    DistrS( ((Tab *)t)->back_stack, DistrWP );
    DistrS(((Tab *)t)->forward_stack, DistrWP );
    free(t);   
}

//4.3
//inchide ultimul tab din lista
//seteaza tabul curent pe penultimul tab
void CloseTab(TabList *L, TabList *current)
{
    TabList p, ant=NULL;
    if(*L==NULL) return;
    //parcurg lista pana la ultimul tab retinand penultimul
    for(p=*L; (p->urm)!=NULL; ant=p, p=p->urm);
    //daca lista are cel putin 2 elemente
    //disctrug ultimul element si setez current la penultimul
    if(ant) 
    {
        if(*current == p)
            *current=ant;
        ant->urm=NULL;
        DistrTab(p->info);
        free(p);
    }
    //daca lista are doar un tab deschis, atunci il inchidem
    //si lista si current ul vor fi setate pe NULL
    else
    {
        *L=NULL;
        *current=NULL;
    }
}

/* functie de elberare a spatiului ocupat de o lista*/
void DistrugeL(TabList *L, void (*DistrTab)(void*))
{
    TabList aux=NULL, p;
    p=*L;
    while( p!=NULL )
    {
        aux=p;
        p=p->urm;
        DistrTab(aux->info);
        free(aux);
    }
    *L=NULL;
}

//4.4
//schimba tab ul curent in functie de un index dat
void ChangeCurrTab(TabList L, int index, TabList *current)
{
    TabList p;
    int i=0;
    for(p=L; p!=NULL; p=p->urm)
    {
        if(i==index)
        {
            *current=p;
            break;
        }
        i++;
    }

}

//4.5
//afiseaza URL ul unui tab primit ca parametru
void AfiTab(void *el, FILE *out)
{
    Tab *t=(Tab *)(el);
    if(t->current_page==NULL)
        fprintf(out, " empty)\n");
    else 
        fprintf(out, " %s)\n",  (t->current_page)->URL1);
}
//printeaza lista de tab uri in forma ceruta ( <index>: <URL>)
void PrintTabs(TabList L, void (*AfiEl)(void*, FILE *), FILE *out)
{
    TabList p;
    int i=0;
    for(p=L; p!=NULL; p=p->urm)
    {
        fprintf(out, "(%d:", i);
        i++;
        AfiEl(p->info, out);
    }
}

//4.6
void GoTo(char URL[MAX_URL_NAME], TabList current, void *history)
{
    //aloc memorie pentru o pagina web si ii dau numele
    WebPage *w=(WebPage *)malloc(sizeof(WebPage));
    strcpy(w->URL1, URL);
    //obtin resurse
    w->resources=get_page_resources(w->URL1, &(w->num_res));
    
    //daca exita deja o pagina deschisa
    //o introduc in backstack
    if( (((Tab *)((current)->info))->current_page)!=NULL )
    {
        Push( ((Tab *)((current)->info))->back_stack, 
            (void *)(((Tab *)((current)->info))->current_page));
        free( ((Tab *)((current)->info))->current_page );
    }
    //resetez stiva de fw
    ResetS( ((Tab *)((current)->info))->forward_stack, DistrWP );

    ((Tab *)((current)->info))->current_page=w;
    //adaug site-ul in istoric
    IntrQ(history, (void *)(URL));
}

//4.7
void Back(Tab *t, FILE *out)
{
    //verific daca stiva de back e vida
    if( Vida(t->back_stack) )
    {
        fprintf(out, "can't go back, no pages in stack\n");
        return;
    }
    //aloc un elemnt de dimensiunea specifica
    void *el=malloc(DIMES(t->back_stack));
    //scot un element din back
    Pop(t->back_stack, el);
    //trimit pagina curenta in fw 
    Push(t->forward_stack, t->current_page);
    //setez pagina curenta pe element
    free(t->current_page);
    t->current_page=el;
}

//4.8
void Forward(Tab *t, FILE *out)
{
    //verific daca stiva de fw e vida
    if( Vida(t->forward_stack) )
    {
        fprintf(out, "can't go forward, no pages in stack\n");
        return;
    }
    //aloc un elemnt de dimensiunea specifica
    void *el=malloc(DIMES(t->forward_stack));
    //scot un element din fw 
    Pop(t->forward_stack, el);
    //trimit pagina curenta in back
    Push(t->back_stack, t->current_page);
    //setez pagina curenta pe element
    free(t->current_page);
    t->current_page=el;
}

//4.10
void DelHistory(void *h, int nr_entries)
{
    //sterg primele nr_entries adrese din istoric
    if(nr_entries>0)
    {
        int i=0;
        while(i != nr_entries)
        {
            void *el=malloc(DIMEQ(h));
            ExtrQ(h, el);
            free(el);
            i++;
        }
    }
    //daca nr_entries e 0, sterg toate adresele din istoric
    if(nr_entries == 0)
    {
        ResetQ(h, DistrugeLGQ, free);
    }
}

//4.11
void PrintResources(WebPage *w, FILE *out)
{
    int i;
    if(w)
    {
        for(i=0; i < w->num_res; i++)
        {
            fprintf(out, "[%d - \"%s\" : %lu]\n", i, w->resources[i].name,
                w->resources[i].dimension);
        }

    }
}

//4.12
void PrintDownloads(void *el, FILE *out)
{
    fprintf(out, "[\"%s\" : %lu/%lu]\n", ((Resource *)el)->name, 
         ((Resource *)el)->dimension-((Resource *)el)->currently_downloaded, ((Resource *)el)->dimension );
}
void PrintFinnishedDownloads(void *el, FILE *out)
{
    fprintf(out, "[\"%s\" : completed]\n", ((Resource *)el)->name);
}
//4.13
void Download(WebPage *w, int ind, void *d)
{
    //daca coada e vida, introduc resursa in ea
    if(w && w->num_res>ind)
    {
        int ok=0;
        if(VidaQ(d))
            IntrQ(d, &(w->resources[ind]));
        else
        {
            /* folosesc o coada auxiliara in care pun toate elementele
            de la inceputul cozii originale ce au prioritatea mai mare
            decat elementul de introdus, apoi elementul*/
            void *c=InitQ(DIMEQ(d));
            while(!VidaQ(d))
            {
                void *el=malloc(DIMEQ(d));
                ExtrQ(d, el);
                //daca am gasit locul elementului, il introduc in coada
                //apoi introduc ultimul element scos  
                if(w->resources[ind].dimension - 
                    w->resources[ind].currently_downloaded <= 
                    ( ((Resource*)el)->dimension - 
                        ((Resource*)el)->currently_downloaded) )
                {
                    IntrQ(c, &(w->resources[ind]));
                    IntrQ(c, el);
                    free(el);
                    ok=1;
                    break;
                }
                else if(w->resources[ind].dimension - 
                    w->resources[ind].currently_downloaded > 
                        ( ((Resource*)el)->dimension - 
                          ((Resource*)el)->currently_downloaded))
                {
                    IntrQ(c, el);
                }

                free(el);


            }
            if(VidaQ(d) && ok==0)
                IntrQ(c, &(w->resources[ind]));
            //concatenez coada auxiliara cu elementele ramase in coada originala
            ConcatQ(c, d);
            //aduc inapoi toate elementele in coada originala
            ConcatQ(d, c);
            
            free(c);
        }
    }
}
//4.14
void Wait(int seconds, int bandwidth, void *d, void *f_d)
{
    int w=seconds*bandwidth;

    /* scot cate o resursa din coada si downloadez din ea w Bytes
    pana cand coada devine vida sau se termina w*/
    while(w)
    {
        void *el=malloc(DIMEQ(d));
        //cat mai trebuie downloadat din resursa 
        int rest=((Resource*)(IC(d)->info))->dimension - 
            ((Resource*)(IC(d)->info))->currently_downloaded;
        //daca putem descarca toata resursa, o introduc in coada
        //de resurse descarcate complet
        if(w >= rest)
        {
            ((Resource*)(IC(d)->info))->currently_downloaded =
                ((Resource*)(IC(d)->info))->dimension;
            ExtrQ(d, el);
            IntrQ(f_d, el);
            w-=rest;
        } 
        //altfel, descarc w bytes din ea si o reintroduc in coada
        //de descarcari nefinalizate
        else if(w < rest)
        { 
            ((Resource*)(IC(d)->info))->currently_downloaded+=w;
            w=0;
        }
        
        free(el);
    }

}