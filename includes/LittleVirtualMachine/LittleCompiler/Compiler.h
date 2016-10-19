#ifndef LITTLEVIRTUALMACHINE_COMPILER_STATES_H
#define LITTLEVIRTUALMACHINE_COMPILER_STATES_H

#include <string>
#include <exception>
#include <istream>
#include <ostream>
#include <vector>

#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/deep_history.hpp>
#include <boost/statechart/shallow_history.hpp>
#include <boost/statechart/exception_translator.hpp>

#include <boost/mpl/list.hpp>

#include "LittleVirtualMachine/LittleCompiler/ICodeGenerator.h"
#include "LittleVirtualMachine/LittleCompiler/CompilerExceptions.h"
#include "LittleVirtualMachine/LittleCompiler/FileHelper.h"

namespace sc = boost::statechart;
namespace mpl = boost::mpl;

namespace lvm {
	namespace compiler {
		/*
		 * Events
		 */

		struct LoadFileEvent : sc::event<LoadFileEvent> {
			LoadFileEvent(std::string& path) : path_(path) {}
			std::string& get_path() const { return path_; }

		private:
			std::string& path_;
		};

		struct ProcessCharEvent : sc::event<ProcessCharEvent> {
		};

		struct WriteByteCodeEvent : sc::event<WriteByteCodeEvent> {
			WriteByteCodeEvent(std::ostream& output) : output_(output) {}
			std::ostream& get_stream() const { return output_; }

		private:
			std::ostream& output_;
		};

		/*
		 * State machine declaration
		 */

		struct Initial;
		struct Parsing;
		struct WriteByteCode;
		struct Failed;

		struct Compiler : sc::state_machine<Compiler, Initial, std::allocator<void>, sc::exception_translator<>> {
			Compiler(ICodeGenerator& generator, IStreamHelper& inputHelper) : generator_(generator), input_(inputHelper) {}

			IStreamHelper& get_input() const { return input_; }

			ICodeGenerator& get_generator() const { return generator_; }

		private:
			ICodeGenerator& generator_;
			IStreamHelper& input_;
		};

		/*
		 * Initial state
		 */

		struct Initial : sc::simple_state<Initial, Compiler> {
			typedef mpl::list<sc::custom_reaction<sc::exception_thrown>, sc::custom_reaction<LoadFileEvent>> reactions;

			sc::result react(const LoadFileEvent& ev) {
				context<Compiler>().get_input().open(ev.get_path());

				return transit<Parsing>();
			}

			sc::result react(const sc::exception_thrown&) {
				try {
					throw;
				}
				catch (const CompilerException& e) {
					std::cerr << "Compilation error: " <<  e.what() << std::endl;
					return transit<Failed>();
				}
				catch (const std::runtime_error& e) {
					std::cerr << "Run-time error: " <<  e.what() << std::endl;
					return transit<Failed>();
				}
				catch (...) {
					return forward_event();
				}
			}
		};

		/*
		 * Failed state
		 */

		struct Failed : sc::simple_state<Failed, Compiler> {
			Failed() {
				std::cerr << "Build failed..." << std::endl;
			}
		};

		/*
		 * Parsing state and substates
		 */

		struct NewLine;
		struct Instruction;
		struct Number;
		struct Label;
		struct String;
		struct EndOfFile;
		struct Comment;

		struct Parsing : sc::simple_state<Parsing, Compiler, NewLine> {
			typedef sc::custom_reaction<sc::exception_thrown> reactions;

			sc::result react(const sc::exception_thrown&) {
				try {
					throw;
				}
				catch (const ParsingError& e) {
					std::cerr << "Something went wrong with the parsing: " << e.what() << std::endl;
					return transit<Failed>();
				}
				catch (const WrongOpcode& e) {
					std::cerr << "Invalid instruction: " << e.what() << std::endl;
					return transit<Failed>();
				}
				catch (const MissingLabel& e) {
					std::cerr << "Label hasn't been declared: " << e.what() << std::endl;
					return transit<Failed>();
				}
				catch (const DuplicateLabel& e) {
					std::cerr << "Label was declared more that once: " << e.what() << std::endl;
					return transit<Failed>();
				}
				catch (...) {
					return forward_event();
				}
			}
		};

		struct NewLine : sc::simple_state<NewLine, Parsing> {
			typedef sc::custom_reaction<ProcessCharEvent> reactions;

			sc::result react(const ProcessCharEvent&) {
				int first = context<Compiler>().get_input().get_ro_stream().peek();

				if (std::isalpha(first)) { return transit<Instruction>(); }

				if (std::isdigit(first)) { return transit<Number>(); }

				if (first == '"') { return transit<String>(); }

				if (first == ':' || first == '@') { return transit<Label>(); }

				if (first == ';') { return transit<Comment>(); }

				if (first == -1) { return transit<EndOfFile>(); }

				if (std::isspace(first)) {
					context<Compiler>().get_input().get_ro_stream().get();
					return discard_event();
				}

				throw ParsingError("Unknown indentifier: " + first);
			}
		};

		struct Instruction : sc::simple_state<Instruction, Parsing> {
			typedef sc::custom_reaction<ProcessCharEvent> reactions;
			sc::result react(const ProcessCharEvent&) {
				int read = context<Compiler>().get_input().get_ro_stream().get();

				if (std::isalpha(read)) {
					buffer += read;
					return discard_event();
				} else if (std::isspace(read)) {
					std::transform(buffer.begin(), buffer.end(), buffer.begin(), std::ptr_fun<int, int>(std::toupper));

					if (buffer == "PRINT")
						context<Compiler>().get_generator().GeneratePrint();
					else
						context<Compiler>().get_generator().GenerateInstruction(buffer);

					return transit<NewLine>();
				}

				return discard_event();
			}

		private:
			std::string buffer;
		};

		struct Number : sc::simple_state<Number, Parsing> {
			typedef sc::custom_reaction<ProcessCharEvent> reactions;
			sc::result react(const ProcessCharEvent&) {
				int read = context<Compiler>().get_input().get_ro_stream().get();

				if (std::isdigit(read)) {
					buffer += read;
					return discard_event();
				} else if (std::isspace(read)) {
					context<Compiler>().get_generator().GenerateValue(buffer);
					return transit<NewLine>();
				}

				return discard_event();
			}

		private:
			std::string buffer;
		};

		struct Label : sc::simple_state<Label, Parsing> {
			typedef sc::custom_reaction<ProcessCharEvent> reactions;

			sc::result react(const ProcessCharEvent&) {
				int read = context<Compiler>().get_input().get_ro_stream().get();

				if (read == ':') {
					type = LabelDef;
					return discard_event();
				}
				else if (read == '@') {
					type = LabelRef;
					return discard_event();
				}
				else if (std::isspace(read))
				{
					switch (type)
					{
						case LabelDef:
							context<Compiler>().get_generator().GenerateLabelDef(buffer);
							return transit<NewLine>();
						case LabelRef:
							context<Compiler>().get_generator().GenerateLabelRef(buffer);
							return transit<NewLine>();
						case Unknown:
						default:
							throw std::exception();
					}
				}
				else {
					buffer += read;
				}

				return discard_event();
			}

		private:
			std::string buffer;

			enum ReferenceType {
				Unknown,
				LabelDef,
				LabelRef
			};

			ReferenceType type;
		};

		struct String : sc::simple_state<String, Parsing> {
			typedef sc::custom_reaction<ProcessCharEvent> reactions;
			sc::result react(const ProcessCharEvent&) {
				int read = context<Compiler>().get_input().get_ro_stream().get();

				if (read == '"') {
					delimiter++;

					if (delimiter >= 2) {
						context<Compiler>().get_generator().GenerateStringValue(buffer);
						return transit<NewLine>();
					}

					return discard_event();
				}

				buffer += read;
				return discard_event();
			}

		private:
			int delimiter = 0;
			std::string buffer;
		};

		struct Comment : sc::simple_state<Comment, Parsing> {
			typedef sc::custom_reaction<ProcessCharEvent> reactions;

			sc::result react(const ProcessCharEvent&) {
				int read = context<Compiler>().get_input().get_ro_stream().get();
				if (read == '\n') {
					return transit<NewLine>();
				}

				return discard_event();
			}
		};

		struct EndOfFile : sc::simple_state<EndOfFile, Parsing> {
			typedef sc::transition<WriteByteCodeEvent, WriteByteCode> reactions;
		};

		/*
		 * WritingByteCode state
		 */

		struct WriteByteCode : sc::simple_state<WriteByteCode, Compiler> {
			typedef sc::custom_reaction<WriteByteCodeEvent> reactions;

			sc::result react(const WriteByteCodeEvent& e) {
				context<Compiler>().get_generator().OutputCode(e.get_stream());
				return transit<Initial>();
			}
		};
	}
}
#endif //LITTLEVIRTUALMACHINE_COMPILER_STATES_H
