#include "windows.h"
#include <iostream>
#include <map>
#include <String>
#include <ctime>

using namespace std;

map <char*, int> wordCounter(char* text, LARGE_INTEGER size) {
	map <char*, int> freq;
	int start = -1;
	int lenght = 0;
	char* word;

	for (int i = 0; i < size.QuadPart; i++) {
		if (isalpha(text[i])) {
			if (start == -1) {
				start = i;
			}
			lenght += 1;
		} else {
			if (lenght != 0) {
				word = (char*)malloc(lenght);

				for (int i = 0; i < lenght; i++) {
					word[i] = tolower(text[start + i]);
				}
				freq[word] += 1;

				lenght = 0;
				start = -1;
			}
		}
	}
	return freq;
}

map <string, int> standartWordCounter(char* text, LARGE_INTEGER size) {
	map <string, int> freq;
	string word = "";

	for (int i = 0; i < size.QuadPart; i++) {
		if (isalpha(text[i])) {
			word += tolower(text[i]);
		} else {
			freq[word] += 1;
			word = "";
		}
	}
	return freq;
}

int main()
{
	HANDLE File = CreateFile(L"book.txt", GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	LARGE_INTEGER Size;
	GetFileSizeEx(File, &Size);

	cout << "Size is " << Size.QuadPart << "\n";

	char* Text = new char[Size.QuadPart + 1];
	DWORD textSize;
	ReadFile(File, Text, Size.QuadPart, &textSize, NULL);
	CloseHandle(File);

	clock_t start = clock();
	map <char*, int> first = wordCounter(Text, Size);
	clock_t end = clock();
	cout << "Time of first method is " << ((end - start) * 1000) / CLOCKS_PER_SEC << " ms\n";

	start = clock();
	map <string, int> second = standartWordCounter(Text, Size);
	end = clock();
	cout << "Time of second method is " << ((end - start) * 1000) / CLOCKS_PER_SEC << " ms\n";

	map<string, int>::iterator it;
	for (it = second.begin(); it != second.end(); it++)
		cout << it->first << " : " << it->second << '\n';
}