#include <stdio.h> 
#include <string>
#include <stdlib.h>
#include <iostream>

const int ERROR = -2;

struct Node 
{
	std::string m_data;
	Node* m_left;
	Node* m_right;
	Node* m_parent;

	Node(std::string data, Node* left = NULL, Node* right = NULL, Node* parent = NULL);
	~Node();
	void NewBranch(std::string name, std::string trait);
};

struct Tree
{
	Node* m_root;
	
	Tree();
	Node* Guess();
	void CheckAndAdd(Node* cur);
	void Write(FILE* write_file);
	Node* Read(FILE* read_file);
};

struct Game
{
	Tree m_tree;
	
	void Choice();
	void Play();
	int Save(char* filename);
	int Load(char* filename);
};

Node::Node(std::string data, Node* left, Node* right, Node* parent):
	m_left(left),
	m_right(right),
	m_parent(parent)
	{
		m_data = data;
	}
	
Node::~Node()
{
	m_data = "This object has been destructed.";
}
	
void Node::NewBranch(std::string name, std::string trait)
{
	m_right = new Node(name);
	this->m_right->m_parent = this;
	m_left = new Node(m_data);
	this->m_left->m_parent = this;
	m_data = "Ваш персонаж ";
	m_data += trait;
}

Tree::Tree()
{
	std::string question = "Ваш персонаж реален";
	std::string no = "Человек-паук";
	std::string yes = "Овчинкин";
	m_root = new Node(question, new Node(no), new Node(yes));
	m_root->m_left->m_parent = m_root;
	m_root->m_right->m_parent = m_root;
}

Node* Tree::Guess()
{
	Node* cur = m_root;
	std::string yes = "Да"; 
	std::string no = "Нет";
	std::string answer;
	for (;cur->m_right != NULL;)
	{
		printf("%s?[Да/Нет]\n", cur->m_data.c_str());
		std::getline(std::cin, answer);
		if (answer == yes) cur = cur->m_right;
		else if (answer == no) cur = cur->m_left;
		else printf("Ответ должен быть дан одним словом: Да или Нет.\n");
	}
	printf("Это %s.\n", cur->m_data.c_str());
	return cur;
}

void Tree::CheckAndAdd(Node* cur)
{
	printf("Верно?[Да/Нет]\n");
	std::string yes = "Да"; 
	std::string no = "Нет";
	std::string answer;
	std::getline(std::cin, answer);
	while ((answer != no) && (answer != yes)) {
		printf("Ответ должен быть дан одним словом: Да или Нет.\n");
		std::getline(std::cin, answer);
	}
	if (answer == no) {
		printf("Какого персонажа вы загадали?\n");
		std::string name;
		std::getline(std::cin, name);
		printf("Напишите признак, который отличает вашего персонажа от угаданного.\n");
		std::string trait;
		std::getline(std::cin, trait);
		cur->NewBranch(name, trait);
	}
}

void Tree::Write(FILE* write_file)
{
	char c = '[';
	fwrite(&c, sizeof(char), 1, write_file);
	size_t str_size = m_root->m_data.size();
	fwrite(&str_size, sizeof(size_t), 1, write_file);
	fwrite(m_root->m_data.data(), sizeof(char), str_size, write_file);
	c = '_';
	char n = 'N';
	if (m_root->m_left != NULL) {
		fwrite(&c, sizeof(char), 1, write_file);
		m_root = m_root->m_left;
		this->Write(write_file);
		m_root = m_root->m_parent;
	}
	else fwrite(&n, sizeof(char), 1, write_file);
	if (m_root->m_right != NULL) {
		fwrite(&c, sizeof(char), 1, write_file);
		m_root = m_root->m_right;
		this->Write(write_file);
		m_root = m_root->m_parent;
	}
	else fwrite(&n, sizeof(char), 1, write_file);
	c = ']';
	fwrite(&c, sizeof(char), 1, write_file);
}

Node* Tree::Read(FILE* read_file)
{
	char c = 0;
	fread(&c, sizeof(char), 1, read_file);
	size_t str_size = 0;
	fread(&str_size, sizeof(size_t), 1, read_file);
	std::string str(str_size, '\0');
	fread(&str[0], sizeof(char), str_size, read_file);
	m_root = new Node(str);
	fread(&c, sizeof(char), 1, read_file);
	if (c == '_') {
		Node temp_node_l("Это временный узел");
		m_root->m_left = &temp_node_l;
		Node* temp_root = m_root;
		m_root = m_root->m_left;
		Node* new_left = this->Read(read_file);
		m_root = temp_root;
		m_root->m_left = new_left;
		m_root->m_left->m_parent = m_root;
	}
	fread(&c, sizeof(char), 1, read_file);
	if (c == '_') {
		Node temp_node_r("Это временный узел");
		m_root->m_right = &temp_node_r;
		m_root->m_right = &temp_node_r;
		Node* temp_root = m_root;
		m_root = m_root->m_right;
		Node* new_right = this->Read(read_file);
		m_root = temp_root;
		m_root->m_right = new_right;
		m_root->m_right->m_parent = m_root;
	}
	fread(&c, sizeof(char), 1, read_file);
	return m_root;
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
		Save("savefile.txt");
		printf("\nВаш прогресс был успешно созранён!\n");
		Choice();
		break;
		case '3':
		Load("savefile.txt");
		Choice();
		break;
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

int Game::Save(char* filename)
{
	FILE* write_file = fopen(filename, "wb");
	if (write_file == NULL) 
		return ERROR;
	m_tree.Write(write_file);
	fclose(write_file);
	return 0;
}

int Game::Load(char* filename)
{
	FILE* read_file = fopen(filename, "rb");
	if (read_file == NULL) {
		printf("Не открывается файл\n");
		return ERROR;
	}
	m_tree.Read(read_file);
	fclose(read_file);
	return 0;
}

int main()
{
	Game game;
	game.Choice();
}