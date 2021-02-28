// IORDACHE TIBERIU-MIHAI 312CD
#include "pozitie_libera.h"


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


int suma_raspunsurilor(int r1, int r2) {
    // returneaza valoarea mai mare
    if (r1 <= r2) 
        return r2;
    else 
        return r1;
}


void actualizare_cu_delta(Nod *nod, int v2) {
    // functia modificata
    if (nod->interval->capat_stanga == nod->interval->capat_dreapta) {
        // imi pune valoare ceruta pe frunza
        nod->info = v2;
    }
    else {
        // actualizeza nodurile
        nod->info = suma_raspunsurilor(nod->copil_stanga->info,
                                      nod->copil_dreapta->info);
    }
}


DateIntrare* citeste_fisier_in(char *nume_fisier_intrare) {
    // TODO citeste datele din fisier si populeaza pointerul la structura
    
    // deschid fisierul
    FILE* in = fopen(nume_fisier_intrare, "rt");
    // verific deschiderea
    if (in == NULL) {
        fprintf(stderr, "ERROR: Can't open file %s", nume_fisier_intrare);
        return NULL;
    }
    
    // citesc elementul maxim si numarul de elemente
    int e_max, n;

    fscanf(in, "%d", &e_max);
    
    fscanf(in, "%d", &n);
    
    // citesc fiecare element din lista
    int sir[n];
    int i;
    for (i = 0; i < n; i++) {
        // le salvez intr-o variabila sir
        fscanf(in, "%d", &sir[i]);
    }
    
    // inchid fisierul
    fclose(in);

    // aloc spatiu structurii
    DateIntrare* date = (DateIntrare*) malloc(sizeof(DateIntrare));
    // verificare alocare
    if (date == NULL) {
        printf("Eroare alocare memorie \n");
        exit(1);
    }

    // completez campurile

    date->numarul_maxim_din_intrari = e_max;

    date->numar_intrari = n;
    
    // aloc spatii listei 
    date->intrari = (int*) malloc(n* sizeof(int));
    // verificare alocare
    if (date->intrari == NULL) {
        free(date);
        printf("Eroare alocare memorie \n");
        exit(1);
    }

    // pun fiecare valoare din sir in lista
    for (i = 0; i < n; i++) 
        date->intrari[i] = sir[i];
    
    // returnez structura
    return date;
}


int* calculeaza_vector_raspuns(DateIntrare *date_intrare) {
    // TODO creeaza vectorul raspuns folosind arbori de intervale


    // aloc spatiu vectorului
    int* T = (int*) calloc((date_intrare->numarul_maxim_din_intrari + 1), 
                                                            sizeof(int));
    // verificare alocare
    if (T == NULL) {
        printf("Eroare alocare memorie \n");
        exit(1); 
    }

    // construiesc arborele
    ArboreDeIntervale *arbore = construieste_arbore(0,
            date_intrare->numarul_maxim_din_intrari, 0,
            actualizare_cu_delta, suma_raspunsurilor);

    // actualizez datele din noduri si frunze
    seteaza_info_in_arbore_la_valoare_capat_dreapta(arbore);
    
    // guard pt cate elemente am pus in vector
    int contor = 0;
    // cu el o sa construiesc intervale
    Interval* interval;

    // parcurg lista de numere
    int i;
    for (i = 0; i < date_intrare->numar_intrari; i++) {
        // guard
        if (contor == date_intrare->numarul_maxim_din_intrari + 1)
            break;

        // fac o copie a numarului curent pt a
        // urmari mai usor codul
        int n = date_intrare->intrari[i];

        // construiesc intervalul unei frunze
        interval = construct_interval(n, n);

        // verific frunza
        if (interogare_interval_in_arbore(arbore, interval) == n) {
            // daca este goala adaug elementul in vectorul rezultat
            T[n] = n;
            
            // actualizez frunza la 0, iar restul nodurilor la 
            // valoarea respectiva
            actualizare_interval_in_arbore(arbore, interval, 0);

            // cresc cate nr am adaugat in vector
            contor++;
        }
        else {
            // frunza nu este goala, trb sa gasesc alt loc de inserare

            // construiesc noul intervel cu care o sa caut pozita
            interval = construct_interval(0, n);
            
            // calculez pozitia
            int poz = interogare_interval_in_arbore(arbore, interval);

            // cazuri
            if (poz == 0 && T[poz] == 0) {
                // daca pozitia gasita este 0 si pot insera pe prima pozitie
                // (evit cazul in care poz == 0 si am un element pe pozitia 0
                // a vectorului)

                // adaug in vector
                T[poz] = n;

                // construiesc intervalul frunzei la care am adaugat
                interval = construct_interval(poz, poz);

                // actualizez arborele
                actualizare_interval_in_arbore(arbore, interval, 0);

                // cresc cate nr am adaugat in vector
                contor++;
            }
            else 
                if (poz != 0) {
                    // cazul general

                    // adaug in vector
                    T[poz] = n;

                    // construiesc intervalul frunzei la care am adaugat
                    interval = construct_interval(poz, poz);

                    // actualizez arborele
                    actualizare_interval_in_arbore(arbore, interval, 0);

                    // cresc cate nr am adaugat in vector
                    contor++;
                }
        }
    }

    // returnez vectorul solutie
    return T;
}


int* calculeaza_vector_raspuns_trivial(DateIntrare *date_intrare) {
    // TODO creeaza vectorul raspuns folosind 2 for-uri

    int i, j;

    // aloc spatiu vectorului
    int* S = (int*) calloc((date_intrare->numarul_maxim_din_intrari + 1),
                                                             sizeof(int));
    // verificare alocare
    if (S == NULL) {
        printf("Eroare alocare memorie\n");
        exit(1);
    }

    // parcurg fiecare element din lista
    for (i = 0; i < date_intrare->numar_intrari; i++) {
        // verific daca pozitia la care vreau sa inserez este libera
        if (S[date_intrare->intrari[i]] == 0) {
            // adaug in vector
            S[date_intrare->intrari[i]] = date_intrare->intrari[i];
        }
        else 
            // parcurg restul vectorului in sens invers 
            for (j = date_intrare->intrari[i]; j >= 0; j--) {
                // verific daca pozita este libera
                if (S[j] == 0) {
                    // adaug in vector si ies
                    S[j] = date_intrare->intrari[i];
                    break;
                }
            }
    }

    // returnez vectorul solutie
    return S;
}