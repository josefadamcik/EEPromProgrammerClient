#include <utility>
#include <string>
#include "DataFileReader.hpp"
//
// Created by josef on 25.10.18.
//

#ifndef EEPROMPROGCLIENT_HEXDATAFILEREADER_HPP
#define EEPROMPROGCLIENT_HEXDATAFILEREADER_HPP




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

    ~HexDataFileReader() override = default;
private:
    const std::string filename;

    bool isEof(const std::string &line);

    DataFileItem processLine(const std::string &line);

    unsigned int decodeByte(const int offset, const std::string &line) const;
};


#endif //EEPROMPROGCLIENT_HEXDATAFILEREADER_HPP
