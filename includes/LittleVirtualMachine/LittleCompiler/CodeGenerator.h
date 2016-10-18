//
// Created by kaspersk on 10/12/16.
//

#ifndef LITTLEVIRTUALMACHINE_CODEGENERATOR_H
#define LITTLEVIRTUALMACHINE_CODEGENERATOR_H

#include <LittleVirtualMachine/LittleShared/TypeList.h>
#include <LittleVirtualMachine/LittleShared/SupportedTypes.h>
#include <LittleVirtualMachine/LittleShared/opcodes.h>
#include <map>
#include <vector>


namespace lvm {
    namespace compiler{
        class CodeGenerator {

            struct label {
                int address = -1;
                std::vector<int> references;
            };

            std::map<std::string, label> LabelMap;
            std::map<std::string, const int> OpcodeMap;
            std::vector<int> code;

        public:

            CodeGenerator() {
                lvm::shared::MakeOpcodeMap<lvm::shared::SupportedOpcodes>::run(OpcodeMap);
            }

            void GenerateInstruction(const std::string& instruction);
            void GeneratePrint();
            void GenerateLabelDef(const std::string& labelDef);
            void GenerateLabelRef(const std::string& labelRef);
            void GenerateValue(const std::string& value);
            void GenerateStringValue(const std::string& string);
            void OutputCode(std::ostream &out);

        };
    }
}


#endif //LITTLEVIRTUALMACHINE_CODEGENERATOR_H
