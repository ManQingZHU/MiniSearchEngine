
#ifndef _INVERTED_FILE_INDEX_
#define _INVERTED_FILE_INDEX_


#include <iostream>
#include <unordered_map>
#include <string>
#include <map>
#include <fstream> 
#include <vector>
#include <algorithm>
#include <string.h>

#define STOP_WORD_FREQUENCY 1000 //the setting freqency of stop word
#define INC 50
using namespace std;

struct Position{ // the positon of term in the file
   long int row;
   long int col;
};

typedef struct Position *  pos;

struct Document{ 
   long int frequency; // the freqency of word in all the files
   map<long int,vector<pos> >information; // <the document number, position of term> 
};

extern struct stemmer * create_stemmer(void);

extern string stemfile(struct stemmer * z, char * s, string term);

extern unordered_map<string, Document> InvFileInd; // the inverted file index
extern vector <string> Filename; // the vector of File name
//extern vector<string> wordlist; // the vector of words, for convenience of oder
extern vector<string> stopwordlist; // the vector of stopwords
//extern unordered_map<string, int> stopwordmap;// the map of stopwords , <word,frequency>
string Case_convert(string word);
string Punc_deletion(string word);
void Readin_filename(string dirname);
void Readin_stopword(string dirname);
//void Create_stopword(string dirname);
void Build_invFileInd(string dirname);

#endif
