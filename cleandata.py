import xml.etree.ElementTree as ET 
import os
import re


def normalize(input_matrix):
    """
    Normalizes the rows of a 2d input_matrix so they sum to 1
    """

    row_sums = input_matrix.sum(axis=1)
    try:
        assert (np.count_nonzero(row_sums)==np.shape(row_sums)[0]) # no row should sum to zero
    except Exception:
        raise Exception("Error while normalizing. Row(s) sum to zero")
    new_matrix = input_matrix / row_sums[:, np.newaxis]
    return new_matrix

       
class Corpus(object):

    """
    A collection of documents.
    """

    def __init__(self, documents_path, clean_path):
        """
        Initialize empty document list.
        """
        self.vocabulary = dict()
        self.documents_path = documents_path
        self.clean_path = clean_path

    def build_clean(self):
        stopwords = dict()
        with open("stopwords.txt") as swf:
            for line in swf:
                line = re.sub(r'[^a-zA-Z]','', line)
                if not stopwords.get(line):
                    stopwords[line] = 1
        print(len(stopwords))
        print(self.documents_path)
        print(self.clean_path)

        count = 0
        for subdir, dirs, files in os.walk(self.documents_path):
            for filename in files:
                filepath = subdir + os.sep + filename
                writefile = self.clean_path + '\\' + filename
                writefile = writefile.replace("txt", "csv")
                with open(filepath) as f:
                    document = dict()
                    for line in f:
                        line = line.lower()
                        #print(line)
                        words = line.split()
                        for word in words:
                            #word = re.sub(r'\W+', '', word)
                            word = re.sub(r'[^a-zA-Z]','', word)
                            #word = re.sub(r'[^a-zA-Z0-9]','', word)
                            #print(word + " ")
                            if (len(word) > 0) :
                                if not stopwords.get(word):
                                    if self.vocabulary.get(word):
                                        self.vocabulary[word] += 1
                                    else:
                                        self.vocabulary[word] = 1
                                    if document.get(word):
                                        document[word] += 1
                                    else:
                                        document[word] = 1
                        count = count + 1
                        fo = open(writefile, "w")
                        for key, value in document.items():
                            fo.write(key + ',' + str(value) + "\n")
                        fo.close()                            
        vocabfile = self.clean_path + '\\vocabulary.csv'
        fv = open(vocabfile, "w")
        for key, value in self.vocabulary.items():
            fv.write(key + ',' + str(value) + "\n")
        fv.close()                            
                           
        #self.vocabulary = list(vocab.keys()) 
        #log = open("vocab.csv", "w")
        #print(self.vocabulary, file = log) 
        #print(self.vocabulary) 
        self.vocabulary_size = len(self.vocabulary)
        print("number of docs: " + str(count))     
        print("vocabulary size: " + str(len(self.vocabulary)))     


def main():
    #documents_path = 'data/test.txt' 
    documents_path = "C:\\programs\\CS410_Pres_raw"
    clean_path = "C:\\programs\\CS410_Pres_cleanalpha"
    
    corpus = Corpus(documents_path, clean_path)  # instantiate corpus
    corpus.build_clean()
    print("Total vocabulary:" + str(len(corpus.vocabulary)))


if __name__ == '__main__':
    main()
