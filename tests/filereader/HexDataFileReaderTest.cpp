#include <string>
#include <iostream>
#include <filereader/HexDataFileReader.hpp>
#include <unistd.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

const std::string baseDataPath = "tests/filereader/data/";

TEST(HexDataFileReaderTest, ReadsEndOfFile) {
    HexDataFileReader fileReader(baseDataPath + "onlyeof.hex");

    auto result = fileReader.read();
    EXPECT_EQ(result.size(), 0);
}

TEST(HexDataFileReaderTest, FailsForNonexistingFile) {

    HexDataFileReader fileReader(baseDataPath + "nonexisting");

    try {
        auto result = fileReader.read();
        FAIL();
    } catch (DataFileReaderException& ex) {
        SUCCEED();
    }

}


TEST(HexDataFileReaderTest, FailsOnDataOnlyNoEOF) {

    HexDataFileReader fileReader(baseDataPath + "onlysimpledata.hex");

    try {
        auto result = fileReader.read();
        FAIL();
    } catch (DataFileReaderException& ex) {
        SUCCEED();
    }
}

TEST(HexDataFileReaderTest, ReadsARecordFromFile) {
    HexDataFileReader fileReader(baseDataPath + "simledata_eof.hex");

    auto result = fileReader.read();
    EXPECT_EQ(result.size(), 1);
}


TEST(HexDataFileReaderTest, FailsWhenRecordDoesNotStartWithSemicoln) {
    HexDataFileReader fileReader(baseDataPath + "simledata_eof_nosemicol.hex");

    try {
        auto result = fileReader.read();
        FAIL();
    } catch (DataFileReaderException& ex) {
        SUCCEED();
    }
}

TEST(HexDataFileReaderTest, ReadsCorrectAddressFromFile) {
    unsigned int expectedAddress = 0x0100;
    HexDataFileReader fileReader(baseDataPath + "simledata_eof.hex");

    auto result = fileReader.read();
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].address, expectedAddress);
}


TEST(HexDataFileReaderTest, FailWhenIncorrectDataLength) {
    HexDataFileReader fileReader(baseDataPath + "simledata_eof_wronglength.hex");

    try {
        auto result = fileReader.read();
        FAIL();
    } catch (DataFileReaderException& ex) {
        SUCCEED();
    }
}

TEST(HexDataFileReaderTest, FailWhenUnsupportedDataLength) {
    HexDataFileReader fileReader(baseDataPath + "simledata_eof_unsupportedlength.hex");

    try {
        auto result = fileReader.read();
        FAIL();
    } catch (DataFileReaderException& ex) {
        SUCCEED();
    }
}

TEST(HexDataFileReaderTest, FailWhenUnsupportedRecordType) {
    HexDataFileReader fileReader(baseDataPath + "simledata_eof_wrongrecordtype.hex");

    try {
        auto result = fileReader.read();
        FAIL();
    } catch (DataFileReaderException& ex) {
        SUCCEED();
    }
}

TEST(HexDataFileReaderTest, FailWhenInvalidChecksum) {
    HexDataFileReader fileReader(baseDataPath + "simledata_eof_wrongchksum.hex");

    try {
        auto result = fileReader.read();
        FAIL();
    } catch (DataFileReaderException& ex) {
        SUCCEED();
    }
}


TEST(HexDataFileReaderTest, ReadsCorrectData) {
    HexDataFileReader fileReader(baseDataPath + "simledata_eof.hex");
    std::array<unsigned char, 16> expectedResult = {
            0x21,0x46,0x01,0x36,0x01,0x21,0x47,0x01,
            0x36,0x00,0x7E,0xFE,0x09,0xD2,0x19,0x01};

    auto result = fileReader.read();
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].data, expectedResult);
}


TEST(HexDataFileReaderTest, ReadsDataShorterRowIsFilledWithZeroes) {
    HexDataFileReader fileReader(baseDataPath + "simledata_eof_short.hex");
    std::array<unsigned char, 16> expectedResult = {
            0xC3, 0x5A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
            };

    auto result = fileReader.read();
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].data, expectedResult);
}
