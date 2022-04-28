#include "JetToolHelpers/InputVariable.h"
#include "JetToolHelpers/Jet.h"
#include "test/Test.h"

/**
 * @brief Tests specification of InputVariable.
 * STEVEN : We should refactor to throw invalid_argument exception
 * in case of non supported arguments no ? 
 * What we test for : 
 * - Creating supported variables (jetVar or not jetVar).
 * - Creating unsupported variables (jetVar or not).
 * - getting var name, getting scale, setting scale.
 * - getting GeV setting GeV...
 */
int main() {
    std::unique_ptr<InputVariable> c = InputVariable::createVariable("e", "float", true);
    ASSERT_THROW(c->getName() == "e");

    c = InputVariable::createVariable("e", "double", true);
    ASSERT_THROW(c->getName() == "e");

    std::unique_ptr<InputVariable> a = InputVariable::createVariable("random", "float", true);
    ASSERT_THROW(a == nullptr);

    std::unique_ptr<InputVariable> b = InputVariable::createVariable("e", "double", false);
    ASSERT_THROW(b == nullptr);

    return 0;
}