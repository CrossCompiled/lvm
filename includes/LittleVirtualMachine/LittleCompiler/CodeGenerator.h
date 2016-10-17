//
// Created by kaspersk on 10/12/16.
//

#ifndef LITTLEVIRTUALMACHINE_CODEGENERATOR_H
#define LITTLEVIRTUALMACHINE_CODEGENERATOR_H

namespace lvm {
    namespace compiler{
        class CodeGenerator {

            struct label {
                int address = 0;
                std::vector<int> references;
            };

            std::map<std::string, label> LabelMap;
            std::map<std::string, int> OpcodeMap;
            std::vector<int> code;

        public:

            CodeGenerator(){
                MakeOpcodeMap<SupportedTypes>::run(OpcodeMap);
            }

            void GenerateInstruction(const std::string& instruction);
            void GeneratePrint();
            void GenerateLabelDef(const std::string& labelDef);
            void GenerateLabelRef(const std::string& labelRef);
            void GenerateValue(const std::string& value);
            void GenerateStringValue(const std::string& string);

        };
    }
}


#endif //LITTLEVIRTUALMACHINE_CODEGENERATOR_H
