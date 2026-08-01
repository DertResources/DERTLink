fileContents = \
"""#pragma once
#include <type_traits>

template <typename T>
struct return_type;

template <typename C, typename R, typename... Args>
struct return_type<R (C::*)(Args...)> {
    using type = std::remove_reference_t<R>;
};

#define GET_MACRO1("""

numOfEntries = 20
for i in range(1, numOfEntries+2):
    fileContents += "_" + str(i) + ", "
fileContents += \
"""NAME, ...) NAME

"""
funcName = "EXTRAPOLATE_TYPE"

for i in range(1, numOfEntries+1):
    fileContents += "#define " + funcName + str(i) + "(Class, func1"
    if(i > 1):
        for j in range(2, i+1):
            fileContents += ", func" + str(j)
    fileContents += ") \\ \n"
    if(i > 1):
        fileContents += "using _1 = return_type<decltype(&Class::func1)>::type; \\\n"
        for j in range(2, i):
            fileContents += "using _" + str(j) + " = return_type<decltype(&_" + str(j-1) + "::func" + str(j) + ")>::type; \\\n"
        fileContents += "using Result = return_type<decltype(&_" + str(i-1) + "::func" + str(i) + ")>::type; \\\n"
    else:
        fileContents += "using Result = return_type<decltype(&Class::func1)>::type; \\\n"
    fileContents += "\n\n"

fileContents += "#define EXPAND(x) x\n" 
fileContents += "#define " + funcName + "(...) EXPAND(GET_MACRO1(__VA_ARGS__"
for i in range(0, numOfEntries):
    fileContents += ", " + funcName + str(numOfEntries-i)
fileContents += ")(__VA_ARGS__) )\n"



# NEW FILE
fileContents += "\n\n\n\n"
# fileContents = ""
funcName = "EMPLACE_DATA"

count2 = 20
fileContents += "#include <any>\n\n"

for i in range(1, count2+1):
    fileContents += "#define " + funcName + str(i) + "(CreateInfoClass"
    for j in range(1, i+1):
        fileContents += ", Function" + str(j) + ", StringKey" + str(j)
    fileContents += ") \\\n"
    # end of define header
    fileContents += "[](std::unique_ptr<std::any> obj, ShortDev& sd, DynamicBuffer& dbf, DynamicBuffer& dbd, DeviceBuilder& db) {    \\\n"
    for k in range(0, i):
        fileContents += "   input([&](std::any arg) \\\n   {                                             \\\n"
        if( k != 0 ):
            fileContents += "       EXTRAPOLATE_TYPE(CreateInfoClass"
            for j in range(1, k+1):
                fileContents += ", Function" + str(j)
            fileContents += ")  \\\n"
        else:
            fileContents += "       using Result = CreateInfoClass;        \\\n"
        fileContents += "       RunFunc(obj.get(), arg, &Result::Function" + str(k+1) + ");        \\\n"
        fileContents += "   }, StringKey" + str(k+1) + ", sd, dbf, dbd, db);\\\n"
        if(k != i-1):
            fileContents += "                         \\\n"
        else:
            fileContents += "}  \n\n"

fileContents += "#define GET_MACRO2("
for i in range(1, 2*count2+2):
    fileContents += "_" + str(i) + ", "
fileContents += "NAME, ...) NAME\n\n"


# accessor function
fileContents += "#define " + funcName + "(...) EXPAND(GET_MACRO2(__VA_ARGS__"
for i in range(0, count2):
    fileContents += ", " + funcName + str(count2-i) 
    if(i != count2-1):
        fileContents += ", __" + str(i)

fileContents += ")(__VA_ARGS__) )\n"
# auto wrapper = [](std::any obj, std::any arg) { FOO(RootClass, F1, F2) \
#                                                 RunFunc(obj, arg, &Result::funcName); }

with open("include/AutoGenFile.h", "w", encoding="utf-8") as f:
    f.write(fileContents)