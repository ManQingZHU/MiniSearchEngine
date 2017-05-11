#include "PreciseSearch.h"
#include <set>
using namespace std;

//print out exact one line from the file
void OutPutLine(string dirname,int FileNum, int row){
    string name;
    string line;
    int count=1;
    name=Filename[FileNum];
    ifstream file(dirname + name + "_copy.txt");
    while(getline(file,line)){
        if(count == row){
            cout<<'\t'<<line<<endl;
            cout << endl;
            break;
        }
        count++;
    }
}


void PreciseSearch(string dirname, vector<string>& Query, int ResultNum){

    vector<string> UseWord;
    set<long int> FileNumSet1;  //file number set for each useful word
    set<long int> FileNumSet2;  // there are three sets because under
    set<long int> FileNumSet3;  // precise search, the length of query is no
    set<long int> intersect;    // larger than three.
    map<long int, vector<pos> >::iterator FileIter;
    string word;

    vector<string>::iterator iter = Query.begin();
    vector<string> tempstr;
    while(iter != Query.end())
    {
        word = *iter;
        word = Case_convert(word);  // transfer to lower case
        word = Punc_deletion(word); // delete the punctuation
        if(find(stopwordlist.begin(), stopwordlist.end(), word) == stopwordlist.end()) // delete stop words in the query
            tempstr.push_back(word);  // the rest words are stored in the temp vector temporarily
        iter = Query.erase(iter);
    }

    if(tempstr.empty())  // necessarily check in case that the query
    {                    // is totally made of stop words
        cout << "Your search words are too vague, please enter some interesting words :)" << endl;
        return;          // if that, a message will be sent to the user
    }                    // and the program will wait for the next query


    // stemming words
    struct stemmer* z = create_stemmer();
    char* s = (char *)malloc(i_max+1);
    iter = tempstr.begin();
    while(iter != tempstr.end())
    {
        word = *iter;
        word = stemfile(z,s, word);
        UseWord.push_back(word);
        iter = tempstr.erase(iter); // the word will be put into UseWord vector
    }

    int WordNum=0;
    //store the file of each word in FileNumSet
    for(iter=UseWord.begin();iter!=UseWord.end();iter++){
        word=*iter;
        for(FileIter=(InvFileInd[word]).information.begin();FileIter!=(InvFileInd[word]).information.end();FileIter++) {
                if (WordNum == 0)
                    FileNumSet1.insert(FileIter->first);  // store the first word
                else if (WordNum == 1)
                    FileNumSet2.insert(FileIter->first);  //store the second word
                else if (WordNum == 2)
                    FileNumSet3.insert(FileIter->first);  //store the third word
            }
        WordNum++;
        }

    int ResultCnt,flag;
    long int FileNum;
    long int row, col,row2,col2,row3,col3,prerow;
    Position Pos;
    vector<pos>::iterator PosIter,PosIter2,PosIter3;
    long int number;
    set<long int>::iterator SetIter;

    if(WordNum==1){     // if only one key word, simply output the line contains the word.
        prerow=-1;    // prerow is used to avoid output the same line more than once
        ResultCnt=0;
        for(FileIter=((InvFileInd[UseWord[0]]).information).begin();FileIter!=((InvFileInd[UseWord[0]]).information).end();FileIter++){
            FileNum=FileIter->first;
            cout<<"File Name:  < "+Filename[FileNum]+" >"<<endl;   // output the file name
            for(PosIter=((InvFileInd[UseWord[0]]).information)[FileNum].begin();PosIter!=((InvFileInd[UseWord[0]]).information)[FileNum].end()&&ResultCnt<ResultNum;PosIter++){
                row=(**PosIter).row;
                col=(**PosIter).col;
                if(row!=prerow){
                    OutPutLine(dirname, FileNum,row);        // output the line
                    ResultCnt++;
                }
                prerow=row;
            }
        }
    }
    else if(WordNum==2){
        ResultCnt=0;
        //store the intersection of two sets in set intersect
        set_intersection(FileNumSet1.begin(), FileNumSet1.end(), FileNumSet2.begin(), FileNumSet2.end(), inserter(intersect, intersect.begin()));
        for(SetIter=intersect.begin();SetIter!=intersect.end();SetIter++){   // read in a file
            number=*SetIter;
            flag=0;
            prerow=-1;
            for(PosIter=((InvFileInd[UseWord[0]].information)[number]).begin();PosIter!=((InvFileInd[UseWord[0]].information)[number]).end();PosIter++){
                row=(**PosIter).row;                // get position of the first word
                col=(**PosIter).col;
                for(PosIter2=((InvFileInd[UseWord[1]].information)[number]).begin();PosIter2!=((InvFileInd[UseWord[1]].information)[number]).end();PosIter2++){
                    row2=(**PosIter2).row;             // get position of the second word
                    col2=(**PosIter2).col;
                    if(row==row2&&col2-col==1){      // the line won't be output unless the two key word combine the phrase we want.
                        if(flag==0){
                            cout<<"File Name:  < "+Filename[FileNum]+" >"<<endl;    // output the file name
                            flag=1;
                        }
                        if(ResultCnt<ResultNum&&prerow!=row){
                            OutPutLine(dirname, number,row);     // output the line
                            prerow=row;
                            ResultCnt++;
                        }
                    }
                    else if(row2>row)
                        break;
                }
            }
        }
    }
    else if(WordNum==3){        // the operation is very similar to that of two key words.
        ResultCnt=0;
        set<long int> temp;   // do intersection for three sets
        set_intersection(FileNumSet1.begin(),FileNumSet1.end(),FileNumSet2.begin(),FileNumSet2.end(), inserter(temp,temp.end()));
        set_intersection(FileNumSet3.begin(),FileNumSet3.end(),temp.begin(),temp.end(), inserter(intersect,intersect.begin()));

        for(SetIter=intersect.begin();SetIter!=intersect.end();SetIter++){   // read in a file
            number=*SetIter;
            prerow=-1;
            flag=0;
            for(PosIter=((InvFileInd[UseWord[0]].information)[number]).begin();PosIter!=((InvFileInd[UseWord[0]].information)[number]).end();PosIter++){
                row=(**PosIter).row;       // get position of the first word
                col=(**PosIter).col;
                for(PosIter2=((InvFileInd[UseWord[1]].information)[number]).begin();PosIter2!=((InvFileInd[UseWord[1]].information)[number]).end();PosIter2++){
                    row2=(**PosIter2).row;           // get position of the second word
                    col2=(**PosIter2).col;
                    if(row==row2&&col2-col==1){         // continue only if the first two word is connected in the correct order
                        for(PosIter3=((InvFileInd[UseWord[2]].information)[number]).begin();PosIter3!=((InvFileInd[UseWord[2]].information)[number]).end();PosIter3++){
                            row3=(**PosIter3).row;           // get position of the third word
                            col3=(**PosIter3).col;
                            if(row3==row2&&col3-col2==1){      // the line won't be output unless the three key word combine the phrase we want.
                                if(flag==0){
                                    cout<<"File Name:  < "+Filename[FileNum]+" >"<<endl;   // output the file name
                                    flag=1;
                                }
                                if(ResultCnt<ResultNum&&prerow!=row){      // output the line
                                    OutPutLine(dirname, number,row);
                                    prerow=row;
                                    ResultCnt++;
                                }
                            }
                            else if(row3>row2)
                                break;
                        }
                    }
                    else if(row2>row)
                        break;
                }
            }
        }
    }
}

