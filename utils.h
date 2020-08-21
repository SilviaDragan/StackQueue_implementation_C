/*DRAGAN Silvia 311CB*/
#include "TStiva.h"

#ifndef _DOWNLOAD_RESOURCE_
#define _DOWNLOAD_RESOURCE_
#define MAX_URL_NAME 30
typedef struct
{
    char name[100];
    unsigned long dimension;
    unsigned long currently_downloaded;
} Resource;

typedef struct webpage
{
	char URL1[MAX_URL_NAME];
	int num_res;
	Resource *resources;
}WebPage;

typedef struct tab
{
	WebPage *current_page;
	void *back_stack;
	void *forward_stack;
}Tab;

typedef struct cell
{
	void *info;
	struct cell *urm;
}TCell, *TabList;


Resource* get_page_resources(const char *URL_Name, int *n);
Tab *InitBrowser();
Tab *AlocTab();
TabList AlocCelula(void *x);
void AfiTab(void *el, FILE *out);
void PrintTabs(TabList L, void (*AfiEl)(void*, FILE *), FILE *out);
void OpenNewTab(TabList *L, void *x, TabList *current);
void DistrWP(void *w);
void DistrTab(void *t);
void DistrugeL(TabList *L, void (*DistrTab)(void*));
void CloseTab(TabList *L, TabList *current);
void ChangeCurrTab(TabList L, int index, TabList *current);
void GoTo(char URL[MAX_URL_NAME], TabList current, void *history);
void Back(Tab *t, FILE *out);
void Forward(Tab *t, FILE *out);
void DelHistory(void *h, int nr_entries);
void PrintResources(WebPage *w, FILE *out);
void PrintDownloads(void *el, FILE *out);
void PrintFinnishedDownloads(void *el, FILE *out);
void Download(WebPage *w, int index, void *downloads);
void Wait(int seconds, int bandwidth, void *d, void *f_d);

#endif /* _DOWNLOAD_RESOURCE_ */
