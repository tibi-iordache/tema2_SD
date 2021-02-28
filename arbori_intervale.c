// IORDACHE TIBERIU-MIHAI 312CD
#include "arbori_intervale.h"

Nod* creare_nod(int st, int dr) {
    Nod* nod = (Nod*) malloc(sizeof(Nod));
    // verificare alocare
    if (nod == NULL) {
        printf("Eroare alocare memorie\n");
        exit(1);
    }

    // initializez campurile
    nod->info = 0;
    
    nod->interval = (Interval*) malloc(sizeof(Interval));
    // verifcare alocare
    if (nod->interval == NULL) {
        free(nod);
        printf("Eroare alocare memorie\n");
        exit(1);
    }

    // setez intervalul
    nod->interval->capat_stanga = st;
    nod->interval->capat_dreapta = dr;

    // apelurile recursive
    if (st < dr) {
        int mij = (st + dr) / 2;

        nod->copil_stanga = creare_nod(st, mij);

        nod->copil_dreapta = creare_nod(mij + 1, dr);
    }

    return nod;
}


ArboreDeIntervale *construieste_arbore(int capat_stanga, int capat_dreapta,
        int valoare_predifinita_raspuns_copil,
        void (*f_actualizare)(Nod *nod, int v_actualizare),
        int (*f_combinare_raspunsuri_copii)(int raspuns_stanga, int raspuns_dreapta)) {
    // TODO implementati functia de constructie a arborelui prezentata in enunt
    // TODO initializati campurile unui ArboreDeIntervale*

    ArboreDeIntervale* adi = (ArboreDeIntervale*) malloc(sizeof(ArboreDeIntervale));
    // verificare alocare
    if (adi == NULL) {
        printf("Eroare alocare memorie\n");
        exit(1);
    }

    // initializez campurile

    adi->radacina = creare_nod(capat_stanga, capat_dreapta);
    // verifcare alocare
    if (adi->radacina == NULL) {
        free(adi);
        printf("Eroare alocare memorie\n");
        exit(1);
    }

    adi->dimensiune = ((capat_dreapta - capat_stanga) / 2) - 1;

    adi->valoare_predifinita_raspuns_copil = valoare_predifinita_raspuns_copil;

    adi->f_actualizare = f_actualizare;

    adi->f_combinare_raspunsuri_copii = f_combinare_raspunsuri_copii;

    return adi;
}


void actualizare_interval_pentru_nod(ArboreDeIntervale *arbore, Nod *nod,
                                        Interval *interval, int v_actualizare) {
    // TODO implementati functia de actualizare pentru un nod
    // Hint: pentru a actualiza un nod cand este cazul folositi arbore->f_actualizare(nod, v_actualizare);
    
    if (interval->capat_stanga <= nod->interval->capat_stanga 
        && nod->interval->capat_dreapta <= interval->capat_dreapta) {
            arbore->f_actualizare(nod, v_actualizare);
    }
    else {
        int mij = (nod->interval->capat_stanga + nod->interval->capat_dreapta) / 2;

        if (interval->capat_stanga <= mij)
            actualizare_interval_pentru_nod(arbore, nod->copil_stanga, 
                                            interval, v_actualizare);
    
        if (mij < interval->capat_dreapta) 
            actualizare_interval_pentru_nod(arbore, nod->copil_dreapta,
                                            interval, v_actualizare);

        arbore->f_actualizare(nod, v_actualizare);
    }
}


// Functia este deja implementata, se cheama functia de mai sus cu radacina arborelui
void actualizare_interval_in_arbore(ArboreDeIntervale *arbore, 
                                        Interval *interval, int v_actualizare) {
    actualizare_interval_pentru_nod(arbore, arbore->radacina, interval, v_actualizare);
}


int interogare_interval_pentru_nod(ArboreDeIntervale *arbore, Nod *nod, Interval *interval) {
    // TODO implementati functia de interogare pentru un nod prezentata in enunt
    // Hint: cand vreti sa calculati combinarea raspunsurilor folositi
    // arbore->f_combinare_raspunsuri_copii(raspuns_copil_stanga, raspuns_copil_dreapta)
    
    if (interval->capat_stanga <= nod->interval->capat_stanga 
        && nod->interval->capat_dreapta <= interval->capat_dreapta) {
            return nod->info;
    }
    else {
        int mij = (nod->interval->capat_stanga + nod->interval->capat_dreapta) / 2;
        
        int raspuns_copil_stanga = 0;

        int raspuns_copil_dreapta = 0;

        if (interval->capat_stanga <= mij) 
            raspuns_copil_stanga = interogare_interval_pentru_nod(arbore,
                                            nod->copil_stanga, interval);
        
        if (mij < interval->capat_dreapta) 
            raspuns_copil_dreapta = interogare_interval_pentru_nod(arbore,
                                            nod->copil_dreapta, interval);
        
        return arbore->f_combinare_raspunsuri_copii(raspuns_copil_stanga,
                                            raspuns_copil_dreapta);
    }
}


// Functia este deja implementata, se cheama functia de mai sus cu radacina arborelui
int interogare_interval_in_arbore(ArboreDeIntervale *arbore, Interval *interval) {
    return interogare_interval_pentru_nod(arbore, arbore->radacina, interval);
}


// ----- DOAR pentru bonus si DOAR daca considerati ca e necesara ----- //
void seteaza_info_in_nod_la_valoare_capat_dreapta(Nod* nod) {
    // TODO cred ca e destul de clar ce vrea sa faca functia asta
    
    if (nod->copil_dreapta == NULL &&  nod->copil_stanga == NULL) {
        // actualizez o frunza
        nod->info = nod->interval->capat_dreapta;
    }
    else {
        // aplez recursiv si actualizez fiecare nod
        seteaza_info_in_nod_la_valoare_capat_dreapta(nod->copil_stanga);

        nod->info = nod->interval->capat_dreapta;

        seteaza_info_in_nod_la_valoare_capat_dreapta(nod->copil_dreapta);
        
        nod->info = nod->interval->capat_dreapta;
    }
}


void seteaza_info_in_arbore_la_valoare_capat_dreapta(ArboreDeIntervale* arbore) {
    seteaza_info_in_nod_la_valoare_capat_dreapta(arbore->radacina);
}
