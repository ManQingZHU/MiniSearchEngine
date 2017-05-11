#include "InvFileInd.h"
#include "RoughSearch.h"
#include "PreciseSearch.h"
using namespace std;

#define MAXSEARCHWORD 20

// the function is to output a pattern as a prompt
void Prompt()
{
    string welc = "   Welcome, what can I do for you?   ";
    string upper( welc.size()-2, '-');
    string prom1 = "(0)    EXIT";
    string prom2 = "(1)  SEARCH";
    string blank(welc.size(), ' ' );
    string blank1((welc.size() - prom1.size() ) / 2, ' ');        // to make sure the text will output
    string blank2((welc.size() - prom2.size() ) / 2, ' ');        // in the middle of the line

    cout << "+ " + upper + " +" << endl;
    cout << '|' + blank + '|' << endl;
    cout << '|' + blank + '|' << endl;
    cout << '|' + welc + '|' << endl;
    cout << '|' + blank + '|' << endl;
    cout << '|' + blank + '|' << endl;
    cout << '|' << blank1 << prom1 << blank1 << '|' << endl;
    cout << '|' + blank + '|' << endl;
    cout << '|' << blank2 << prom2 << blank2 << '|' << endl;
    cout << '|' + blank + '|' << endl;
    cout << '|' + blank + '|' << endl;
    cout << "+ " + upper + " +" << endl;

}

void UserInterface(string dirname)
{
    string term;
    vector<string> Query;
    int MaxResultNum;
    double Threshold;
    while(true)
    {
        Query.clear();
        Prompt();
        cin >> term;
        if(term == "0") {                    // the iteration stops when the user input a '0'
            cout << "bye" <<endl;
            break;
        }

        // get a query from user
        cout << endl;
        cout << "Enter the Query with an individual // as its ending:" << endl;
        while(cin >> term)
        {
            if(term == "//")    // the query is expected to be followed with an "//"
                break;          // as the mark of ending
            Query.push_back(term);
        }

        //Check the length of query, if it's too long, Only the first certain words will be accepted.
        if(Query.size() > MAXSEARCHWORD)     // And a message will be sent to the user.
        {
            cout << "The query is too long. The search engine will only handle with the first "<< MAXSEARCHWORD <<" words." <<endl;
            Query.erase(Query.begin()+MAXSEARCHWORD, Query.end());
        }

        // get  MaxResultNum, if the number of result is bigger than MaxResultNum,
        cout << endl;          // only the first MaxResultNum records will be output.
        cout << "How many records at most would you like to see?" << endl;
        cin >> MaxResultNum;

        // choose different search mode.
        if(Query.size() > 3)
        {                        // If the query contains more than three words,
            cout << endl;        // use Rough Search Mode
            cout << "Enter the Threshold (0 ~ 1):" <<endl;
            cin >> Threshold;     // user can input threshold to get different precision
            RoughSearch(dirname, Query,MaxResultNum, Threshold);
        }
        else{   // If the query contains no more than three words,
            PreciseSearch(dirname, Query, MaxResultNum);  // use Rough Search Mode
        }
    }

}

int main(int argc, const char **argv){
    /*use for iox*/
    /*string dirname;
    const char *p = strrchr(argv[0], '/');    // this part is to get the current absolute path
    if (p != 0)                               //  so that the program can carry out file operation correctly
    {                                        // all the text files should be put in a "documents" file folder
        dirname = string(argv[0], p - argv[0] + 1);
        dirname += ".documents/";
    }*/
    /*use for windows*/
    string dirname = ".\\documents\\";

    // preprocess part
    Readin_filename(dirname);      // read in filename list 
    Readin_stopword(dirname);      // read in stopword list
    Build_invFileInd(dirname);     // build Inverted file index
    UserInterface(dirname);        // handle with query
    return 0;
}
