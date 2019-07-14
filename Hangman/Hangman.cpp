// Hangman.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <conio.h>
#include <iomanip>
#include <Windows.h>
using namespace std;



struct Account {
	string username;
	string password;
	int score;
	Account* left;
	Account* right;
};

struct WordList {
	WordList* next;
	WordList* prev;
	string word;
};

struct WordMap {
	WordMap* next;
	char character;
};
WordMap* arr[20];

struct CharStack {
	CharStack* next;
	CharStack* prev;
	char character;
};

CharStack* user = NULL;
CharStack* cp = NULL;

Account* accounts = NULL;
Account* current = NULL;
WordList* head = NULL;
WordList* tail = NULL;
WordList* currentWord = NULL;

Account* GetNewNode(string username, string password) {
	Account* newNode = new Account;
	newNode->username = username;
	newNode->password = password;
	newNode->score = 0;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}


void Push(char c, CharStack* node) {
	CharStack* newNode = new CharStack;
	newNode->character = c;
	newNode->next = NULL;
	if (node == NULL) {
		newNode->prev = NULL;
		node = newNode;
	}
	else {
		newNode->prev = node;
		node->next = newNode;
		node = newNode;
	}

	user = node;
}

char Pop(CharStack* node) {
	char n = node->character;
	node = node->prev;
	user = node;
	return n;
}

bool empty(CharStack* node) {
	if (user == NULL) {
		return false;
	}
	else {
		return true;
	}
}

Account* Insert(Account* root, string username, string password) {
	if (root == NULL)
		root = GetNewNode(username, password);
	else if (username < root->username)
		root->left = Insert(root->left, username, password);
	else {
		root->right = Insert(root->left, username, password);
	}

	return root;
}

void InsertWords(string word) {
	WordList* newNode = new WordList;
	newNode->word = word;
	if (head == NULL) {
		newNode->next = NULL;
		newNode->prev = NULL;
		tail = newNode;
		head = newNode;
	}
	else {
		newNode->next = NULL;
		newNode->prev = tail;
		tail->next = newNode;
		tail = newNode;
	}
}

void GetWords(string type) {
	ifstream wordsList("word.txt");
	string word;
	string types;
	while (wordsList >> word >> types) {
		if (type == types) {
			InsertWords(word);
			//cout << word << endl;
		}
	}
}

void GetAccountList() {

	ifstream accountsText("accounts.txt");
	string password;
	string username;
	int score;
	while (accountsText >> username >> password>> score) {
		 accounts = Insert(accounts, username, password);
	}
}


bool Search(Account* root, string username, string password) {
	if (root == NULL) {
		return false;
	}
	else if (root->username == username && root->password == password) {
		current = root;
		return true;
	}
	else if (root->username <= username) {
		return(Search(root->left, username, password));
	}
	else {
		return(Search(root->right, username, password));
	}
}


bool CheckAccount(string username, string password) {
	if (Search(accounts, username, password)) {
		return true;
	}
	else {
		return false;
	}
	
	
}

void TitleScreen() {
	cout << R"(
   _   _    _       _____                __  __      _       _____  U _____ u      _   _      _      _   _     ____    __  __      _      _   _     
U |"|u| |  |"|     |_ " _|     ___     U|' \/ '|uU  /"\  u  |_ " _| \| ___"|/     |'| |'| U  /"\  u | \ |"| U /"___|uU|' \/ '|uU  /"\  u | \ |"|    
 \| |\| |U | | u     | |      |_"_|    \| |\/| |/ \/ _ \/     | |    |  _|"      /| |_| |\ \/ _ \/ <|  \| |>\| |  _ /\| |\/| |/ \/ _ \/ <|  \| |>   
  | |_| | \| |/__   /| |\      | |      | |  | |  / ___ \    /| |\   | |___      U|  _  |u / ___ \ U| |\  |u | |_| |  | |  | |  / ___ \ U| |\  |u   
 <<\___/   |_____| u |_|U    U/| |\u    |_|  |_| /_/   \_\  u |_|U   |_____|      |_| |_| /_/   \_\ |_| \_|   \____|  |_|  |_| /_/   \_\ |_| \_|    
(__) )(    //  \\  _// \\_.-,_|___|_,-.<<,-,,-.   \\    >>  _// \\_  <<   >>      //   \\  \\    >> ||   \\,-._)(|_  <<,-,,-.   \\    >> ||   \\,-. 
    (__)  (_")("_)(__) (__)\_)-' '-(_/  (./  \.) (__)  (__)(__) (__)(__) (__)    (_") ("_)(__)  (__)(_")  (_/(__)__)  (./  \.) (__)  (__)(_")  (_/  
		 
		)" << '\n';
}

bool RegisterUser() {
	string username;
	string password;
	ofstream accountText("accounts.txt");
	cout << "Enter Username: ";
	getline(cin, username);
	cin.ignore();
	cout << "Enter Password: ";
	getline(cin, password);
	if (CheckAccount(username, password)) {
		cout << "Account already exists!" << endl;
	}
	else {
		accountText << username << " " << password << " " << "0" << endl;
		Insert(accounts, username, password);
		cout << "Account Successfully registered!" << endl;
		return false;
	}
}

bool Login() {
	string username;
	string password;
	char option;
	cout << "Enter Username: ";
	getline(cin, username);
	cout << "Enter Password: ";
	getline(cin, password);
	if (CheckAccount(username, password)) {
		return false;
	}
	else {
		cout << "Would you like to register? (Y/N): ";
		cin >> option;
		if (option == 'y')
			RegisterUser();
		else {
			return false;
		}
	}
}

int Hash(char c) {
	return int(c) % 15;
}

void HashMap(int key, char val) {
	WordMap* holder = arr[key];
	WordMap* newNode = new WordMap;
	newNode->character = val;
	newNode->next = NULL;
	if (arr[key] == NULL) {
		arr[key] = newNode;
	}
	else if (arr[key]->character != val) {
		while (holder->next != NULL) {
			holder = holder->next;
		}
		holder->next = newNode;
	}
}


void MapWords(string word) {
	for (int i = 0; i < word.length(); i++) {
		HashMap(Hash(word[i]), word[i]);
	}
}

bool search(char c) {
	WordMap* list = new WordMap;
	for (int i = 0; i < 20; i++) {
		list = arr[i];
		while (list) {
			if (list->character == c)
				//pick is correct
				return true;
			else
				list = list->next;
		}
	}
	//pick is wrong
	return false;
}

char PickCharacter() {
	char c;
	cout << "Type a letter: ";
	cin >> c;
	return c;
}

bool WordDisplay(char c, char arr[], int &ctr) {
	string word = currentWord->word;
	cout << "Current Word: ";
	for (int i = 0; i < word.length(); i++){
		if (word[i] == c && arr[i] != c) {
			arr[i] = c;
			ctr--;
		}
		cout << arr[i];
	}
	cout << endl;

	if (ctr == 0)
		return false;
	else
		return true;
	

}

void display() {
	WordMap* list = new WordMap;
	for (int i = 0; i < 20; i++) {
		list = arr[i];
		while (list) {
			cout << list->character << endl;
			list = list->next;
		}
	}
}

void FlushMap(char chara[]) {
	for (int i = 0; i < 20; i++) {
		arr[i] = NULL;
		chara[i] = ' ';
	}
}

void GenRand(int sarr[]) {
	
	int i, j, temp;
	srand(time(NULL));
	for (i = 0; i < 26; i++)
		sarr[i] = i + 1;

	for (i = 0; i < 26; i++) {
		j = (rand() % 25) + 1;

		temp = sarr[i];
		sarr[i] = sarr[j];
		sarr[j] = temp;
	}

	
}

char ComputerPick(int picker, int sarr[]) {
	char alphabet[26] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g',
						'h', 'i', 'j', 'k', 'l', 'm', 'n',
						'o', 'p', 'q', 'r', 's', 't', 'u',
						'v', 'w', 'x', 'y', 'z' };
	
	return alphabet[sarr[picker]];

}

void DamageDisplay(int hp) {
	if (hp == 100) {
		cout << R"(
	_______
	|   
	|  
	|  
      __|__
		)" << '\n';
	}
	else if (hp >= 80 && hp <= 99) {
		cout << R"(
	_______
	|   O
	|  
	|  
      __|__
		)" << '\n';
	}
	else if (hp >= 60 && hp <= 79) {
		cout << R"(
	_______
	|   O
	|   |
	|  
      __|__
		)" << '\n';
	}
	else if (hp >= 40 && hp <= 59) {
		cout << R"(
	_______
	|   O
	|  /|
	|  
      __|__
		)" << '\n';
	}
	else if (hp >= 20 && hp <= 39) {
		cout << R"(
	_______
	|   O
	|  /|\
	|  
      __|__
		)" << '\n';
	}
	else if (hp >= 0 && hp <= 19) {
		cout << R"(
	_______
	|   O
	|  /|\
	|  / 
      __|__
		)" << '\n';
	}
	else {
		cout << R"(
	_______
	|   O
	|  /|\
	|  / \
      __|__
		)" << '\n';
	}
}

void GamePlay() {
	system("CLS");
	int cpPicker = 0;
	int ctr = 0;
	int currPlayer = 1;
	int CPHP = 100;
	int UserHP = 100;
	int sarr[26];
	int dmg = 20;
	char c = ' ';
	char arr[20]= " ";
	currentWord = head;
	while (currentWord) {
		system("CLS");
		GenRand(sarr);
		cpPicker = 0;
		MapWords(currentWord->word);
		ctr = currentWord->word.length();
		display();
		cout << currentWord->word << endl;
		
		//start of real gameplay
		do {
			system("CLS");
			cout << "User HP: " << UserHP << endl;
			DamageDisplay(UserHP);
			cout << endl << "Computer HP: " << CPHP << endl;
			DamageDisplay(CPHP);

			cout << "\n\n-------------------------------------------------\n\n";
			if (WordDisplay(c, arr, ctr)) {

				if (currPlayer) {
					c = PickCharacter();
					if (!search(c)) {
						Push(c, user);
						c = ' ';
						currPlayer = 0;
					}
				}
				else {
					cout << "Computer is picking" << endl;
					Sleep(3000);
					c = ComputerPick(cpPicker, sarr);
					cout << "Computer Picked: " << c << endl;
					if (!search(c)) {
						Push(c, cp);
						c = ' ';
						currPlayer = 1;
					}
					cpPicker++;
				}

			}
			else {
				cout << "finish na" << endl;
				_getch();
			}

		} while (ctr != 0);
		
		currentWord = currentWord->next;
		c = ' ';
		FlushMap(arr);
		if (currPlayer) {
			while (empty(user)) {
				if (Pop(user)) {
					dmg--;
				}
			}
			CPHP = CPHP - dmg;
		}
		else {
			while (empty(cp)) {
				if (Pop(cp)) {
					dmg--;
				}
			}
			UserHP = UserHP - dmg;
		}
		
	}
}



int MainMenu() {
	system("CLS");
	TitleScreen();
	int choice = 0;
		cout << "[1] Play" << endl;
		cout << "[2] View Scores" << endl;
		cout << "[3] How to Play" << endl;
		cout << "[4] Log out" << endl;
		cout<<"Enter Choice: ";
		cin >> choice;

		return choice;
}

bool LoginMenu() {
	system("CLS");
	TitleScreen();
	int option;
	bool flag = true;
	GetAccountList();
	do {
		cout << "[1] Login" << endl;
		cout << "[2] Register Account" << endl;
		cout << "Enter Option: ";
		cin >> option;
		cin.ignore();
		if (option == 1) {
			do {
				flag = Login();
			} while (flag);

		}
		else {
			RegisterUser();
		}
	} while (flag);

	return flag;
}

string TopicSelect() {
	int choice = 0;
	string topic;
	cout << "Enter Topic" << endl;
	cout << "[1] Job" << endl;
	cout << "[2] Money" << endl;
	cout << "[3] Exit" << endl;
	cout << "Pick a choice: ";
	cin >> choice;
	if (choice == 1) {
		topic = "job";
	}
	else if (choice == 2) {
		topic = "money";
	}else{
		topic = "NONE";
	}
	

	return topic;
}

int main()
{
	int choice = 0;
	string topic;
	bool flag = true;
	WordList* newNode = new WordList;
	do {
		if (!LoginMenu()) {
			do {
				choice = MainMenu();
				if (choice == 1) {
					topic = TopicSelect();
					if (topic != "NONE") {
						GetWords(topic);
						GamePlay();
						
					}
				}
				else if (choice == 2) {

				}
				else if (choice == 3) {

				}
				else if (choice == 4) {
					current = NULL;
					flag = true;
				}
			} while (choice != 4);
		}
	} while (flag);
}