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

namespace sc = boost::statechart;
namespace mpl = boost::mpl;

namespace lvm {
	namespace compiler {
		/*
		 * Events
		 */

		struct LoadFileEvent : sc::event<LoadFileEvent> {
			LoadFileEvent(std::istream* input) : input_(input) {}
			std::istream* get_stream() const { return input_; }

		private:
			std::istream* input_;
		};

		struct ProcessCharEvent : sc::event<ProcessCharEvent> {
		};

		struct WriteByteCodeEvent : sc::event<WriteByteCodeEvent> {
			WriteByteCodeEvent(std::ostream* output) : output_(output) {}
			std::ostream* get_stream() const { return output_; }

		private:
			std::ostream* output_;
		};

		/*
		 * State machine declaration
		 */

		struct Initial;
		struct Parsing;
		struct WriteByteCode;
		struct Failed;

		struct Compiler : sc::state_machine<Compiler, Initial, std::allocator<void>, sc::exception_translator<>> {
			Compiler(lvm::compiler::ICodeGenerator* generator) : generator_(generator) {}

			std::istream* input_;
			lvm::compiler::ICodeGenerator* generator_;
		};

		/*
		 * Initial state
		 */

		struct Initial : sc::simple_state<Initial, Compiler> {
			typedef mpl::list<sc::custom_reaction<sc::exception_thrown>, sc::custom_reaction<LoadFileEvent>> reactions;

			sc::result react(const LoadFileEvent& ev) {
				if (!ev.get_stream()->good()) {
					throw std::exception();
				}

				context<Compiler>().input_ = ev.get_stream();

				return transit<Parsing>();
			}

			sc::result react(const sc::exception_thrown&) {
				try {
					throw;
				}
				catch (const std::exception&) {
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
		struct Letter;
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
				catch (const std::exception&) {
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
				int first = context<Compiler>().input_->peek();

				if (std::isalpha(first)) { return transit<Letter>(); }
				else if (std::isdigit(first)) { return transit<Number>(); }
				else if (first == '"') { return transit<String>(); }
				else if (first == ':' || first == '@') { return transit<Label>(); }
				else if (first == ';') { return transit<Comment>(); }
				else if (first == EOF) { return transit<EndOfFile>(); }
				else if (std::isspace(first)) {
					context<Compiler>().input_->get();
					return discard_event();
				}
				else {
					throw std::exception();
				}
			}
		};

		struct Letter : sc::simple_state<Letter, Parsing> {
			typedef sc::custom_reaction<ProcessCharEvent> reactions;
			sc::result react(const ProcessCharEvent&) {
				int read = context<Compiler>().input_->get();

				if (std::isalpha(read)) {
					buffer += read;
					return discard_event();
				} else if (std::isspace(read)) {
					std::string uppercase;
					for (auto c : buffer)
						uppercase += std::toupper(c);

					if (uppercase == "PRINT")
						context<Compiler>().generator_->GeneratePrint();
					else
						context<Compiler>().generator_->GenerateInstruction(buffer);

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
				int read = context<Compiler>().input_->get();

				if (std::isdigit(read)) {
					buffer += read;
					return discard_event();
				} else if (std::isspace(read)) {
					context<Compiler>().generator_->GenerateValue(buffer);
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
				int read = context<Compiler>().input_->get();

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
							context<Compiler>().generator_->GenerateLabelDef(buffer);
							return transit<NewLine>();
						case LabelRef:
							context<Compiler>().generator_->GenerateLabelRef(buffer);
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
				int read = context<Compiler>().input_->get();

				if (read == '"') {
					delimiter++;

					if (delimiter >= 2) {
						context<Compiler>().generator_->GenerateStringValue(buffer);
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
				int read = context<Compiler>().input_->get();
				if (read == '\n') {
					return transit<NewLine>();
				}

				return discard_event();
			}
		};

		struct EndOfFile : sc::simple_state<EndOfFile, Parsing> {
			typedef sc::transition<WriteByteCode, WriteByteCode> reactions;
		};

		/*
		 * WritingByteCode state
		 */

		struct WriteByteCode : sc::simple_state<WriteByteCode, Compiler> {
			typedef sc::custom_reaction<WriteByteCodeEvent> reactions;

			sc::result react(const WriteByteCodeEvent&) {
				return discard_event();
			}
		};
	}
}
#endif //LITTLEVIRTUALMACHINE_COMPILER_STATES_H
