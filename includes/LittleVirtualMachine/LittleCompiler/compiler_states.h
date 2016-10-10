#ifndef LITTLEVIRTUALMACHINE_COMPILER_STATES_H
#define LITTLEVIRTUALMACHINE_COMPILER_STATES_H

#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/deep_history.hpp>
#include <boost/statechart/shallow_history.hpp>

namespace sc = boost::statechart;

/*
 * Events
 */

struct LoadFileEvent : sc::event<LoadFileEvent> {};
struct ProcessCharEvent : sc::event<ProcessCharEvent>{};
struct WriteByteCodeEvent : sc::event<WriteByteCodeEvent>{};

/*
 * State machine declaration
 */

struct Initial;
struct Parsing;
struct WriteByteCode;
struct Compiler : sc::state_machine<Compiler, Initial> {};

/*
 * Initial state
 */

struct Initial : sc::simple_state<Initial, Compiler> {};

/*
 * Parsing state
 */

struct Parsing : sc::simple_state<Parsing, Compiler> {};

/*
 * WritingByteCode state
 */

struct WriteByteCode : sc::simple_state<WriteByteCode, Compiler> {};

#endif //LITTLEVIRTUALMACHINE_COMPILER_STATES_H
