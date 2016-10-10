#ifndef LITTLEVIRTUALMACHINE_COMPILER_STATES_H
#define LITTLEVIRTUALMACHINE_COMPILER_STATES_H

#include <string>
#include <exception>
#include <fstream>
#include <vector>

#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/deep_history.hpp>
#include <boost/statechart/shallow_history.hpp>
#include <boost/statechart/exception_translator.hpp>

namespace sc = boost::statechart;

/*
 * Events
 */

namespace lvm {
	namespace compiler {
		struct LoadFileEvent : sc::event<LoadFileEvent> {
			LoadFileEvent(std::string path) : path_(path) {}

			std::string get_path() const { return path_; }

		private:
			std::string path_;
		};

		struct ProcessCharEvent : sc::event<ProcessCharEvent> {
		};

		struct WriteByteCodeEvent : sc::event<WriteByteCodeEvent> {
		};

		/*
		 * State machine declaration
		 */

		struct Initial;
		struct Parsing;
		struct WriteByteCode;
		struct Failed;

		struct Compiler : sc::state_machine<Compiler, Initial> {
			std::fstream fileStream;
			std::vector<std::string> instructions;
		};

		/*
		 * Initial state
		 */

		struct Initial : sc::simple_state<Initial, Compiler> {
			typedef boost::mpl::list<sc::custom_reaction<LoadFileEvent>, sc::custom_reaction<sc::exception_thrown>> reactions;

			sc::result react(const LoadFileEvent& ev) {
				std::string path = ev.get_path();

				if (path.empty()) { throw std::exception(); }

				context<Compiler>().fileStream.open(path.c_str(), std::ios_base::in);
				if (!context<Compiler>().fileStream.good()) { throw std::exception(); }

				return transit<Parsing>();
			}

			sc::result react(const sc::exception_thrown&) {
				try {
					throw;
				}
				catch (const std::exception&) {
					transit<Failed>();
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
			Failed() {}

			~Failed() {}
		};

		/*
		 * Parsing state and substates
		 */

		struct NewLine;
		struct Alphanumeric;
		struct Number;
		struct Label;
		struct EndOfFile;
		struct Comment;

		struct Parsing : sc::simple_state<Parsing, Compiler, NewLine> {
			typedef sc::custom_reaction<sc::exception_thrown> reactions;

			sc::result react(const sc::exception_thrown&) {
				try {
					throw;
				}
				catch (const std::exception&) {
					transit<Initial>();
				}
				catch (...) {
					return forward_event();
				}
			}
		};

		struct NewLine : sc::simple_state<NewLine, Parsing> {
			typedef sc::custom_reaction<ProcessCharEvent> reactions;

			sc::result react(const ProcessCharEvent&) {
				int first = context<Compiler>().fileStream.peek();

				if (std::isalnum(first)) {
					context<Compiler>().instructions.push_back("");
					return transit<Alphanumeric>();
				} else if (std::isdigit(first)) {
					context<Compiler>().instructions.push_back("");
					return transit<Number>();
				} else if (first == ':' || first == '@') {
					return transit<Label>();
				} else if (first == EOF) {
					return transit<EndOfFile>();
				} else if (first == ';') {
					return transit<Comment>();
				} else {
					throw std::exception();
				}
			}
		};

		struct Alphanumeric : sc::simple_state<Alphanumeric, Parsing> {
			typedef sc::custom_reaction<ProcessCharEvent> reactions;

			sc::result react(const ProcessCharEvent&) {
				int read = context<Compiler>().fileStream.get();

				if (std::isalnum(read)) {
					context<Compiler>().instructions.back().append(1, read);
				} else if (std::isspace(read)) {
					// Check OPCode
				} else if (read == '\n') {
					transit<NewLine>();
				}
			}
		};

		struct Number : sc::simple_state<Number, Parsing> {
			typedef sc::custom_reaction<ProcessCharEvent> reactions;

			sc::result react(const ProcessCharEvent&) {
				int read = context<Compiler>().fileStream.get();

				if (std::isdigit(read)) {
					context<Compiler>().instructions.back().append(1, read);
				} else if (std::isspace(read)) {
					// Check OPCode
				} else if (read == '\n') {
					transit<NewLine>();
				}
			}
		};

		struct Label : sc::simple_state<Label, Parsing> {
			typedef sc::custom_reaction<ProcessCharEvent> reactions;

			sc::result react(const ProcessCharEvent&) {

			}
		};

		struct EndOfFile : sc::simple_state<EndOfFile, Parsing> {
			EndOfFile() {
				context<Compiler>().fileStream.close();
			}
		};

		struct Comment : sc::simple_state<Comment, Parsing> {
			typedef sc::custom_reaction<ProcessCharEvent> reactions;

			sc::result react(const ProcessCharEvent&) {
				int read = context<Compiler>().fileStream.get();
				if (read == '\n') {
					transit<NewLine>();
				}
			}
		};

		/*
		 * WritingByteCode state
		 */

		struct WriteByteCode : sc::simple_state<WriteByteCode, Compiler> {
		};
	}
}
#endif //LITTLEVIRTUALMACHINE_COMPILER_STATES_H
