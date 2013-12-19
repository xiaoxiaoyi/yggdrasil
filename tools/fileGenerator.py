#!/usr/bin/python

# Yggdrasil Project (2013)
# Laurent "Otium" Noel | Vincent "Hekiat" Lesueur 
# laurent.noel.c2ba[at]gmail.com | lesueurv[at]gmail.com

import sys

if len(sys.argv) != 3 :
    sys.exit("At least 2 arguments needed.Format: ./fileGenerator.py ClassName moduleName (moduleName Directory must be created)")
    

fileName = sys.argv[1];
module = sys.argv[2];

# Creation HPP
print "Creating " + fileName + ".hpp "

f = open("../yggdrasil/"+fileName+".hpp", "w")

f.write("// Yggdrasil Project (2013)\n")
f.write("// Laurent \"Otium\" Noel | Vincent \"Hekiat\" Lesueur \n")
f.write("// laurent.noel.c2ba[at]gmail.com | lesueurv[at]gmail.com\n\n")

f.write("#ifndef _YGG_"+fileName.upper()+"_HPP_\n")
f.write("#define _YGG_"+fileName.upper()+"_HPP_\n\n")

f.write("namespace ygg {\n\n")
f.write("class "+ fileName +" {\n")
f.write("public:\n")
f.write("    "+fileName+"();\n")
f.write("    ~"+fileName+"();\n\n")

f.write("private:\n\n")


f.write("};\n\n}\n")

f.write("\n#endif //"+"_YGG_"+fileName.upper()+"_HPP_\n")

f.close()

# Creation CPP

print "Creating " + fileName + ".cpp "

f = open("../yggdrasil/"+fileName+".cpp", "w")

f.write("// Yggdrasil Project (2013)\n")
f.write("// Laurent \"Otium\" Noel | Vincent \"Hekiat\" Lesueur \n")
f.write("// laurent.noel.c2ba[at]gmail.com | lesueurv[at]gmail.com\n\n")

f.write("#include \""+fileName+".hpp\"\n")

f.write("\n\nnamespace ygg {\n\n")
f.write(fileName+"::"+fileName+"() {\n\n}\n\n")
f.write(""+fileName+"::~"+fileName+"() {\n\n}\n\n")

f.write("}\n")

f.close()

