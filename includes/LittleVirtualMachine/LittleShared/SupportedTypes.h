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

        typedef MakeTypeList<In, Out, Add, Sub, Mul, Duplicate, JumpEqual, Jump, Pop, Swap, Mod, CopyOver>::List SupportedOpcodes;

        template <typename TL>
        struct MakeOpcodeMap{

            static void run(std::map<std::string, const int> & innerMap){
                innerMap.insert(std::pair<std::string, const int>(std::string(TL::First::name), TL::First::code));
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
