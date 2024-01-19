package edu.montana.csci.csci366.examples;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class RPNCalculatorTest {

    public void testBasicRPNExpression() {
        RPNCalculator calc = new RPNCalculator();
        assertEquals(2, calc.evaluate("1 1 +"));
    }

    public void testBadExpressionThrowsIllegalArgumentException() {
        RPNCalculator calc = new RPNCalculator();
        assertThrows(IllegalArgumentException.class, () -> calc.evaluate("1 1 1 +"));
    }

}
