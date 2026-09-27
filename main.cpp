#include <iostream>
#include <string>
#include <cctype>

// Helper to determine if a character is part of an address token
static inline bool isTokenChar(char c) {
    return std::isdigit(static_cast<unsigned char>(c)) || c == '.' || c == ':';
}

// Parses a standalone candidate token. Returns true if and only if
// the token strictly matches: octet.octet.octet.octet[:port]
static bool validateAndParseToken(const std::string& token, unsigned long& outAddress, int& outPort) {
    size_t i = 0;
    size_t n = token.length();

    unsigned long octets[4] = {0, 0, 0, 0};

    for (int oct = 0; oct < 4; ++oct) {
        if (i >= n || !std::isdigit(static_cast<unsigned char>(token[i]))) {
            return false;
        }

        size_t digitStart = i;
        unsigned long val = 0;

        while (i < n && std::isdigit(static_cast<unsigned char>(token[i]))) {
            val = val * 10 + (token[i] - '0');
            if (val > 255) return false;
            ++i;
        }

        size_t digitCount = i - digitStart;
        if (digitCount < 1 || digitCount > 3) return false;
        // Leading zero rule: "0" is valid, but "01", "00", etc. are invalid
        if (digitCount > 1 && token[digitStart] == '0') return false;

        octets[oct] = val;

        if (oct < 3) {
            if (i >= n || token[i] != '.') return false;
            ++i; // consume '.'
        }
    }

    int parsedPort = -1;

    // Check for optional port
    if (i < n) {
        if (token[i] != ':') return false;
        ++i; // consume ':'

        if (i >= n || !std::isdigit(static_cast<unsigned char>(token[i]))) {
            return false; // Trailing colon with no digits is invalid
        }

        size_t portStart = i;
        unsigned long pVal = 0;

        while (i < n && std::isdigit(static_cast<unsigned char>(token[i]))) {
            pVal = pVal * 10 + (token[i] - '0');
            if (pVal > 65535) return false;
            ++i;
        }

        size_t portDigits = i - portStart;
        if (portDigits < 1 || portDigits > 5) return false;
        if (portDigits > 1 && token[portStart] == '0') return false;

        parsedPort = static_cast<int>(pVal);
    }

    // Must consume the entirety of the candidate token
    if (i != n) return false;

    outAddress = (octets[0] << 24) | (octets[1] << 16) | (octets[2] << 8) | octets[3];
    outPort = parsedPort;
    return true;
}

bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort) {
    outAddress = 0;
    outPort = -1;

    bool found = false;
    unsigned long candidateAddr = 0;
    int candidatePort = -1;

    size_t i = 0;
    size_t n = str.length();

    while (i < n) {
        // Skip garbage non-token characters
        while (i < n && !isTokenChar(str[i])) {
            ++i;
        }
        if (i >= n) break;

        // Isolate maximal contiguous token
        size_t start = i;
        while (i < n && isTokenChar(str[i])) {
            ++i;
        }

        std::string token = str.substr(start, i - start);
        unsigned long tempAddr = 0;
        int tempPort = -1;

        if (validateAndParseToken(token, tempAddr, tempPort)) {
            // Assignment specifies: exactly one valid address per line
            if (found) {
                outAddress = 0;
                outPort = -1;
                return false;
            }
            found = true;
            candidateAddr = tempAddr;
            candidatePort = tempPort;
        }
    }

    if (found) {
        outAddress = candidateAddr;
        outPort = candidatePort;
        return true;
    }

    return false;
}

int main() {
    std::string line;
    while (true) {
        std::cout << "Enter a string (or 'END' to quit): ";
        if (!std::getline(std::cin, line)) break;
        if (line == "END") {
            std::cout << "Program terminated." << std::endl;
            break;
        }

        unsigned long address = 0;
        int port = -1;

        if (extractIPv4(line, address, port)) {
            unsigned int a = (address >> 24) & 0xFF;
            unsigned int b = (address >> 16) & 0xFF;
            unsigned int c = (address >> 8) & 0xFF;
            unsigned int d = address & 0xFF;

            std::cout << "Extracted IPv4 address: " << a << "." << b << "." << c << "." << d
                      << " (decimal value: " << address << ", port: ";
            if (port == -1) {
                std::cout << "none";
            } else {
                std::cout << port;
            }
            std::cout << ")" << std::endl;
        } else {
            std::cout << "Invalid input: no valid IPv4 address found" << std::endl;
        }
    }
    return 0;
}
