#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#define WSIZE 21
#define NSIZE 301

typedef struct slovo {
	char* s;
	int use;
}slovo;

typedef struct Node {
	slovo word;
	struct Node* left, * right;
} *PNode;

typedef struct Listn {
	PNode Tree;
	struct Listn* next;
}*List;

typedef struct Queue {
	List first, last;
}*Tqueue;


Tqueue newQueue();

int empty(Tqueue q);

Tqueue push(Tqueue q, PNode Tree);

PNode front(Tqueue q);

Tqueue pop(Tqueue q);

void PrintIt(PNode Tree) {
	printf("%s(%d) ", Tree->word.s, Tree->word.use);
}

void SetIt(PNode Tree, char *str, int x) {
	Tree->word.s = str;
	Tree->word.use = x;
	Tree->left = NULL;
	Tree->right = NULL;
}

PNode NewTree() {
	PNode Tree;
	Tree = NULL;
	return Tree;
}

PNode AddToTree(PNode Tree, char* str, int x) {
	if (!Tree) {
		Tree = (PNode)malloc(sizeof(struct Node));
		Tree->word.s = str;
		Tree->word.use = x;
		Tree->left = NULL;
		Tree->right = NULL;
		return Tree;
	}
	if (strcmp(str, Tree->word.s) < 0) {
		Tree->left = AddToTree(Tree->left, str, x);
	}	
	else {
		Tree->right = AddToTree(Tree->right, str, x);
	}
	return Tree;
}

PNode Search(PNode Tree, char* str) {
	if (!Tree)
		return NULL; 
	if (strcmp(str, Tree->word.s)==0) 
		return Tree;
	if (strcmp(str, Tree->word.s) < 0)
		return Search(Tree->left, str);
	if (strcmp(str, Tree->word.s) > 0)
		return Search(Tree->right, str);
}

PNode MakeIt(char *name, PNode Tree) {
	FILE* f;
	if ((f = fopen(name, "r")) == NULL) {
		printf("К большому сожалению, вылезла ошибка...");
	}
	else {
		char* str;
		while (!feof(f)) {
			str = (char*)malloc(WSIZE * sizeof(char));
			fscanf(f, "%s", str);
			if (!feof(f)) {
				PNode p;
				p = Search(Tree, str);
				if (p!=NULL) {
					p->word.use++;
				}
				else {
					Tree = AddToTree(Tree, str, 1);
				}
					
				
			}
		}
	}
	return Tree;
}

void KLP(PNode Tree) {
	if (!Tree) return;
	PrintIt(Tree);
	KLP(Tree->left);
	KLP(Tree->right);
}

void LKP(PNode Tree) {
	if (!Tree) return;
	LKP(Tree->left);
	PrintIt(Tree);
	LKP(Tree->right);
}

void LPK(PNode Tree) {
	if (!Tree) return;
	LPK(Tree->left);
	LPK(Tree->right);
	PrintIt(Tree);
}

void PKL(PNode Tree) {
	if (!Tree) return;
	PKL(Tree->right);
	PrintIt(Tree);
	PKL(Tree->left);
}

void KPL(PNode Tree) {
	if (!Tree) return;
	PrintIt(Tree);
	KPL(Tree->right);
	KPL(Tree->left);
}

void PLK(PNode Tree) {
	if (!Tree) return;
	KPL(Tree->right);
	KPL(Tree->left);
	PrintIt(Tree);
}

void BFS(PNode Tree) {
	if (!Tree) return;
	Tqueue queue;
	queue = newQueue();
	PNode p;
	queue = push(queue, Tree);
	while (!empty(queue)) {
		p = front(queue);
		PrintIt(p);
		queue = pop(queue);
		if (p->left != NULL)
			queue = push(queue, p->left);
		if (p->right != NULL)
			queue = push(queue, p->right);
	}
	printf("\n");
}

int Count(PNode Tree, int x, Tqueue q) {
	if (!Tree) return 0;
	int c = 0;
	if (x == Tree->word.use) {
		q = push(q, Tree);
		c++;
	}
	c += Count(Tree->left, x, q);
	c += Count(Tree->right, x, q);
	return c;
}

void DoIt(PNode Tree, int x) {
	Tqueue q;
	q = newQueue();
	int count = 0;
	count = Count(Tree, x, q);
	printf("Слов с заданной повторяемостью в тексте: %d\n", count);
	if (count) {
		printf("Вот они слева направо:\n");
		PNode p;
		while (!empty(q)) {
			p = front(q);
			PrintIt(p);
			q = pop(q);
		}
		printf("\n");
	}
}

int isLeave(PNode Tree) {
	if (Tree->left == NULL && Tree->right == NULL)
		return 1;
	else
		return 0;
}

PNode RemoveTree(PNode Tree, char *str) {
	if (!Tree) return Tree;
	if (strcmp(str, Tree->word.s) < 0) {
		Tree->left = RemoveTree(Tree->left, str);
		return Tree;
	}
	if (strcmp(str, Tree->word.s) > 0) {
		Tree->right = RemoveTree(Tree->right, str);
		return Tree;
	}
	if (!strcmp(str, Tree->word.s)) {
		if (isLeave(Tree)) {
			free(Tree);
			Tree = NULL;
			return Tree;
		}
		if (Tree->right == NULL) {
			PNode p = Tree->left;
			free(Tree);
			Tree = p;
			return Tree;
		}
		if (Tree->left == NULL) {
			PNode p = Tree->right;
			free(Tree);
			Tree = p;
			return Tree;
		}
		PNode p = Tree->left;
		while (p->right != NULL)
			p = p->right;
		Tree->word = p->word;
		Tree->left = RemoveTree(Tree->left, p->word.s);
		return Tree;
	}
}

void DeleteTree(PNode Tree) {
	char* str;
	while (!Tree) {
		str = Tree->word.s;
		Tree = RemoveTree(Tree, str);
	}
}

int main() {
	setlocale(LC_ALL, "rus");
	char name[] = "D:\\text13.txt";
	PNode Tree;
	Tree = NULL;
	Tree = MakeIt(name, Tree);
	
	printf("Дерево, прямой обход (KLP):\n");
	KLP(Tree);
	printf("\n");

	printf("Дерево, симметричный обход (LКP):\n");
	LKP(Tree);
	printf("\n");

	printf("Дерево, обратный обход (LPК):\n");
	LPK(Tree);
	printf("\n");

	printf("Дерево, вывод в ширину : \n");
	BFS(Tree);
	printf("\n");
	
	int x;
	printf("Введите число: ");
	scanf("%d", &x);
	DoIt(Tree, x);

	DeleteTree(Tree);

	return 0;

}

Tqueue newQueue() {
	Tqueue q;
	q = (Tqueue)malloc(sizeof(struct Queue));
	q->first = NULL;
	q->last = NULL;
}

int empty(Tqueue q) {
	if (q->first == NULL)
		return 1;
	return 0;
}

Tqueue push(Tqueue q, PNode Tree) {
	List l = (List)malloc(sizeof(struct Listn));
	l->next = NULL;
	l->Tree = Tree;
	if (empty(q)) {
		q->last = l;
		q->first = q->last;
		return q;
	}
	q->last->next = l;
	q->last = l;
	return q;
}

PNode front(Tqueue q) {
	return q->first->Tree;
}

Tqueue pop(Tqueue q) {
	if (empty(q))
		return q;
	if (q->first == q->last) {
		free(q->first);
		q->first = NULL;
		q->last = NULL;
		return q;
	}
	List l = q->first;
	q->first = q->first->next;
	free(l);
	return q;
}