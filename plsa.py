import numpy as np
import math


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

    def __init__(self, documents_path):
        """
        Initialize empty document list.
        """
        self.documents = []
        self.vocabulary = []
        self.likelihoods = []
        self.documents_path = documents_path
        self.term_doc_matrix = None 
        self.document_topic_prob = None  # P(z | d) - INITIALIZE TO RANDOM
        self.topic_word_prob = None  # P(w | z) - INITiALIZE TO RANDOM
        self.topic_prob = None  # P(z | d, w) - NORMALIZED document_topic_prob * topic_word_prob for each doc, each word 

        self.number_of_documents = 0
        self.vocabulary_size = 0

    def build_corpus(self): # NOT USED, DID IT ALL IN 1 FUNCTION build_vocabulary
        return
        """
        Read document, fill in self.documents, a list of list of word
        self.documents = [["the", "day", "is", "nice", "the", ...], [], []...]
        
        Update self.number_of_documents
        # #############################
        # your code here
        # #############################
        """
               
       
        #pass    # REMOVE THIS

    def build_vocabulary(self):
        """
        Construct a list of unique words in the whole corpus. Put it in self.vocabulary
        for example: ["rain", "the", ...]

        Update self.vocabulary_size
        # #############################
        # your code here
        # #############################
        """
        
        vocab = dict()
        docnumber = 0
        with open(self.documents_path, encoding='utf8') as f:
            for line in f:
                line.strip()
                words = line.split()
                self.documents.append([])
                for word in words:
                    if word != "0" and word != "1": # ignore the first word, it is the 0 or 1
                        self.documents[docnumber].append(word)
                        if vocab.get(word):
                            vocab[word] += 1
                        else:
                            vocab[word] = 1
                docnumber = docnumber + 1
            self.number_of_documents = len(self.documents)
            print(self.number_of_documents)
            self.vocabulary = list(vocab.keys())
            self.vocabulary_size = len(self.vocabulary)

    def build_term_doc_matrix(self):
        """
        Construct the term-document matrix where each row represents a document, 
        and each column represents a vocabulary term.

        self.term_doc_matrix[i][j] is the count of term j in document i
        # ############################
        # your code here
        # ############################
        """
        doccount = 0
        mymatrix = []
        for document in self.documents:
            # initialize the variables for this doc
            mymatrix.append([])
            # count the words for this doc
            vocab = dict()
            for word in document:
                if vocab.get(word):
                    vocab[word] += 1
                else:
                    vocab[word] = 1
            wordcount = 0
            for uniqueword in self.vocabulary:
                if vocab.get(uniqueword):
                    mymatrix[doccount].append(vocab.get(uniqueword))
                else:
                    mymatrix[doccount].append(0)
            
            doccount = doccount + 1
        self.term_doc_matrix = mymatrix

        
        #pass    # REMOVE THIS


    def initialize_randomly(self, number_of_topics):
        """
        Randomly initialize the matrices: document_topic_prob and topic_word_prob
        which hold the probability distributions for P(z | d) and P(w | z): self.document_topic_prob, and self.topic_word_prob

        Don't forget to normalize! 
        HINT: you will find numpy's random matrix useful [https://docs.scipy.org/doc/numpy-1.15.0/reference/generated/numpy.random.random.html]
        # ############################
        # your code here
        # ############################
        #np.random.random_sample((3, 2)) 
        """
        self.document_topic_prob = np.random.random_sample((self.number_of_documents, number_of_topics))
        self.document_topic_prob = normalize(self.document_topic_prob)
        #print(self.document_topic_prob)

        self.topic_word_prob = np.random.random_sample((number_of_topics, len(self.vocabulary)))
        self.topic_word_prob = normalize(self.topic_word_prob)
        #print(self.topic_word_prob)

        #pass    # REMOVE THIS
        

    def initialize_uniformly(self, number_of_topics):
        """
        Initializes the matrices: self.document_topic_prob and self.topic_word_prob with a uniform 
        probability distribution. This is used for testing purposes.

        DO NOT CHANGE THIS FUNCTION
        """
        self.document_topic_prob = np.ones((self.number_of_documents, number_of_topics))
        self.document_topic_prob = normalize(self.document_topic_prob)

        self.topic_word_prob = np.ones((number_of_topics, len(self.vocabulary)))
        self.topic_word_prob = normalize(self.topic_word_prob)

    def initialize(self, number_of_topics, random=False):
        """ Call the functions to initialize the matrices document_topic_prob and topic_word_prob
        """
        print("Initializing...")

        if random:
            self.initialize_randomly(number_of_topics)
        else:
            self.initialize_uniformly(number_of_topics)

    def expectation_step(self, number_of_topics):
        """ The E-step updates P(z | w, d)
        """
        return;
                

    def maximization_step(self, number_of_topics):
        print("E step:")
        
        self.topic_prob = np.ones((self.number_of_documents, number_of_topics, self.vocabulary_size))
        for docindex in range(0, self.number_of_documents):
            for wordindex in range(0, self.vocabulary_size):
                mysum = 0
                #print(self.topic_prob[docindex,topicindex])
                for topicindex in range(0, number_of_topics):
                    self.topic_prob[docindex][topicindex][wordindex] = self.topic_word_prob[topicindex, wordindex] * self.document_topic_prob[docindex, topicindex]
                    mysum += self.topic_prob[docindex][topicindex][wordindex]
                for topicindex in range(0, number_of_topics):
                    self.topic_prob[docindex,topicindex,wordindex] = self.topic_prob[docindex,topicindex,wordindex] / mysum

        print("M step:")
        for docindex in range(0, self.number_of_documents):
            for topicindex in range(0, number_of_topics):
                mysum = 0
                for wordindex in range(0, self.vocabulary_size):
                    mysum += self.topic_prob[docindex,topicindex,wordindex] * self.term_doc_matrix[docindex][wordindex]
                self.document_topic_prob[docindex][topicindex] = mysum
        self.document_topic_prob = normalize(self.document_topic_prob)
        #print(self.document_topic_prob)
            
        # update P(z | d) self.document_topic_prob
        for topicindex in range(0, number_of_topics):
            for wordindex in range(0, self.vocabulary_size):
                mysum = 0
                for docindex in range(0, self.number_of_documents):
                    mysum += self.topic_prob[docindex,topicindex,wordindex] * self.term_doc_matrix[docindex][wordindex]
                self.topic_word_prob[topicindex][wordindex] = mysum
        self.topic_word_prob = normalize(self.topic_word_prob)
        #print(self.topic_word_prob)

        


    def calculate_likelihood(self, number_of_topics):
        """ Calculate the current log-likelihood of the model using
        the model's updated probability matrices
        
        Append the calculated log-likelihood to self.likelihoods

        Likelihood:
        For each doc sum:
        C(w,d) * log (sum(Prob of that topic * prob of that word in topic)
        
        loop over docs (variable in self) - docnumber
            loop over words (variable in self) - wordnumber
                multiply Prob of that topic * prob of that word in topic
        log of this
                
        look up np matrix multplication
        
        Then sum over docs
        # ############################
        # your code here
        # ############################
        """
        #print (self.document_topic_prob)
        #print (self.topic_word_prob)
        #print (self.term_doc_matrix)
        Matrix_result = np.matmul(self.document_topic_prob, self.topic_word_prob)
        #print (Matrix_result)
        #arr_result = np.sum(Matrix_result, axis = 0)
        arr_result = Matrix_result[0]
        #print (arr_result)
        #arr_result1 = np.sum(Matrix_result, axis = 1)
        #print (arr_result1)
        arr_likelyhood = np.log10(arr_result)
       
        # sum the rows
        #arr_result = np.log10(Matrix_result)
        #arr_likelyhood = np.sum(arr_result, axis = 0)
        
        #print (arr_result)
        #print (self.term_doc_matrix)
        # log the sums
        #arr_likelyhood = np.log10(arr_result)
        #arr_likelyhood = np.log10(arr_result)
        #print (arr_likelyhood)
        #print (self.term_doc_matrix)
        # multiply by counts
        #self.likelihoods = np.matmul(arr_likelyhood.transpose(), self.term_doc_matrix)
        newlikelihoods = []
        for docindex in range(0, self.number_of_documents):
            #docscore = np.matmul(arr_likelyhood, self.term_doc_matrix[docindex])
            #docscore = np.multiply(arr_likelyhood, self.term_doc_matrix[docindex])
            docscore = np.multiply(arr_likelyhood, self.term_doc_matrix[docindex])
            #print(self.term_doc_matrix[docindex])
            #print(arr_likelyhood)
            #print(docscore)
            docscore = np.sum(docscore)
            #print(docscore)
            newlikelihoods.append(docscore)
        #print(newlikelihoods)
        #print(self.likelihoods)
        #self.likelihoods = np.matmul(arr_likelyhood, self.term_doc_matrix)
        new_likelihood = np.sum(newlikelihoods)
        #print(new_likelihood)
        self.likelihoods.append(new_likelihood)
        
        #return 

    def plsa(self, number_of_topics, max_iter, epsilon):

        """
        Model topics.
        """
        print ("EM iteration begins...")
        
        # build term-doc matrix
        self.build_term_doc_matrix()
        
        # Create the counter arrays.
        
        # P(z | d, w)
        self.topic_prob = np.zeros([self.number_of_documents, number_of_topics, self.vocabulary_size], dtype=np.float)

        # P(z | d) P(w | z)
        self.initialize(number_of_topics, random=False)

        # Run the EM algorithm
        self.calculate_likelihood(number_of_topics)

        current_likelihood = self.likelihoods[-1]

        for iteration in range(0, max_iter):
            print("Iteration #" + str(iteration + 1) + "...")
            self.expectation_step(number_of_topics)
            self.maximization_step(number_of_topics)
            self.calculate_likelihood(number_of_topics)
            
            new_likelihood = self.likelihoods[-1]
            print(current_likelihood)
            print(new_likelihood)
            #return
            newepsilon = abs(new_likelihood - current_likelihood)
            #print(newepsilon)
            if (newepsilon <= epsilon):
                #return
                print("Converge")
                break
            current_likelihood = new_likelihood

            """
            # ############################
            # your code here
            # ############################

            pass    # REMOVE THIS
            """
        #print("Document/Topic")
        #for index in range(0, 15):
            #print(self.document_topic_prob[index])
        #print("Topic/Word")
        #print(self.topic_word_prob)
        



def main():
    #documents_path = 'data/test.txt'
    documents_path = 'data/DBLP.txt'
    #documents_path = 'data/test5.txt'
    corpus = Corpus(documents_path)  # instantiate corpus
    #corpus.build_corpus()
    corpus.build_vocabulary()
    #print(corpus.vocabulary)
    print("Vocabulary size:" + str(len(corpus.vocabulary)))
    print("Number of documents:" + str(len(corpus.documents)))
    #corpus.build_term_doc_matrix()  # testing only REMOVE
    number_of_topics = 2
    #max_iterations = 500
    max_iterations = 50
    epsilon = 0.000
    corpus.plsa(number_of_topics, max_iterations, epsilon)



if __name__ == '__main__':
    main()
