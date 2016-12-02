//
// Created by piotrek on 01.12.16.
//

#ifndef TRAVELLINGSALESMANPROBLEMALGORITHMS_INDEXOUTOFBOUNDSEXCEPTION_H
#define TRAVELLINGSALESMANPROBLEMALGORITHMS_INDEXOUTOFBOUNDSEXCEPTION_H

#pragma once
#include<iostream>
#include <exception>
#include <typeinfo>
/*Klasa wyjątku.
Wyj¹tek ten jest wyrzucany, jeœli szukany indeks jest poza rozmiarem struktury.*/
class IndexOutOfBoundsException : std::runtime_error
{
public:
    IndexOutOfBoundsException(const char* msg) : std::runtime_error(msg) {

    };
    ~IndexOutOfBoundsException() {};

    virtual const char* what() const throw();
};

#endif //TRAVELLINGSALESMANPROBLEMALGORITHMS_INDEXOUTOFBOUNDSEXCEPTION_H
