#include <iostream>
#include <fstream>
#include "Bytecode.h"

namespace som {

    // Bytecode serialization
    void Value::serialize(std::ofstream& file)
    {
        file.write(reinterpret_cast<char*>(&tag), sizeof tag);
    }

    void IntValue::serialize(std::ofstream& file)
    {
        Value::serialize(file);
        file.write(reinterpret_cast<char*>(&value), sizeof value);
    }

    void DoubleValue::serialize(std::ofstream& file)
    {
        Value::serialize(file);
        file.write(reinterpret_cast<char*>(&value), sizeof value);
    }

    void StringValue::serialize(std::ofstream& file)
    {
        Value::serialize(file);
        int32_t size = value.size();
        file.write(reinterpret_cast<char*>(&size), sizeof size);
        file.write(value.c_str(), value.size());
    }

    void MethodValue::serialize(std::ofstream& file)
    {
        Value::serialize(file);
        file.write(reinterpret_cast<char*>(&name), sizeof name);
        file.write(reinterpret_cast<char*>(&nargs), sizeof nargs);
        file.write(reinterpret_cast<char*>(&nlocals), sizeof nlocals);
        for (const auto& ins : *code) {
            // ins->serialize(file);
        }
    }
    
    void ClassValue::serialize(std::ofstream& file)
    {
        Value::serialize(file);
        for (int32_t slot : slots) {
            file.write(reinterpret_cast<char*>(&slot), sizeof slot);
        }
    }

    /* INSTRUCTIONS SERIALIZATION */
    
    void ByteIns::serialize(std::ofstream& file)
    {
        file.write(reinterpret_cast<char*>(&op), sizeof op);
    }

    void SlotIns::serialize(std::ofstream& file)
    {
        ByteIns::serialize(file);
        file.write(reinterpret_cast<char*>(&slotIdx), sizeof slotIdx);
    }

    void SetSlotIns::serialize(std::ofstream& file)
    {
        ByteIns::serialize(file);
        file.write(reinterpret_cast<char*>(&slotIdx), sizeof slotIdx);
    }

    void GetSlotIns::serialize(std::ofstream& file)
    {
        ByteIns::serialize(file);
        file.write(reinterpret_cast<char*>(&slotIdx), sizeof slotIdx);
    }
    
    void CallSlotIns::serialize(std::ofstream& file)
    {
        ByteIns::serialize(file);
        file.write(reinterpret_cast<char*>(&methodIdx), sizeof methodIdx);
    }

    void SetLocalIns::serialize(std::ofstream& file)
    {
        ByteIns::serialize(file);
        file.write(reinterpret_cast<char*>(&idx), sizeof idx);
    }

    void GetLocalIns::serialize(std::ofstream& file)
    {
        ByteIns::serialize(file);
        file.write(reinterpret_cast<char*>(&idx), sizeof idx);
    }

    void LitIns::serialize(std::ofstream& file)
    {
        ByteIns::serialize(file);
        file.write(reinterpret_cast<char*>(&idx), sizeof idx);
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
        for (const auto& ins : *(code)) {
            std::cout << "   ";
            ins->print();
        }
    }

    void ClassValue::print()
    {
        std::cout << "CLASS slots: ";
        for (auto i = 0; i < slots.size(); ++i) {
            std::cout << slots.at(i) << (i + 1 == slots.size() ? "\n" : ", ");
        }
    }

    void ByteIns::print()
    {
        std::cout << "NOP" << std::endl;
    }

    void SlotIns::print()
    {
        std::cout << "GET slot: " << slotIdx << std::endl;
    }
    
    void SetSlotIns::print()
    {
        std::cout << "SET slot: " << slotIdx << std::endl;
    }

    void GetSlotIns::print()
    {
        std::cout << "GET slot: " << slotIdx << std::endl;
    }
    
    void CallSlotIns::print()
    {
        std::cout << "CALL slot: " << methodIdx << " arity: " << arity << std::endl;
    }

    void SetLocalIns::print()
    {
        std::cout << "SET local: " << idx << std::endl;
    }

    void GetLocalIns::print()
    {
        std::cout << "GET local: " << idx << std::endl;
    }

    void LitIns::print()
    {
        std::cout << "LIT " << idx << std::endl;
    }

}