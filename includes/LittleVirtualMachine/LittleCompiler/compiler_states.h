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
		};

		/*
		 * State machine declaration
		 */

		struct Initial;
		struct Parsing;
		struct WriteByteCode;
		struct Failed;

		struct Compiler : sc::state_machine<Compiler, Initial> {
			std::istream* input;
			std::vector<std::string> instructions;
		};

		/*
		 * Initial state
		 */

		struct Initial : sc::simple_state<Initial, Compiler> {
			typedef boost::mpl::list<sc::custom_reaction<LoadFileEvent>, sc::custom_reaction<sc::exception_thrown>> reactions;

			sc::result react(const LoadFileEvent& ev) {
				if (!ev.get_stream()->good()) { throw std::exception(); }

				context<Compiler>().input = ev.get_stream();

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
			Failed() {}
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
					return transit<Initial>();
				}
				catch (...) {
					return forward_event();
				}
			}
		};

		struct NewLine : sc::simple_state<NewLine, Parsing> {
			typedef sc::custom_reaction<ProcessCharEvent> reactions;

			sc::result react(const ProcessCharEvent&) {
				int first = context<Compiler>().input->peek();

				if (std::isalpha(first)) {
					context<Compiler>().instructions.push_back("");
					return transit<Letter>();
				}

				if (std::isdigit(first)) {
					context<Compiler>().instructions.push_back("");
					return transit<Number>();
				}

				if (first == ':' || first == '@') { return transit<Label>(); }
				if (first == ';') { return transit<Comment>(); }
				if (first == '"') {return transit<String>(); }
				if (first == EOF) { return transit<EndOfFile>(); }

				throw std::exception();
			}
		};

		struct Letter : sc::simple_state<Letter, Parsing> {
			typedef sc::custom_reaction<ProcessCharEvent> reactions;

			sc::result react(const ProcessCharEvent&) {
				int read = context<Compiler>().input->get();

				if (std::isalpha(read)) {
					context<Compiler>().instructions.back().append(1, read);
					return discard_event();
				} else if (std::isspace(read)) {
					return transit<NewLine>();
				}

				return discard_event();
			}
		};

		struct Number : sc::simple_state<Number, Parsing> {
			typedef sc::custom_reaction<ProcessCharEvent> reactions;

			sc::result react(const ProcessCharEvent&) {
				int read = context<Compiler>().input->get();

				if (std::isdigit(read)) {
					context<Compiler>().instructions.back().append(1, read);
					return discard_event();
				} else if (std::isspace(read)) {
					return transit<NewLine>();
				}

				return discard_event();
			}
		};

		struct Label : sc::simple_state<Label, Parsing> {
			typedef sc::custom_reaction<ProcessCharEvent> reactions;

			sc::result react(const ProcessCharEvent&) {
				return discard_event();
			}
		};

		struct String : sc::simple_state<String, Parsing> {
			typedef sc::custom_reaction<ProcessCharEvent> reactions;
			sc::result react(const ProcessCharEvent&) {
				int read = context<Compiler>().input->get();
				if (read == '\n') {
					return transit<NewLine>();
				}

				context<Compiler>().instructions.back().append(1, read);
				return discard_event();
			}
		};

		struct EndOfFile : sc::simple_state<EndOfFile, Parsing> {
			typedef sc::transition<WriteByteCode, WriteByteCode> reactions;

			EndOfFile() {}
		};

		struct Comment : sc::simple_state<Comment, Parsing> {
			typedef sc::custom_reaction<ProcessCharEvent> reactions;

			sc::result react(const ProcessCharEvent&) {
				int read = context<Compiler>().input->get();
				if (read == '\n') {
					return transit<NewLine>();
				}

				return discard_event();
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
