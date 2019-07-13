// Hangman.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
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


Account* accounts = NULL;
Account* current = NULL;
WordList* head = NULL;
WordList* tail = NULL;

Account* GetNewNode(string username, string password) {
	Account* newNode = new Account;
	newNode->username = username;
	newNode->password = password;
	newNode->score = 0;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
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
		head = newNode;
		tail = newNode;
	}
	else {
		newNode->next = NULL;
		newNode->prev = tail;
		tail = newNode;
	}
}

void GetWords(string type) {
	ifstream wordsList("words.txt");
	string word;
	string types;
	while (wordsList >> word >> types) {
		if (type == types) {
			InsertWords(word);
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



int main()
{
	string username;
	string password;
	char choice;
	int score;
	int option;
	bool flag = true;
	GetAccountList();
	
	cout<<R"(
   _   _    _       _____                __  __      _       _____  U _____ u      _   _      _      _   _     ____    __  __      _      _   _     
U |"|u| |  |"|     |_ " _|     ___     U|' \/ '|uU  /"\  u  |_ " _| \| ___"|/     |'| |'| U  /"\  u | \ |"| U /"___|uU|' \/ '|uU  /"\  u | \ |"|    
 \| |\| |U | | u     | |      |_"_|    \| |\/| |/ \/ _ \/     | |    |  _|"      /| |_| |\ \/ _ \/ <|  \| |>\| |  _ /\| |\/| |/ \/ _ \/ <|  \| |>   
  | |_| | \| |/__   /| |\      | |      | |  | |  / ___ \    /| |\   | |___      U|  _  |u / ___ \ U| |\  |u | |_| |  | |  | |  / ___ \ U| |\  |u   
 <<\___/   |_____| u |_|U    U/| |\u    |_|  |_| /_/   \_\  u |_|U   |_____|      |_| |_| /_/   \_\ |_| \_|   \____|  |_|  |_| /_/   \_\ |_| \_|    
(__) )(    //  \\  _// \\_.-,_|___|_,-.<<,-,,-.   \\    >>  _// \\_  <<   >>      //   \\  \\    >> ||   \\,-._)(|_  <<,-,,-.   \\    >> ||   \\,-. 
    (__)  (_")("_)(__) (__)\_)-' '-(_/  (./  \.) (__)  (__)(__) (__)(__) (__)    (_") ("_)(__)  (__)(_")  (_/(__)__)  (./  \.) (__)  (__)(_")  (_/  
		 
		)"<<'\n';

	
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
	

	
}




