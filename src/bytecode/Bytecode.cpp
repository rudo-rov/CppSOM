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
        int32_t blockSize = code->size();
        file.write(reinterpret_cast<char*>(&blockSize), sizeof blockSize);
        for (const auto& ins : *code) {
            ins->serialize(file);
        }
    }

    void BlockValue::serialize(std::ofstream& file)
    {
        Value::serialize(file);
        file.write(reinterpret_cast<char*>(&nargs), sizeof nargs);
        int32_t blockSize = code->size();
        file.write(reinterpret_cast<char*>(&blockSize), sizeof blockSize);
        for (const auto& ins : *code) {
            ins->serialize(file);
        }
    }

    void PrimitiveValue::serialize(std::ofstream& file)
    {
        Value::serialize(file);
        file.write(reinterpret_cast<char*>(&name), sizeof name);
        file.write(reinterpret_cast<char*>(&nargs), sizeof nargs);
    }
    
    void ClassValue::serialize(std::ofstream& file)
    {
        Value::serialize(file);
        file.write(reinterpret_cast<char*>(&identifier), sizeof identifier);
        file.write(reinterpret_cast<char*>(&superclass), sizeof superclass);
        int32_t slotsNr = slots.size();
        file.write(reinterpret_cast<char*>(&slotsNr), sizeof slotsNr);
        for (int32_t slot : slots) {
            file.write(reinterpret_cast<char*>(&slot), sizeof slot);
        }
    }

    void ArrayValue::serialize(std::ofstream& file)
    {
        Value::serialize(file);
        int32_t elemsNr = values->size();
        file.write(reinterpret_cast<char*>(&elemsNr), sizeof elemsNr);
        for (const auto& elem : *values) {
            elem->serialize(file);
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

    void BlockIns::serialize(std::ofstream& file)
    {
        ByteIns::serialize(file);
        file.write(reinterpret_cast<char*>(&idx), sizeof idx);
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
    
    void SendIns::serialize(std::ofstream& file)
    {
        ByteIns::serialize(file);
        file.write(reinterpret_cast<char*>(&methodIdx), sizeof methodIdx);
        file.write(reinterpret_cast<char*>(&arity), sizeof arity);
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

    void GetArgIns::serialize(std::ofstream& file)
    {
        ByteIns::serialize(file);
        file.write(reinterpret_cast<char*>(&idx), sizeof idx);
    }

    void LitIns::serialize(std::ofstream& file)
    {
        ByteIns::serialize(file);
        file.write(reinterpret_cast<char*>(&idx), sizeof idx);
    }

    void ReturnIns::serialize(std::ofstream& file)
    {
        ByteIns::serialize(file);
    }

    void ReturnNLIns::serialize(std::ofstream& file)
    {
        ByteIns::serialize(file);
        file.write(reinterpret_cast<char*>(&lvl), sizeof lvl);
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

    void ArrayValue::print()
    {
        std::cout << "ARRAY\n";
        for (const auto& elem : *values) {
            std::cout << "  ";
            elem->print(); 
        }
    }

    void MethodValue::print()
    {
        std::cout << "METHOD name:" << name << " nargs:" << nargs << " nlocals:" << nlocals << std::endl;
        for (const auto& ins : *(code)) {
            std::cout << "   ";
            ins->print();
        }
    }

    void BlockValue::print()
    {
        std::cout << "BLOCK nargs: " << nargs << std::endl;
        for (const auto& ins: *code) {
            std::cout << "   ";
            ins->print();
        } 
    }

    void PrimitiveValue::print()
    {
        std::cout << "PRIMITIVE name: " << name << " nargs: " << nargs << std::endl;
    }

    void ClassValue::print()
    {
        std::cout << "CLASS identifier: " << identifier << " slots: ";
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

    void BlockIns::print()
    {
        std::cout << "BLOCK " << idx << std::endl;
    }
    
    void SetSlotIns::print()
    {
        std::cout << "SET slot: " << slotIdx << std::endl;
    }

    void GetSlotIns::print()
    {
        std::cout << "GET slot: " << slotIdx << std::endl;
    }
    
    void SendIns::print()
    {
        std::cout << "SEND slot: " << methodIdx << " arity: " << arity << std::endl;
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

    void ReturnIns::print()
    {
        std::cout << "RET" << std::endl;
    }

    void GetArgIns::print()
    {
        std::cout << "GET arg: " << idx << std::endl;
    }

    void GetSelfIns::print()
    {
        std::cout << "GET SELF" << std::endl;
    }

    void ReturnNLIns::print()
    {
        std::cout << "RETNL " << lvl << std::endl;
    }

}