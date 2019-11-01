/**
 * CS 2110 Fall 2019 HW2
 * Part 1 - Coding a bit vector
 *
 * @author Benjamin Melnick
 *
 * Global rules for this file:
 * - You may not use multiplication, division or modulus in any method.
 * - You may not use more than 2 conditionals per method, and you may only use
 *   them in select methods. Conditionals are if-statements, if-else statements,
 *   or ternary expressions. The else block associated with an if-statement does
 *   not count toward this sum.
 * - You may not use looping constructs in most methods. Looping constructs
 *   include for loops, while loops and do-while loops. See below for exceptions
 * - In those methods that allow looping, you may not use more than 2 looping
 *   constructs, and they may not be nested.
 * - You may not declare any file-level variables.
 * - You may not declare any objects, other than String in select methods.
 * - You may not use switch statements.
 * - You may not use casting.
 * - You may not use the unsigned right shift operator (>>>)
 * - You may not write any helper methods, or call any other method from this
 *   file, another file, or the Java API to implement any method.
 * - You may only perform addition or subtraction with the number 1.
 *
 * Method-specific rules for this file:
 * - You may declare exactly one String variable, in toPaddedBinaryString only.
 * - Iteration may not be used in set, clear, toggle, isSet or isClear.
 * - Conditionals may not be used in set, clear, toggle, isSet, or isClear.
 */
public class BitVector
{
    /**
     * 32-bit data initialized to all zeros. Here is what you will be using to represent
     * the Bit Vector. Do not change its scope from private.
     */
    private int bits;

    /** You may not add any more fields to this class other than the given one. */

    /**
     * Sets the bit (sets to 1) pointed to by index.
     * You may not use iteration or conditionals in this method.
     * @param index index of which bit to set.
     *        0 for the least significant bit (right most bit).
     *        31 for the most significant bit.
     */
    public void set(int index) {
        bits = bits | (0x1 << index);
    }

    /**
     * Clears the bit (sets to 0) pointed to by index.
     * You may not use iteration or conditionals in this method.
     * @param index index of which bit to set.
     * 	      0 for the least significant bit (right most bit).
     * 	      31 for the most significant bit.
     */
    public void clear(int index) {
        bits = bits & (~(0x1 << index));
    }

    /**
     * Toggles the bit (sets to the opposite of its current value) pointed to by index.
     * You may not use iteration or conditionals in this method.
     * @param index index of which bit to set.
     * 	      0 for the least significant bit (right most bit).
     * 	      31 for the most significant bit.
     */
    public void toggle(int index) {
        bits = bits ^ (0x1 << index);
    }

    /**
     * Returns true if the bit pointed to by index is currently set.
     * You may not use iteration in this method.
     * @param index index of which bit to check.
     * 	      0 for the least significant bit (right-most bit).
     * 	      31 for the most significant bit.
     * @return true if the bit is set, false if the bit is clear.
     *         If the index is out of range (index >= 32), then return false.
     */
    public boolean isSet(int index) {
        return !(index >= 32) && (bits & (0x1 << index)) == (0x1 << index);
    }

    /**
     * Returns true if the bit pointed to by index is currently clear.
     * You may not use iteration in this method.
     * @param index index of which bit to check.
     * 	      0 for the least significant bit (right-most bit).
     * 	      31 for the most significant bit.
     * @return true if the bit is clear, false if the bit is set.
     *         If the index is out of range (index >= 32), then return true.
     */
    public boolean isClear(int index) {
        return (index >= 32) || ((bits & (0x1 << index)) == 0);
    }

    /**
     * Returns a string representation of this object.
     * Return a string with the binary representation of the bit vector.
     * You may use String concatenation (+) here.
     * You must return a 32-bit string representation.
     * i.e if the bits field was 2, then return "00000000000000000000000000000010"
     *
     * You may declare one String variable in this method.
     */
    public String toPaddedBinaryString()
    {
       String bitString = "";
       int shift = 0x1;
       for (int i = 0; i < 32; i++) {
           bitString = ((bits & (shift)) == shift) ? "1" + bitString : "0" + bitString;
           shift = shift << 0x1;
       }
       return bitString;
    }

    /**
     * Returns the number of bits currently set (=1) in this bit vector.
     * You may obviously use the ++ operator to increment your counter.
     */
    public int onesCount() {
        int count = 0;
        for (int i = 0; i < 32; i++) {
            if ((bits & (0x1 << i)) != 0) {
                count++;
            }
        }
        return count;
    }

    /**
     * Returns the number of bits currently clear (=0) in this bit vector.
     * You may obviously use the ++ operator to increment your counter.
     */
    public int zerosCount() {
        int count = 0;
        for (int i = 0; i < 32; i++) {
            if ((bits & (0x1 << i)) == 0) {
                count++;
            }
        }
        return count;
    }

    /**
     * Returns the "size" of this BitVector. The size of this bit vector is defined
     * to be the minimum number of bits that will represent all of the ones.
     * For example, the size of the bit vector 00010000 will be 5.
     */
    public int size() {
        int size = 32;
        for (int i = 31; i > 0; i--) {
            //start from most significant bit
            if ((bits & (0x1 << i)) == 0) {
                //keep going if bit is cleared
                size--;
            } else {
                //stop once we find a set bit
                return size;
            }
        }
        return size;
    }
}
