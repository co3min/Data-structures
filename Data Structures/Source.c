#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

typedef struct contabilitateAprozar contabilitateAprozar;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;
typedef struct NodA NodA;
typedef struct NodPrincipalG NodPrincipalG;
typedef struct NodSecundarG NodSecundarG;
typedef enum Tip Tip;
enum Tip { kg, leg, buc };

struct contabilitateAprozar {
	long id;
	char* denumire;
	Tip tip;
	float pret;
	int nr_cantitati;
	int* cantitati_vandute;
};

char* getTip(Tip t) {

	switch (t)
	{
	case 0:
		return "kilogram";
		break;
	case 1:
		return "legatura";
		break;
	case 2:
		return "bucata";
		break;
	default:
		return "gresela";
		break;
	}
}

//lista dubla
struct Nod {
	contabilitateAprozar info;
	Nod* prev;
	Nod* next;
};

struct ListaDubla {
	Nod* prim;
	Nod* ultim;
};

contabilitateAprozar initilizareProdus(long id, const char* nume, Tip tip, float pret, int nrCant, int* cantitati) {
	contabilitateAprozar produs;
	produs.id = id;
	produs.denumire = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(produs.denumire, nume);
	produs.tip = tip;
	produs.pret = pret;
	produs.nr_cantitati = nrCant;
	produs.cantitati_vandute = (int*)malloc(sizeof(int) * produs.nr_cantitati);
	for (int i = 0; i < produs.nr_cantitati; i++) {
		produs.cantitati_vandute[i] = cantitati[i];
	}
	return produs;
}

contabilitateAprozar citireProdus(FILE* f) {
	if (f != NULL) {
		contabilitateAprozar c;
		char aux[50];
		fgets(aux, 10, f);
		c.id = atol(aux);

		fgets(aux, 50, f);
		char* sir = strtok(aux, "\n");
		c.denumire = (char*)malloc(sizeof(char) * (strlen(sir) + 1));
		strcpy(c.denumire, sir);

		fgets(aux, 10, f);
		c.tip = atoi(aux);

		fgets(aux, 10, f);
		c.pret = atof(aux);

		fgets(aux, 10, f);
		c.nr_cantitati = atoi(aux);

		c.cantitati_vandute = (int*)malloc(sizeof(int) * c.nr_cantitati);
		for (int i = 0; i < c.nr_cantitati; i++) {
			fgets(aux, 10, f);
			c.cantitati_vandute[i] = atoi(aux);
		}

		return c;
	}
	else {
		printf("Fisierul este gol !");
	}
}

void afisareContAprozar(contabilitateAprozar cont) {
	printf("Id produs %ld, denumire: %s, a fost vandut la pretul %.2f / %s urmatoarele cantitati:  ",
		cont.id, cont.denumire, cont.pret, getTip(cont.tip));
	for (int i = 0; i < cont.nr_cantitati; i++) {
		printf("%d  ", cont.cantitati_vandute[i]);
	}
	printf("\n");
}

ListaDubla inserareSfrasit(contabilitateAprozar c, ListaDubla lista) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = c;
	nou->prev = lista.ultim;
	nou->next = NULL;
	if (lista.ultim) {
		lista.ultim->next = nou;
		lista.ultim = nou;
	}
	else {
		lista.prim = nou;
		lista.ultim = nou;
	}
	return lista;
}

void traversareStangaDreapta(ListaDubla lista) {
	Nod* aux = lista.prim;
	while (aux) {
		afisareContAprozar(aux->info);
		aux = aux->next;
	}
}

void stergereDupaChar(ListaDubla* lista, const char* denumire) {

	if (lista->prim) {
		Nod* aux = lista->prim;
		while (aux) {
			if (strcmp(aux->info.denumire, denumire) == 0) {
				if (aux->prev == NULL) {
					lista->prim = aux->next;
					lista->prim->prev = NULL;
					free(aux->info.denumire);
					free(aux->info.cantitati_vandute);
					free(aux);
					break;
				}
				if (aux->next == NULL) {
					lista->ultim = aux->prev;
					lista->ultim->next = NULL;
					free(aux->info.denumire);
					free(aux->info.cantitati_vandute);
					free(aux);
					break;
				}
				if (aux->prev != NULL) {
					aux->prev->next = aux->next;
					aux->next->prev = aux->prev;
					free(aux->info.denumire);
					free(aux->info.cantitati_vandute);
					free(aux);
					break;
				}
			}
			aux = aux->next;
		}
	}
}

ListaDubla stergereLista(ListaDubla lista) {
	Nod* aux = lista.ultim;
	if (lista.ultim) {
		while (aux) {
			free(aux->info.denumire);
			free(aux->info.cantitati_vandute);
			aux = aux->prev;
			if (aux) {
				free(aux->next);
			}
		}
	}
	free(aux);
	lista.prim = NULL;
	lista.ultim = NULL;
	return lista;
}

// ARBORE
struct NodA {
	contabilitateAprozar info;
	NodA* stanga;
	NodA* dreapta;
};

void inserareInArbore(contabilitateAprozar c, NodA** radacina) {
	if ((*radacina) == NULL) {
		NodA* nou = (NodA*)malloc(sizeof(NodA));
		nou->info = c;
		nou->dreapta = NULL;
		nou->stanga = NULL;
		(*radacina) = nou;
	}
	else {
		if (c.id > (*radacina)->info.id) {
			inserareInArbore(c, &((*radacina)->dreapta));
		}
		else {
			inserareInArbore(c, &((*radacina)->stanga));
		}
	}
}

void parcurgeareInOrdine(NodA* radacina) {
	if (radacina) {
		parcurgeareInOrdine(radacina->stanga);
		afisareContAprozar(radacina->info);
		parcurgeareInOrdine(radacina->dreapta);

	}
}

void parcurgerePreordine(NodA* radacina) {
	if (radacina) {
		afisareContAprozar(radacina->info);
		parcurgerePreordine(radacina->stanga);
		parcurgerePreordine(radacina->dreapta);
	}
}

NodA* nodMinim(NodA* nod)
{
	NodA* nou = nod;
	while (nou && nou->stanga != NULL)
	{
		nou = nou->stanga;
	}
	return nou;
}

NodA* stergereNod(NodA* radacina, long id)
{
	if (radacina == NULL)
	{
		return radacina;
	}
	if (id < radacina->info.id)
	{
		radacina->stanga = stergereNod(radacina->stanga, id);
	}
	else if (id > radacina->info.id)
	{
		radacina->dreapta = stergereNod(radacina->dreapta, id);
	}
	else {
		if (radacina->stanga == NULL)
		{
			NodA* aux = radacina->dreapta;
			free(radacina);
			return aux;
		}
		else if (radacina->dreapta == NULL)
		{
			NodA* aux = radacina->stanga;
			free(radacina);
			return aux;
		}

		NodA* aux = nodMinim(radacina->dreapta);
		radacina->info.id = aux->info.id;
		radacina->dreapta = stergereNod(radacina->dreapta, aux->info.id);
	}
	return radacina;
}

void dezalocareArbore(NodA** radacina) {
	if ((*radacina) != NULL) {
		dezalocareArbore(&((*radacina)->stanga));
		dezalocareArbore(&((*radacina)->dreapta));
		free((*radacina)->info.denumire);
		free((*radacina)->info.cantitati_vandute);
		free((*radacina));
	}
	(*radacina) = NULL;
}

// GRAF
struct NodSecundarG {
	NodPrincipalG* info;
	NodSecundarG* next;
};

struct NodPrincipalG {
	contabilitateAprozar info;
	NodPrincipalG* next;
	NodSecundarG* vecini;
};

void inserarePrincipalG(NodPrincipalG** cap, contabilitateAprozar prod) {
	NodPrincipalG* nou = (NodPrincipalG*)malloc(sizeof(NodPrincipalG));
	nou->info = prod;
	nou->next = NULL;
	nou->vecini = NULL;
	NodPrincipalG* aux = (*cap);
	if (*cap) {
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		(*cap) = nou;
	}
}

void inserareSecundarG(NodSecundarG** caps, NodPrincipalG* info) {
	NodSecundarG* nou = (NodSecundarG*)malloc(sizeof(NodSecundarG));
	nou->next = NULL;
	nou->info = info;
	NodSecundarG* aux = (*caps);
	if (*caps) {
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		(*caps) = nou;
	}
}

NodPrincipalG* cautareNodG(NodPrincipalG* cap, int id) {
	while (cap && cap->info.id != id) {
		cap = cap->next;
	}
	return cap;
}

void inserareVeciniG(NodPrincipalG* cap, int id1, int id2) {
	NodPrincipalG* nod1 = cautareNodG(cap, id1);
	NodPrincipalG* nod2 = cautareNodG(cap, id2);

	if (nod1 != NULL && nod2 != NULL) {
		inserareSecundarG(&nod1->vecini, nod2);
		inserareSecundarG(&nod2->vecini, nod1);
	}
}

void afisareListaGraf(NodPrincipalG* cap) {
	while (cap) {
		afisareContAprozar(cap->info);
		NodSecundarG* aux = cap->vecini;
		printf("Vecini: \n");
		while (aux) {
			afisareContAprozar(aux->info->info);
			aux = aux->next;
		}
		printf("\n");
		cap = cap->next;
	}
}

void stergereNodGraf(NodPrincipalG* cap, int id) {
	NodPrincipalG* cautat = cautareNodG(cap, id);
	NodSecundarG* secundar = cautat->vecini;
	while (secundar) {
		NodPrincipalG* vecin = secundar->info;

		if (vecin && vecin->vecini->info->info.id == id) {
			NodSecundarG* aux = vecin->vecini;
			vecin->vecini = vecin->vecini->next;
			free(aux);
		}
		else {
			NodSecundarG* aux = vecin->vecini;
			while (aux && aux->next && aux->next->info->info.id != id) {
				aux = aux->next;
			}
			if (aux->next) {
				NodSecundarG* aux2 = aux->next;
				aux->next = aux2->next;
				free(aux2);
			}
		}

		NodSecundarG* aux = secundar;
		secundar = secundar->next;
		free(aux);
	}
	if (cap == cautat) {
		NodPrincipalG* aux = cap;
		cap = cap->next;
		free(aux->info.denumire);
		free(aux->info.cantitati_vandute);
		free(aux);
	}
	else {
		NodPrincipalG* grf = cap;
		while (grf->next && grf->next->info.id != id) {
			grf = grf->next;
		}
		if (grf->next) {
			NodPrincipalG* aux = grf->next;
			grf->next = aux->next;
			free(aux->info.denumire);
			free(aux->info.cantitati_vandute);
			free(aux);
		}
	}
}

//stergere si dezalocare Graf
void dezalocareGraf(NodPrincipalG** cap) {
	NodPrincipalG* graf = *cap;
	while (graf) {
		NodSecundarG* vecinii = graf->vecini;
		while (vecinii) {
			NodSecundarG* aux = vecinii;
			vecinii = vecinii->next;
			free(aux);
		}
		NodPrincipalG* aux = graf;
		graf = graf->next;
		free(aux->info.denumire);
		free(aux->info.cantitati_vandute);
		free(aux);
	}
	*cap = NULL;
}

int main() {
	//lista dubla
	printf("============LISTA DUBLA=============\n");
	ListaDubla lista;
	lista.prim = NULL;
	lista.ultim = NULL;

	FILE* f = fopen("produse.txt", "r");
	int nrStructuri = 0;
	if (f != NULL) {
		char aux[10];
		fgets(aux, 10, f);
		nrStructuri = atoi(aux);
	}
	for (int i = 0; i < nrStructuri; i++) {
		lista = inserareSfrasit(citireProdus(f), lista);
	}

	traversareStangaDreapta(lista);

	printf("\n==========Stergere dupa char*===========\n");
	stergereDupaChar(&lista, "Vinete");
	traversareStangaDreapta(lista);

	lista = stergereLista(lista);


	printf("=========== DEZALOCARE LISTA ==============\n");
	traversareStangaDreapta(lista);


	//Arbore
	printf("\n================= ARBORE =================\n");
	NodA* radacina = NULL;

	FILE* f2 = fopen("produseArbore.txt", "r");
	int nrstr = 0;
	if (f2 != NULL) {
		char aux[10];
		fgets(aux, 10, f2);
		nrstr = atoi(aux);
	}
	for (int i = 0; i < nrstr; i++) {
		inserareInArbore(citireProdus(f2), &radacina);
	}

	parcurgeareInOrdine(radacina);

	//printf("\n\n");
	//parcurgerePreordine(radacina);

	printf("\n================= STERGERE DIN ARBORE =================\n");
	stergereNod(radacina, 9);
	parcurgeareInOrdine(radacina);

	printf("=========== DEZALOCARE ARBORE ==============\n");
	dezalocareArbore(&radacina);
	parcurgeareInOrdine(radacina);

	printf("\n================= GRAF =================\n");
	NodPrincipalG* capG = NULL;

	FILE* f3 = fopen("produseGraf.txt", "r");
	int nrG = 0;
	if (f3 != NULL) {
		char aux[10];
		fgets(aux, 10, f3);
		nrG = atoi(aux);
	}
	for (int i = 0; i < nrG; i++) {
		inserarePrincipalG(&capG, citireProdus(f3));
	}

	inserareVeciniG(capG, 1, 2);
	inserareVeciniG(capG, 1, 5);
	inserareVeciniG(capG, 2, 3);
	inserareVeciniG(capG, 2, 4);
	inserareVeciniG(capG, 2, 6);
	inserareVeciniG(capG, 4, 7);
	inserareVeciniG(capG, 5, 6);
	inserareVeciniG(capG, 6, 8);
	inserareVeciniG(capG, 7, 8);

	afisareListaGraf(capG);

	printf("\n================= STERGERE DIN GRAF =================\n");
	stergereNodGraf(capG, 8);
	afisareListaGraf(capG);


	printf("========== DEZALOCARE GRAF ===============\n");
	dezalocareGraf(&capG);

	afisareListaGraf(capG);

	return 0;
}