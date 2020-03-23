//#include"Echo.h"
#include<mutex>
#include <iostream>
#include<vector>
#include<thread>
#include<string>
#include<Windows.h>
using namespace std;
mutex mtx;

class Echo {
private:
	int delay;
	string word;
	int number_of_repeats;
	static int max_repeats;
public:
	static int total_repeats;
	Echo(int delay, string word, int number_of_repeats) {
		this->delay = delay;
		this->word = word;
		this->number_of_repeats = number_of_repeats;
	}
	void Say() {

		for (int i = 0; i < this->number_of_repeats; i++) {
			if (total_repeats >= max_repeats) {
				break;
			}
			else {
				mtx.lock();
				total_repeats++;
				cout << Echo::get_repeats()<<"\t"<<this->word << endl;
				mtx.unlock();
				this_thread::sleep_for(chrono::milliseconds(delay));
			}
		}

	}
	static void set_max(int limit)
	{
		max_repeats = limit;
	}
	static int get_repeats() {
		return total_repeats;
	}

};
int Echo::total_repeats = 0;
int Echo::max_repeats = 0;

int main()
{
	setlocale(LC_ALL, "rus");
	vector<Echo*>echoes;
	string str = "thread ";
	cout << "Введите общее число сообщений: ";
	int max;
	cin >> max;
	Echo::set_max(max);
	cout << "Введите число тредов: ";
	int len;
	cin >> len;
	for (int i = 0; i < len; i++) {
		cout << "Введите число повторений " << i + 1 << " треда : ";
		int repeats;
		cin >> repeats;
		cout << "Введите интервал между сообщениями (в мс): ";
		int delay;
		cin >> delay;
		cout << "Введите сообщение для треда " << i + 1 << ": ";

		string st;
		getline(cin, st);
		string str;
		getline(cin, str);
		echoes.push_back(new Echo(delay, str, repeats));
	}	
	vector<thread>threads;
	for (int i = 0; i < echoes.size(); i++) {
		threads.push_back(thread(&Echo::Say, echoes[i]));
	}
	for (int i = 0; i < echoes.size(); i++) {
		if (threads[i].joinable()) {
			threads[i].join();
		}
	}
	cout << "Общее количество сообщений " << Echo::get_repeats() << endl;
}
