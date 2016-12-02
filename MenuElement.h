//
// Created by piotrek on 01.12.16.
//

#ifndef TRAVELLINGSALESMANPROBLEMALGORITHMS_MENUELEMENT_H
#define TRAVELLINGSALESMANPROBLEMALGORITHMS_MENUELEMENT_H


#pragma once
#include <string>
class MenuElement
{
    friend class Menu;
    MenuElement* previous;
    MenuElement* next;
    std::string name;

public:
    MenuElement(MenuElement* prev, const char* _name, MenuElement* nxt);
    ~MenuElement();
};

#endif //TRAVELLINGSALESMANPROBLEMALGORITHMS_MENUELEMENT_H
