/**
 * CS 2110 Fall 2019 HW2
 * Part 2 - Coding with bases
 *
 * @author Benjamin Melnick
 *
 * Global rules for this file:
 * - You may not use more than 2 conditionals per method. Conditionals are
 *   if-statements, if-else statements, or ternary expressions. The else block
 *   associated with an if-statement does not count toward this sum.
 * - You may not use more than 2 looping constructs per method. Looping
 *   constructs include for loops, while loops and do-while loops.
 * - You may not use nested loops.
 * - You may not declare any file-level variables.
 * - You may not declare any objects, other than String in select methods.
 * - You may not use switch statements.
 * - You may not use the unsigned right shift operator (>>>)
 * - You may not write any helper methods, or call any other method from this or
 *   another file to implement any method.
 * - The only Java API methods you are allowed to invoke are:
 *     String.length()
 *     String.charAt()
 * - You may not invoke the above methods from string literals.
 *     Example: "12345".length()
 * - When concatenating numbers with Strings, you may only do so if the number
 *   is a single digit.
 *
 * Method-specific rules for this file:
 * - You may not use multiplication, division or modulus in any method, EXCEPT
 *   decimalStringToInt.
 * - You may declare exactly one String variable each in intToBinaryString and
 *   and intToHexString.
 */
public class Bases
{
    /**
     * Convert a string containing ASCII characters (in binary) to an int.
     * You do not need to handle negative numbers. The Strings we will pass in will be
     * valid binary numbers, and able to fit in a 32-bit signed integer.
     *
     * Example: binaryStringToInt("111"); // => 7
     */
    public static int binaryStringToInt(String binary) {
        int total = 0;
        int power = 0; //represents the power of 2 for a given bit position
        for (int i = binary.length() - 1; i >= 0; i--) {
            if (binary.charAt(i) == '1') {
                total += (1 << power);
            }
            power++;
        }
        return total;
    }

    /**
     * Convert a string containing ASCII characters (in decimal) to an int.
     * You do not need to handle negative numbers. The Strings we will pass in will be
     * valid decimal numbers, and able to fit in a 32-bit signed integer.
     *
     * Example: decimalStringToInt("123"); // => 123
     *
     * You may use multiplication, division, and modulus in this method.
     */
    public static int decimalStringToInt(String decimal) {
        int total = 0;
        int power = 1;
        for (int i = decimal.length() - 1; i >= 0; i--) {
            //subtracting 0 from ASCII digit returns an integer
            total += ((decimal.charAt(i) - '0') * power);
            power = power * 10;
        }
        return total;
    }

    /**
     * Convert a string containing ASCII characters (in hex) to an int.
     * The input string will only contain numbers and uppercase letters A-F.
     * You do not need to handle negative numbers. The Strings we will pass in will be
     * valid hexadecimal numbers, and able to fit in a 32-bit signed integer.
     *
     * Example: hexStringToInt("A6"); // => 166
     */
    public static int hexStringToInt(String hex) {
        int total = 0;
        int power = 0;
        for (int i = hex.length() - 1; i >= 0; i--) {
            //capital letters have greater ASCII values than digits
            //if the character is a letter,
            total += (hex.charAt(i) > '9' ? hex.charAt(i) - '0' - 7 : hex.charAt(i) - '0') << power;
            power += 4; //move the power to the next set of 4 bits
        }
        return total;
    }

    /**
     * Convert a int into a String containing ASCII characters (in binary).
     * You do not need to handle negative numbers.
     * The String returned should contain the minimum number of characters necessary to
     * represent the number that was passed in.
     *
     * Example: intToBinaryString(7); // => "111"
     *
     * You may declare one String variable in this method.
     */
    public static String intToBinaryString(int binary) {
        String binaryString = "";
        int val = binary;
        while (val != 0) {
            binaryString = (((val & 1) == 1) ? '1' : '0') + binaryString;
            val = val >> 1;
        }
        return (binaryString.length() == 0 ? "0" : binaryString);
    }

    /**
     * Convert a int into a String containing ASCII characters (in hexadecimal).
     * The output string should only contain numbers and uppercase letters A-F.
     * You do not need to handle negative numbers.
     * The String returned should contain the minimum number of characters necessary to
     * represent the number that was passed in.
     *
     * Example: intToHexString(166); // => "A6"
     *
     * You may declare one String variable in this method.
     */
    public static String intToHexString(int hex)
    {
        String string = "";
        int value = hex;
        for (int i = value; i >= 1; i = i >> 4) {
            string = (i - ((i >> 4) << 4)) > 9 ? ((char) (i - ((i >> 4) << 4) + 55) + string) : i - ((i >> 4) << 4) + string;
        }
        return (string.length() == 0 ? "0" : string);
    }
}
