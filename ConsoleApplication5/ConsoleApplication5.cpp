#include <iostream>
using namespace std;


struct Wezel {
	int wartosc;
	Wezel* dziecko = NULL;
	Wezel* brat = NULL;
	Wezel* rodzic = NULL;
	explicit Wezel(int wartosc);
	void dodaj_dziecko(int wartosc);
	void dodaj_brata(int wartosc);
};

Wezel::Wezel(int wartosc) {
	this->wartosc = wartosc;
}
void Wezel::dodaj_dziecko(int wartosc) {
	Wezel* nowy = new Wezel(wartosc);
	nowy->rodzic = this;
	this->dziecko = nowy;
}
void Wezel::dodaj_brata(int wartosc) {
	Wezel* nowy = new Wezel(wartosc);
	nowy->rodzic = this->rodzic;
	this->brat = nowy;
}

int czytaj(char& tmp, int& ile_pustych, int& a) {
	//static int a = 0;
	string wartosc = "";
	cin.get(tmp);
	while (tmp != ',' && tmp != '(' && tmp != ')') {
		wartosc += tmp;
		cin.get(tmp);
	}

	if (atoi(wartosc.c_str()) > 0) return atoi(wartosc.c_str());
	else {
		ile_pustych++;
		a--;
		return a;
	}
}

int porownaj_galezie_z_liscmi(Wezel* wezel, int wartosc) {
	Wezel* obecny = wezel->dziecko;
	int ile_powtorzen = 0;
	if (obecny->wartosc == wartosc) ile_powtorzen++;
	while (true) {
		if (obecny->dziecko != NULL) {
			obecny = obecny->dziecko;

		}
		else if (obecny->brat != NULL) {
			obecny = obecny->brat;

		}
		else if (obecny->rodzic != NULL) {
			while (obecny != wezel && obecny->rodzic->brat == NULL)
				obecny = obecny->rodzic;
			if (obecny == wezel);
			else if (obecny->rodzic != wezel) obecny = obecny->rodzic->brat;
			else obecny = obecny->rodzic;
		}
		if (obecny == wezel) break;
		if (obecny->wartosc == wartosc) ile_powtorzen++;
	}
	return ile_powtorzen;
}

void brut(int wymiar_i, int wymiar_j, int** tab, int obecna_suma, int& max) {
	if (wymiar_i == 1 || wymiar_j == 1) {
		for (int i = 0; i < wymiar_i; i++) {
			for (int j = 0; j < wymiar_j; j++) {
				if (obecna_suma + tab[i][j] > max) max = obecna_suma + tab[i][j];
			}
		}
	}
	else {
		int** nowa = new int* [wymiar_i - 1];
		for (int i = 0; i < wymiar_i - 1; i++) {
			nowa[i] = new int[wymiar_j - 1];
		}
		if (wymiar_i < wymiar_j) {
			for (int x = 0; x < wymiar_j; x++) {
					for (int i = 0, k = 0; i < wymiar_i - 1; i++, k++) {
						if (k == i) k++;
						for (int j = 0, l = 0; j < wymiar_j - 1; j++, l++) {
							if (x == j) l++;
							nowa[i][j] = tab[k][l];
						}
					}
					brut(wymiar_i - 1, wymiar_j - 1, nowa, obecna_suma + tab[0][x], max);
				}
		}
		else {
			for (int x = 0; x < wymiar_i; x++) {
					for (int i = 0, k = 0; i < wymiar_i - 1; i++, k++) {
						if (x == i) k++;
						for (int j = 0, l = 0; j < wymiar_j - 1; j++, l++) {
							if (l == j) l++;
							nowa[i][j] = tab[k][l];
						}
					}
					brut(wymiar_i - 1, wymiar_j - 1, nowa, obecna_suma + tab[x][0], max);
			}
		}
		for(int i=0; i<wymiar_i-1; i++)
			delete[] nowa[i];
		delete[] nowa;
	}
}

Wezel** stworz_tablice_dzieci(Wezel* wezel, int rozmiar) {
	Wezel* obecny = wezel->dziecko;
	Wezel** tab = new Wezel * [rozmiar];
	for (int j = 0; j < rozmiar; j++) {
		tab[j] = obecny;
		obecny = obecny->brat;
	}
	return tab;
}

int rozmiar_tablicy_dzieci(Wezel* wezel) {
	int i = 1;
	Wezel* obecny = wezel->dziecko;
	while (obecny->brat != NULL) {
		obecny = obecny->brat;
		i++;
	}

	return i;
}

int zwroc_indeks(Wezel** tab, int rozmiar, Wezel* szukany) {
	for (int i = 0; i < rozmiar; i++) {
		if (tab[i] == szukany) return i;
	}
	return -1;
}




int stworz_tablice(Wezel** pierwsze_drzewo, Wezel** drugie_drzewo, int ile_pustych1, int ile_pustych2, int ile_wezlow1, int ile_wezlow2, int ile_lisci1, int ile_lisci2) {
	int max_c = 0;
	int** tab = new int* [ile_wezlow1];
	for (int i = 0; i < ile_wezlow1; i++) {
		tab[i] = new int[ile_wezlow2];
	}
	for (int i = 0; i < ile_lisci1; i++) {
		for (int j = 0; j < ile_lisci2; j++) {
			if (pierwsze_drzewo[i]->wartosc == drugie_drzewo[j]->wartosc) tab[i][j] = 1;
			else tab[i][j] = 0;
		}
	}
	for (int i = 0; i < ile_pustych1; i++) {
		for (int j = 0; j < ile_lisci2; j++) {
			tab[i + ile_lisci1][j] = porownaj_galezie_z_liscmi(pierwsze_drzewo[i + ile_lisci1], drugie_drzewo[j]->wartosc);
		}
	}
	for (int i = 0; i < ile_lisci1; i++) {
		for (int j = 0; j < ile_pustych2; j++) {
			tab[i][j + ile_lisci2] = porownaj_galezie_z_liscmi(drugie_drzewo[j + ile_lisci2], pierwsze_drzewo[i]->wartosc);
		}
	}



	for (int i = ile_pustych1 - 1; i >= 0; i--) {

		for (int j = ile_pustych2 - 1; j >= 0; j--) {
			int max_t = 0;

			Wezel** temp1 = stworz_tablice_dzieci(pierwsze_drzewo[i + ile_lisci1], rozmiar_tablicy_dzieci(pierwsze_drzewo[i + ile_lisci1]));
			Wezel** temp2 = stworz_tablice_dzieci(drugie_drzewo[j + ile_lisci2], rozmiar_tablicy_dzieci(drugie_drzewo[j + ile_lisci2]));
			int** tmp = new int* [rozmiar_tablicy_dzieci(pierwsze_drzewo[i + ile_lisci1])];
			for (int x = 0; x < rozmiar_tablicy_dzieci(pierwsze_drzewo[i + ile_lisci1]); x++) {
				tmp[x] = new int[rozmiar_tablicy_dzieci(drugie_drzewo[j + ile_lisci2])];
				for (int y = 0; y < rozmiar_tablicy_dzieci(drugie_drzewo[j + ile_lisci2]); y++) {
					int a = zwroc_indeks(pierwsze_drzewo, ile_wezlow1, temp1[x]);
					int b = zwroc_indeks(drugie_drzewo, ile_wezlow2, temp2[y]);
					i; j;
					tmp[x][y] = tab[a][b];
				}
			}
			brut(rozmiar_tablicy_dzieci(pierwsze_drzewo[i + ile_lisci1]), rozmiar_tablicy_dzieci(drugie_drzewo[j + ile_lisci2]), tmp, 0, max_t);
			for (int x = 0; x < rozmiar_tablicy_dzieci(drugie_drzewo[j + ile_lisci2]); x++) {
				int a = tab[i + ile_lisci1][zwroc_indeks(drugie_drzewo, ile_lisci2 + ile_pustych2, temp2[x])];
				if (a > max_t) max_t = a;
			}
			for (int x = 0; x < rozmiar_tablicy_dzieci(pierwsze_drzewo[i + ile_lisci1]); x++) {
				int a = tab[zwroc_indeks(pierwsze_drzewo, ile_lisci1 + ile_pustych1, temp1[x])][j + ile_lisci2];
				if (a > max_t) max_t = a;
			}
			tab[i + ile_lisci1][j + ile_lisci2] = max_t;
			
			
			
			for(int x=0; x<rozmiar_tablicy_dzieci(pierwsze_drzewo[i+ile_lisci1]); x++)
				delete[]tmp[x];
			delete[] tmp;
			delete[] temp1;
			delete[] temp2;
		}
	}
	for (int i = 0; i < ile_lisci1 + ile_pustych1; i++) {
		for (int j = 0; j < ile_lisci2 + ile_pustych2; j++) {
			if (tab[i][j] > max_c) max_c = tab[i][j];
		}
	}
	for(int i=0; i<ile_wezlow1; i++)
		delete[]tab[i];
	delete[] tab;
	return max_c;
}


int main()
{
	int ilosc_drzew;
	cin >> ilosc_drzew;
	int* ile_pustych = new int[ilosc_drzew];
	int* ile_wezlow = new int[ilosc_drzew];
	int* ile_lisci = new int[ilosc_drzew];
	Wezel** roots = new Wezel* [ilosc_drzew];
	Wezel*** tab_wezlow = new Wezel** [ilosc_drzew];
	for (int i = 0; i < ilosc_drzew; i++) {
		getchar();
		ile_pustych[i] = 1;
		Wezel* root = new Wezel(0);
		Wezel* obecny = root;
		int a = 0;
		int indeks = 0;
		char tmp = '0';
		cin.get(tmp);
		while (true) {
			if (tmp == '(') {
				obecny->dodaj_dziecko(czytaj(tmp, ile_pustych[i], a));
				indeks++;
				obecny = obecny->dziecko;
			}
			else if (tmp == ')') {
				obecny = obecny->rodzic;
				cin.get(tmp);
			}
			else if (tmp == ',') {
				obecny->dodaj_brata(czytaj(tmp, ile_pustych[i], a));
				indeks++;
				obecny = obecny->brat;
			}
			else if (tmp == ';') {
				break;
			}
		}
		roots[i] = obecny;
		ile_wezlow[i] = indeks + 1;
		ile_lisci[i] = ile_wezlow[i] - ile_pustych[i];
		tab_wezlow[i] = new Wezel * [ile_wezlow[i]];
		Wezel** tab_pustych = new Wezel * [ile_pustych[i]];
		int puste_w_tablicy = 1;
		tab_pustych[0] = obecny;
		obecny = obecny->dziecko;
		int j = -1;
		if (obecny->wartosc > 0) {
			j++;
			tab_wezlow[i][j] = obecny;
		}
		else {
			tab_pustych[puste_w_tablicy] = obecny;
			puste_w_tablicy++;
		}
		while (true) {
			if (obecny->dziecko != NULL) {
				obecny = obecny->dziecko;

			}
			else if (obecny->brat != NULL) {
				obecny = obecny->brat;

			}
			else if (obecny->rodzic != NULL) {
				while (obecny != roots[i] && obecny->rodzic->brat == NULL)
					obecny = obecny->rodzic;
				if (obecny != roots[i]) obecny = obecny->rodzic->brat;
			}
			if (obecny == roots[i]) break;
			if (obecny->wartosc > 0) {
				j++;
				tab_wezlow[i][j] = obecny;
			}
			else {
				tab_pustych[puste_w_tablicy] = obecny;
				puste_w_tablicy++;
			}
		}
		for (int x = 0; x <= ile_pustych[i]; x++) {
			tab_wezlow[i][j + x + 1] = tab_pustych[x];
		}
		delete[] tab_pustych;
	}
	for (int i = 0; i < ilosc_drzew; i++) {
		for (int j = i + 1; j < ilosc_drzew; j++) {
			cout << ile_lisci[i] - stworz_tablice(tab_wezlow[i], tab_wezlow[j], ile_pustych[i], ile_pustych[j], ile_wezlow[i], ile_wezlow[j], ile_lisci[i], ile_lisci[j]) << endl;
		}
		delete[] roots[i];
	}

	delete[] roots;
	delete[] tab_wezlow;
	delete[]ile_lisci;
	delete[]ile_pustych;
	delete[]ile_wezlow;
}
