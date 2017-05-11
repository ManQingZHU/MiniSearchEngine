#include "InvFileInd.h"
#include "Stemming.h"

unordered_map<string, Document> InvFileInd; // the inverted file index
vector <string> Filename; // the vector of File name
vector<string> wordlist; // the vector of words, for convenience of oder
vector<string> stopwordlist; // the vector of stopwords
unordered_map<string, int> stopwordmap;// the map of stopwords , <word,frequency>

//to Convert the capital char into lower case
string Case_convert(string word){
   string::iterator ptr;
   for(ptr = word.begin(); ptr != word.end(); ptr++){
      if( * ptr >= 'A' && * ptr <= 'Z') * (ptr) = * (ptr) - 'A' + 'a';
   }
   return word;
}
//to delete the punctuation
string Punc_deletion(string word){
  
   if(ispunct( * (word.end() - 1)))
      word.erase(word.end() - 1);
      
   return word;
}

//DEBUG************************//
int count_file_num = 0;  
//DEBUG************************//
void Readin_stopword(string dirname){
   ifstream filename(dirname + "stop_word_list.txt");
   string word;
   while(filename >> word){//read in the file
       stopwordlist.push_back(word);
   }
}

void Readin_filename(string dirname){
   ifstream filename(dirname + "index.txt");
   string name;
   while(filename >> name){
   	  name.erase(name.size() - 4, 4);//delete the .txt in the filename
	  //cout << name << endl; 
      Filename.push_back(name);   
   }
}


void Build_invFileInd(string dirname){
   long int count_file = 0;
   vector<string>::iterator it_filename;
   for(it_filename = Filename.begin(); it_filename != Filename.end(); it_filename ++){//read in the file
      
      string title = *(it_filename);
      ifstream file(dirname + title + ".txt");
      string word;
      //long int count_word = 0;
      int count_row = 1;
      int count_col = 1;
      while(file >> word){
      	 char word_begin = *(word.begin());
      	 if(word_begin >= '0' && word_begin <= '9')
      	    continue;
         char word_end = * (word.end() - 1);//record to judge the change of row or col 
         int flag = 0;// to judge the way  to resh the row and col in the end of the loop 
      
         if( word_end == '.' || word_end == '!'){
         //input '\n' if there is . or ! as the end of setence
            flag = 1;
         }
         else{
            flag = 0;
         }
         //find the word in the stop wordlist
         vector<string>::iterator it_stop_word;
         for(it_stop_word = stopwordlist.begin(); it_stop_word != stopwordlist.end(); it_stop_word++){
         	if(*(it_stop_word) == word)
         	  break;
		 }
         if(!(it_stop_word == stopwordlist.end()))//the word is a stopword
          continue;
         word = Case_convert(word);//change all the words into lower case
         word = Punc_deletion(word);//delete the punctuation in the end word, like "hello!" -> "hello"
         //to stemming the word
		 struct stemmer * z = create_stemmer();
         char * s = (char *) malloc(INC + 1);
         word = stemfile(z,s,word);
         unordered_map<string,Document>::iterator it = InvFileInd.find(word);
		 if(it == InvFileInd.end()){//the word not found in the Inverted file index
            //initialize the Document *
			Document doc;
            doc.frequency = 1;
            //initialize the Position *
            pos ptopos;
            ptopos = (pos)malloc(sizeof(struct Position));
            ptopos->row = count_row;
            ptopos->col = count_col;
            //Insert the position and frequency information to Document *
            //doc.information[count_file] = ptopos;
            vector<pos> vector_pos;
            vector_pos.push_back(ptopos);
            doc.information.insert(map<long int,vector<pos> >::value_type(count_file,vector_pos));
            //Insert the word and Document * into the inverted file index
		 	InvFileInd.insert(unordered_map<string,Document>::value_type(word,doc));
		 }
         else{//the word is in the Inverted file index
            //refresh the frequency
         	it->second.frequency ++;
         	//insert the new postiton
         	//initialize the ptr tot pos
         	pos ptopos;
         	ptopos = (pos)malloc(sizeof(struct Position));
         	ptopos->col = count_col;
         	ptopos->row = count_row;
         	//insert the file number and position information
         	map<long int,vector<pos> >::iterator it_inf = it->second.information.find(count_file);
			if(it_inf == it->second.information.end()){
				vector<pos> vector_pos;
				vector_pos.push_back(ptopos);
				it->second.information.insert(map<long int,vector<pos> >::value_type(count_file,vector_pos));
			}
			else{
				it_inf->second.push_back(ptopos);
			}
		 }
      if(flag == 1){
        count_row ++;
        count_col = 1;
      }   
      else{
        count_col ++;
      }
      
      }
      count_file ++;
   }
 
   //test ocde
   //to see wether the inverted_file_index contain the right information
   /*****************************************/
   /*ofstream result_test("result_test.txt");
   unordered_map<string, Document>::iterator itdoc_test;
   for(itdoc_test = InvFileInd.begin(); itdoc_test != InvFileInd.end(); itdoc_test++){
   	  result_test << itdoc_test->first << " ";
   	  result_test << "frequency:" << itdoc_test->second.frequency << " ";
   	  map<long int, vector<pos> >::iterator itinf_test;
   	  for(itinf_test = itdoc_test->second.information.begin(); itinf_test != itdoc_test->second.information.end(); itinf_test++){
   	  	    result_test << "file:" << itinf_test->first << " ";
   	  	    vector<pos>::iterator it_pos;
   	  	    for(it_pos = itinf_test->second.begin(); it_pos != itinf_test->second.end(); it_pos++)
   	  	    result_test << "positon " << "row:" << (*it_pos)->row << " " << "col:" << (*it_pos)->col;
		 }
	  result_test << "\n";
   } */
   /****************************************/
   
}

