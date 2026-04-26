#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <format>
#include <string_view>
#include <algorithm> // For std::ranges::find_if

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

//  Ahmad Sarraj
//  Apr 2026
//******************************************************************************

// Forward declarations
std::string to_scientific(std::string_view input);
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
    bool negative = input.starts_with('-');

    // Form new string, eliminating negative sign
    std::string_view real_string = negative ? input.substr(1) : input;

    int decimal_pt {};
    decimal_pt = input.find(".");
    
    // If no decimal pt detected, assign it the length of the number
    if(decimal_pt == std::string_view::npos) {
        decimal_pt = static_cast<int>(input.size()); 
    }
    
    // Step 3: Locate first sig fig
    auto it = std::ranges::find_if(input, [](unsigned char c) { return std::isdigit(c) && c != '0'; } );
    
    int first_sig_fig {};
    first_sig_fig = std::ranges::distance(real_string.begin(), it);

    if (it == input.end()) {      // The input doesn't contain a nonzero number
        return std::string(negative ? "-0e0" : "0e0");
    }

    // Step 4: Calculate left/right shift
    // Step 5: Update exponent
    // At the end we want first_sig_index == decimal_index + 1

    // left shift, increment exponent
    int exponent = (decimal_pt > first_sig_fig)
        ? (decimal_pt - first_sig_fig - 1)
        : (decimal_pt - first_sig_fig);

    return format_result(negative, exponent, input);
}

std::string format_result(bool negative, int exponent, std::string_view input) {
        
    auto it = std::ranges::find_if(input, [](unsigned char i) { return std::isdigit(i) && i != '0'; });

    std::string mantissa;
    // Add leading negative sign if required
    if (negative) {
        mantissa.push_back('-');
    }
    // Insert decimal after the sig fig
    mantissa += { *it, '.' };

    // Push back after first non zero digit
    for (auto i = std::next(it); i != input.end(); ++i) {
        if (std::isdigit(static_cast<unsigned char>(*i))) {
            if (std::isdigit(*it)) mantissa.push_back(*i);
        }
    }
    return std::format("{}e{}", mantissa, exponent);
}
