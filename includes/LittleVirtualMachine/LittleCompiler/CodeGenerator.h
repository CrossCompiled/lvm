#ifndef LITTLEVIRTUALMACHINE_CODEGENERATOR_H
#define LITTLEVIRTUALMACHINE_CODEGENERATOR_H

#include <map>
#include <vector>

#include "LittleVirtualMachine/LittleCompiler/ICodeGenerator.h"
#include "LittleVirtualMachine/LittleShared/TypeList.h"
#include "LittleVirtualMachine/LittleShared/SupportedTypes.h"
#include "LittleVirtualMachine/LittleShared/opcodes.h"

namespace lvm {
  namespace compiler {
    class CodeGenerator : public ICodeGenerator {
		public:
			CodeGenerator() {
				lvm::shared::MakeOpcodeMap<lvm::shared::SupportedOpcodes>::run(OpcodeMap);
			}

			virtual ~CodeGenerator() {}

			void GenerateInstruction(const std::string& instruction);

			void GeneratePrint();

			void GenerateLabelDef(const std::string& labelDef);

			void GenerateLabelRef(const std::string& labelRef);

			void GenerateValue(const std::string& value);

			void GenerateStringValue(const std::string& string);

			void OutputCode(std::ostream* out);

		private:
			struct label {
				int address = -1;
				std::vector<int> references;
			};

			std::map<std::string, label> LabelMap;
			std::map<std::string, const int> OpcodeMap;
			std::vector<int> code;
		};
  }
}


#endif //LITTLEVIRTUALMACHINE_CODEGENERATOR_H
