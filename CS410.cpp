// basic file operations

#include "CS410.h"
#include <dirent.h>

// Driver function to sort the vector elements 
// by second element of pairs 
bool sortbysec(const pair<string,double> &a, 
              const pair<string,double> &b) 
{ 
    return (a.second > b.second); 
} 


/*
    Constructor
 */
CorpusClass::CorpusClass() {
    pdocarray = 0;
}

/*
    Destructor
 */
CorpusClass::~CorpusClass() {
}

void CorpusClass::write_topics()
{
    fstream newfileout;
    string outfile = output_path + "\\" + time_slice + ".csv";
    newfileout.open(outfile,ios::out); //open a file to perform write operation using file object
    if (newfileout.is_open()) //checking whether the file is open
    {   
        
        for(int topicindex = 0; topicindex < number_of_topics; topicindex++)
        {
            vector<pair<string,double>> topicout;
            for(int wordindex = 0; wordindex < VocublarySize; wordindex++)
            {
                topicout.push_back(make_pair(VocabularyList[wordindex],TopicWordProb[topicindex][wordindex]));
                //newfileout << VocabularyList[wordindex] << "," << TopicWordProb[topicindex][wordindex] << ",";
            }
            //newfileout << "\n";
            sort(topicout.begin(), topicout.end(), sortbysec); 
            for(int wordindex = 0; wordindex < VocublarySize; wordindex++)
            {
                newfileout << topicout[wordindex].first << "," << topicout[wordindex].second << ",";
            }
            newfileout << "\n";
         }

        newfileout.close(); //close the file object.
    }
}

/*
    Construct a list of unique words in the whole corpus. Put it in self.vocabulary
    for example: ["rain", "the", ...]
 */
void CorpusClass::build_vocabulary()
{
    string datadate = time_slice;
    string datadir = document_path;
    string myfile;


    struct dirent *entry = nullptr;
    DIR *dp = nullptr;

    DocumentNumber = 0;
    VocublarySize = 0;
    VocabularyMap.clear();
    DocumentWordMatrix.clear();
    VocabularyList.clear();
    TopicWordProb.clear();
    DocTopicProb.clear();
    delete_array(); // pdocarray
    
    //int count = 0;
    dp = opendir(datadir.c_str());
    if (dp != nullptr) 
    {
        while ((entry = readdir(dp)))
        {
            myfile = entry->d_name;
            size_t found = myfile.find(datadate); 
            if (found != string::npos) 
            {
                string filename = datadir + "//" + myfile;
                //cout << filename << "\n"; //print the data of the string
                fstream newfile;
                newfile.open(filename,ios::in); //open a file to perform read operation using file object
                if (newfile.is_open()) //checking whether the file is open
                {   
                    string tp;
                    while(getline(newfile, tp)) //read data from file object and put it into string.
                    {
                        istringstream iss(tp);
                        vector<string> results(istream_iterator<string>{iss}, istream_iterator<string>());      
                        for (vector<string>::iterator wordit = results.begin() ; wordit != results.end(); ++wordit)
                        {
                            //std::cout << ' ' << *it; 
                            // update vocabulary map
                            string newline = *wordit;
                            std::string::size_type pos = newline.find(',');
                            string newword = newline.substr(0, pos);
                            map <string, int>::iterator  mapit;
                            mapit = VocabularyMap.find(newword);
                            if (mapit != VocabularyMap.end())
                            {
                                mapit->second++;
                            }
                            else
                            {
                                VocabularyMap.insert(make_pair(newword, 1));
                                VocabularyList.push_back(newword);
                            } 
                        }
                    }

                    //newfile.clear();
                    //newfile.seekg(0, ios::beg);
                    newfile.close(); //close the file object.
                }   
            }                
        }
    }
    VocublarySize = VocabularyMap.size();
    cout << "Vocublary Size: " << VocublarySize << "\n";
    //cout << "Count: " << count << "\n";

    closedir(dp);
    dp = opendir(datadir.c_str());
    if (dp != nullptr) 
    {
        while ((entry = readdir(dp)))
        {
            myfile = entry->d_name;
            size_t found = myfile.find(datadate); 
            if (found != string::npos) 
            {
                string filename = datadir + "//" + myfile;
                fstream newfile;
                newfile.open(filename,ios::in); //open a file to perform read operation using file object
                if (newfile.is_open()) //checking whether the file is open
                {   
                    string tp;
                    
                    map <string, int> docwords;
                    while(getline(newfile, tp)) //read data from file object and put it into string.
                    {
                        //cout << tp << "\n"; //print the data of the string
                        //cout << "oy \n";
                        istringstream iss(tp);
                        vector<string> results(istream_iterator<string>{iss}, istream_iterator<string>());                         
                        map <string, int>::iterator  mapit;
                        for (vector<string>::iterator wordit = results.begin() ; wordit != results.end(); ++wordit)
                        {
                            string newline = *wordit;
                            std::string::size_type pos = newline.find(',');
                            string newword = newline.substr(0, pos);
                            mapit = docwords.find(newword);
                            if (mapit != docwords.end())
                            {
                                mapit->second++;
                                //cout << "OY " << mapit->second << "\n";
                            }
                            else
                            {
                                docwords.insert(make_pair(newword, 1));
                            }
                        }
                     }

                    //cout << "Words in this doc: " << docwords.size() << "\n";
                    //DocumentWords.push_back(docwords);
                    vector<int> wordcount;
                    // for each word
                    for (vector <string>::iterator wordit = VocabularyList.begin() ; wordit != VocabularyList.end(); ++wordit)
                    {
                        map <string, int>::iterator  mapit;
                        int vocabcount = 0;
                        mapit = docwords.find(*wordit);
                        if (mapit != docwords.end())
                        {
                            vocabcount = mapit->second;
                        }
                        wordcount.push_back(vocabcount);
                        //cout << "word Count: " << vocabcount << "\n";
                    }
                    //cout << "words: " << wordcount.size() << "\n";
                    DocumentWordMatrix.push_back(wordcount);
                    newfile.close(); //close the file object.
                }          
            }
        }
    //cout << "Count: " << count << "\n";

    }   
    closedir(dp);
    DocumentNumber = DocumentWordMatrix.size();
    cout << "Documents: " << DocumentNumber << "\n";
    //cout << DocumentWordMatrix  << "\n";
    cout << "Doc Word matrix complete: " << "\n";
  
}
void CorpusClass::initialize_prob()
{
    srand(static_cast<unsigned int>(std::time(nullptr))); 
    //vector<vector<double>> DocTopicProb;
    // doc topic prob
    for(int i = 0; i < DocumentNumber; i++)
    {
        vector<double> prob;
        double probsum = 0;
        int yy;
        for(yy = 0; yy < number_of_topics; yy++)
        {
            //double newprob = 1;
            double newprob = (double)rand()/RAND_MAX; 
            probsum += newprob;
            prob.push_back(newprob);
        }
        for(yy = 0; yy < number_of_topics; yy++)
        {
            prob[yy] = prob[yy]/probsum;
        }
        DocTopicProb.push_back(prob);
    }
    
    //vector<vector<double>> TopicWordProb;
    for(int ii = 0; ii < number_of_topics; ii++)
    {
        vector<double> prob;
        double probsum = 0;
        int yy;
        for(yy = 0; yy < VocublarySize; yy++)
        {
            double newprob = (double)rand()/RAND_MAX; 
            //double newprob = 1;
            probsum += newprob;
            prob.push_back(newprob);
        }
        for(yy = 0; yy < VocublarySize; yy++)
        {
            prob[yy] = prob[yy]/probsum;
        }
        TopicWordProb.push_back(prob);
    }
    //cout << "Initialization complete: " << "\n";

}

void CorpusClass::setup_array()
{
    pdocarray = new double**[DocumentNumber];
    for (int i = 0; i < DocumentNumber; i++)
    {
        double** ptopicarray = new double*[number_of_topics];
        //cout << "Allocated topic array \n";
        pdocarray[i] = ptopicarray;
        for (int ii = 0; ii < number_of_topics; ii++)
        {
            double* pwordarray = new double[VocublarySize];
            //cout << "Allocated word array \n";
            ptopicarray[ii] = pwordarray;
            //for (int iii = 0; iii < VocublarySize; iii++)
                //pwordarray[iii] = 0;
        }
    }
    cout << "E Initialization Setup \n";
}

void CorpusClass::delete_array()
{
    if ( pdocarray != 0)
    {
        for (int i = 0; i < DocumentNumber; i++)
        {
            double** ptopicarray = pdocarray[i];
            for (int ii = 0; ii < number_of_topics; ii++)
            {
                double* pwordarray = ptopicarray[ii];
                delete[] pwordarray;
            }
            delete[] ptopicarray;
        }
        delete[] pdocarray;    
    }
    pdocarray = 0;
}

void CorpusClass::run_iteration()
{
    //cout << "E Step \n";
    //cout << "Allocated doc array \n";
    for(int docindex = 0; docindex < DocumentNumber; docindex++)
    {
        //cout << "E Doc \n";
        for(int wordindex = 0; wordindex < VocublarySize; wordindex++)
        {
            double mysum = 0;
            //cout << "E Word \n";
            for(int topicindex = 0; topicindex < number_of_topics; topicindex++)
            {
                //cout << "E Topic \n";
                double newval = TopicWordProb[topicindex][wordindex] * DocTopicProb[docindex][topicindex];
                //cout << "E Topic1 \n";
                double** ptopicarray = pdocarray[docindex];
                double* pwordarray = ptopicarray[topicindex];
                pwordarray[wordindex] = newval;

                mysum += newval;
                //cout << " " << mysum << " ";
            }
            //cout << "\n";
            if (mysum != 0)
                for(int topicindex = 0; topicindex < number_of_topics; topicindex++)
                {
                    //cout << "E Topic2 \n";
                    double** ptopicarray = pdocarray[docindex];
                    double* pwordarray = ptopicarray[topicindex];
                    pwordarray[wordindex] = pwordarray[wordindex]/mysum;
                }
        }
   }
    
    //cout << "M Step \n";
    for(int docindex = 0; docindex < DocumentNumber; docindex++)
    {
        double tonormal = 0;
        for(int topicindex = 0; topicindex < number_of_topics; topicindex++)
        {
            double mysum = 0;
            for(int wordindex = 0; wordindex < VocublarySize; wordindex++)
            {
                double** ptopicarray = pdocarray[docindex];
                double* pwordarray = ptopicarray[topicindex];
                double newval = pwordarray[wordindex] * DocumentWordMatrix[docindex][wordindex];
                mysum += newval;
                //cout << "probs: " << DocumentWordMatrix[docindex][wordindex] << "\n";
            }
            DocTopicProb[docindex][topicindex] = mysum;
            //cout << "DocTopicProb: " << DocTopicProb[docindex][topicindex] << "\n";
            tonormal += mysum;
        }
        // normalize
        if (tonormal != 0)
            for(int topicindex = 0; topicindex < number_of_topics; topicindex++)
            {
                DocTopicProb[docindex][topicindex] = DocTopicProb[docindex][topicindex]/tonormal;
                //cout << " " << DocTopicProb[docindex][topicindex] << " ";
            }
        //cout << "\n";
    }
    for(int topicindex = 0; topicindex < number_of_topics; topicindex++)
    {
        double tonormal = 0;
        for(int wordindex = 0; wordindex < VocublarySize; wordindex++)
        {
            double mysum = 0;
            for(int docindex = 0; docindex < DocumentNumber; docindex++)
            {
                double** ptopicarray = pdocarray[docindex];
                double* pwordarray = ptopicarray[topicindex];
                double newval = pwordarray[wordindex] * DocumentWordMatrix[docindex][wordindex];
                mysum += newval;
            }
            TopicWordProb[topicindex][wordindex] = mysum;
            tonormal += mysum;
            //cout << " " << tonormal << " ";
        }
        if (tonormal != 0)
            for(int wordindex = 0; wordindex < VocublarySize; wordindex++)
            {
                TopicWordProb[topicindex][wordindex] = TopicWordProb[topicindex][wordindex]/tonormal;
                //cout << " " << TopicWordProb[topicindex][wordindex] << " ";
            }
        //cout << "\n";
    }
}

double CorpusClass::calculate_likelyhood()
{
    double newlikely = 0;
    for(int docindex = 0; docindex < DocumentNumber; docindex++)
    {
        long double docsum = 0;
        for(int wordindex = 0; wordindex < VocublarySize; wordindex++)
        {
            long double mysum = 0;
            //cout << "DocTopicProb: " ;
            for(int topicindex = 0; topicindex < number_of_topics; topicindex++)
            {
                //cout <<  TopicWordProb[topicindex][wordindex]  << " ";
                //cout <<  DocTopicProb[docindex][topicindex] << " ";
                //cout <<  TopicWordProb[topicindex][wordindex] * DocTopicProb[docindex][topicindex] << ", ";
                mysum += TopicWordProb[topicindex][wordindex] * DocTopicProb[docindex][topicindex];
            }
            //cout << "mysum: " << mysum << "\n";
            //cout << "\n";
            //cout << " " << mysum << ",";
            if (mysum != 0)
                mysum = log10(mysum);
            //cout << mysum << ",";
            //cout << DocumentWordMatrix[docindex][wordindex] << ",";
            //cout << mysum << ",";
            mysum = mysum * DocumentWordMatrix[docindex][wordindex];
            //cout << mysum << " ";
            docsum += mysum;
        }
        //cout << "\n";
        newlikely += docsum;
        //cout << docsum << " ";
        //cout << "docsum: " << docsum << "\n";
        
        //docindex = DocumentNumber;
    }
    //cout << "\n";
    return newlikely;
}

void CorpusClass::PLSA()
{

    double current_likelihood = calculate_likelyhood();
    //cout << "Baseline Likely: " << current_likelihood << "\n";
    for (int i = 0; i < max_iterations; i++)
    {
        //cout << "Iteration: " << i << "\n";
        try 
        {
            run_iteration();         
        }
        catch (const std::bad_alloc& e) 
        {
            std::cout << "Allocation failed: " << e.what() << '\n';
            return;
        }        
        double newlikely = calculate_likelyhood();
        //cout << "Likely: " << newlikely << "\n";
        if(abs(current_likelihood - newlikely) < epsilon)
        {
            cout << "Converge \n";
            cout << "Iteration: " << i << "\n";
            write_topics();
            return;       
        }
        current_likelihood = newlikely; 
    }
    cout << "Converge Failed \n";
    write_topics();
}

int main () {
    CorpusClass corpus;
    corpus.document_path = "c:\\programs\\CS410_Pres_newalpha";
    corpus.output_path = "c:\\programs\\CS410_Pres_Output";
    corpus.time_slice = "2000.11.05";
    corpus.number_of_topics = 30;
    corpus.max_iterations = 250;
    corpus.epsilon = 0.001;
    
    fstream newfile;
    newfile.open("timeslices.txt",ios::in); //open a file to perform read operation using file object
    if (newfile.is_open()) //checking whether the file is open
    {   
        string tp;
        while(getline(newfile, tp)) //read data from file object and put it into string.
        {
            istringstream iss(tp);
            vector<string> results(istream_iterator<string>{iss}, istream_iterator<string>());      
            for (vector<string>::iterator wordit = results.begin() ; wordit != results.end(); ++wordit)
            {
                corpus.time_slice = *wordit;
                corpus.build_vocabulary();
                cout << "build_vocabulary \n";
                corpus.initialize_prob();
                cout << "initialize_prob \n";
                corpus.setup_array();
                cout << "setup_array \n";
              
                corpus.PLSA();
                
                corpus.delete_array();
            }
        }
     }
   
    


    return 0;
}