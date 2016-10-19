#ifndef LITTLEVIRTUALMACHINE_ICODEGENERATOR_H
#define LITTLEVIRTUALMACHINE_ICODEGENERATOR_H

namespace lvm {
	namespace compiler {
		class ICodeGenerator {
		public:
			virtual ~ICodeGenerator() {};
			virtual void GenerateInstruction(const std::string& instruction) = 0;
			virtual void GeneratePrint() = 0;
			virtual void GenerateLabelDef(const std::string& labelDef) = 0;
			virtual void GenerateLabelRef(const std::string& labelRef) = 0;
			virtual void GenerateValue(const std::string& value) = 0;
			virtual void GenerateStringValue(const std::string& string) = 0;
			virtual void OutputCode(std::ostream& out) = 0;

		protected:
			ICodeGenerator() {};
		};
	}
}

#endif //LITTLEVIRTUALMACHINE_ICODEGENERATOR_H
