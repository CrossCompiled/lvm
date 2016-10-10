//
// Created by Kalle Møller on 10/10/2016.
//

#ifndef LITTLEVIRTUALMACHINE_LITTLESHARED_OPCODES_H
#define LITTLEVIRTUALMACHINE_LITTLESHARED_OPCODES_H

class BaseOpcode {
public:
    static const uint8_t code;
    static const std::string name;
};

class Add : BaseOpcode {
public:
    static const uint8_t code = 0x02;
    static const std::string name = "ADD";
};


#endif //LITTLEVIRTUALMACHINE_LITTLESHARED_OPCODES_H
