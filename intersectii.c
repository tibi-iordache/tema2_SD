// IORDACHE TIBERIU-MIHAI 312CD
#include "intersectii.h"


// functie ajutatoare
void afiseaza_lista_intervale(ListaIntervale2D *lista) {
    int i;
    for (i = 0; i < lista->dimensiune; i++) {
        Interval2D *interv = lista->intervale[i];
        printf("{punct stanga x: %d, punct stanga y: %d, punct dreapta x: %d, punct dreapta y: %d}\n",
                    interv->punct_stanga->x, interv->punct_stanga->y,
                    interv->punct_dreapta->x, interv->punct_dreapta->y);
    }
}


ListaIntervale2D* citeste_intrare(char *nume_fisier_intrare) {
    // TODO citeste intervalele din fisierul dat ca parametru
    // si le salveaza in lista de intervale

    // deschid fisierul
    FILE* in = fopen(nume_fisier_intrare, "rt");
    // verific deschiderea
    if (in == NULL) {
        fprintf(stderr, "ERROR: Can't open file %s", nume_fisier_intrare);
        return NULL;
    }

    // nr linii si coloane
    int n; 
    // citesc n
    fscanf(in, "%d", &n);

    // i si j pt for, matrix matricea unde o sa stochez intervalele
    int i, j, matrix[n][4];

    for (i = 0; i < n; i++) 
        for (j = 0; j < 4; j++)
            // citesc fiecare valoare din matrice
            fscanf(in, "%d", &matrix[i][j]);

    // inchid fisierul
    fclose(in);

    // aloc spatiu pt structura
    ListaIntervale2D* lista_intervale = 
        (ListaIntervale2D*) malloc(sizeof(ListaIntervale2D));
    // verificare alocare
    if (lista_intervale == NULL) {
        printf("Eroare alocare memorie\n");
        exit(1);
    }

    // completez campurile

    lista_intervale->dimensiune = n;

    // aloc spatiu pt vectorul de intervale
    lista_intervale->intervale = (Interval2D**) malloc(n * sizeof(Interval2D*));
    // verificare alocare
    if (lista_intervale->intervale == NULL) {
        free(lista_intervale);
        printf("Eroare alocare memorie\n");
        exit(1);
    }
    
    for (i = 0; i < n; i++) {
        // aloc spatiu pt fiecare interval in parte
        lista_intervale->intervale[i] = (Interval2D*) malloc(sizeof(Interval2D));
    }

    // parcurg fiecare interval
    for (i = 0; i < n; i++) {
        // aloc spatiu pt structura unui punct
        lista_intervale->intervale[i]->punct_stanga =
         (Punct2D*) malloc(sizeof(Punct2D));

        // completez coordonatele fiecarui punct
        lista_intervale->intervale[i]->punct_stanga->x = matrix[i][0];
        lista_intervale->intervale[i]->punct_stanga->y = matrix[i][1];

        // aloc spatiu pt structura unui punct
        lista_intervale->intervale[i]->punct_dreapta = 
        (Punct2D*) malloc(sizeof(Punct2D));

        // completez coordonatele fiecarui punct
        lista_intervale->intervale[i]->punct_dreapta->x = matrix[i][2];
        lista_intervale->intervale[i]->punct_dreapta->y = matrix[i][3];
    }

    // returnez structura
    return lista_intervale;
}


// ! Functie pentru actualizarea in arbore
// ! O veti da ca parametru cand initializati arborele
void actualizare_cu_delta(Nod *nod, int v2) {
    nod->info += v2;
}

// ! Functie pentru combinarea raspunsurilor in arbore
// ! O veti da ca parametru cand initializati arborele
int suma_raspunsurilor(int r1, int r2) {
    return r1 + r2;
}


void swap(Element** a, Element** b) {
    // functie care schimba 2 elemente intre ele
    Element* aux = *a;
    *a = *b;
    *b = aux;
}

void sortare(VectorElemente** vec) {
    // funcite de sortare(select sort)
    
    int i, j, min;
    // parcurg vectorul
    for (i = 0; i < (*vec)->dim; i++) {
        // initializez minim
        min = i;

        // parcurg restul vectorului
        for (j = i + 1; j < (*vec)->dim; j++) {
            // caut elementeul cu cel mai mic x
            if ((*vec)->elemente[j]->interval->x <
                (*vec)->elemente[min]->interval->x) 
                // ii salvez pozita
                min = j;
            
            // daca gasesc o valoare egala, sortez dupa y in continuare
            if (((*vec)->elemente[j]->interval->x == 
                 (*vec)->elemente[min]->interval->x) && 
                 ((*vec)->elemente[j]->interval->y <
                  (*vec)->elemente[min]->interval->y)) 
                // ii salvez pozita
                min = j;
        }

        // schimb elementul curent cu elementul minim
        swap(&((*vec)->elemente[min]), &((*vec)->elemente[i]));
    }
}

// functie care imi construieste un interval
Interval* construct_interval(int st, int dr) {
    // aloc spatiu
    Interval *interval = malloc(sizeof(Interval));
    if (interval == NULL) {
        printf("Eroare alocare memorie\n");
        exit(1);
    }

    // initializez campurile
    interval->capat_stanga = st;
    interval->capat_dreapta = dr;

    // returnez intervalul
    return interval;
}


int calculeaza_numar_intersectii(ListaIntervale2D *lista) {
    // TODO calculati numarul de intersectii folosind arbori de intervale conform enuntului
    // Hint: initializarea arborelui: 
    // ArboreDeIntervale *arbore = construieste_arbore(0, y_max, 0, actualizare_cu_delta, suma_raspunsurilor);

    // initializez rezultatul
    int nr_intersectii = 0;

    // calculez y_max
    int y_max = 0, i;
    for (i = 0; i < lista->dimensiune; i++) {
        if (lista->intervale[i]->punct_dreapta->y > y_max) 
            y_max = lista->intervale[i]->punct_dreapta->y;

        if (lista->intervale[i]->punct_stanga->y > y_max)
            y_max = lista->intervale[i]->punct_stanga->y;
    }

    // creeare arbore
    ArboreDeIntervale *arbore = construieste_arbore(0, y_max, 0,
                         actualizare_cu_delta, suma_raspunsurilor);
    // verifcare alocare
    if (arbore == NULL) {
        printf("Eroare alocare memorie\n");
        exit(1);
    }

    // mut punctele din structura intr un vector de structuri

    // creez structura
    VectorElemente* el = (VectorElemente*) malloc(sizeof(VectorElemente));
    // verificare alocare
    if (el == NULL) {
        free(arbore);
        printf("Eroare alocare memorie\n");
        exit(1);
    }

    // aloc spatiu pt vectorul de structuri
    el->elemente = (Element**) malloc((lista->dimensiune * 2) * sizeof(Element*));
    // verificare alocare
    if (el->elemente == NULL) {
        free(el);
        free(arbore);
        printf("Eroare alocare memorie\n");
        exit(1);
    }

    // aloc spatiu pt fiecare element din vector
    for (i = 0; i < (lista->dimensiune * 2); i++) {
        el->elemente[i] = (Element*) malloc(sizeof(Element));

        el->elemente[i]->interval = (Punct2D*) malloc(sizeof(Punct2D));
    }

    el->dim = lista->dimensiune * 2;

    // parcurg fiecare interval
    int j = 0;
    for (i = 0; i < lista->dimensiune; i++) {
        if (lista->intervale[i]->punct_stanga->x == lista->intervale[i]->punct_dreapta->x) {
            // segement vertical

            // completez campurile
            el->elemente[j]->interval->x = lista->intervale[i]->punct_stanga->x;
            el->elemente[j]->interval->y = lista->intervale[i]->punct_stanga->y;
            el->elemente[j]->tip = 1;
            el->elemente[j]->capat = 0;

            // iterez
            j++;

            // completez campurile
            el->elemente[j]->interval->x = lista->intervale[i]->punct_dreapta->x;
            el->elemente[j]->interval->y = lista->intervale[i]->punct_dreapta->y;
            el->elemente[j]->tip = 1;
            el->elemente[j]->capat = 1;

            // iterez
            j++;
        }
        else if (lista->intervale[i]->punct_stanga->y == lista->intervale[i]->punct_dreapta->y) {
            // segment orizontal

            // completez campurile
            el->elemente[j]->interval->x = lista->intervale[i]->punct_stanga->x;
            el->elemente[j]->interval->y = lista->intervale[i]->punct_stanga->y;
            el->elemente[j]->tip = 0;
            el->elemente[j]->capat = 0;

            // iterez
            j++;

            // completez campurile
            el->elemente[j]->interval->x = lista->intervale[i]->punct_dreapta->x;
            el->elemente[j]->interval->y = lista->intervale[i]->punct_dreapta->y;
            el->elemente[j]->tip = 0;
            el->elemente[j]->capat = 1;

            // iterez
            j++;
        }
    }

    // sortez vectorul
    sortare(&el);

    // parcurg vectorul de structuri
    for (i = 0; i < el->dim; i++) {
        if (el->elemente[i]->tip == 0) {
            // oriz

            //verfic capul din care face parte si actualizez
            if (el->elemente[i]->capat == 0) {
                int v_actualizare = 1;

                // construiesc intervalul care trb actualizat
                Interval* inter = construct_interval(el->elemente[i]->interval->y,
                                                     el->elemente[i]->interval->y);

                // actualizez
                actualizare_interval_in_arbore(arbore, inter, v_actualizare);
            }

            if (el->elemente[i]->capat == 1) {
                int v_actualizare = -1;

                Interval* inter = construct_interval(el->elemente[i]->interval->y,
                                                     el->elemente[i]->interval->y);

                actualizare_interval_in_arbore(arbore, inter, v_actualizare);
            }
        }
        else {
            // vert 

            // construiesc intervalul care trb actualizat
            Interval* inter = construct_interval(el->elemente[i]->interval->y,
                                             el->elemente[i + 1]->interval->y);

            // cresc numarul de intersectii
            nr_intersectii += interogare_interval_in_arbore(arbore, inter);
            
            // iterez pt a sarii de celalalt capat al segmentului vertical
            i++;
        }
    }

    // returnez rezultatul
    return nr_intersectii;
}
    

int calculeaza_numar_intersectii_trivial(ListaIntervale2D *lista) {
    // TODO: solutia triviala, verific fiecare segment cu fiecare segment daca se intersecteaza 
    
    // nr de intervale din structura
    int nr_seg = lista->dimensiune; 

    // intializez rezultatul
    int nr_intersectii = 0;

    // parcurg fiecare interval
    int i, j;
    for (i = 0; i < nr_seg; i++) {
        // salvez coordonatele segmentului initial
        int x1 = lista->intervale[i]->punct_stanga->x;
        int y1 = lista->intervale[i]->punct_stanga->y;

        int x2 = lista->intervale[i]->punct_dreapta->x;
        int y2 = lista->intervale[i]->punct_dreapta->y;
        
        // parcurg restul intervalelor
        for (j = i + 1; j < nr_seg; j++) {
            // salvez coordonatele segmentului de referinta
            int x1_ref = lista->intervale[j]->punct_stanga->x;
            int y1_ref = lista->intervale[j]->punct_stanga->y;

            int x2_ref = lista->intervale[j]->punct_dreapta->x;
            int y2_ref = lista->intervale[j]->punct_dreapta->y;

            // verific natura segmentului initial si cea a celui de referinta
            if (x1 == x2 && y1_ref == y2_ref) {
                // seg e vertical si ref e orizontal

                // daca se intersecteaza, cresc rezultatul
                if ((x1_ref <= x1 && x1 <= x2_ref) &&
                    (y1 <= y1_ref && y1_ref <= y2)) 
                    nr_intersectii++; 
            }
            else if (y1 == y2 && x1_ref == x2_ref) {
                // seg e orizontal si ref e vertical

                // daca se intersecteaza, cresc rezultatul
                if ((y1_ref <= y1 && y1 <= y2_ref) &&
                    (x1 <= x1_ref && x1_ref <= x2)) 
                    nr_intersectii++;
            }
        }
    }

    // returnez rezultatul
    return nr_intersectii;
}
