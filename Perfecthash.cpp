// Idealhash.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
using namespace std;
const int p = 101;
const int af = rand() % p;
const int bf = rand() % p;
const int m = 10;
struct slvlhtable
{
	int a;
	int b;
	int n;
	vector <int> table;
};
void Nulling(slvlhtable& cur)//making all values on table=0
{
	for (unsigned int i = 0; i < cur.table.size(); i++)
	{
		cur.table[i] = 0;
	}
}
void adding_value(int new_val, slvlhtable& cur)
{
	if (cur.table.empty())
	{
		cur.table.push_back(new_val);
		cur.a = 0;
		cur.b = 0;
		cur.n = 1;
	}
	else
	{
		int place = ((cur.a*new_val + cur.b) % p) % cur.table.size();                          //checking that we havent  already
		if (new_val != cur.table[place])                                                       //such value in the table
		{
			cur.n++;
			int *temptable = new int[cur.n];
			int counter = 0;
			for (unsigned int i = 0; i < cur.table.size(); i++)// saving values from hash table 
			{
				if (cur.table[i] != 0)
				{
					temptable[counter] = cur.table[i];
					counter++;
				}
				if (counter == cur.n - 1) break;
			}
			temptable[counter] = new_val;
			counter++;
			cur.table.resize(cur.n*cur.n);
			bool collision = true;
			while (collision)
			{
				Nulling(cur);
				cur.a = rand() % p;
				cur.b = rand() % p;
				for (int i = 0; i < cur.n; i++)
				{
					int place = ((cur.a*temptable[i] + cur.b) % p) % cur.table.size();
					if (cur.table[place] == 0) cur.table[place] = temptable[i];
					else break;
					if (i == cur.n - 1) collision = false;
				}
			}
			delete(temptable);
		}
	}
	
}
void search(int val, slvlhtable * flvlhtable)
{
	int fplace = (((af*val + bf) % p) % m);
	slvlhtable cur = flvlhtable[fplace];
	int secplace;
	if (cur.table.size()) secplace = ((cur.a*val + cur.b) % p) % cur.table.size();
	if (cur.table.size() && val == cur.table[secplace]) cout << "Position:[" << fplace << "][" << secplace << "]" << endl;
	else cout << "Nothing to show,try again"<<endl;

}
void menu(slvlhtable * maintable)
{
	int temp;
	cout << "What do you want to do?\n1.Search\n2.End" << endl;
	cin >> temp;
	temp--;
	if (!temp)
	{
		cout << "What are you looking for ?\n";
		cin >> temp;
		search(temp, maintable);
		system("pause");
		system("cls");
		menu(maintable);
	}
}
int main()
{
	slvlhtable  flvlhtable[m];	
	int new_value;
	cout << "Values:";
	for (int i = 0; i < m; i++)
	{
		new_value = rand() % 100 + 1;
		int place = (((af*new_value + bf) % p) % m);
		cout << new_value << " ";
		adding_value(new_value, flvlhtable[place]);
	}
	cout << endl;
	menu(flvlhtable);

	system("pause");
	return 0;
}

