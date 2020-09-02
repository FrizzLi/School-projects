/*
Nech A je mno�ina objektov, pri�om ka�d� z objektov m� priraden� svoju cenu. Na za�iatku je A pr�zdna mno�ina. Na vstupe je zadan� postupnos� pr�kazov (indexujeme od 0), ktor� m��u zv��ova� mno�inu objektov A a meni� cenu objektom z mno�iny A. Ka�d� z pr�kazov m��e sp�sobi� v�pis jedn�ho riadku do v�stupn�ho s�boru.

Zoznam pr�kazov:

v "a"
Do mno�iny A pribudne nov� objekt s cenou "a". ��slo "a" je cel� ��slo medzi 0 a 2^30.
z "i" "a"
Cena objektu pridan�ho i-tym pr�kazom sa zmen� na "a". ��sla "i" a "a" s� cel� ��sla medzi 0 a 2^30. Ak A neobsahuje objekt pridan� i-tym pr�kazom, na v�stup sa vyp�e jeden riadok obsahuj�ci text "Chyba".
m
Na v�stup sa vyp�e medi�n z cien objektov. V pr�pade, �e po�et objektov je p�rny, vyp�e sa priemer cien dvoch objektov s medi�nov�mi cenami zaokr�hlen� na cel� ��sla nadol. Ak A e�te neobsahuje prvky, na v�stup sa vyp�e jeden riadok obsahuj�ci text "Chyba".
Form�t vstupu

Na prvom riadku vstupu sa n�chadza nez�porn� cel� ��slo -- po�et podvstupov. Nasleduj�ci riadok je pr�zdny. N�sledne na vstupe nasleduje zodpovedaj�ci po�et podvstupov. Za ka�d�m podvstupom sa nach�dza pr�zdny riadok. Ka�d� podvstup sa za��na riadkom s jedn�m nez�porn�m cel�m ��slom 1<=k<=1000000 ud�vaj�cim po�et prikazov v podvstupe. Nasleduje k riadkov obsahuj�cich jednotliv� pr�kazy.

Form�t v�stupu

V�stupy jednotliv�ch podvstupov s� oddelen� pr�zdnym riadkom. Ka�d� v�pis je obsiahnut� vo svojom vlastnom riadku.

Pr�klad vstupu:

4

5
v 8
v 5
m
z 1 8
m

5
v 8
v 5
z 3 1
v 4
m

10
v 5
v 4
v 11
v 1022454
v 54
m
z 2 10000000
m
z 0 1
z 5 1

16
v 5
v 3
v 2
v 11
v 13
v 17
v 21
v 31
v 1
m
z 8 40
z 1 40
m
z 0 41
z 2 41
m

Pr�klad v�stupu (zodpoved� uk�kov�mu vstupu):

6
8

Chyba
5

11
54
Chyba

11
17
31
*/



// uloha-2.cpp -- Uloha 2
// Peter Markus, 13.4.2017 12:58:10

#include<iostream>
#include<stdio.h>
#include<limits.h>
#include<stdlib.h>
#include<set>
#define N               1000000	//(1073741824)
#define AVG(max, min)   (max+min)/2

typedef struct product {
    int val;
	int id;
} PRODUCT;

typedef struct compare {
    bool operator()(const PRODUCT& left, const PRODUCT& right) const {
        if(left.val < right.val)
            return true;
        else if(left.val > right.val)
            return false;
        else if(left.val == right.val) {
        	if(left.id < right.id)
            	return true;
            else if(left.id > right.id)
            	return false;
        }
      	return false;
    }
} COMPARE;

std::set<PRODUCT, COMPARE> smaller;
std::set<PRODUCT, COMPARE>::iterator sit;
std::set<PRODUCT, COMPARE> bigger;
std::set<PRODUCT, COMPARE>::iterator bit;
int median = 0;

void insertNew(PRODUCT buff) {
  	if(bigger.size() == smaller.size()) {
    	if(buff.val < median) {
        	smaller.insert(buff);
        	sit = smaller.end();
            median = (*--sit).val;
        }
        else {
        	bigger.insert(buff);
            bit = bigger.begin();
            median = (*bit).val;
        }
    }
    else if(smaller.size() > bigger.size()) {
    	if(buff.val < median) {
        	sit = smaller.end();
          	bigger.insert(*--sit);		// pridanie prveho prvku zo small do big
          	smaller.erase(*sit);
          	smaller.insert(buff);
        }
        else
          	bigger.insert(buff);
      	bit = bigger.begin();
    	sit = smaller.end();
    	median = AVG((*bit).val, (*--sit).val);
  	}
    else{
    	if(buff.val < median)
        	smaller.insert(buff);
        else {
        	bit = bigger.begin();
          	smaller.insert(*bit);
          	bigger.erase(*bit);
          	bigger.insert(buff);
       	}
      	bit = bigger.begin();
    	sit = smaller.end();
    	median = AVG((*bit).val, (*--sit).val);
 	}
}

int main()
{
  	PRODUCT buff;
  	int *array;

    int i, j, k, l;
    int a, b;
    char c;
	int itemAdded;

    scanf("%d\n\n", &k);
    for(i = 0; i < k; i++) {
      	smaller.clear();
      	bigger.clear();
      	array = (int*)malloc(N*sizeof(int));
        scanf("%d\n", &l);
        itemAdded = 0;
        if(i != 0)
            printf("\n");
        for(j = 0; j < l; j++) {
            scanf("%c\n", &c);
            switch(c) {
            case 'v' :
              	itemAdded = 1;
                scanf(" %d\n", &a);
              	buff.val = a;
              	buff.id = j;
              	array[j] = a;
              	insertNew(buff);
                break;
            case 'z' :
                scanf(" %d %d\n", &a, &b);
                if(j <= a || array[a] == 0) {
                    printf("Chyba\n");
                    break;
                }
              	buff.id = a;
              	buff.val = array[a];

              // este v tom co musim erasovat, musim skontrolovat najprv
              // ci tam nieje menej prvkov nez v druhom sete
              if(median <= array[a]) {			// porovnavanie chyba maybe <=?
                if(bigger.size() < smaller.size()) {
                    sit = smaller.end();
          			bigger.insert(*--sit);		// pridanie prveho prvku zo small do big
          			smaller.erase(*sit);
                }
                bit = bigger.find(buff);
                bigger.erase((*bit));
              }
              else {
                if(smaller.size() < bigger.size()) {
                  	bit = bigger.begin();
          			smaller.insert(*bit);
          			bigger.erase(*bit);
                }
                sit = smaller.find(buff);
                smaller.erase((*sit));
              }
              buff.val = b;
              insertNew(buff);
              array[a] = b;
              break;
            case 'm' :
                if(itemAdded == 0 || j == 0) {
                    printf("Chyba\n");
                    break;
                }
                printf("%d\n", median);
                break;
            default : ;
            }
        }
    }
    return 0;
}
