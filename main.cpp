#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <format>
#include <string_view>

// Forward declarations
std::string to_scientific(const std::string& input);
bool isInputNegative(std::string_view in);
std::string formatResult(std::string mantissa, bool negative, int exp);

int main() {

    const std::vector<std::string> tests = 
    {
        "12.000",
        "43212",
        "0.000643",
        "1042.4402",
        "-0.00000099"
    };

    for (const auto& i : tests) {
        std::cout << std::format("{:<12} -> {:<3}\n", i, to_scientific(i));
    };
}

std::string to_scientific(const std::string& input) {

    // Step 1: Detect if input is negative
    bool negative = false;
    negative = isInputNegative(input);

    int exponent {};
    int first_sig_fig {};
    int decimal_pt {};

    // Step 2: Determine initial position. I.e. populate variables

    // 2.1 Locate decimal point
    bool found_decimal = false;
    
    for(std::size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '.') {
            decimal_pt = static_cast<int>(i);
            found_decimal = true;
            break;
        }
    }

    // If no decimal pt detected, assign it the length of the number
    if(!found_decimal) {
        decimal_pt = static_cast<int>(input.size()); 
    }

    // 2.2 Locate first sig fig
    bool found_sig_fig = false;
    for (std::size_t i = 0; i < input.size(); ++i) {
        if (input[i] != '.' && input[i] != '0' && std::isdigit(static_cast<unsigned char>(input[i]))) {
            first_sig_fig = static_cast<int>(i);
            found_sig_fig = true;
            break;
        }
    }

    if(!found_sig_fig) {      // The input doesn't contain a nonzero number
        return std::string(negative ? "-" : "") + "0e0";
    }

    // Step 3: Calculate left/right shift and exponent
    // At the end we want first_sig_index == decimal_index = 1

    // left shift, increment exponent
    if (decimal_pt > first_sig_fig) {
        exponent = decimal_pt - first_sig_fig - 1;
    }

    // right shift, decrement exponent
    else if (first_sig_fig > decimal_pt) {
        exponent = -(first_sig_fig - decimal_pt);
    }

    else {
        exponent = 0;
    }

    // Step 4: Construction
    
    std::vector<char> digits;
    for (char c : input) {
        if (std::isdigit(static_cast<unsigned char>(c))) {
            digits.push_back(c);
        }
    }

    // Ensure first digit is nonzero by trimming leading zeros
    std::size_t lead {0};

    while (lead < digits.size() - 1 && digits[lead] == '0') {
        ++lead;
    }

    // Insert decimal after the sig fig
    std::string mantissa;
    mantissa.push_back(digits[lead]);
    if (lead + 1 < digits.size()) {
        mantissa.push_back('.');
        for (std::size_t i = lead + 1; i < digits.size(); ++i) {
            mantissa.push_back(digits[i]);
        }
    }

    // Step 5: Final Formatting
    return formatResult(mantissa, negative, exponent);
}

bool isInputNegative(std::string_view in) {
    bool negative = false;
    for (std::size_t i = 0; i < in.size(); ++i) {
    char c = in[i];

    if (i == 0 && (c == '-')) { // Detected a negative number
        negative = true;
        continue;
        } 
    }
    return negative;
}

std::string formatResult(std::string mantissa, bool negative, int exponent) {
    std::string result;
    if(negative) {
        result.push_back('-');
    }
    result += mantissa;

    // Add the 'e'
    result.push_back('e');

    // Add the value of the exponent
    result += std::to_string(exponent);

    return result;
}
