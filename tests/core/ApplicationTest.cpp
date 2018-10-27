//
// Created by josef on 24.10.18.
//

#include <core/Application.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "controller/EEPromProgControllerSerial.hpp"
#include "controller/testing/EEPromProgControllerMock.hpp"

using ::testing::AtLeast;
using ::testing::AnyNumber;
using ::testing::Exactly;
using ::testing::Return;
using std::move;
using std::make_unique;
const std::string baseDataPath = "tests/core/data/";



TEST(ApplicationTest, ApplicationWritesWholedHexFile) {
    Application application;
    std::unique_ptr<EEPromProgControllerMock> mockController = make_unique<EEPromProgControllerMock>();
    auto rawMockPtr = mockController.get(); //a bit hacky, but we need to access mocked instance.

    application.eePromProgController = move(mockController);

    EXPECT_CALL(*rawMockPtr, isConnected)
            .Times(AnyNumber())
            .WillRepeatedly(Return(true))
            ;

    EXPECT_CALL(*rawMockPtr, sendCmdWrite)
        .Times(Exactly(3))
        ;

    application.programFromHexfile(baseDataPath + "sampledata.hex");


}

