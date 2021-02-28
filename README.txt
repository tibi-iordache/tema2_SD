IORDACHE TIBERIU-MIHAI 312CD

Arbori de intervale:

Functii:

    -Nod* creare_nod(int st, int dr): Functie recursiva care aloca memorie si
initializeza campurile nodurilor din arbore, realizata cu ajutorul pseudocodului
din enuntul temei.

    -ArboreDeIntervale *construieste_arbore: Aloc spatiu pt structura arborelui
dupa care completez fiecare camp in parte, campul radacina este format cu 
ajutorul functiei creare_nod.

    -void actualizare_interval_pentru_nod: Am implementat pseudocodul din enunt.

    -int interogare_interval_pentru_nod: Am implementat pseudocodul din enunt.



Intersecții de intervale:

Structuri:
    - structua element ce contine coordonatele unui punct, tipul dreptei din 
care face parte(verticala sau orizontala), si tipul capatului din care face 
parte(capat stanga sau capat dreapta).

    - structura VectorElemente contine un vector de structuri de tipul Element
si dimensiunea acestui vector.

Functii:

    -ListaIntervale2D* citeste_intrare: Am citit datele din fisier si am stocat
intervalele intr-o matrice dupa care am creeat structura care va contine aceste
intervale si am completat campurile necesare.
    
    -void swap: Schimba 2 elemente intre ele.

    -void sortare: Select sort implementat pt vectorul de structuri
folosit la varianta arbori de intervale. Sorteaza dupa x,insa daca 
acestea sunt egale, sorteaza dupa y.

    -Interval* construct_interval: Functie care imi construieste un interval.

    -int calculeaza_numar_intersectii: Am urmarit pasii din enuntul problemei.
Astfel am cautat ymax si am creeat un arbore de intervale cu intervalui [0, ymax],
am mutat fiecare punct din structura de intervale intr o structura noua(pentru 
o sortare mai usoara si pt a retine informatii suplimentare), dupa care am sortat
aceasta structura nou creata. Dupa aceea am parcurs punctele sortate si in functie 
de natura dreptei din care facea parte, am urmarit algoritmul prezentat in enunt.

    -int calculeaza_numar_intersectii_trivial: Am parcurs fiecare interval
salvandu i coordonatele si l am comparat cu restul intervalelor astfel:
daca segmentul intial era vert iar cel de referinta orizontal atunci verificam daca
coordonatele x ale seg initial sunt cuprinse in cele ale lui ref si daca coordanta
y a lui ref este cuprinsa in cele ale segmentului initial, daca seg initial era oriz
iar cel de ref vertical, verificam intai coordonatele y dupa care x.



Pozitie Libera:

Functii:
    -seteaza_info_in_nod_la_valoare_capat_dreapta (cea din arbori_intervale.c):
    Am creeat o functie recursiva care imi adauga val capatului dreapta a
intervalului la campul info al fiecarui nod.

    -suma_raspunsurilor: Functie care imi va returna valoarea maxima dintre 2 noduri.

    -actualizare_cu_delta: Functia imi va pune valoarea v2 in campul info al unei
frunze, iar pt noduri va pune maximul dintre info urile copiilor.

    -DateIntrare* citeste_fisier_in: Am citit din fisier datele respective dupa care
am creeat o structura in care am completat campurile cu datele respective.

    -int* calculeaza_vector_raspuns: La aceasta functie am creeat un arbore de
intervale cu intervalul [0,e_max] pe care l-am completat astfel: fiecare camp de info
al unui nod sau frunze din arbore ia valoarea capatului din dreapta a intervalului
(fiecare nod contine valoare maxima dintre info urile copiilor).
Dupa care, parcurg lista de elemente si fac urmatoarele actiuni:
- creez un interval pt a cauta frunza la care trebuie adaugat elementul;
- daca frunza are la info valoarea capatului din dreapta, inseamna ca este libera,
deci pot insera in vectorul solutie elementul la pozitia corespunzatoare. Dupa
ce il adaug, info ul frunzei va devenii 0 si actualizez restul nodurilor din arbore,
astfel, la urmatoarea cautare, cand voi interoga arborele, valoarea acestei frunze
nu v a mai aparea.
- daca frunza are info 0 (contine deja un element), caut alta pozitie in care sa 
inserez pornind de la pozitia curenta inspre 0. Astfel, construiesc un interval
[0, n], unde n este nr pe care vreau sa il adaug, iar cand voi interoga acest
interval in arbore el imi va intoarce prima pozitie libera inspre 0, fix unde
vreau sa inserez. Analog voi schimba info ul frunzei in 0 si voi actualiza
restul nodurilor. (caz special cand trb sa adaug la pozitia 0 si acolo deja
se afla un element, voi ignora acest element)
- la fiecare inserare voi creste un contor care daca ajunge sa fie egal cu lungimea
vectorului solutie, stiu ca nu mai pot insera.
- returnez vectorul format

    -int* calculeaza_vector_raspuns_trivial: Am urmat algoritmul din enunt, am 
parcurs lista si verificam daca pozitia din vector era libera pt adaugare, in caz 
contrar ma deplasam de la aceea pozitie catre inceput pana gaseam pozitie libera.


Feedback:

    Tema a fost foarte clara si foarte bine explicata, iar notiunile invatate le 
consider utile.