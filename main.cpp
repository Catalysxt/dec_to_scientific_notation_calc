#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <format>
#include <string_view>

//******************************************************************************
// Decimal to Scientific Notation Calculator
//
// Step 1: Check if number is negative
// Step 2: Determine index of decimal point
// Step 3: Determine index of first significant figure
// Step 4: Perform left or right shifting of decimal point
// Step 5: Update exponent with number of shifts
// Step 6: Format the scientific number
// Step 7: Print the result

//  Steve Stevington
//  Apr 2026
//******************************************************************************

// Forward declarations
std::string to_scientific(std::string_view input);
bool is_input_negative(std::string_view in);
std::string format_result(bool negative, int exp, std::string_view input);

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

std::string to_scientific(std::string_view input) {

    // Step 1: Detect if input is negative
    bool negative = is_input_negative(input);

    int exponent {};
    int first_sig_fig {};
    int decimal_pt {};

    decimal_pt = input.find(".");

    // If no decimal pt detected, assign it the length of the number
    if(decimal_pt == std::string_view::npos) {
        decimal_pt = static_cast<int>(input.size()); 
    }

    // Step 3: Locate first sig fig
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

    // Step 4: Calculate left/right shift
    // Step 5: Update exponent
    // At the end we want first_sig_index == decimal_index = 1

    // left shift, increment exponent
    if (decimal_pt > first_sig_fig) {
        exponent = decimal_pt - first_sig_fig - 1;
    }

    // right shift, decrement exponent
    else if (first_sig_fig > decimal_pt) {
        exponent = decimal_pt - first_sig_fig;
    }

    else {
        exponent = 0;
    }

    // Step 6 and 7: Format and print the result
    return format_result(negative, exponent, input);
}

bool is_input_negative(std::string_view in) {
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

std::string format_result(bool negative, int exponent, std::string_view input) {
        
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

    std::string mantissa;
    // Add leading negative sign if required
    if (negative) {
        mantissa.push_back('-');
    }
    
    // Insert decimal after the sig fig
    mantissa.push_back(digits[lead]);
    if (lead + 1 < digits.size()) {
        mantissa.push_back('.');
        for (std::size_t i = lead + 1; i < digits.size(); ++i) {
            mantissa.push_back(digits[i]);
        }
    }

    std::string result(mantissa + 'e' + std::to_string(exponent));
    return result;
}
