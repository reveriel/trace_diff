#include "calculator/calculator.h"
#include <gtest/gtest.h>
#include <stdexcept>

TEST(CalculatorTest, Add) {
    Calculator calc;
    EXPECT_EQ(calc.add(2, 3), 5);
    EXPECT_EQ(calc.add(-2, 3), 1);
    EXPECT_EQ(calc.add(0, 0), 0);
}

TEST(CalculatorTest, Subtract) {
    Calculator calc;
    EXPECT_EQ(calc.subtract(5, 3), 2);
    EXPECT_EQ(calc.subtract(3, 5), -2);
    EXPECT_EQ(calc.subtract(0, 0), 0);
}

TEST(CalculatorTest, Multiply) {
    Calculator calc;
    EXPECT_EQ(calc.multiply(2, 3), 6);
    EXPECT_EQ(calc.multiply(-2, 3), -6);
    EXPECT_EQ(calc.multiply(0, 5), 0);
}

TEST(CalculatorTest, Divide) {
    Calculator calc;
    EXPECT_EQ(calc.divide(6, 2), 3);
    EXPECT_EQ(calc.divide(-6, 2), -3);
    EXPECT_EQ(calc.divide(0, 5), 0);
    EXPECT_THROW(calc.divide(6, 0), std::invalid_argument);
}
