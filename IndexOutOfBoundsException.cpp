//
// Created by piotrek on 01.12.16.
//

#include "IndexOutOfBoundsException.h"


const char * IndexOutOfBoundsException::what() const throw()
{
    return std::runtime_error::what();
}