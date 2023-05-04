import os

def concate_regex(fPath, rPath):
    with open(fPath, 'r') as r:
        lines = r.readlines()
        concated_regex = '|'.join([line.split('/', 1)[-1].rsplit('/',1)[0].strip() for line in lines])

    with open(rPath, 'w') as w:
        w.write(concated_regex)


if __name__ == "__main__":
    filePath = "/home/fudoyusea/tyy_work/data_real/lyl/java_classes.data"
    resultPath = "/home/fudoyusea/tyy_work/data_real/lyl/java_classes_concate.data"
    concate_regex(filePath, resultPath)