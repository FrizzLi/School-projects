/*
Meno: Peter Marku�
Datum: 30.10.2017

Simulujte nasleduj�cu situ�ciu. V istej firme je 7 program�torov a 5 testerov.
Testeri sa delia na 2 skupiny - dvaja r�chli (testuj� 2s) a traja pomal� (testuj� 3s).
Program�tori a testeri neust�le pracuj� na vyv�janom programe,
pri�om naraz na �om m��u pracova� jedine program�tori alebo testeri, nie spolu, ke�e by to pokazilo v�sledky.

Programovanie trv� program�torovi nejak� �as - v simul�cii 3s, pri�om potom si d�va 2s prest�vku.

Testovanie trv� tie� nejak� �as, pod�a v�konnosti testera - 2s alebo 3s.
Testeri si d�vaj� 3s prest�vku v men��ch skupink�ch po troch, po tom,
�o sa ich nazbiera dostato�n� po�et po testovan� (ka�d� tester si mus� spravi� prest�vku, k�m za�ne znova testova�).

Cela simul�cia nech trv� 30s.

1. Dopl�te do programu po��tadlo po��taj�ce, ko�ko kr�t ktor�
tester a program�tor pracoval na programe. [1b]

2. Zabezpe�te, aby na programe naraz nepracovali testeri a program�tori,
pri�om �lenovia rovnakej skupiny m��u pracova� spolo�ne.
Ak na za�atie pr�ce �ak� u� cel� druh� skupina - 7 program�torov,
alebo 5 testerov u� �al�� �lenovia opa�nej skupiny neza��naj� pr�cu na programe. [4b]

2. Zabezpe�te spr�vne o�etrenie prest�vky testerov v skupinke po troch. [4b]

3. O�etrite v programe spr�vne ukon�enie simul�cie po uplynut� stanoven�ho �asu. [1b]

Pozn�mky:
- na synchroniz�ciu pou�ite iba mutexy a podmienen� premenn�
- nespoliehajte sa na uveden� �asy, simul�cia by mala fungova� aj s in�mi �asmi
- build (console): gcc programatori_a_testeri -o programatori_a_testeri -lpthread
*/


 /// STRUCT predn
 // 2m riadky
 #include<stdio.h>
 #include<stdlib.h>
 #include<pthread.h>
 #include<unistd.h>
 
 pthread_mutex_t mutex    = PTHREAD_MUTEX_INITIALIZER;
 pthread_mutex_t mutex2   = PTHREAD_MUTEX_INITIALIZER;
 pthread_cond_t cond      = PTHREAD_COND_INITIALIZER;
 pthread_cond_t cond2     = PTHREAD_COND_INITIALIZER;
 pthread_cond_t cond3     = PTHREAD_COND_INITIALIZER;
 int testWait = 0;       // pocet aktualne cakajucich
 int progWait = 0;
 
 int testers = 0;        // pocet aktualne testujucich/programujucich
 int progs = 0;
 
 int barr = 0;
 int inBarrier = 0;
 
 
 // signal na zastavenie simulacie
 int stoj = 0;
 
 // programovanie
 void programovanie() {
     sleep(3);
 }
 
 // prestavka programatora
 void prestavka_programator() {
     sleep(2);
 }
 
 // testovanie
 void testovanie(int i) {
     i < 2 ? sleep(2) : sleep(3);
 }
 
 // prestavka testera
 void prestavka_tester(int i) {
     sleep(3);
 }
 
 // programator
 void *programator(void * ptr) {
     int id = (int) ptr;
     int progCounter = 0;
 
     // pokial nie je zastaveny
     while(!stoj) {
         pthread_mutex_lock(&mutex);
         while(testWait == 5 || testers) {     // 5 testovacov caka || niekto testuje
             if(stoj)
                 break;
             progWait++;
             pthread_cond_wait(&cond, &mutex);
         }
         if(stoj)
             break;
         progs++;
         progWait--;
         pthread_mutex_unlock(&mutex);
 
         printf("%d. Programujem...\n", id);
         programovanie();
 
         pthread_mutex_lock(&mutex);
         progCounter++;
         progs--;
         pthread_mutex_unlock(&mutex);
 
         pthread_cond_broadcast(&cond2);
 
         printf("%d. PROG oddychuje..\n", id);
         if(stoj)
             break;
         prestavka_programator();
     }
 
     pthread_cond_broadcast(&cond);
     pthread_mutex_unlock(&mutex);
     printf("%d. programator prog. %d-krat\n", id, progCounter);
     pthread_exit(0);
 }
 
 // tester
 void *tester(void * ptr) {
     int id = (int) ptr;
     int testCounter = 0;
 
     // pokial nie je zastaveny
     while(!stoj) {
 
         pthread_mutex_lock(&mutex2);
         while(progWait == 7 || progs) {  // 7 programatorov caka || niekto programuje
             if(stoj)
                 break;
             testWait++;
             pthread_cond_wait(&cond2, &mutex2);
         }
         if(stoj)
             break;
         testers++;
         testWait--;
         pthread_mutex_unlock(&mutex2);
 
         printf("                                    %d. Tester testuje..\n", id);
         testovanie(id);
 
         pthread_mutex_lock(&mutex2);
         testCounter++;
         barr++;
         testers--;
         if(barr == 3) {                 // prestavka po troch...
             inBarrier = 1;
             pthread_cond_broadcast(&cond3);
         } else
             while(!inBarrier) {
                 if(stoj)
                     break;
                 pthread_cond_wait(&cond3, &mutex2);
             }
         barr--;
         if(barr == 0) {
             inBarrier = 0;
             pthread_cond_broadcast(&cond3);
         } else
             while(inBarrier) {
                 if(stoj)
                     break;
                 pthread_cond_wait(&cond3, &mutex2);
             }
         pthread_mutex_unlock(&mutex2);
 
         pthread_cond_broadcast(&cond);
 
         printf("                                    %d. tester oddychuje..\n", id);
         if(stoj)
             break;
         prestavka_tester(id);
     }
 
     pthread_cond_broadcast(&cond2);
     pthread_cond_broadcast(&cond3);
     pthread_mutex_unlock(&mutex2);
     printf("%d. tester test. %d-krat\n", id, testCounter);
     pthread_exit(0);
 }
 
 int main(void) {
     int i;
 
     pthread_t programatori[7];
     pthread_t testeri[5];
 
     for (i = 0; i < 7; ++i) {
         pthread_create(&programatori[i], NULL, &programator, (void*) i);
     }
 
     for (i = 0; i < 5; ++i) {
         pthread_create(&testeri[i], NULL, &tester, (void*) i);
     }
 
     sleep(30);
     stoj = 1;
 
     for (i = 0; i < 7; ++i) {
         pthread_join(programatori[i], NULL);
     }
 
     for (i = 0; i < 5; ++i) {
         pthread_join(testeri[i], NULL);
     }
 
     exit(EXIT_SUCCESS);
 }
 