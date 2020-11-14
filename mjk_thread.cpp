#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <iostream>
#include <thread>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
using std::thread;
using namespace std;

void func1() {
	int count = 0;
	time_t a;
	char* b;
	for (int x = 1; x < 3000; x++) {
		a = time(&a);
		b = ctime(&a);
		cout << "" << b << endl;
		count+=1;
		Sleep(10);
	}
	cout << count << endl;
}

void func2() {
	int temp = 0;
	for (int x = 1; x < 10; x++) {
		cin >> temp;
		cout << "" << (temp + 1) << endl;
	}
}

int main() {
	thread t1(func1);
	thread t2(func2);

	t1.join();
	t2.join();
}