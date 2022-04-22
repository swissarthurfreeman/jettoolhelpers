#ifndef JETTOOLHELPERS_TESTS__
#define JETTOOLHELPERS_TESTS__

#include <iostream>
#include <stdexcept>
#include <string>

/*
__FILE__ is a magic constant containing the name of current file !
__LINE__ is the current line
__PRETTY_FUNCTION__ is the current function
*/
namespace Test {
    #define TEST_ASSERT_EQUAL( x, y )                                   \
    {                                                                   \
    if( ( x ) != ( y ) )                                                \
    {                                                                   \
        throw std::runtime_error(   std::string( __FILE__ )             \
                                + std::string( ":" )                    \
                                + std::to_string( __LINE__ )            \
                                + std::string( " in " )                 \
                                + std::string( __PRETTY_FUNCTION__ )    \
                                + std::string( ": " )                   \
                                + std::to_string( ( x ) )               \
                                + std::string( " != " )                 \
                                + std::to_string( ( y ) )               \
        );                                                              \
    }                                                                   \
    }

    #define TEST_ASSERT_THROW( condition )                              \
    {                                                                   \
    if( !( condition ) )                                                \
    {                                                                   \
        throw std::runtime_error(std::string("Condition not satisfied : ") + std::string(#condition) +std::string("\n")\
                                + std::string("In file : ") + std::string( __FILE__ )   \
                                + std::string("\nAt line : ") + std::to_string( __LINE__ ) \
                                + std::string("\nIn Function : ") + std::string( __PRETTY_FUNCTION__ )    \
        );                                                              \
    }                                                                   \
    }

    #define EXPECT_EXCEPTION( expression, exception )                   \
    {                                                                   \
    try                                                                 \
    {                                                                   \
        ( expression );                                                 \
    }                                                                   \
    catch( exception& e )                                               \
    {                                                                   \
    }                                                                   \
    catch( ... )                                                        \
    {                                                                   \
        throw std::runtime_error(   std::string( __FILE__ )             \
                                + std::string( ":" )                    \
                                + std::to_string( __LINE__ )            \
                                + std::string( " in " )                 \
                                + std::string( __PRETTY_FUNCTION__ )    \
        );                                                              \
    }                                                                   \
    }

    #define TEST_BEGIN( name )\
    {\
    std::cerr << "-- Running test \"" << name << "\"...";\
    }

    #define TEST_END() \
    {\
    std::cerr << "finished\n";\
    }
}

#endif