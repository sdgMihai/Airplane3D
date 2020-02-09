Gheoace Mihai
334CA

	Proiectul reprezinta o implementare a jocului FlappyBird.
	Jocul este implementat in mai multe clase, fiecare avand o functie
bine definita.
	Fiecare clasa are definitia in Nume_clasa.cpp si declaratia in Nume_clasa.h.
	Logica jocului, lumea este implementata in clasa Tema1. Ea apeleaza direct
sau indirect pe celelalte.
	Astfel, elementele de baza ale jocului sunt implementate de clasele
Bird si Blocks.
	Aspectele de baza sunt implementate in clasele Collision si GameStatistics.
	Pentru transformari 2D este clasa Transform2D iar pentru salvarea
parametrilor de scalare, translatie si rotire este folosita clasa Transform si Pipe.
Pipe mosteneste pe Transform, adaugand si un id.

Blocks are un mod unitar de a trata stalpii, atat stalpul cat si capul lui,
dar ii reprezinta intern in doua liste de elemente.

Clasa GameStatistics se ocupa de statistici si afisari in consola.
Clasa Collision se ocupa de coliziuni si semnalarea evitarilor coliziunilor,
adica o marire de punctaj, pe care o paseaza clasei Collision.

Clasele Collision, GameStatistics si Tester sunt implementate singleton.
Asadar ele NU vor fi instantiate, ci vor fi apelate conform principului
design pattern-ului lor.
Clasa Tester, este folositoare pentru debug, pentru a afisa in fisiere
de log ce se intampla cu membrii claselor.