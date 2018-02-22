// Fibheap.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
using namespace std;
struct Node 
{
	Node * p;
	Node * child;
	Node * right;
	Node * left;
	int degree;
	bool mark;
	int key;
	Node()
	{
		p = NULL;
		child = NULL;
		right = NULL;
		left = NULL;
		degree = 0;
		mark = false;
		key = 0;
	}
	Node(int k)
	{
		p = NULL;
		child = NULL;
		right = this;
		left = this;
		degree = 0;
		mark = false;
		key = k;
	}
};
struct Fib_heap
{
	Node * min;
	int n;
	Fib_heap()
	{
		n = 0;
		min = NULL;
	}
};
Fib_heap *Fib_heap_union(Fib_heap * H1, Fib_heap * H2)
{
	Fib_heap * H = new Fib_heap;
	H->min = H1->min;
	Node*x = H2->min;
	Node*l = x->left;
	if ( H2->min != NULL)
	{
		if (H1->min == NULL) H->min = H2->min;
		else 
		{
			x->right = H->min->right;
			H->min->right->left = x;
			H->min->right = l;
			l->left = H->min;
		}
	}
	if (H1->min == NULL || (H2->min != NULL && H2->min->key < H1->min->key))
	{
		H->min = H2->min;
	}
	H->n = H1->n + H2->n;
	delete H1;
	delete H2;
	return H;
}
void Insert(Fib_heap *H,Node * x)
{	
	x->p = NULL;
	x->mark = false;
	if (H->min == NULL)
	{
		x->right = x;
		x->left = x;
		H->min = x;
		H->n = 1;
	}
	else
	{
		x->right = H->min->right;                      //insert the element 
		(H->min->right)->left = x;                     //to the right
		x->left = H->min;                              // of the 
		H->min->right = x;                             //min
		if (H->min->key > x->key)
		{
			H->min = x;
		}
		(H->n)++;
	}
}
void Cut(Fib_heap * H, Node * x, Node * cur_p)
{
	cur_p->child = x->right;
	(x->right)->left = x->left;
	(x->left)->right = x->right;
		if (cur_p->degree==1)
	{
		cur_p->child = NULL;
	}
	cur_p->degree--;
	Insert(H, x);
	x->mark = false;
	x->p = NULL;    
}
void Cuscading_cut(Fib_heap * H, Node * prev_p)
{
	Node *gp = prev_p->p;
	if (gp != NULL)
		if (prev_p->mark == false)
		{
			prev_p->mark = true;
		}
		else {
			    Cut(H, prev_p, gp);
				Cuscading_cut(H, gp);
		      }
}
void Decrease_Key(Fib_heap * H, Node *x, int new_k)
{
	if (new_k > x->key)
	{
		cout << "Error,the new value is greater than the previous.";
		return;
	}
	x->key = new_k;
	Node *cur_p = x->p;
	if (cur_p != NULL && x->key < cur_p->key)
	{
		Cut(H, x, cur_p);
		Cuscading_cut(H, cur_p);
	}
}
void Heap_link(Fib_heap * H, Node * y, Node * x)
{
	if (y == H->min) H->min = y->left;
	y->left->right = y->right;
	y->right->left = y->left;
	
	y->p = x;
	
	if (x->child != NULL)
	{
		y->right = x->child->right;
		y->right->left = y;
		x->child->right = y;
		y->left = x->child;
	}
	else
	{
		x->child = y;
		y->right = y;
		y->left = y;
	}
	x->degree++;
	y->mark = false;
}
void Consolidate(Fib_heap * H)
{
	Node ** A = new Node*[H->n];
	for (int i = 0; i < H->n ; i++)
	{
		A[i] = NULL;
	}
	Node * temp = H->min;
	for (int i=0;i<H->n;i++)
	{
		Node * x = temp;
		temp = temp->right;
		int d = x->degree;
		while (A[d]!= NULL)
		{
			Node * y = A[d];
			if (x->key > y->key)
			{
				int l=x->key;
				x->key = y->key;
				y->key = l;
			}
			Heap_link(H, y, x);
			A[d]=NULL;
			d = d + 1;
		}
		A[d] = x;
	}
	H->min = NULL;
	int new_n=0;
	for (int i = 0; i < H->n; i++)
	{
		if (A[i] != NULL)
		{
			new_n++;
			Insert(H, A[i]);
			if (A[i]->key < H->min->key)
				H->min = A[i];
		}
	}
	H->n = new_n;	
}
Node * Extract_min(Fib_heap *H)
{
	Node * cur = H->min;
	if (cur != NULL)
	{
		Node * temp=cur->child;
		if (temp != NULL)
		{
			for (int i = 0; i < cur->degree; i++)
			{
				Node*x = temp;
				temp = temp->right;
				Insert(H,x);
			}
		}
		cur->right->left = cur->left;
		cur->left->right = cur->right;
		cur->child = NULL;
		cur->degree = 0;
		H->n--;
		if (cur == cur->right)
		{
			H->min = NULL;
		}
		else
		{
			H->min = cur->right;
			Consolidate(H);
		}
		
		cout <<"Deleted value"<< cur->key;
	}
	else cout << "Nothing to delete\n";
	return cur;
}

void Fib_heap_delete(Fib_heap * H, Node * x)
{
	Decrease_Key(H, x, 0);
	Extract_min(H);
}
int Fib_heap_min(Fib_heap * H)
{
	return H->min->key;
}
void out(Fib_heap * H)
{
	Node *x = H->min;
	if (x != NULL)
	{
		for (int i = 0; i < H->n; i++)
		{
			cout << x->key << " "<<x->degree<<"*";
			x = x->right;
		}
	}
	else cout << "Empty";
	cout << endl;
}
void menu(Fib_heap *H )
{
	int temp;
	out(H);
	cout << "What do you want to do?\n1.Add Node\n2.Find minimal element\n3.Delete minimal element\nEnd" << endl;
	cin >> temp;
	switch (temp)
	{
	case(1):
	{
		cout << "Key:";
		int k;
		cin >> k;
		Node *new_n = new Node(k);
		Insert(H, new_n);
		system("pause");
		system("cls");
		menu(H);
		break;
	}
	case(2):
		cout << "Minimal:" << H->min->key;
		system("pause");
		system("cls");
		menu(H);
		break;
	case(3):
		Extract_min(H);
		system("pause");
		system("cls");
		menu(H);
		break;

	
	}
}
int main()
{
	Fib_heap *H = new Fib_heap;
	menu(H);
	system("pause");
    return 0;
}

