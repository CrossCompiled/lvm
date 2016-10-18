//
// Created by kaspersk on 10/12/16.
//


#include "../../includes/LittleVirtualMachine/LittleCompiler/CodeGenerator.h"
#include "../../includes/LittleVirtualMachine/LittleCompiler/CompilerExceptions.h"
#include <string>
#include <fstream>
#include <algorithm>

namespace lvm {
    namespace compiler {

        void CodeGenerator::GenerateInstruction(const std::string &instruction) {
            if(OpcodeMap.count(instruction)) {
                code.push_back(OpcodeMap[instruction]);
            } else {
                throw WrongOpcode(instruction);
            }
        }

        void CodeGenerator::GenerateLabelDef(const std::string &labelDef) {
            if(LabelMap.count(labelDef) && LabelMap[labelDef].address != -1){
                throw DuplicateLabel(labelDef);
            }

            LabelMap[labelDef].address = code.size();
        }

        void CodeGenerator::GenerateLabelRef(const std::string &labelRef) {
            code.push_back(0);
            LabelMap[labelRef].references.push_back(code.size() -1);
        }

        int GetEscape(char& x){
            switch (x){
                case 'n':
                    return '\n';
                case 't':
                    return '\t';
                case 'a':
                    return '\a';
                case '\\':
                    return '\\';
                default:
                    return static_cast<int>(x);
            }
        }

        void CodeGenerator::GenerateStringValue(const std::string &userString) {
            std::vector<int> stringy;
            bool backslash;
            std::for_each(userString.begin(), userString.end(), [&backslash, &stringy](std::string::value_type x) {
                if(x == '\\'){
                    backslash == true;
                } else {
                    if(backslash){
                        stringy.push_back(GetEscape(x));
                        backslash = false;
                    } else {
                        stringy.push_back(x);
                    }
                }
            });

            code.push_back(0);
            std::for_each(stringy.rbegin(), stringy.rend(), [this](std::vector<int>::reference x){
                code.push_back(x);
            });
        }

        void CodeGenerator::GeneratePrint() {
            int print = code.size();
            int end = print + 5;
            code.push_back(OpcodeMap["DUP"]);
            code.push_back(0);
            code.push_back(end);
            code.push_back(OpcodeMap["JE"]);
            code.push_back(OpcodeMap["OUT"]);
            code.push_back(print);
            code.push_back(OpcodeMap["JMP"]);
            code.push_back(OpcodeMap["POP"]);
        }

        void CodeGenerator::OutputCode(std::ostream* out) {
            std::for_each(LabelMap.begin(), LabelMap.end(), [this](std::pair<std::string, label> x){
                std::for_each(x.second.references.begin(), x.second.references.end(), [this, &x](std::vector<int>::value_type y){
                   code[y] = x.second.address;
                });
            });

            std::for_each(code.begin(), code.end(), [&out](std::vector<int>::value_type x){
                out->write(reinterpret_cast<char*>(&x), sizeof(int));
            });

        }

        void CodeGenerator::GenerateValue(const std::string &value) {
            code.push_back(OpcodeMap["PUSH"]);
            code.push_back(stoi(value));
        }
    }
}