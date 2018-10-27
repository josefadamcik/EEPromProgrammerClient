//
// Created by josef on 25.10.18.
//

#include <vector>
#include "DataFileItem.hpp"

#ifndef EEPROMPROGCLIENT_DATAFILEREADER_HPP
#define EEPROMPROGCLIENT_DATAFILEREADER_HPP


class DataFileReaderException: public std::logic_error {
public:
    explicit DataFileReaderException(const std::string& msg): std::logic_error{msg} {};
};

/**
 * Reads file and provides list of DataFileItems (address + 16bytes of data)
 */
class DataFileReader {
public:
    virtual ~DataFileReader() = default;

    /**
     * Read the datafile.
     * @return
     */
    virtual
    const std::vector<DataFileItem>
    read() = 0;
};


#endif //EEPROMPROGCLIENT_DATAFILEREADER_HPP
