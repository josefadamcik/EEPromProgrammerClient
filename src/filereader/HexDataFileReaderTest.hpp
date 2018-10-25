//
// Created by josef on 25.10.18.
//

#ifndef EEPROMPROGCLIENT_HEXDATAFILEREADER_HPP
#define EEPROMPROGCLIENT_HEXDATAFILEREADER_HPP

#include <string>
#include "DataFileReader.hpp"


/**
 * Reads file in Intel HEX format ( https://en.wikipedia.org/wiki/Intel_HEX )
 * Partial implementation:
 * - max data length is 0x10
 * - only data end EOF record types
 */
class HexDataFileReader: public DataFileReader {
public:
    explicit HexDataFileReader(const std::string& filename): filename{filename} {};

    const std::vector<DataFileItem> read() override;

private:
    const std::string filename;
};


#endif //EEPROMPROGCLIENT_HEXDATAFILEREADER_HPP
