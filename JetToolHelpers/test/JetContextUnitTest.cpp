/**
 * @file JetContextUnitTest.cpp
 * @author A. Freeman (swissarthur@gmail.com)
 * @brief JetContext is a class that contains additional runtime
 * configured properties which are not a part of Jet. They 
 * are specified with setValue() and have to be a supported type. 
 * @version 0.1
 * @date 2022-04-23
 * 
 * @copyright Copyright (c) 2022
 */

/**
 * 
 * To do : 
 * STEVEN : With variant code any unsupported type results in a 
 * compile error, do we want to return an exception instead ?
 * What happens when we pass a nullptr to a reference ?  
 * What we have to test is :  => results in compilation error.
 * - Inserting unsupported types
 * - Inserting supported types
 */

#include "JetToolHelpers/JetContext.h"
#include "test/Test.h"

int main() {
    TEST_BEGIN("JetContext Unit Test");
    JetContext jc;
    
    ASSERT_EQUAL(jc.setValue("machuPichu", 23.04f), true);
    ASSERT_EQUAL(jc.isAvailable("machuPichu"), true);
    ASSERT_EQUAL(jc.isAvailable("randomKey"), false);
    
    ASSERT_EQUAL(jc.getValue<float>("machuPichu"), 23.04f);

    // no overwrite allowed, we expect nothing to be set.
    ASSERT_EQUAL(jc.setValue("machuPichu", 23), false);

    // overwrite allowed, we expect value to be set.
    ASSERT_EQUAL(jc.setValue("machuPichu", 23, true), true);
    ASSERT_EQUAL(jc.getValue<int>("machuPichu"), 23);

    ASSERT_EQUAL(jc.setValue("atchoum", 10), true);
    ASSERT_EQUAL(jc.getValue<int>("atchoum"), 10);

    // empty string names are not allowed. (user might want to name inputVarJetContext)
    // with same name as a jc property, and emptystring isn't allowed.
    ASSERT_EQUAL(jc.setValue("", 10), false);    
    
    // here "atchoum" is an int variable, asking another supported type should cause an error.
    // an unsupported type (e.g. not part of the types of unordered_map) won't even compile.
    float value{0};
    EXPECT_EXCEPTION(jc.getValue("atchoum", value), std::bad_variant_access);        
    EXPECT_EXCEPTION(jc.getValue<float>("randomKey"), std::invalid_argument);
    
    EXPECT_EXCEPTION(jc.getValue("randomKey", value), std::invalid_argument);
    EXPECT_EXCEPTION(jc.getValue("", value), std::invalid_argument);

    EXPECT_EXCEPTION(jc.setValue("double!", 12.25), std::invalid_argument);

    TEST_END("JetContext Unit Test");
}