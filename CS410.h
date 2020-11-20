#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>      // std::istringstream
#include <math.h>
#include <ctime>
#include "HashTable.h"

using namespace std;

struct docinfo
{
	int index;
	int totalwords;
	string filename;
};

class CorpusClass {
public:
	CorpusClass();
	~CorpusClass();
    
    void setup_array();
    void delete_array();
    void build_vocabulary();
    void initialize_prob();
    void run_iteration();
    double calculate_likelyhood();
    void PLSA();
   
    string document_path;
    int number_of_topics;
    int max_iterations;
    double epsilon;
    
   
    int DocumentNumber;
    //vector<map <string, int>> DocumentWords;
    int VocublarySize;
    map <string, int> VocabularyMap;
    
    vector<vector<int>> DocumentWordMatrix;
    
    vector<vector<double>> TopicWordProb;
    vector<vector<double>> DocTopicProb;
    double*** pdocarray;

};
