
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2020
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
	L->Act = NULL;
    L->First = NULL;
    L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
	tDLElemPtr current = L->First;
    while(current!=NULL)
    {
        tDLElemPtr next_element = current->rptr;
        free(current);
        current = next_element;
    }
    L->Act = NULL;
    L->First = NULL;
    L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	tDLElemPtr new = (tDLElemPtr)malloc(sizeof(struct tDLElem));
    if(new == NULL)
    {
        DLError();
    }
    else
    {
        new->data = val;
        new->lptr = NULL;
        new->rptr = L->First;
        if(L->First == NULL)
        {
            L->Last = new; 
        }
        else
        {
            L->First->lptr = new;
        }
        
        L->First = new;
    }
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	
	tDLElemPtr new = (tDLElemPtr)malloc(sizeof(struct tDLElem));
    if(new == NULL)
    {
        DLError();
    }
    else
    {
        new->data = val;
        new->rptr = NULL;
        new->lptr = L->Last;
        if(L->Last == NULL)
        {
            L->First = new;
        }
        else
        {
            L->Last->rptr = new;
            
        }
        L->Last = new;
    }
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    if(L->First == NULL)
    {
        DLError();
    }
    else
    {
        *val = L->First->data;
    }
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if(L->First == NULL)
    {
        DLError();
    }
    else
    {
        *val = L->Last->data;
    }
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	if(L->First!=NULL)
    {
        if(L->Act == L->First)
        {
            L->Act = NULL;
        }
        tDLElemPtr tmp = L->First;
        //list has only 1 element
        if(L->First == L->Last)
        {
            L->First = NULL;
            L->Last = NULL;
        }
        else
        {
            //set left pointer of next element to null
            tmp->rptr->lptr = NULL;
            //set next element as first and free memory
            L->First = tmp->rptr;
        }
        free(tmp);
    }
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L.
** Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/ 
    //list has any elements
	if(L->First != NULL)
    {
        //remove active element, if active == last
        if(L->Act == L->Last)
        {
            L->Act = NULL;
        }
        tDLElemPtr tmp = L->Last;
        //list has 1 element
        if(L->Last == L->First)
        {
            L->First = NULL;
            L->Last = NULL;
        } 
        else
        {
            //set prev element rptr to null - 
            tmp->lptr->rptr = NULL;
            //set prev element as last and free memory
            L->Last = tmp->lptr;
        }
        free(tmp);
    }
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
    if(L->Act != NULL && L->Act != L->Last)
    {
        tDLElemPtr tmp = L->Act->rptr;
        //set rptr to active -> active+2
        L->Act->rptr = tmp->rptr;
        //element which is being deleted was last - update last to active
        if(tmp == L->Last)
        {
            L->Last = L->Act;
        }
        //update rptr of active+2 only if active+1 is not last
        if(tmp->rptr != NULL)
        {
            //set lptr of active+2 to active 
            tmp->rptr->lptr = L->Act;
        }

        free(tmp);
    }
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
	if(L->Act != NULL && L->Act != L->First)
    {
        tDLElemPtr tmp = L->Act->lptr;
        //set lptr to active -> active-2
        L->Act->lptr = tmp->lptr;
        if(tmp == L->First)
        {
            L->First = L->Act;
        }
        else
        {
            //set rptr of active-2 to active
             tmp->lptr->rptr = L->Act;
        }
        free(tmp);
    }
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if(L->Act != NULL)
    {
        tDLElemPtr new = (tDLElemPtr)malloc(sizeof(struct tDLElem));
        if(new == NULL)
        {
            DLError();
        }
        else
        {
            new->data = val;
            //set pointers of new element
            //left = active and right = active + 2 
            new->rptr = L->Act->rptr;
            new->lptr = L->Act;
            if(L->Act == L->Last)
            {
                L->Last = new;
            }
            else
            {
                //set active+1 left pointer to new element
                L->Act->rptr->lptr = new;
            }
             //right pointer of active to new element
            L->Act->rptr = new;
            
        }
        
    }
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if(L->Act != NULL)
    {
        tDLElemPtr new = (tDLElemPtr)malloc(sizeof(struct tDLElem));
        if(new == NULL)
        {
            DLError();
        }
        else
        {
            new->data = val;
            //set pointers of new element
            //left pointer = active-1 and right pointer = active
            new->lptr = L->Act->lptr;
            new->rptr = L->Act;
            if(L->Act == L->First)
            {
                L->First = new;
            }
            else
            {
                //set active-1 right pointer to new
                L->Act->lptr->rptr = new;
            }
            //active left pointer to new
            L->Act->lptr = new;

        }
    }
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
	if(L->Act == NULL)
    {
        DLError();
    }
    else
    {
        *val = L->Act->data;
    }
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	if(L->Act != NULL)
    {
        L->Act->data = val;
    }
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	
	if(L->Act != NULL)
    {
        L->Act = L->Act->rptr;
    }
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	if(L->Act != NULL)
    {
        L->Act = L->Act->lptr;
    }
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	return (L->Act != NULL);
}

/* Konec c206.c*/
