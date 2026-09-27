# A1: Extracting IPv4 Addresses from Noisy Text

**Course:** Fall 2026  
**Language:** C++ 
**Deliverables:** Source Code (`main.cpp`), AI Disclosure Log (`AI_DISCLOSURE.md`), Automated Test Cases (`test_cases.txt`), and Documentation (`README.md`)

---

## 1. Project Overview

This program scans arbitrary, unstructured lines of text to isolate, extract, and strictly validate embedded IPv4 addresses with optional port numbers.

Per the assignment specifications:
* **No standard conversion libraries:** Implemented without `atoi`, `atol`, `strtol`, `stoi`, `stol`, `stoul`, `sscanf`, or formatted `scanf`. All base-10 numerical values are accumulated manually using `val = val * 10 + (c - '0')`.
* **No network libraries:** Implemented without `inet_aton`, `inet_pton`, `inet_addr`, or related networking headers.
* **No regular expressions:** Implemented without `<regex>` or POSIX `regex.h`. Parsing is handled purely via character-by-character validation.
* **Grammar & Token Rules:** Candidate tokens consist strictly of digits (`0-9`), periods (`.`), and colons (`:`). All other characters are treated as noise/delimiters. Exactly 4 octets (`0–255`) and an optional port (`0–65535`) are permitted. No leading zeros are allowed unless the numeric value is exactly `0`.
* **No Partial Matches:** If a candidate token has trailing delimiters, extra octets, or a malformed port, the entire candidate token is rejected.
* **Single Match Enforcement:** Exactly one valid IPv4 address is allowed per input line; multiple addresses on a single line trigger validation failure.

---

## 2. Repository Layout

```text
.
├── main.cpp          # Full C++ source implementation (extractIPv4 and CLI loop)
├── AI_DISCLOSURE.md  # Detailed log of generative AI prompts, errors, and fixes
├── test_cases.txt    # Comprehensive input test suite ready for CLI redirection
└── README.md         # Documentation, build instructions, and validation matrix
```

---

## 3. Build & Execution

### Compile
Compile the program using `g++` with standard warnings enabled:

```bash
g++ -Wall -Wextra -pedantic -std=c++17 main.cpp -o ipparser
```

### Run Interactively
Run the binary and provide inputs directly. Terminate the program by entering `END` (case-sensitive):

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

### Run Automated Batch Tests
Pipe the comprehensive test suite directly through standard input:

```bash
./ipparser < test_cases.txt
```

---

## 4. Test Matrix & Edge Case Coverage

| # | Input String | Status | Extracted IP | Decimal Value | Port | Tested Category |
|---|---|---|---|---|---|---|
| 1 | `connecting to 192.168.1.1 now` | **Valid** | `192.168.1.1` | `3232235777` | none | Standard assignment sample |
| 2 | `server=10.0.0.255:8080end` | **Valid** | `10.0.0.255` | `167772415` | 8080 | Address with valid port embedded in text |
| 3 | `192a168.1.1.1` | **Valid** | `168.1.1.1` | `2818638081` | none | Non-token character (`a`) splits tokens; valid candidate extracted |
| 4 | `192.168.1.1.` | **Invalid** | — | — | — | Trailing dot included in candidate token; rejects partial match |
| 5 | `Connection from 192.168.1.1 refused` | **Valid** | `192.168.1.1` | `3232235777` | none | Clean address embedded within text |
| 6 | `192.168.01.1` | **Invalid** | — | — | — | Octet with disallowed leading zero (`01`) |
| 7 | `1.2.3.4:99999` | **Invalid** | — | — | — | Port out of range (> 65535); rejects entire address |
| 8 | `12.34.56` | **Invalid** | — | — | — | Truncated address (only 3 octets) |
| 9 | `no number here` | **Invalid** | — | — | — | Pure noise text with no candidate tokens |
| 10 | `Boundary min 0.0.0.0:0 test` | **Valid** | `0.0.0.0` | `0` | 0 | Lower numerical boundaries for IP and port |
| 11 | `Boundary max 255.255.255.255:65535 test` | **Valid** | `255.255.255.255` | `4294967295` | 65535 | Upper numerical boundaries for IP and port |
| 12 | `Check port overflow 10.0.0.1:65536` | **Invalid** | — | — | — | Port upper bound overflow (+1) |
| 13 | `Check octet overflow 10.0.0.256` | **Invalid** | — | — | — | Octet upper bound overflow (+1) |
| 14 | `.192.168.1.1` | **Invalid** | — | — | — | Leading dot in candidate token |
| 15 | `192.168.1.1:` | **Invalid** | — | — | — | Colon present without port digits |
| 16 | `192.168.1.1::80` | **Invalid** | — | — | — | Consecutive colons in port specification |
| 17 | `192..168.1.1` | **Invalid** | — | — | — | Consecutive dots / empty octet |
| 18 | `192.168.1.1:080` | **Invalid** | — | — | — | Disallowed leading zero in port number |
| 19 | `10.0.0.00` | **Invalid** | — | — | — | Octet with multiple zeros (`00`) |
| 20 | `address 1.1.1.1 and 2.2.2.2 present` | **Invalid** | — | — | — | Multiple valid addresses on the same line |

---

## 5. AI Usage Disclosure Link

The documentation required by the course regarding AI tool usage, prompt history, bug diagnoses, code revisions, and personal verification is available in [AI_DISCLOSURE.md](AI_DISCLOSURE.md).
