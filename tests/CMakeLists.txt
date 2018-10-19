add_executable(simpletest simpletest.cpp)
add_test(simple simpletest)

set_property(
    TEST simple
    PROPERTY PASS_REGULAR_EXPRESSION "^Test!"
)