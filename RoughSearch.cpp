#include "RoughSearch.h"
#include <set>
using namespace std;

#define INTERVAL 4

// redefine a compare function for sort() function
bool cmp(const string& a, const string& b)                      // ascending order according to the frequency in
{                                                               // the Inverted file index
    return InvFileInd[a].frequency < InvFileInd[b].frequency ;
}

void RoughSearch(string dirname, vector<string>& Query, int MAXRESULTNUM, double THRESHOLD)
{
    string word;

    vector<string>::iterator v_str_iter = Query.begin();
    vector<string> temp;
    while(v_str_iter != Query.end())
    {
        word = *v_str_iter;
        word = Case_convert(word);   // transfer to lower case,
        word = Punc_deletion(word);   // delete the punctuation
        if(find(stopwordlist.begin(), stopwordlist.end(), word) == stopwordlist.end())  // delete stop words in the query
            temp.push_back(word);              // the rest words are stored in the temp vector temporarily
        v_str_iter = Query.erase(v_str_iter);
    }

    if(temp.empty())        // necessarily check in case that the query
    {                       // is totally made of stop words
        cout << "Your search words are too vague, please enter some interesting words :)" << endl;
        return;           // if that, a message will be sent to the user
    }                     // and the program will wait for the next query


    // stemming words
    struct stemmer* z = create_stemmer();
    char* s = (char *)malloc(i_max+1);
    v_str_iter = temp.begin();
    while(v_str_iter != temp.end())
    {
        word = *v_str_iter;
        word = stemfile(z,s, word);
        Query.push_back(word);
        v_str_iter = temp.erase(v_str_iter); // the word will be put back to Query vector
    }

    // sort the word according to frequency
    vector<string> SearchQuery(Query);
    sort(SearchQuery.begin(), SearchQuery.end(),cmp);  // SearchQuery stores the stemmed words after sorting
    // delete the key words out of threshold
    int key_num = (int)(Query.size() * THRESHOLD) < 1 ? 1 : (int)(Query.size() * THRESHOLD);    // make sure at least
    SearchQuery.erase(SearchQuery.begin()+key_num, SearchQuery.end());                          // one key word

    v_str_iter = Query.begin();                        // adjust the original Query, only retain key words
    while (v_str_iter != Query.end()) {
        if(find(SearchQuery.begin(), SearchQuery.end(), *v_str_iter) == SearchQuery.end())
            v_str_iter = Query.erase(v_str_iter);
        else v_str_iter++;
    }

    // find the file that contains all words
    v_str_iter = SearchQuery.begin();
    vector<set<long int> > file_set_vector;
    map<long int,vector<pos> >::const_iterator m_li_iter, m_li_iter_end;

    set<long int> file_set_result;
    set<long int> file_set;
    for(; v_str_iter != SearchQuery.end(); ++v_str_iter)
    {
        file_set.clear();
        m_li_iter = (InvFileInd[*v_str_iter].information).begin();
        m_li_iter_end = (InvFileInd[*v_str_iter].information).end();
        for(;m_li_iter != m_li_iter_end; ++m_li_iter)   // get the fileID of the file which contains the key word
            file_set.insert(m_li_iter->first);    // throw the fileID  into a set
        file_set_vector.push_back(file_set);       // push the set into vector

        // if there are 2 set in the vector, do intersection
        if(file_set_vector.size() == 2)
        {
            file_set_result.clear();
            set_intersection(file_set_vector[0].begin(), file_set_vector[0].end(), file_set_vector[1].begin(), file_set_vector[1].end(), inserter(file_set_result, file_set_result.begin()));

            file_set_vector.clear();
            file_set_vector.push_back(file_set_result);
        }
    }
    // now files that contain all the key words is in the file_set_result
    file_set_result = file_set_vector[0];
    // further selection for the file
    // Output_position storeed the filename and row which can be output
    map<long int, vector<long int> > Output_pos;


    long int target_row, target_col;
    long int row_in, col_in;
    vector<pos>::iterator v_pos_iter, v_pos_iter_end;
    vector<pos>::iterator v_pos_iter_in, v_pos_iter_end_in;
    bool match = true;

    set<long int>::const_iterator s_li_iter = file_set_result.begin();      // iterator for common file
    for(; s_li_iter != file_set_result.end(); ++s_li_iter)
    {
        v_str_iter = Query.begin();             //the first term
        v_pos_iter = InvFileInd[*v_str_iter].information[*s_li_iter].begin();  // iterator for position of first term in this file
        v_pos_iter_end = (InvFileInd[*v_str_iter].information)[*s_li_iter].end();
        for(;v_pos_iter != v_pos_iter_end; ++v_pos_iter)
        {
            v_str_iter = Query.begin();             //the first term
            target_row = (*v_pos_iter)->row;       // row of first term
            target_col = (*v_pos_iter)->col;       //col of first term

            for(++v_str_iter;v_str_iter != Query.end(); ++v_str_iter)    // change the term
            {
                match = false;
                v_pos_iter_in = (InvFileInd[*v_str_iter].information)[*s_li_iter].begin();   // iterator for position of other term in this file
                v_pos_iter_end_in = (InvFileInd[*v_str_iter].information)[*s_li_iter].end();

                for (;v_pos_iter_in != v_pos_iter_end_in; ++v_pos_iter_in)    // change positiion of this term
                {
                    row_in = (*v_pos_iter_in)->row;       // check whether row and col of other term can match
                    if(row_in < target_row)
                        continue;
                    if(row_in == target_row)
                    {
                        col_in = (*v_pos_iter_in)->col;
                        if(col_in < target_col)
                            continue;
                        if(col_in <= target_col + INTERVAL)
                        {
                            target_col = col_in;
                            match = true;
                            break;
                        }
                    }
                    if(row_in > target_row)
                        break;
                }

                if(!match)
                    break;
            }

            if(v_str_iter == Query.end() && match)
                Output_pos[*s_li_iter].push_back(target_row);  // record the file and the row
        }

        //duplication
        if(Output_pos.find(*s_li_iter) != Output_pos.end()) {
                    vector<long int> &row = Output_pos[*s_li_iter];
                    sort(row.begin(), row.end());
                    row.erase(unique(row.begin(), row.end()), row.end());
                }
    }

    // Output
    long int cnt_row = 0;
    int Output_num = 0;
    string title;
    string line;
    long int fileID;
    map<long int, vector<long int> >::const_iterator m_li_v_iter = Output_pos.begin(); // iterator for file and row
    vector<long int>::const_iterator v_li_iter, v_li_iter_end;  // iterator for row
    if(Output_pos.size() > 1)
        cout <<  Output_pos.size() << " files are found."<< endl;
    
    for(;m_li_v_iter != Output_pos.end(); ++m_li_v_iter)
    {
        fileID = m_li_v_iter->first;
        title = Filename[fileID];

        cout << "File Name:  < " << title << " >"<< endl;
        if(Output_num <= MAXRESULTNUM)                        // whether continue output row
        {
            ifstream fin(dirname + title + "_copy.txt");              // open the file

            cnt_row = 0;
            v_li_iter = Output_pos[fileID].begin();
            v_li_iter_end = Output_pos[fileID].end();
            for(;v_li_iter != v_li_iter_end && Output_num < MAXRESULTNUM; ++v_li_iter)
            {
                while (cnt_row != *v_li_iter)
                {
                    getline(fin, line);                    // output the line
                    cnt_row++;
                }
                cout<<'\t'<<line<<endl;
                Output_num++;
            }
            fin.close();
            cout << endl;
        }
    }

}
