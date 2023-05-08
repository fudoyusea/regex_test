import os
from xeger import Xeger

# import sys
# reload(sys)
# sys.setdefaultencoding('utf-8')


class dataGene(object):
    def __init__(self, regex, dataLen=1024, dataNum=5000):
        dataGene.dataLen = dataLen
        dataGene.regex = regex
        dataGene.dataNum = dataNum
    

    def matchedGen(self):
        outputStrs = ''
        for i in range(self.dataNum):
            geneStr = Xeger(limit=self.dataLen)
            geneMatchedStr = geneStr.xeger(self.regex)
            outputStrs += (geneMatchedStr.strip())
            geneUnmatchedStr = geneStr.xeger("(1(3|5|7|8|9)\d{9})")
            outputStrs += (geneUnmatchedStr.strip())

        print(len(outputStrs))
        return outputStrs


def readRegex(path):
    regexList = []
    f = open(path)
    line = f.readline()
    while line:
        # line = eval(str(line))
        regexList.append(str(line))
        line = f.readline()
    return regexList

def geneData(regexs, output, dScale):
    f = open(output, 'w')
    for regex in regexs:
        # print(regex)
        dG = dataGene(regex, dataNum=dScale)
        data = dG.matchedGen()
        for line in data:
            f.write(line)
            # f.write('\n')
    f.close()
        

if __name__ == "__main__":
    dataScale = 10
    outputPath = "./data_gen_results/dataGen_regex_javaclassconcate" + str(dataScale) +".txt"
    regexPath = "../regex/xxx.data"
    regexList = readRegex(regexPath)
    geneData(regexList, outputPath, dataScale)
    
