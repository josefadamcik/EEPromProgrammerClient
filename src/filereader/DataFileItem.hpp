//
// Created by josef on 25.10.18.
//

#include <array>

#ifndef EEPROMPROGCLIENT_DATAFILEITEM_HPP
#define EEPROMPROGCLIENT_DATAFILEITEM_HPP


/**
 * Simple container for address and 16 bytes of data.
 */
class DataFileItem {
public:
    DataFileItem(const unsigned int address, const std::array<unsigned char,16>& data)
        : address{address}, data{data} {}
    const unsigned int address;
    const std::array<unsigned char,16> data;
};


#endif //EEPROMPROGCLIENT_DATAFILEITEM_HPP
