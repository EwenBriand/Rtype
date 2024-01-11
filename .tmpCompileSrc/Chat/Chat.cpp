/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Chat.cpp
*/

#include "Chat.hpp"
#include "Engine.hpp"
#include "NetworkExceptions.hpp"

MANAGED_RESOURCE(Chat)

void Chat::OnAddComponent(int entityID)
{
    _entity = entityID;
    eng::Engine::GetEngine()->SetGlobal<int>("chatEntity", entityID);
}


void Chat::sendMessage()
{
    eng::Engine::GetEngine()->GetClient().Send(serv::Instruction(serv::I_MESSAGE, 0, serv::bytes(_textField->GetText())));
}

void Chat::checkSizeOldText()
{
    // remove every \n if there is multiple \n in the sane line
    int j = 0;
    while (j < _text->GetText().size()){
        if (_text->GetText()[j] == '\n' && _text->GetText()[j + 1] == '\n') {
            // _text->UpdateText(_text->GetText().substr(0, j) + _text->GetText().substr(j + 1));
            // j--;
        }
        j++;
    }
    

    // count the number of \n in the text, if there is more than 3, delete the first line
    int count = 0;
    for (int i = 0; i < _text->GetText().size(); i++) {
        if (_text->GetText()[i] == '\n')
            count++;
    }
    std::cout << "Chat::count:" << count << std::endl;
    if (count >= 3) {
        int i = 0;
        // parse the string until the number of \n is equal to 3 and delete the first line
        while (i < _text->GetText().size() && count > 3) {
            if (_text->GetText()[i] == '\n')
                count--;
            i++;
        }
        // while (_text->GetText()[i] != '\n')
        //     i++;
        _text->UpdateText(_text->GetText().substr(i + 1));
    }

}

void Chat::Start()
{
    // _audioSource = &SYS.GetComponent<AudioSource>(_entity);
    // _audioSource->AddMusicName("Muse/gas_gas_gas.ogg");
    // _audioSource->Play<Music>("Muse/gas_gas_gas.ogg");
    // auto &transform = SYS.GetComponent<CoreTransform>(_entity);
    // transform.x = 150;
    // transform.y = 150;
    try {
        _text = &SYS.GetComponent<Text>(_entity);
        _text->SetPosition({ 1200, 700 });
        

        _inputField = &SYS.GetComponent<InputField>(_entity);


        _textField = &SYS.GetComponent<TextField>(_entity);
        _textField->SetSize({ 150, 50 });
        _textField->RegisterOnEnterCallback([this]() {
            // std::cout << "Chat::ENTER PRESSED" << std::endl;
            if (eng::Engine::GetEngine()->IsClient()) {
                // std::cout << "Chat::SENDING MESSAGE TO SERVER" << std::endl;
                // sendMessage();
                _textBuffer += _inputField->GetText();
                std::cout << "Chat::_textBuffer:" << _textBuffer << std::endl;
                std::cout << "chat InputField::text:" << _inputField->GetText() << std::endl;
                // _text->UpdateText("Chat: \n" + _textBuffer + "\n");
                if (_textBuffer != "") {
                    _text->UpdateText(_textBuffer + "\n");
                    std::cout << "Chat::MESSAGE SENT:" << _text->GetText() << std::endl;
                    auto &client = eng::Engine::GetEngine()->GetClient();
                    client.Send(serv::Instruction(serv::I_MESSAGE, 0, serv::bytes(_text->GetText())));
                }
                _inputField->SetText("");
                _textField->SetText("");
                _textBuffer = "";
                checkSizeOldText();
                // get client from engine and send message

            }
        });
        std::cout << "on enter callback is set" << std::endl;
    } catch (const std::exception &ex) {
        std::cout << "\rcouldnt start chat cpt: " << ex.what() << std::endl;
    }


    // TextField for chat

    // div.SetParentID(_entity);
    // div.SetPosition({ 1200, 700 });
    // div.SetSize({ 300, 100 });
    // div.SetColor({ 33, 33, 33, 255 });
    // _inputField = &SYS.GetComponent<InputField>(_entity);
    // text = std::make_shared<InputField>();
    // _inputField.SetParent(div);
    // _inputField->SetPosition({ 1200, 730 });
    // _inputField->SetSize({ 150, 50 });
    // _inputField.SetText("Type here ...");
    
    // eng::Engine::GetEngine()->SetGlobal<int>("killCount", 0);
}

void Chat::Update(int e)
{
    // int killcount = eng::Engine::GetEngine()->GetGlobal<int>("killCount");
    // _textField->SetText("Kill Count: " + std::to_string(killcount));
    // std::cout << 


    // std::cout << "Chat::Update" << std::endl;
    // draw the chat


}

void Chat::AddMessage(std::string message)
{
    // std::cout << "Chat::AddMessage: message is : [" << message << "]" << std::endl;
    // _textBuffer += message;
    // std::cout << "Chat::_textBuffer:" << _textBuffer << std::endl;
    // std::cout << "chat InputField::text:" << _inputField->GetText() << std::endl;
    // _text->UpdateText("Chat: \n" + _textBuffer + "\n");

    // convert message to string
    // bytes::ToBytes toBytes;
    // std::string str = toBytes.toString(message);
    serv::bytes bytes = message;
    std::string str = bytes.toString();
    std::cout << "Chat::AddMessage: str is : [" << str << "]" << std::endl;
    if (str != "" && str != "\n")
        _text->UpdateText(str);
    std::cout << "Chat::RECIEVED: " << _text->GetText() << std::endl;
    // _inputField->SetText("");
    // _textField->SetText("");
    // _textBuffer = "";
    checkSizeOldText();
}