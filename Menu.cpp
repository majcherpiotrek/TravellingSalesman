//
// Created by piotrek on 01.12.16.
//

#include "Menu.h"



MenuElement * Menu::search(int index)
{
    //Jeśli index wykracza poza listę to zwraca wskaźnik NULL.
    if (index < 0 || index >= menu_size)
        return nullptr;

    MenuElement* element;
    /*Jeśli szukany indeks w pierwszej połowie listy
    to zaczynamy szukanie od wskaźnika na głowę listy.*/
    if (index <= menu_size / 2) {

        element = head;
        for (int i = 0; i != index; i++)
            element = element->next;

    }
        /*W przeciwnym wypadku zaczynamy szukanie od ostatniego elementu listy.*/
    else {
        element = tail->previous;
        for (int i = menu_size - 1; i != index; i--)
            element = element->previous;
    }

    return element;
}

Menu::Menu()
{
    head = nullptr;
    tail = nullptr;
    menu_size = 0;
}


Menu::~Menu()
{
    delete head;
    delete tail;
}

void Menu::add(const char* optionHeader)
{
    if (menu_size == 0) {
        // Tworzę nową "głowę" listy o wartości _value, oba wskaźniki ustawione na NULL
        head = new MenuElement(nullptr, optionHeader, nullptr);

        // Tworzę nowy "ogon" listy, czyli wskaźnik na pierwsze wolne miejsce na końcu listy.
        // Lewy wskaźnik pokazuje póki co na "głowę", ponieważ jest tylko jeden element w liście,
        // później będzie pokazywał zawsze na ostatni element listy. Prawy wsk na NULL.
        tail = new MenuElement(head, "", nullptr);

        // Teraz przypisuję do prawego wsk "głowy" (czyli teraz ostatniego elementu tablicy) adres "ogona".
        head->next = tail;

        menu_size++;
    }
    else {
        // Do "ogona" listy przypisuję wartość nowego elementu.
        tail->name = optionHeader;
        // Do prawego wskaźnika przypisuję adres nowego elementu, który będzie teraz ogonem.
        tail->next = new MenuElement(tail, "", nullptr);
        // Do wskaźnika na ogon listy przypisuję adres elementu utworzonego linię wyżej.
        tail = tail->next;

        menu_size++;
    }

}

std::string Menu::get(int index)
{
    MenuElement* element = search(index);
    if (element == nullptr)
        throw IndexOutOfBoundsException("\nNie ma takiego indeksu w menu!");
    else
        return element->name;
}

std::ostream & operator<<(std::ostream & output, Menu & menu)
{
    for (int i = 0;i < menu.menu_size;i++)
        output << menu.get(i) << std::endl;
    return output;
}