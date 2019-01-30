#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

struct Node 
{
	char* m_data;
	Node* m_left;
	Node* m_right;
	Node* m_parent;

	Node(char* data, Node* left = NULL, Node* right = NULL, Node* parent = NULL);
	~Node();
	void NewBranch(char* name, char* trait);
};

struct Tree
{
	Node* m_root;
	
	Tree();
	Node* Guess();
	void CheckAndAdd(Node* cur);
	//void Write(char* filename);
	//void Read(char* filename);
};

struct Game
{
	Tree m_tree;
	
	void Choice();
	void Play();
};

Node::Node(char* data, Node* left, Node* right, Node* parent):
	m_left(left),
	m_right(right),
	m_parent(parent)
	{
		m_data = new char[strlen(data) + 1];
		strcpy(m_data, data);
	}
	
Node::~Node()
{
	delete[] m_data;
}
	
void Node::NewBranch(char* name, char* trait)
{
	m_right = new Node(name);
	this->m_right->m_parent = this;
	m_left = new Node(m_data);
	this->m_left->m_parent = this;
	char question[269] = "Ваш персонаж ";
	strcat(question, trait);
	delete[] m_data;
	m_data = new char[strlen(question) + 1];
	strcpy(m_data, question);
}

Tree::Tree()
{
	char question[] = "Ваш персонаж реален";
	char no[] = "Человек-паук";
	char yes[] = "Андрей Егоров";
	m_root = new Node(question, new Node(no), new Node(yes));
	m_root->m_left->m_parent = m_root;
	m_root->m_right->m_parent = m_root;
}

Node* Tree::Guess()
{
	Node* cur = m_root;
	char yes[] = "Да"; 
	char no[] = "Нет";
	char answer[1000] = "";
	for (;cur->m_right != NULL;)
	{
		printf("%s?[Да/Нет]\n", cur->m_data);
		gets(answer);
		if (strcmp(yes, answer) == 0) cur = cur->m_right;
		else if (strcmp(no, answer) == 0) cur = cur->m_left;
		else printf("Ответ должен быть дан одним словом: Да или Нет.\n");
	}
	printf("Это %s.\n", cur->m_data);
	return cur;
}

void Tree::CheckAndAdd(Node* cur)
{
	printf("Верно?[Да/Нет]\n");
	char yes[] = "Да"; 
	char no[] = "Нет";
	char answer[1000] = "";
	gets(answer);
	while (strcmp(no, answer) != 0 && strcmp(yes, answer) != 0) {
		printf("Ответ должен быть дан одним словом: Да или Нет.\n");
		gets(answer);
	}
	if (strcmp(no, answer) == 0) {
		printf("Какого персонажа вы загадали?\n");
		char name[256] = "";
		gets(name);
		printf("Напишите признак, который отличает вашего персонажа от угаданного.\n");
		char trait[256] = "";
		gets(trait);
		cur->NewBranch(name, trait);
	}
}

char GetDigit_1to5()
{
	char c = getchar();
	int trigger = 0;
	while (getchar() != '\n')
		trigger = 1;
	if (((c - '0') > 5) || ((c - '0') < 1) || (trigger == 1)) {
		printf("Введите число от 1 до 5: ");
		c = GetDigit_1to5();
	} 
	else return c;
}

void Game::Choice()
{
	printf("\nИграть.[1]\nСохранить.[2]\nЗагрузить.[3]\nОпределение.[4]\nВыйти.[5]\nДля выбора введите число от 1 до 5: ");
	char ch = GetDigit_1to5();
	switch (ch) {
		case '1': 
		Play();
		break;
		case '2': 
		case '3':
		case '4':
		printf("\nДанная опция находится в разработке. Извините за предоставленные неудобства.\n");
		Choice();
		break;
		case '5':
		break;
	}
}

void Game::Play()
{
	Node* character = m_tree.Guess();
	m_tree.CheckAndAdd(character);
	Choice();
}

int main()
{
	Game game;
	game.Choice();
}
	