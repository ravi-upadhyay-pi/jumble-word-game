#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstring>
#include <time.h>
#include <stdlib.h>
#include <vector>

using namespace std;

/**************************************************************
          dictionary.txt for dictionary 
          common_words.txt is source for words to be jumbled
          
          files must match to this [a-z\n] regex
	  
**************************************************************/

string dictionary = "dictionary", jumblesource = "common_words";

//////////////////////////////TRIE////////////////////////////
//check = 1 if a word ends here
typedef struct trie_node
{
	int check;
	int prefix_count;
	trie_node *child[26];
}trie_node;

trie_node *newNode(void)
{
	trie_node *t = new trie_node;
	for(int i = 0; i < 26; i++)
		t->child[i] = NULL;
	t->prefix_count = 0;
	t->check = 0;
}


//t must be allocated before call
void insert(trie_node *t, const char *str)
{
	int ch = str[0] - 'a';
	t->prefix_count++;
	if(str[0] == '\0') 
	{
		t->check = 1;
		return;
	}
	else 
	{
		if(t->child[ch] == NULL)
			t->child[ch] = newNode();
		insert(t->child[ch], str + 1);
	}
}

//returns NULL if str is not prfix of any word in trie
//else returns the trie node where str[0] found
trie_node* search(trie_node *t, const char *str)
{
	int ch = str[0] - 'a';
	if(str[0] == 0)
		return t;
	else if(t->child[ch] == NULL)
		return NULL;
	else 
		return search(t->child[ch], str + 1);
}
///////////////////////Trie Ends//////////////////////////////////

//puts shuffled s2 in s1
void shuffle(string& s1, const string& s2)
{
	s1 = s2;
	int i, len = s1.length();
	for(i = len; i >= 1; i--)
		swap(s1[i-1], s1[rand()%i]);
}

trie_node* create_dictionary()
{
	string line;
	ifstream dict(dictionary.c_str(), ios::in);
	trie_node *t = newNode();
	if(dict.is_open())
	{
		while( getline(dict, line) )
			insert(t, line.c_str());
		dict.close();
	}
	else
	{
		cout << dictionary << " not found\n";
	}
	return t;
}

void create_word_table(vector<string>& table)
{
	string line;
	ifstream dict(jumblesource.c_str(), ios::in);
	table.clear();
	if(dict.is_open())
	{
		while(getline(dict, line))
			table.push_back(line.c_str());
	}
	else
	{
		cout << jumblesource << " not found\n";
	}
	dict.close();
}

//returns a random string from table and removes it
//by replacing it with last string in table and 
//popping the the last element from table
void get_random_word(vector<string>& table, string& s)
{
	int random = rand()%table.size();
	s = table[random];
	table[random] = table[table.size()-1];
	table.pop_back();
}

//checks if s1, and s2 are anagram or not
//assumes that s1 and s2 are lower case string
bool anagram_check(const string& s1, const string& s2)
{
	int i, freq[26];
	for(i = 0; i < 26; i++) 
		freq[i] = 0;
	for(i = 0; i < s1.length(); i++)
		freq[s1[i]-'a']++;
	for(i = 0; i < s2.length(); i++)
		freq[s2[i]-'a']--;
	for(i = 0; i < 26; i++)
		if(freq[i] != 0)
			return false;
	return true;
}

void dump_stdin_line()
{
	while(getchar()!='\n');
	return;
}


/*/////////////////tests trie///////////////////
int main()
{
	trie_node *t = newNode();
	string s1;
	int in, out;
	cin >> in >> out;
	while(in--){
		cin >> s1;
		insert(t, s1.c_str());
	}
	while(out--){
		cin >> s1;
		if(search(t, s1.c_str())){
			cout << "Word Present" << endl;
		}
	}
	return 0;
}
////////////////end test/////////////////*/

int main()
{
	vector<string> word_list;
	string word, e_word, j_word;
	trie_node *t, *search_result;
	char *input = new char[100];
	int low, high;

	srand(time(NULL));
	t = create_dictionary();
	create_word_table(word_list);
	
	cin >> low >> high;
	dump_stdin_line();

	while(word_list.size())
	{
		get_random_word(word_list, e_word);
		if(e_word.length() < low || e_word.length() > high)
			continue;
		shuffle(j_word, e_word);
		cout << "\n---------------\n";
		cout << j_word << endl;
		cout << "---------------\n";
		while(1)
		{
			//read string
			int i = 0;
			input[0] = 0;
			while( (input[i++] = getchar()) != '\n' );
			input[i-1] = 0;
			//end read string
			
			if(input[0] == 0)
			{
				cout << "Answer:" << e_word;
				break;
			}
			
			else if(anagram_check(string(input), j_word) 
				&& (search_result = search(t, input)) != NULL 
				&& search_result->check == true)
			{
				cout << "Correct!!";
				break;
			}
			else cout << "Wrong!\n\n";
		}	       
		if(getchar() !='\n')
			break;
	}
	cout << "Word List is empty now!\n";
	return 0;
}
//*/	
