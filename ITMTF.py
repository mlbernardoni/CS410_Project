import xml.etree.ElementTree as ET 
import os

class CleanData(object):

    """
    A collection of documents.
    """

    def __init__(self, documents_path):
        """
        Initialize empty document list.
        """
        self.documents_path = documents_path
        
    def cleanxml(self):
        print(self.documents_path)
        count = 0;
        for subdir, dirs, files in os.walk(self.documents_path):
            for filename in files:
                filepath = subdir + os.sep + filename
                #print (filepath)        
                day = 0
                month = 0
                year = 0
                docs = []
                # create element tree object 
                tree = ET.parse(filepath) 
                # get root element 
                root = tree.getroot() 
                head = tree.find('head')
                meta = head.findall('meta')
                for metadata in meta:
                    if metadata.attrib['name'] == 'publication_day_of_month' :
                        day = metadata.attrib['content']
                    if metadata.attrib['name'] == 'publication_month' :
                        month = metadata.attrib['content']
                    if metadata.attrib['name'] == 'publication_year' :
                        year = metadata.attrib['content']
                #print(year, month, day) 

                body = root.find('body')
                content = body.find('body.content')
                for block in content:
                    if block.attrib['class'] == 'full_text' :
                        for para in block :
                            if (para.text.find('Gore') != -1) or (para.text.find('Bush') != -1) :
                                mystring = 'Pres\\' + str(year) + "."  + str(month).zfill(2) + "." + str(day).zfill(2) + "." + str(count).zfill(8) + '.txt'
                                f = open(mystring, "w")
                                f.write(para.text)
                                f.close()                                
                                count = count + 1
                                #docs.append(para.text)
                            #print(para.text)
        print(count)


def main():
    #documents_path = "C:\\programs\\CS410_data\\2000\\07\\01"
    documents_path = "C:\\programs\\CS410_data"
    #documents_path = '1211543.xml' 
    cleandata = CleanData(documents_path)  # instantiate cleandata
    cleandata.cleanxml()

#from IPython.display import Javascript
#Javascript("Jupyter.notebook.execute_cells([0])")
if __name__ == '__main__':
    main()
    