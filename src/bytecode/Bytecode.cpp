#include <iostream>
#include "Bytecode.h"

namespace som {

    void IntValue::print()
    {
        std::cout << "INT " << value << std::endl;
    }

    void DoubleValue::print()
    {
        std::cout << "DOUBLE " << value << std::endl;
    }

    void StringValue::print()
    {
        std::cout << "STR '" << value << "'" << std::endl; 
    }

    void MethodValue::print()
    {
        std::cout << "METHOD name:" << name << " nargs:" << nargs << " nlocals:" << nlocals << std::endl;  
    }

    void ClassValue::print()
    {
        std::cout << "CLASS slots: ";
        for (auto i = 0; i < slots.size(); ++i) {
            std::cout << slots.at(i) << (i + 1 == slots.size() ? "\n" : ", ");
        }
    }

}