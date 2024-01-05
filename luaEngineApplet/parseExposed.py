##
## EPITECH PROJECT, 2023
## rtype
## File description:
## parseExposed.py
##

import os
import sys
import regex as re

template_includes = """
/**
    * @file luaExposed.hpp
    * @brief File that contains all the exposed classes and functions to Lua
    * @date 2020
    * DISCLAIMER: This file is generated automatically by the parseExposed.py script.
    *             Do not modify it directly, or your changes will be lost.
*/
#pragma once
#include <LuaBridge/LuaBridge.h>

"""

template_include = """
#include "%s"
"""

template_prototype = """
static void lua_init_exposed(lua_State *L) {
"""

template_class_with_namespace = """
    luabridge::getGlobalNamespace(L)
        .beginNamespace("%s")
            .beginClass<%s>("%s")
"""

template_class_without_namespace = """
    luabridge::getGlobalNamespace(L)
            .beginClass<%s>("%s")
"""

template_add_constructor = """
                .addConstructor<void (*) (void)>()
"""

template_function = """
                .addFunction("%s", &%s::%s)
"""

template_property = """
                .addProperty("%s", &%s::%s)
"""

template_end_class = """
            .endClass();
"""

template_end_includes = """
}
"""

def lines2words(text):
    words = []
    for line in text:
        tmp = line.split(' ')
        tmp = [x.strip() for x in tmp]
        tmp = [x for x in tmp if x != '']
        words += tmp
    return words

class CXXClass:
    def __init__(self, text, namespace = ""):
        self.text = text
        if (self.is_virtual()):
            raise Exception("Error: Virtual classes are not supported")
        self.namespace = namespace
        self.name = ""
        self.parseName()
        self.exposedMethods = []
        self.parseMethods()
        self.export = ""
        self.generateExport()

    def parseName(self):
        for word in self.text:
            if word == "class":
                self.name = self.text[self.text.index(word) + 1]
                return

    def is_virtual(self):
        for word in self.text:
            if word == "virtual":
                return True
        return False

    def parseMethods(self):
        """
        any method that is exposed to Lua must be declared like this:
        _LUAM <optional additional keywords> <return type> <method name>(<args>)
        The optional keywords could, for example, be 'static' or 'const', or even 'template <...>'
        """
        pattern = r'_LUAM\s*(?P<additional_keywords>[\w\s]*)\s+(?P<return_type>[^\s]+)\s+(?P<method_name>\w+)\((?P<args>[^)]*)\)'
        source = " ".join(self.text)
        i = 0
        while i < len(source):
            matches = list(re.finditer(pattern, source[i:]))
            for match in matches:
                method = {
                    'name': match.group('method_name'),
                    'arguments_types': [arg.strip() for arg in match.group('args').split(',') if arg.strip()],
                    'return_type': match.group('return_type'),
                }
                method['arguments_types'] = [arg.split(' ')[:-1] for arg in method['arguments_types']]
                method['arguments_types'] = [arg[0] for arg in method['arguments_types']]
                is_duplicate = False
                for m in self.exposedMethods:
                    if m['name'] == method['name']:
                        is_duplicate = True
                        break
                if not is_duplicate:
                    self.exposedMethods.append(method)
            i += 1


    def generateExport(self):
        if self.namespace != "":
            self.export += template_class_with_namespace % (self.namespace, self.namespace + "::" + self.name, self.name)
            self.name = self.namespace + "::" + self.name
        else:
            self.export += template_class_without_namespace % (self.name, self.name)
        self.export += template_add_constructor
        for method in self.exposedMethods:
            self.export += template_function % (method['name'], self.name, method['name'])
        self.export += template_end_class
        self.export = re.sub(r'\n\n', r'\n', self.export)

    def __str__(self):
        return self.export

class File:
    def __init__(self, name):
        self.name = name
        self.classes = []

def parseClass(lines):
    pass

def goToEndOfClass(words, i):
    openBrackets = 0
    while i < len(words):
        if words[i] == "};":
            if openBrackets == 1:
                return i
        if words[i] == "{":
            openBrackets += 1
        elif words[i] == "}":
            openBrackets -= 1
        i += 1


def parseFile(file):
    namespace = ""
    file = open(file, "r")
    lines = file.readlines()
    file.close()
    classes = []
    words = lines2words(lines)
    i = 0
    while i < len(words):
        if words[i] == "namespace":
            namespace = words[i + 1]
        if words[i] == "class":
            j = i - 1
            while j > 0 and words[j] == "serialize":
                j -= 1
            if words[j] != "_LUAC":
                i += 1
                continue
            try :
                c = CXXClass(words[i:goToEndOfClass(words, i)], namespace)
                print("-- Lua: Parsing class " + c.name)
                classes.append(c)
            except:
                pass

        i += 1
    return classes

def parseExposed(rootDir, outputDir):
    files = []
    i = 0
    for dir in rootDir:
        for root, dirs, filenames in os.walk(dir):
            for filename in filenames:
                if filename.endswith(".hpp"):
                    full_path = os.path.join(root, filename)
                    file = File(full_path)
                    file.classes = parseFile(file.name)
                    files.append(file)
    output = template_includes
    for file in files:
        output += template_include % file.name.split("/")[-1] if not file.classes.__len__() == 0 else ""
    output += template_prototype
    for file in files:
        for c in file.classes:
            output += str(c)
    output += template_end_includes
    if not os.path.exists(outputDir):
        os.makedirs(outputDir)
    with open(outputDir + "/luaExposed.hpp", "w") as f:
        f.write(output)
    print("Lua: Parsing files... [DONE]")


if __name__ == "__main__":
    args = sys.argv[1:]
    rootDir = None
    outputDir = None
    try:
        rootDir = args[:-1]
        outputDir = args[-1]
    except:
        print("Error: Invalid arguments")
        exit(84)
    parseExposed(rootDir, outputDir)