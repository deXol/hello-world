#include <iostream>
#include <cstdlib>
#include <cstdio>

/**********************************
****     Kotelezo feladat     *****
****   Rendezettmintafa (c++) *****
****   2 pointeres tarolassal *****
****                          *****
****    Nev: Toth Oliver      *****
****    Eha: TOOTAAT.SZE      *****
****  Gyak.vez.: Godo Zita    *****
**********************************/


using namespace std;

template <class K, class V>
class fa {
public:
        class fapont {
        public:
            K key ;
            V value ;
            fapont * fiu, * tespa;
            int meret;
            bool bal;
            fapont() : fiu(NULL), tespa(NULL), meret(1), bal(false){};
            fapont(K k, V v) : key(k), value(v) {fiu=NULL; tespa=NULL; meret=1; bal=false;}
            ~fapont(){
                fiu=NULL; tespa=NULL;
                delete fiu; delete tespa;
                }
        };
	fapont * gyoker ;
	fa() {
		gyoker=NULL;
	}

/*******
Segedfuggvenyek
********/

//Adott pont balfia hatarozza meg
	fapont * balfia(fapont * p){
        if (p->fiu && p->fiu->bal) return p->fiu;
        else return NULL;
	}

//Adott pont jobbfiat hatarozza meg
	fapont * jobbfia(fapont * p){
        if (p->fiu && p->fiu->bal==false) return p->fiu;
        else if (p->fiu && p->fiu->bal && p->fiu->tespa!=p) return p->fiu->tespa;
        else return NULL;
	}

//Adott pont apjat hatarozza meg
	fapont * apja (fapont * p){
        if (p->bal==false || (p->bal and p->tespa->fiu==p)) return p->tespa;
        else if (p->bal and p->tespa->fiu!=p and p->tespa!=NULL) return p->tespa->tespa;
        else return NULL;
        }

//Beszurasnal a meretet aktualizalja
    void beszjav(fapont * p)
	{
	    while (p!=gyoker) {
            p=apja(p);
            p->meret++;
	    }
	}

//Torlesnel a meretet aktualizalja
	void torljav(fapont * p)
	{
	    while (p!=gyoker) {
            p=apja(p);
            p->meret--;
	    }
	}

/*************
Beszur fuggveny
bool beszur(<K> key,<V> value) O(logn)
**************/
	bool beszur(K x,V v) {
     fapont* p=new fapont(x, v);
     fapont* szulo;
        if(gyoker==NULL) //ures a fa
        {
            gyoker=p;
            return true;
        }
        else
        {
            fapont* seg;
            seg=gyoker;
            while(seg) //megkeressuk a beszurando csucs helyet
            {
                szulo=seg;
                if(p->key > seg->key)
                {
                    if (seg->fiu==NULL) seg=seg->fiu;
                    else
                        {
                             if (seg->fiu->tespa==seg && seg->fiu->bal)
                                {
                                    seg->fiu->tespa=NULL;
                                    seg=seg->fiu->tespa;
                                }
                                 else if (seg->fiu->tespa==seg)
                                {
                                    szulo=seg->fiu;
                                    seg=seg->fiu->fiu;
                                }
                                else {seg=seg->fiu->tespa;}
                        }
                }
                             else if(p->key < seg->key)
                                {
                                    if(seg->fiu==false)
                                        seg=seg->fiu;
                                    else if(jobbfia(seg) && balfia(seg)==NULL)
                                    {
                                        seg=NULL;
                                    }
                                    else
                                    {
                                        seg=seg->fiu;
                                    }
                                }
                    else return false;

            }
            if(p->key < szulo->key) //p bal fiu lesz
            {
                if(szulo->fiu==NULL) //apjanak nincs fia
                {
                    szulo->fiu=p;
                    p->tespa=szulo;
                    p->bal=true;
                }
                else //apjanak van fia
                {
                    p->tespa=szulo->fiu;
                    szulo->fiu=p;
                    p->bal=true;
                }
                beszjav(p); //meret javitasa
                return true;
            }
            else if(p->key > szulo->key) //p jobb fiu lesz
            {
                p->tespa=szulo;
                if(szulo->fiu!=NULL) //p apjanak van fia
                {
                    (szulo->fiu->tespa)=p;
                }
                else //p apjanak nincs fia
                {
                    szulo->fiu=p;
                }
                beszjav(p); //meret javitasa
                return true;
            }
        }
     return false;
	}


/*************
Mindetkiir fuggveny
mindetkiir(fapont p) O(n)
**************/
	void mindetkiir(fapont * p) {
		if (p!=NULL) {
			mindetkiir(balfia(p)) ;
			cout << p->key << " merete: " << p->meret << " rang: "<< rang(p)<<endl;
			mindetkiir(jobbfia(p)) ;

		}
	}

/*************
Elemszam fuggveny
int elemszam(fapont p) O(1)
**************/
	int elemszama(fapont * p){
        return p->meret;
	}


/*************
Rangkeres fuggveny
fapont p rangkeres(int x) O(logn)
**************/

    fapont * rangkeres(int x){
        return rangkeresseg(gyoker,x);
    }

//rangkeres segedfuggvenye
	fapont * rangkeresseg(fapont * p, int x){
	    int r;
        if (balfia(p)!=NULL) r=balfia(p)->meret+1;
            else r=1;
        if (x==r) return p;
        if (x<r) return rangkeresseg(balfia(p),x);
        else return rangkeresseg(jobbfia(p),x-r);
	}

/*************
Rang fuggveny
int rang(fapont p) O(logn)
**************/
	int rang (fapont * p){
        int r;
        if (balfia(p)!=NULL) r=balfia(p)->meret+1;
            else r=1;
        fapont * q=p;
        while (q!=gyoker)
            {
                if (q==jobbfia(apja(q)))
                  {
                     if (balfia(apja(q))!=NULL) r+=balfia(apja(q))->meret+1;
                     else r+=1;
                  }
                q=apja(q);
            }
        return r;
	}

/*************
Keres fuggveny
fapont keres(<K> x) O(logn)
**************/
	fapont * keres(K x){
        fapont * p=gyoker;
        while (p->key!=x)
         {
             if (x<p->key) p=balfia(p);
             else p=jobbfia(p);
             if (p==NULL) return NULL;
         }
      return p;
	}

/*************
Kovetkezo elem fuggveny
fapont rákövetkezõ(fapont p) O(logn)
**************/
	fapont * kovetkezo(fapont * p){
	    if(p==NULL) return NULL; //ha p nem letezik
		if(keres(p->key)==NULL) return NULL; //ha nincs adott kulcsu elem
        if (jobbfia(p)!=NULL) //van jobbfia
        {
            p=jobbfia(p);
            while (balfia(p)!=NULL) p=balfia(p);
            return p;
        } else //nincs jobbfia
            {
              while (apja(p)!=NULL){
               if (balfia(apja(p))==p) return apja(p);
               p=apja(p);
              }
             return NULL;
            }
	}

/*************
Torol fuggveny
bool torol(fapont p) O(logn)
**************/
bool torol (fapont * p){
	if(p==NULL) return false; //nincs adott p elem
	if(balfia(p)==NULL && jobbfia(p)==NULL && p!=gyoker) //p-nek nincs gyereke és nem a gyoker, tehat level lesz
	{
	    torljav(p);
		if(p->bal && p->tespa==apja(p)) //p bal gyerek es nincs testvere
			{ p->tespa->fiu=NULL; }
		else if(p->bal)  //p bal gyerek es van testvere
			{ p->tespa->tespa->fiu=p->tespa; }
		else if(p->bal==false && p->tespa->fiu==p) //p jobb gyerek es nincs testvere
			{ p->tespa->fiu=NULL;  }
		else //p jobb gyerek es van testvere
		{
			p->tespa->fiu->tespa=apja(p);
		}
		delete p;
	}
	else if(balfia(p)==NULL && jobbfia(p)==NULL) //p gyoker es nincs gyereke
	{
		gyoker=NULL;
		delete p;
	}
	else if(jobbfia(p) && balfia(p)==NULL)  //p-nek csak jobbfia van
	{
	    torljav(p);
		if(p->tespa==NULL) //p gyoker
		{
			gyoker=jobbfia(p);
			gyoker->tespa=NULL;
		}
		else if(apja(p) && apja(p)->fiu==p) //p nem gyoker es balfiu
		{
			p->fiu->tespa=apja(p);
			apja(p)->fiu=p->fiu;
		}
		else
		{
			p->fiu->tespa=apja(p);
			apja(p)->fiu->tespa=p->fiu;
		}
		delete p;
	}
	else if(jobbfia(p)==NULL && balfia(p)) //bal gyereke van, de jobb nincs
	{
	    torljav(p);
		if(p->tespa==NULL) //ha a gyokerrol van szo
		{
			gyoker=balfia(p);
			gyoker->tespa=NULL;
		}
		else if(p->tespa==apja(p)) //nincs  testvere
		{
			p->fiu->tespa=apja(p);
			apja(p)->fiu=p->fiu;
		}
		else  // van testvere
		{
			apja(p)->fiu=p->fiu;
			p->fiu->tespa=p->tespa;
		}
		delete p;
	}
	else if(jobbfia(p) && balfia(p)) //bal es jobb gyereke is van
	{
	    torljav(p);
		fapont* seg=kovetkezo(p);
		p->key=seg->key; p->value=seg->value; p->meret--;
		if(seg->bal==false && jobbfia(seg)==NULL) //seged balfiu es nincs fia
            {
                p->fiu->tespa=p;
            }
        else if (seg->bal==false && jobbfia(seg))  //seged jobbfiu és van fia
            {
                p->fiu->tespa=p->fiu->tespa->fiu;
                p->fiu->tespa->tespa=p;
            }
		else if (seg->tespa==apja(seg)) //seged balfiu es nincs testvere
            {
                apja(seg)->fiu=NULL;
            }
		     else {
		           apja(seg)->fiu=seg->tespa; //seged balfiu es van testvere
                  }
        delete seg;
	}
	return true;
 }

/*************
Mindettorol fuggveny
mindettorol(fapont p) O(logn)
**************/
//mindettorol segedfuggvenye
	void mindettorol(fapont * p) {
		if (p!=NULL) {
			mindettorol(balfia(p)) ;
			torol(p);
			mindettorol(jobbfia(p)) ;

		}
	}
};

/*******
Main fuggveny
*******/
int main()
{
    fa<int,int> mintafa;
    cout<<"Peldaprogram: "<<endl;

    //elemek beszurasa
    mintafa.beszur(10,4);
    mintafa.beszur(6,33);
    mintafa.beszur(15,6);
    mintafa.beszur(3,5);
    mintafa.beszur(8,11);
    mintafa.beszur(13,40);
    mintafa.beszur(20,20);

    cout<<"A fa a kovetkezo elemeket tartalmazza:"<<endl;
    mintafa.mindetkiir(mintafa.gyoker);
/*
Beszuras
*/
    bool benn=true;
     while (benn)
      {
          char k;
          int key,value;
          cout<<"Szeretne meg elemet beszurni? (y/n)"<<endl;
          cin>>k; getchar();
          if (k=='y')
           {
               cout<<"Adja meg az kulcsot: ";
               cin>>key; getchar();
               cout<<"Adja meg az erteket: ";
               cin>>value; getchar();
               if (mintafa.beszur(key,value)) {
                   cout<<"A beszuras sikeres volt."<<endl;
                   cout<<"Folytatashoz nyomjon ENTER-t."<<endl;
                   getchar();
                   system("cls");
                   mintafa.mindetkiir(mintafa.gyoker);}
               else cout<<"Hiba tortent"<<endl;
           }
           else if (k=='n') {benn=false; system("cls"); mintafa.mindetkiir(mintafa.gyoker);}
           else cout<<"Nem jo parancs, probalja ujra."<<endl;
      }

/*
Torles
*/
     benn=true;
     while (benn)
      {
          char k;
          int key;
          cout<<"Szeretne elemet torolni? (y/n)"<<endl;
          cin>>k; getchar();
          if (k=='y')
           {
               cout<<"Adja meg az kulcsot: ";
               cin>>key; getchar();
               if (mintafa.keres(key)) {
                   mintafa.torol(mintafa.keres(key));
                   cout<<"A torles sikeres volt."<<endl;
                   cout<<"Folytatashoz nyomjon ENTER-t."<<endl;
                   getchar();
                   system("cls");
                   mintafa.mindetkiir(mintafa.gyoker);}
               else cout<<"Nincs adott kulcsu elem."<<endl;
           }
           else if (k=='n') {benn=false; system("cls"); mintafa.mindetkiir(mintafa.gyoker);}
           else cout<<"Nem jo parancs, probalja ujra."<<endl;
      }

/*
Rakovetkezo elem
*/
      int el;
      cout<<"Valasszon egy elemet: "<<endl;
      cin>>el; getchar();
      if (mintafa.keres(el)){
        if (mintafa.kovetkezo(mintafa.keres(el))) cout<<"A rakovetkezo elem: "<<mintafa.kovetkezo(mintafa.keres(el))->key<<endl;
         else cout<<"Nincs rakovetkezo elem."<<endl;
      } else cout<<"Nincs ilyen kulcsu elem."<<endl;


      cout<<endl<<"A peldaprogram vege."<<endl;
      getchar();
    return 0;
}
