//
// Created by josef on 25.10.18.
//

#include <iostream>
#include <fstream>
#include <core/hex_to_num.hpp>
#include "HexDataFileReader.hpp"

using std::string;
using std::ifstream;
using std::cout;
using std::vector;
using std::array;

const vector<DataFileItem> HexDataFileReader::read() {
    vector<DataFileItem> result;
    string line;
    ifstream hexfile(filename);
    bool foundEof = false;
    if (!hexfile.good()) {
        throw DataFileReaderException("File does not exist");
    }
    if (hexfile.is_open())
    {
        while ( getline (hexfile, line) )
        {
            if (isEof(line)) {
                foundEof = true;
                break;
            } else {
                result.push_back(processLine(line));
            }
            cout << line << '\n';
        }
        hexfile.close();
    } else {
        throw DataFileReaderException("Unable to open file");
    }

    if (!foundEof) {
        throw DataFileReaderException("Invalid HEX file - no EOF found");
    }

    return result;
}

bool HexDataFileReader::isEof(const string &line) {
    return line == ":00000001FF";
}

DataFileItem HexDataFileReader::processLine(const string &line) {
    const int dataLengthOffset = 1;
    const int addressOffset = 3;
    const int recordTypeOffset = 7;
    const int dataOffset = 9;
    const int maxDataLength = 0x10;
    unsigned int sum = 0;

    if (line[0] != ':') {
        throw DataFileReaderException("Invalid HEX file - no ':' on the beginning of line");
    }

    //data length
    unsigned int dataLength = decodeByte(dataLengthOffset, line);
    sum += dataLength;

    //address
    unsigned int address = decodeByte(addressOffset, line);
    unsigned int addressLsb = decodeByte(addressOffset + 2, line);
    sum += address + addressLsb;
    address = (address << 8) + addressLsb;

    //recordType
    unsigned int recordType = decodeByte(recordTypeOffset, line);
    sum += recordType;

    if (recordType != 0) {
        throw DataFileReaderException("Unsupported or invalid record type: " + std::to_string(recordType));
    }

    //data
    auto expectedDataLengthChars = line.size() - 1 /*:*/ - 2 /*data len*/ - 4 /*address*/ - 2/*record type*/ - 2 /*checksum*/;
    if (dataLength * 2 != expectedDataLengthChars) {
        throw DataFileReaderException("Invalid HEX file - wrong data length");
    }

    if (dataLength > maxDataLength) {
        throw DataFileReaderException("Data lengths over 16 (0x10) bytes are not yet implemented.");
    }

    //read data
    array<unsigned char, 16> readBytes;
    int offset = 0;
    for (unsigned int i = 0; i < dataLength; i++) {
        offset = dataOffset + i * 2;
        unsigned int readByte = decodeByte(offset, line);
        readBytes[i] = static_cast<unsigned char>(readByte);
        sum += readByte;
    }

    //compute checksum
    unsigned int computedCheckSum = (~(sum & 0xFF) + 1) & 0xFF;

    //read checksum
    offset += 2;
    unsigned int readChecksum = 0;
    for (int i = 0; i < 2; i++) {
        readChecksum = readChecksum << 4;
        readChecksum |= hexToNum(line[offset + i]);
    }

    if (computedCheckSum != readChecksum) {
        throw DataFileReaderException(
                "Invalid HEX file: invalid checksum, computed: "
                + std::to_string(computedCheckSum)
                + " read: " + std::to_string(readChecksum)
            );
    }

    return {address, readBytes};
}

unsigned int HexDataFileReader::decodeByte(const int offset, const string &line) const {
    unsigned int readByte = 0;
    for (int i = 0; i < 2; i++) {
        readByte = readByte << 4;
        readByte |= hexToNum(line[offset + i]);
    }
    return readByte;
}



