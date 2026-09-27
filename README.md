# A1: Extracting IPv4 Addresses from Noisy Text

**Course:** Fall 2026  
**Language:** C++  
**Deliverables:** Source Code (`main.cpp`), AI Disclosure Log (`AI_DISCLOSURE.md`), Test Specification (`TEST_CASES.md`), and Test Input (`test_cases.txt`)

---

## 1. Project Overview

This command-line tool parses single lines of unstructured, noisy text to isolate, extract, and strictly validate embedded IPv4 addresses (along with optional ports).

Per the assignment specifications:
* **No standard conversion functions:** Implemented without `atoi`, `atol`, `strtol`, `stoi`, `stol`, `stoul`, `sscanf`, or formatted `scanf`. All base-10 numerical values are accumulated manually using `val = val * 10 + (c - '0')`.
* **No network libraries:** Implemented without `inet_aton`, `inet_pton`, `inet_addr`, or related networking headers.
* **No regular expressions:** Implemented without `<regex>` or POSIX `regex.h`. Character scanning and validation are written entirely by hand.
* **Grammar & Token Rules:** Candidate tokens consist strictly of digits (`0-9`), periods (`.`), and colons (`:`). All other characters are treated as noise/delimiters. Exactly 4 octets (`0–255`) and an optional port (`0–65535`) are permitted. Leading zeros are rejected unless the value is exactly `0`.
* **No Partial Matches:** If a candidate token contains extra octets, adjacent delimiters (e.g., `192.168.1.1.`), or a malformed port, the entire candidate token is rejected.
* **Single Match Enforcement:** Exactly one valid IPv4 address is allowed per input line; multiple addresses on a single line trigger validation failure.

---

## 2. Repository Layout

```text
.
├── main.cpp          # Full C++ source implementation (extractIPv4 and CLI loop)
├── AI_DISCLOSURE.md  # Formal disclosure log of AI prompts, code attribution, and bug fixes
├── TEST_CASES.md     # Detailed test matrix, edge-case rationale, and expected outputs
├── test_cases.txt    # Raw test suite ready for CLI stdin redirection
└── README.md         # Project documentation, build instructions, and quickstart
```

---

## 3. Build Instructions

Compile `main.cpp` using `g++` with strict compiler warnings:

```bash
g++ -Wall -Wextra -pedantic -std=c++17 main.cpp -o ipparser
```

---

## 4. How to Run

### Interactive Mode
Run the executable and provide inputs manually. Type `END` (case-sensitive) to terminate:

```bash
./ipparser
```

**Example interactive session:**
```text
Enter a string (or 'END' to quit): connecting to 192.168.1.1 now
Extracted IPv4 address: 192.168.1.1 (decimal value: 3232235777, port: none)
Enter a string (or 'END' to quit): server=10.0.0.255:8080end
Extracted IPv4 address: 10.0.0.255 (decimal value: 167772415, port: 8080)
Enter a string (or 'END' to quit): 192.168.1.1.
Invalid input: no valid IPv4 address found
Enter a string (or 'END' to quit): END
Program terminated.
```

### Automated Batch Testing
Run the complete suite of test cases by redirecting `test_cases.txt` into standard input:

```bash
./ipparser < test_cases.txt
```

---

## 5. Documentation Links

* **Test Suite & Validation Matrix:** Detailed breakdown of all 20 boundary, grammar, and edge-case tests can be found in [TEST_CASES.md](TEST_CASES.md).
* **Generative AI Disclosure:** The chronological record of prompts, model attribution, identified parser bugs, and the verification statement are documented in [AI_DISCLOSURE.md](AI_DISCLOSURE.md).
