#include "pch.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <vector>
#include <algorithm>


using namespace std;

class Task {
public:
	int time; //ile trwa zadanie
	int ex; // ktore to zadanie
	Task() {
		time = 0;
		ex = 0;
	}
	Task(int t, int e) {
		time = t;
		ex = e;
	}
};
void delete_task(vector<vector<Task>> &macierz,int nr) {
	int size = macierz.size() - 1; //3
		macierz[nr].clear();
		for (int i = nr; i < size; i++)
			macierz[i].swap(macierz[i + 1]);
		macierz.resize(size);

	cout << "usunieto zad nr: " << nr + 1 << endl;
}
void print_matrix_time(vector<vector<Task>> macierz) {
	for (int j = 0; j < macierz.size(); ++j) {
		for (int i = 0; i < macierz[j].size(); ++i)
			cout << macierz[j][i].time << " ";
		cout << "\n";
	}
}
void size_of_matrix(vector<vector<Task>> macierz) {
	cout << "wymiary macierzy: [" << macierz.size() << "][" << macierz[0].size() << "]" << endl;
}

vector<int> orderr(vector<vector<Task>> &macierz, int min_ex, int min_m, int min, vector<int> &order) {
	while (macierz.size() >= 1) {
		min_ex = 0;
		min_m = 0;
		min = macierz[0][0].time;
		for (int j = 0; j < macierz.size(); j++) {
			for (int i = 0; i < macierz[j].size(); i++) {
				if (min > macierz[j][i].time) {
					min = macierz[j][i].time;
					min_ex = j; //nr zadania ktory trzeba bedzie usunac i dac do odpowiedniej kolejnosci, od 0
					min_m = i; //tego elementu uzyje, zeby sprawdzic czy jest to pierwsza maszyna (i=0), czy ostatnia(i=n_m-1)
				}
			}
		}
		size_of_matrix(macierz);
		cout << "Najmniejszą wczytaną liczbą jest " << min << endl;
		cout << "maszyna: " << min_m + 1 << endl;
		cout << "zadanie: " << macierz[min_ex][0].ex << endl;
		if (min_m == 0) {  //jesli pierwsza maszyna to na poczatek
			for (int i = 0; i < order.size(); i++) {
				if (order[i] == 0) {
					order[i] = macierz[min_ex][0].ex;
					break;
				}
			}

		}
		else {  //jesli druga maszyna to na koniec
			for (int i = order.size() - 1; i >= 0; i--) {
				if (order[i] == 0) {
					order[i] = macierz[min_ex][0].ex;
					break;
				}
			}
		}
		delete_task(macierz, min_ex);
		if (macierz.size() != 0) {
			size_of_matrix(macierz);
			print_matrix_time(macierz);
		}		
	}
	return order;
}
vector<vector<Task>> read_data(int &number_of_ex, int &n_m) {
	ifstream data("ta000txt.txt");
	data >> number_of_ex;
	data >> n_m;
	vector<vector<Task>> macierz(number_of_ex, vector<Task>(n_m));
	for (int j = 0; j < number_of_ex; j++) {
		for (int i = 0; i < n_m; i++) {
			data >> macierz[j][i].time;
			macierz[j][i].ex = j + 1;
		}
	}
	data.close();
	return macierz;
}
int cmax(vector<int> order, vector<vector<Task>> macierz, vector<vector<int>> Cm, int number_of_ex, int n_m) {
	int n_ex = number_of_ex + 1;
	for (int i = 0; i <= n_m; i++) Cm[0][i] = 0;
	for (int i = 0; i < n_ex; i++) Cm[i][0] = 0;
	for (int i = 1; i <= n_m; i++) {
		for (int j = 1; j <= number_of_ex; j++) {
			Cm[j][i] = max(Cm[j][i - 1], Cm[j - 1][i]) + macierz[order[j - 1] - 1][i - 1].time;
		}

	}
	return Cm[n_ex - 1][n_m];

}
vector<vector<Task>> convert_to_2machines(vector<vector<Task>> &macierz, int &number_of_ex, int n_m) {
	vector<vector<Task>> machines2(number_of_ex, vector<Task>(n_m-1));
	for (int i = 0; i < machines2.size(); i++) {
		machines2[i][0].time = macierz[i][0].time + macierz[i][1].time;
		machines2[i][0].ex = macierz[i][0].ex;
		machines2[i][1].time = macierz[i][1].time + macierz[i][2].time;
		machines2[i][1].ex = macierz[i][0].ex;
	}
	return machines2;
}
int main()
{
	vector<vector<Task>> machines2;
	int number_of_ex, n_m,min, min_ex,min_m;
	vector<vector<Task>> macierz2 = read_data(number_of_ex, n_m);
	vector<vector<Task>> macierz = read_data(number_of_ex, n_m);
	if (n_m == 3) {
		for (int i = 0; i < number_of_ex; i++) macierz[i].resize(n_m - 1);
		macierz = convert_to_2machines(macierz2, number_of_ex, n_m);
	}
	vector<vector<int>> Cm(number_of_ex+1, vector<int>(n_m+1));
	vector<int> order(number_of_ex);
	for (int i = 0; i < number_of_ex; i++) order[i] = 0;
	min_ex = 0;
	min_m = 0;
	min = macierz[0][0].time;
	print_matrix_time(macierz);
	orderr(macierz, min_ex, min_m, min, order);

	for (int i = 0; i < number_of_ex; i++) cout << "kolejnosc: " << order[i] << " " << endl;
	int Cmax = cmax(order, macierz2, Cm, number_of_ex, n_m);
	cout << "Cmax = " << Cmax << endl;

	return 0;
}