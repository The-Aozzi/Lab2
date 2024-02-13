#include <iostream> 
#include <vector> 
#include <cstdint> 
 
using namespace std::string_literals; 
 
/**
 * @brief Represents a large integer.
 */
class BigInt{ 
public: 
    /**
     * @brief Default constructor.
     */
    BigInt(): BigInt(0){} 
 
    /**
     * @brief Constructor that initializes BigInt with a uint32_t number.
     * @param number The number to initialize BigInt with.
     */
    BigInt(uint32_t number){ 
        do{ 
            storage.push_back(number % 10); 
            number /= 10; 
        }while (number); 
    } 
 
    /**
     * @brief Constructor that initializes BigInt with a string representation of a number.
     * @param str The string representation of the number.
     */
    BigInt(std::string str): storage(str.size()){ 
        for (size_t i = 0; i < storage.size(); ++i){ 
            storage[str.size() - 1 - i] = str[i] - '0'; 
        } 
    } 
 
    /**
     * @brief Conversion operator to convert BigInt to string.
     */
    operator std::string (){ 
        std::string str(storage.size(), 0); 
        for (size_t i = 0; i < storage.size(); ++i){ 
            str[i] = storage[str.size() - 1 - i] + '0'; 
        } 
        return str; 
    } 
 
    friend BigInt & operator+=(BigInt &, const BigInt &); 
    friend BigInt operator+(const BigInt &, const BigInt &); 
    friend BigInt & operator*=(BigInt &, const BigInt &); 
    friend BigInt operator*(const BigInt &, const BigInt &); 
    friend BigInt & operator-=(BigInt &, const BigInt &); 
    friend BigInt operator-(const BigInt &, const BigInt &); 
     
    friend std::ostream & operator<<(std::ostream &, const BigInt &); 
 
private: 
    std::vector<int8_t> storage; /**< Stores the digits of the large integer. */
}; 
 
/**
 * @brief Output stream operator for BigInt.
 * @param stream The output stream.
 * @param big_number The BigInt to output.
 * @return The output stream.
 */
std::ostream & operator<<(std::ostream & stream, const BigInt & big_number){ 
    for (size_t i = 0; i < big_number.storage.size(); ++i){ 
            stream << +big_number.storage[big_number.storage.size() - 1 - i]; 
        } 
    return stream; 
} 
 
/**
 * @brief Multiplies a BigInt by another BigInt.
 * @param left The BigInt to be multiplied.
 * @param right The BigInt to multiply by.
 * @return The result of the multiplication.
 */
BigInt & operator*=(BigInt & left, const BigInt & right){ 
    BigInt result; 
 
    result.storage.resize(left.storage.size() + right.storage.size()); 
 
    for (size_t i = 0; i < right.storage.size(); ++i){ 
        int8_t carry = 0; 
        for (size_t j = 0; j < left.storage.size() || carry; ++j){ 
            int8_t product = result.storage[i+j] + (j < left.storage.size() ? right.storage[i] * left.storage[j] : 0) + carry; 
            result.storage[i+j] = product % 10; 
            carry = product / 10; 
        } 
    } 
 
    while (result.storage.size() > 1 && result.storage.back() == 0) result.storage.pop_back(); 
 
    left.storage = std::move(result.storage); 
    return left; 
} 
 
/**
 * @brief Multiplies two BigInts.
 * @param left The first BigInt.
 * @param right The second BigInt.
 * @return The result of the multiplication.
 */
BigInt operator*(const BigInt & left, const BigInt & right){ 
    BigInt result = left; 
    result *= right; 
    return result; 
} 
 
/**
 * @brief Adds a BigInt to another BigInt.
 * @param left The first BigInt.
 * @param right The second BigInt.
 * @return The result of the addition.
 */
BigInt & operator+=(BigInt & left, const BigInt & right){  
    int8_t carry = 0;  
     
    size_t max_size = std::max(left.storage.size(), right.storage.size()); 
    left.storage.reserve(max_size + 1); 
    left.storage.resize(max_size); 
 
    for (size_t i = 0; i < right.storage.size() || carry; ++i){  
        if (i < right.storage.size()) { 
            left.storage[i] += right.storage[i] + carry;  
        } else { 
            if (i == left.storage.size()){ 
                left.storage.push_back(carry); 
                break; 
            } 
            left.storage[i] += carry; 
        } 
        carry = left.storage[i] / 10;  
        left.storage[i] %= 10;  
    }  
    return left;  
} 
 
/**
 * @brief Adds two BigInts.
 * @param left The first BigInt.
 * @param right The second BigInt.
 * @return The result of the addition.
 */
BigInt operator+(const BigInt & left, const BigInt & right){ 
    BigInt result = left; 
    result += right; 
    return result; 
} 
 
/**
 * @brief Subtracts a BigInt from another BigInt.
 * @param left The first BigInt.
 * @param right The second BigInt.
 * @return The result of the subtraction.
 */
BigInt & operator-=(BigInt & left, const BigInt & right){  
    int8_t borrow = 0;  
    for (size_t i = 0; i < right.storage.size() || borrow; ++i){  
        int8_t subtrahend = (i < right.storage.size()) ? right.storage[i] : 0; 
        left.storage[i] -= subtrahend + borrow; 
        if (left.storage[i] < 0) { 
            left.storage[i] += 10; 
            borrow = 1; 
        } else { 
            borrow = 0; 
        } 
    }  
    while (left.storage.size() > 1 && left.storage.back() == 0) left.storage.pop_back(); 
    return left;  
} 
 
/**
 * @brief Subtracts one BigInt from another BigInt.
 * @param left The first BigInt.
 * @param right The second BigInt.
 * @return The result of the subtraction.
 */
BigInt operator-(const BigInt & left, const BigInt & right){  
    BigInt result = left;  
    result -= right;  
    return result;  
}  
 
/**
 * @brief Calculates the factorial of a number.
 * @param number The number to calculate the factorial for.
 * @return The factorial of the number.
 */
BigInt factorial(int number){ 
    BigInt result = 1; 
    while (number) result *= number--; 
    return result; 
} 
 
/**
 * @brief Calculates the power of a base to an exponent.
 * @param base The base number.
 * @param exponent The exponent.
 * @return The result of base raised to exponent.
 */
 BigInt power(int base, int exponent){
    BigInt result = 1;
    while (exponent > 0) {
        if (exponent & 1) {
            result *= base;
        }
        base *= base;
        exponent >>= 1;
    }
    return result;
}

BigInt fibonacci(int number){
    BigInt fibonacci_pair[2] = {0, 1};
    for(; number > 1; number -= 2){
        fibonacci_pair[0] += fibonacci_pair[1];
        fibonacci_pair[1] += fibonacci_pair[0];
    }
    return fibonacci_pair[number];
}

int main(int, char**){
    BigInt x = 111;
    BigInt y = 55;
    x-=y;
    std::cout << x;
    power(2,5);
    return 0;
}