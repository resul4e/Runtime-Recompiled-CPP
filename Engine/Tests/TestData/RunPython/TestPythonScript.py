import os

__location__ = os.path.realpath(os.path.join(os.getcwd(), os.path.dirname(__file__)))

filePath = __location__ + "/PYTHON_OUTPUT_FILE.txt"
if os.path.exists(filePath):
    os.remove(filePath)



f = open(filePath, "a")
f.write("SOME_CONTENT_IN_THE_FILE")
f.close()