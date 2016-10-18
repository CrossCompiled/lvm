//
// Created by kaspersk on 10/10/16.
//

#ifndef LITTLEVIRTUALMACHINE_SUPPORTEDTYPES_H
#define LITTLEVIRTUALMACHINE_SUPPORTEDTYPES_H

#include <LittleVirtualMachine/LittleShared/opcodes.h>
#include <LittleVirtualMachine/LittleShared/TypeList.h>
#include <map>

namespace lvm {
    namespace shared {


        typedef MakeTypeList<opcodes::In, opcodes::Push, opcodes::Div, opcodes::Inc, opcodes::Dec, opcodes::Store, opcodes::Load, opcodes::JumpNotEqual, opcodes::Halt, opcodes::Out, opcodes::Add, opcodes::Sub, opcodes::Mul, opcodes::Duplicate, opcodes::JumpEqual, opcodes::Jump, opcodes::Pop, opcodes::Swap, opcodes::Mod, opcodes::CopyOver>::List SupportedOpcodes;

        template <typename TL>
        struct MakeOpcodeMap{

            static void run(std::map<std::string, const int> & innerMap){
                innerMap.insert(std::pair<std::string, const int32_t>(std::string(TL::First::name), (int32_t)TL::First::template code<int32_t>));
                MakeOpcodeMap<typename TL::Left>::run(innerMap);
            }
        };

        template <>
        struct MakeOpcodeMap<NullType>{

            static void run(std::map<std::string, const int> innerMap)
            {
            }
        };
    }
}



#endif //LITTLEVIRTUALMACHINE_SUPPORTEDTYPES_H
