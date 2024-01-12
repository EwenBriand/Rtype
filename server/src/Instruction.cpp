/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Instruction.cpp
*/

#include "Message.hpp"
#include "ServerUdp.hpp"
#include "AsioClone.hpp"

namespace serv {
    Instruction::Instruction()
        : opcode(0)
        , expectsAnswer(0)
    {
    }

    Instruction::Instruction(uint32_t opcode, uint8_t expectsAnswer, bytes data)
        : opcode(opcode)
        , expectsAnswer(expectsAnswer)
        , data(data)
    {
    }

    Instruction::Instruction(const bytes& bytes)
    {
        if (bytes.size() < 5)
            throw MalformedInstructionException("\rInstruction is too small");
        std::vector<unsigned char> byteData = bytes._data;
        opcode = *(uint32_t*)byteData.data();
        expectsAnswer = *(uint8_t*)(byteData.data() + 4);
        data = std::vector<unsigned char>(byteData.begin() + 5, byteData.end());
    }

    bytes Instruction::ToBytes() const
    {
        std::vector<unsigned char> bytes;
        bytes.insert(bytes.end(), (unsigned char*)&opcode, (unsigned char*)&opcode + sizeof(opcode));
        bytes.insert(bytes.end(), (unsigned char*)&expectsAnswer, (unsigned char*)&expectsAnswer + sizeof(expectsAnswer));
        bytes.insert(bytes.end(), data._data.begin(), data._data.end());
        return bytes;
    }

    Message Instruction::ToMessage(EndpointWrapper endpointW) const
    {
        Message message(ToBytes() + SEPARATOR, std::make_unique<EndpointWrapper>(endpointW));
        return message;
    }

    Instruction Instruction::FromMessage(const Message& message)
    {
        return Instruction(message.data);
    }
}
