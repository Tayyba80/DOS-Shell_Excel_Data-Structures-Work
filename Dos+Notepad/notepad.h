#pragma once
#include<iostream>
#include<windows.h>
#include<list>
#include<stack>
#include<deque>
#include<conio.h>
#include<exception>
#include<fstream>
#include<iterator>
#include <algorithm>
using namespace std;

struct state
{
	int currRow;
	int currCol;
	list<list<char>> text;
	list<list<char>>::iterator rowIter;
	list<char>::iterator colIter;
};
void gotoxy(int row, int col)
{
	COORD coordinates;
	coordinates.X = col;
	coordinates.Y = row;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}
list<string> allFiles;

class notepad
{
	int currRow;
	int currCol;
	list<list<char>> text;
	list<list<char>>::iterator rowIter;
	list<char>::iterator colIter;
	stack<state> redo;
	deque<state>undo;
public:
	notepad()
	{
		text.push_back(list<char>());
		currRow = 0;
		currCol = 0;
		rowIter = text.begin();
		rowIter->push_back(' ');
		colIter = rowIter->begin();
	}
	void creatFile()
	{
		system("cls");
		string name;
		cout << "Enter File Name" << endl;
		cin >> name;
		if (find(allFiles.begin(), allFiles.end(), name) != allFiles.end())
		{
			cout << "File already Exist";
			return;
		}
		allFiles.push_back(name);
		ofstream wrt(name.c_str());
		exit();
		system("cls");

		editFile(wrt);
		system("cls");
		wrt.close();

	}
	void creatFileForDos(string name)
	{
		if (find(allFiles.begin(), allFiles.end(), name) != allFiles.end())
		{
			cout << "File already Exist" << endl;
			return;
		}
		allFiles.push_back(name);
		string folderName = name.substr(0, name.find("-"));
		name = name.substr(folderName.length() + 1, name.length());
		ofstream wrt(name.c_str());
		exit();
		wrt.close();
	}
	
	void editFile(ofstream& wrt)
	{
		system("color F0");
		char letter;
		while (true)
		{
			gotoxy(currRow, currCol);
			letter = _getch();
			if (letter == -32)
			{
				letter = _getch();
				handleSpecialKey(letter);
			}
			else if (letter == 27)//esc
			{
				break;
			}
			else
			{
				handleEditingKey(letter, wrt);
			}

			system("cls");
			print();

		}

	}
	void handleSpecialKey(char letter)
	{
		switch (letter) {
		case 72:
			moveUp();
			break;
		case 80:
			moveDown();
			break;
		case 75:
			moveLeft();
			break;
		case 77:
			moveRight();
			break;
		case 83:
			deleteChar();
			saveUndo();
			break;
		}
	}
	void moveUp()
	{
		if (rowIter != text.begin())
		{
			rowIter--;
			updateColIter();
			currRow--;
		}
	}
	void moveDown()
	{
		if (rowIter != prev(text.end()))
		{
			rowIter++;
			updateColIter();
			currRow++;
		}
	}
	/*void moveLeft()
	{
		if (currCol > 0)
		{
			currCol--;
			colIter--;
		}
		else if (rowIter != text.begin())
		{
			rowIter--;
			updateColIter();
		}
	}*/
	void moveLeft()
	{
		if (currCol > 0)
		{
			currCol--;
			if (colIter != rowIter->begin())
			{
				colIter--;
			}
			else
			{
				if (rowIter != text.begin())
				{
					rowIter--;
					colIter = prev(rowIter->end());
				}
			}
		}
	}
	/*void moveRight()
	{
		if (currCol != 50 && colIter != rowIter->end())
		{
			currCol++;
			colIter++;
		}
		else if (rowIter != text.end())
		{
			rowIter++;
			colIter = rowIter->begin();
			currRow++;
			currCol = 0;
		}

	}*/
	void moveRight()
	{
		if (rowIter != text.end() && colIter != rowIter->end())
		{
			if (currCol < 200)
			{
				colIter++;
				currCol++;
				if (colIter == rowIter->end())
				{
					if (next(rowIter) != text.end())
					{
						rowIter++;
						colIter = rowIter->begin();
						currCol = 0;
						currRow++;
					}
				}
			}
		}
	}
	void deleteChar()
	{
		if (colIter != rowIter->end())
		{
			colIter = rowIter->erase(colIter);
		}
	}
	void handleEditingKey(char letter, ofstream& wrt)
	{
		switch (letter) {
		case 8:
			backspace();
			saveUndo();
			break;
		case 13:
			enter();
			saveUndo();
			break;
		case 19: //save
			save(wrt);
			break;
		case 25:
			DoRedo();
			break;
		case 26:
			DoUndo();
			break;
		default:
			insertCharacter(letter);
			saveUndo();
			break;
		}
	}
	void backspace()
	{
		if (colIter != rowIter->begin())
		{
			colIter = rowIter->erase(--colIter);
			currCol--;
		}
		else if (rowIter != text.begin())
		{
			auto tempRowIter = rowIter;
			tempRowIter--;
			colIter = tempRowIter->end();
			rowIter = text.erase(rowIter);
			rowIter = tempRowIter;
			currRow--;
			updateColIter();
		}
	}
	void enter()
	{
		auto tempRowIter = rowIter;
		tempRowIter++;
		if (colIter != rowIter->end())
		{
			auto newIter = text.insert(tempRowIter, list<char>(colIter, rowIter->end()));
			rowIter->erase(colIter, rowIter->end());
			rowIter = newIter;
		}
		else
		{
			auto newIter = text.insert(tempRowIter, list<char>());
			rowIter = newIter;
		}
		currRow++;
		currCol = 0;
		colIter = rowIter->begin();
	}
	void insertCharacter(char letter)
	{
		if (colIter == rowIter->end())
		{
			rowIter->push_back(letter);
			colIter = prev(rowIter->end());
		}
		else
		{
			colIter = rowIter->insert(colIter, letter);
			colIter++;
		}
		currCol++;
		updateColIter();
	}
	void DoRedo()  //redo
	{
		if (!redo.empty())
		{
			undo.push_back(redo.top());
			state s = redo.top();
			redo.pop();
			loadState(s);
		}
	}
	void DoUndo()  //Undo
	{
		if (!undo.empty())
		{
			state s = undo.back();
			loadState(s);
			redo.push(undo.back());
			undo.pop_back();
		}
	}
	void updateColIter()
	{
		colIter = rowIter->begin();
		int i = 0;
		while (i < currCol && colIter != rowIter->end())
		{
			colIter++;
			i++;
		}
	}
	void print()
	{
		for (auto i = text.begin(); i != text.end(); i++)
		{
			for (auto j = (*i).begin(); j != (*i).end(); j++)
			{
				cout << *j;
			}
			cout << endl;
		}
	}
	void saveUndo()
	{
		if (undo.size() > 5)
			undo.erase(undo.begin());

		state letters;
		letters = saveState();
		undo.push_back(letters);
	}
	state saveState()
	{
		state* st = new state;
		st->text.push_back(list<char>());
		auto rIter = st->text.begin();
		for (auto i = text.begin(); i != text.end(); i++, rIter++)
		{
			st->text.push_back(list<char>());
			for (auto col = (*i).begin(); col != (*i).end(); col++)
			{
				rIter->push_back(*col);
			}
		}
		st->rowIter = st->text.begin();                            
		st->colIter = st->rowIter->begin();
		st->currRow = currRow;
		st->currCol = currCol;
		return *st;
	}
	void loadState(state st)
	{
		text = st.text;
		rowIter = text.begin();
		currCol = st.currCol;
		currRow = st.currRow;
		for (int r = 0; r < st.currRow; r++)
		{
			rowIter++;
		}
		colIter = rowIter->begin();
		for (int c = 0; c < st.currCol; c++)
		{
			colIter++;
		}

	}
	void save(ofstream& wrt)
	{
		for (auto i = text.begin(); i != text.end(); i++)
		{
			for (auto j = (*i).begin(); j != (*i).end(); j++)
			{
				wrt << *j;
			}
			wrt << endl;
		}
	}
	void openFile()
	{
		system("cls");
		string name;
		cout << "Enter file name" << endl;
		cin >> name;

		if (find(allFiles.begin(), allFiles.end(), name) == allFiles.end())
		{
			cout << "File does not exist";
			return;
		}
		ifstream reader(name.c_str());
		openSavedFile(reader);
		reader.close();

		ofstream writer(name.c_str());
		system("cls");
		print();
		editFile(writer);
		writer.close();
	}
	void openFileForDos(string name) 
	{
		bool flag = false;
		for (auto i : allFiles)
		{
			if (i == name)
			{
				flag = true;
			}
		}
		if (!flag)
		{
			cout << "File does not exist" << endl;
			return ;
		}
		string folderName = name.substr(0, name.find("-"));
		name = name.substr(folderName.length() + 1, name.length());
		ifstream reader(name.c_str());
		openSavedFile(reader);
		reader.close();

		ofstream writer(name.c_str());
		system("cls");
		print();
		editFile(writer);
		system("cls");
		system("color 07");
		writer.close();
	}
	void openSavedFile(ifstream& reader)
	{
		char letter;
		while (!reader.eof())
		{
			reader.get(letter);
			if (letter != '\n')
			{
				rowIter->push_back(letter);
			}
			else
			{
				text.push_back(list<char>());
				rowIter++;
			}
			rowIter = text.begin();
			colIter = rowIter->begin();
			currCol = 0;
			currRow = 0;
			gotoxy(currRow, currCol);
		}

	}
	string findString(string name, string word)
	{
		bool flag = false;
		for (auto i : allFiles)
		{
			if (i == name)
			{
				flag = true;
			}
		}
		if (!flag)
		{
			cout << "File does not exist" << endl;
			return " ";
		}
		string folderName = name.substr(0, name.find("-"));
		name = name.substr(folderName.length() + 1, name.length());
		ifstream reader(name.c_str());
		openSavedFile(reader);
		list<char> wordList(word.begin(), word.end());
		for (const auto& line : text)
		{
			auto it = search(line.begin(), line.end(), wordList.begin(), wordList.end());
			if (it != line.end()) {
				cout << endl;
				cout << "       Word found in file: " << name << " " << word << endl;
				return " ";
			}
		}
		cout << "Word not found in file: " << name << endl;
	}

	void input()
	{
		int choice;
		cout << "New File" << endl;
		cout << "open File" << endl;
		cout << "Exit" << endl;
		cout << "Enter your choice" << endl;
		cin >> choice;

		if (choice == 1)
		{
			creatFile();
		}
		else if (choice == 2)
		{
			openFile();
		}
		else if (choice == 3)
		{
			exit();
		}
	}
	void exit()
	{
		ofstream writer;
		writer.open("all_files.txt");
		for (auto i = allFiles.begin(); i != allFiles.end(); i++)
		{
			writer << *i << endl;
		}
		writer.close();
	}
	void loadAllFiles()
	{
		ifstream reader("all_files.txt");
		if (!reader.is_open())
		{
			{
				cout << "Error opening file" << endl;
				return;
			}
		}
		string input;
		reader >> input;

		while (!reader.eof())
		{
			allFiles.push_back(input);
			reader >> input;
		}
	}
};


