#include <iostream>
#include <ostream>
#include "Bytecode.h"

namespace som {

    // Bytecode serialization
    void Value::serialize(std::ostream& file)
    {
        file.write(reinterpret_cast<char*>(&tag), sizeof tag);
    }

    void IntValue::serialize(std::ostream& file)
    {
        file.write(reinterpret_cast<char*>(&tag), sizeof tag);
        file.write(reinterpret_cast<char*>(&value), sizeof value);
    }

    void DoubleValue::serialize(std::ostream& file)
    {
        file.write(reinterpret_cast<char*>(&tag), sizeof tag);
        file.write(reinterpret_cast<char*>(&value), sizeof value);
    }

    void StringValue::serialize(std::ostream& file)
    {
        file.write(reinterpret_cast<char*>(&tag), sizeof tag);
        int32_t size = value.size();
        file.write(reinterpret_cast<char*>(&size), sizeof size);
        file.write(value.c_str(), value.size());
    }

    void MethodValue::serialize(std::ostream& file)
    {
        file.write(reinterpret_cast<char*>(&tag), sizeof tag);
        file.write(reinterpret_cast<char*>(&name), sizeof name);
        file.write(reinterpret_cast<char*>(&nargs), sizeof nargs);
        file.write(reinterpret_cast<char*>(&nlocals), sizeof nlocals);
        for (const auto& ins : code) {
            // ins->serialize(file);
        }
    }

    void ClassValue::serialize(std::ostream& file)
    {
        file.write(reinterpret_cast<char*>(&tag), sizeof tag);
        for (int32_t slot : slots) {
            file.write(reinterpret_cast<char*>(&slot), sizeof slot);
        }
    }

    // Bytecode printing
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