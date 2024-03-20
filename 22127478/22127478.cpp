#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
using namespace std;

string decToBinary(int n) // decimal to binary
{
    stringstream ss;
    for (int i = 7; i >= 0; i--) 
    {
        int k = n >> i;
        if (k & 1)
            ss << "1";
        else
            ss << "0";
    }
    return ss.str();
} 

string decToBinary16bits(int n) // decimal to binary but demonstrated by 16 bits
{
    stringstream ss;
    for (int i = 15; i >= 0; i--)
    {
        int k = n >> i;
        if (k & 1)
            ss << "1";
        else
            ss << "0";
    }
    return ss.str();
} 

char flip(char c) 
{
    return c ^ '1';
}

string convertTwosComplement(string arr) // convert to two's complement
{
    string twos;
    string ones = twos = "";
    for (int i = 0; i < arr.length(); i++)
        ones += flip(arr[i]);

    twos = ones = " ";
    int i;
    for (i = arr.length() - 1; i >= 0; i--)
    {
        if (ones[i] == '1')
            twos[i] = '0';
        else
        {
            twos[i] = '1';
            break;
        }
    }
    if (i == -1)
        twos = '1' + twos;
    return twos;
} 

string sumBitsets(string& arrA, string& arrB) 
{
    int size = arrA.length();
    string sumBits;
    for (int i = 0; i < size; i++) 
        sumBits += '0';
    // initialize string of sum bit
    char carry = '0';
    for (int i = size - 1; i >= 0; i--) 
    {
        char bit1 = arrA[i];
        char bit2 = arrB[i];

        char sum = (bit1 ^ bit2) ^ carry; // sum 2 bits from 2 bitsets, XOR it and store to sum  
        carry = (bit1 & bit2) | (bit1 & carry) | (bit2 & carry);
        sumBits[i] = sum;
    }
    return sumBits;
}

int toDecimal(string binary) 
{
    int decimal = 0;
    int base = 1;

    if (binary[0] == '1')
    {
        // Handle negative binary
        for (int i = binary.length() - 1; i >= 0; i--) 
        {
            if (binary[i] == '1') 
                decimal |= base;
            else if (binary[i] != '0') 
            {
                cout << "Invalid binary input." << endl;
                return -1;
            }
            base <<= 1;
        }

        // Adjust the result for negative numbers
        decimal = -((1 << binary.length()) - decimal);
    }
    else 
    {
        // Handle positive binary
        for (int i = binary.length() - 1; i >= 0; i--) {
            if (binary[i] == '1') 
                decimal |= base;
            else if (binary[i] != '0') 
            {
                cout << "Invalid binary input." << endl;
                return -1;
            }
            base <<= 1;
        }
    }
    return decimal;
}

string add(string arrA, string arrB) 
{
    string sum = sumBitsets(arrA, arrB);
    bool overflow = (arrA[0] == '1' && arrB[0] == '1' && sum[0] == '0') || (arrA[0] == '0' && arrB[0] == '0' && sum[0] == '1');
    // check if overflow occurs
    if (overflow) 
        return convertTwosComplement(sum);
    else 
        return sum;
}

string substract(string arrA, string arrB)
{
    string newArrB;
    int tmp = toDecimal(arrB);
    tmp = -tmp;                     // A - B = A + (-B)
    newArrB = decToBinary(tmp);
    return add(arrA, newArrB);
}

void swapString(string& arrA, string& arrB)
{
    string tmp = arrA;
    arrA = arrB;
    arrB = tmp;
}

string multiply(string arrA, string arrB)
{
    if (toDecimal(arrA) > 0 && toDecimal(arrB) < 0) swap(arrA, arrB);
    else if (toDecimal(arrA) < 0 && toDecimal(arrB) < 0)
    {
        arrA = decToBinary(abs(toDecimal(arrA)));
        arrB = decToBinary(abs(toDecimal(arrB)));
    }
    int result = 0;
    string tmp = decToBinary(result);           // create tmp variable to store 0
    for (int i = 7; i >= 0; i--)
    {
        if (arrB[i] == '1')                     // add arrA and tmp if bit in arrB is 1 
            tmp = add(tmp, arrA);               
        if (toDecimal(arrA) >= 11 && toDecimal(arrB) >= 11) arrA = decToBinary16bits(toDecimal(arrA) << 1); // case when overflow
        else arrA = decToBinary(toDecimal(arrA) << 1); // no overflow
    }
    return tmp;
}

string SHL(string& arrA, string& arrB)             // to shift left the big string which is combined from arrA and arrB by 1 
{
    string newString = arrA + arrB; 
    newString = decToBinary16bits(toDecimal(newString) << 1);
    arrA = newString.substr(0, 8);                 //change value of arrA and arrB when shift left the big string
    arrB = newString.substr(8, 8);
    return newString;
}

string divide(string arrA, string arrB)
{
    int decimalA = toDecimal(arrA);
    string tmp;
    char arrA_sign = arrA[0];   //mark the sign of the input decimal number
    char arrB_sign = arrB[0];
    // if the number is < 0 then change the number into positive
    if (arrA[0] == '1')
    {
        int a = toDecimal(arrA);
        a = -a;
        arrA = decToBinary(a);
    }
    if (arrB[0] == '1') 
    {
        int b = toDecimal(arrB);
        b = -b;
        arrB = decToBinary(b);
    }
    for (int i = 0; i < 8; i++) // create the set tmp of 0 bit
        tmp += '0';

    int k = 8;          //bitset's length
    while (k > 0)
    {
        string newString = SHL(tmp, arrA); //shift left the combined string
        tmp = substract(tmp, arrB); //substract tmp with arrB
        int decimalTmp = toDecimal(tmp);    // get the  decimal value of tmp to check if it's greater than 0
        if (decimalTmp < 0)
        {
            arrA[7] = '0';
            tmp = add(tmp, arrB);
        }
        else arrA[7] = '1';
        k--;
    }
    if ((arrA_sign == '1' && arrB_sign == '0') || (arrA_sign == '0' && arrB_sign == '1')) 
    {
        // return the right sign of the result
        int a_dec = toDecimal(arrA);
        a_dec = -a_dec;
        arrA = decToBinary(a_dec);
    }
    
    return arrA; // return the divisor as the dividend
}

string modulo(string arrA, string arrB) 
{
    char arrA_sign = arrA[0];
    char arrB_sign = arrB[0];
    //if (abs(toDecimal(arrA)) < abs(toDecimal(arrB)) && (toDecimal(arrA) < 0 || toDecimal(arrB) < 0))
      //  swapString(arrA, arrB);

    int decimalA = toDecimal(arrA);
    string tmp;
    
    if (arrA[0] == '1')
    {
        int a = toDecimal(arrA);
        a = -a;
        arrA = decToBinary(a);
    }
    if (arrB[0] == '1')
    {
        int b = toDecimal(arrB);
        b = -b;
        arrB = decToBinary(b);
    }
    for (int i = 0; i < 8; i++)
        tmp += '0';

    int k = 8;        
    while (k > 0)
    {
        string newString = SHL(tmp, arrA); 
        tmp = substract(tmp, arrB); 
        int decimalTmp = toDecimal(tmp);
        if (decimalTmp < 0)
        {
            arrA[7] = '0';
            tmp = add(tmp, arrB);
        }
        else arrA[7] = '1';
        k--;
    }
    if ((arrA_sign == '1' && arrB_sign == '0') || (arrA_sign == '1' && arrB_sign == '1'))
    {
        // A < 0 && B > 0, or A < 0 && B < 0
        int tmp_dec = toDecimal(tmp);
        tmp = decToBinary(-tmp_dec);
    }
    return tmp; //return the modulo
}

int main()
{
    int a, b;
    cout << "Input A (Decimal): ";
    cin >> a;
    cout << "Input B (Decimal): ";
    cin >> b;
    cout << "A (Binary): " << decToBinary(a) << endl;
    cout << "B (Binary): " << decToBinary(b) << endl;

    string arrA = decToBinary(a);
    string arrB = decToBinary(b);
    
    cout << "A + B (Binary): " << add(arrA, arrB) << endl;
    cout << "A + B (Decimal): " << toDecimal(add(arrA, arrB)) << endl;
    cout << "A - B (Binary): " << substract(arrA, arrB) << endl;
    cout << "A - B (Decimal): " << toDecimal(substract(arrA, arrB)) << endl;
    cout << "A * B (Binary): " << multiply(arrA, arrB) << endl;
    cout << "A * B (Decimal): " << toDecimal(multiply(arrA, arrB)) << endl;
    if (toDecimal(arrB) == 0) cout << "Invalid division" << endl;
    cout << "A / B (Binary): " << divide(arrA, arrB) << endl;
    cout << "A / B (Decimal): " << toDecimal(divide(arrA, arrB)) << endl;
    cout << "A % B (Binary): " << modulo(arrA, arrB) << endl;
    cout << "A % B (Decimal): " << toDecimal(modulo(arrA, arrB)) << endl;
    return 0;
}

