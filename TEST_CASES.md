# Test Cases & Validation Suite

**Assignment:** A1 - Extracting IPv4 Addresses from Noisy Text  
**Source File:** `test_cases.txt`  
**Execution Command:** `./ipparser < test_cases.txt`

---

## 1. Test Suite Matrix

| Test ID | Input String | Expected Result | Extracted Address | Port | Category / Edge Case Validated |
|---|---|---|---|---|---|
| **TC-01** | `connecting to 192.168.1.1 now` | **Valid** | `192.168.1.1` (3232235777) | none | Standard assignment sample: address surrounded by words |
| **TC-02** | `server=10.0.0.255:8080end` | **Valid** | `10.0.0.255` (167772415) | 8080 | Embedded candidate with valid port and non-token characters |
| **TC-03** | `192a168.1.1.1` | **Valid** | `168.1.1.1` (2818638081) | none | Non-token delimiter (`a`) splits tokens; valid candidate extracted |
| **TC-04** | `192.168.1.1.` | **Invalid** | — | — | Trailing delimiter is part of token; rejects partial match |
| **TC-05** | `Connection from 192.168.1.1 refused` | **Valid** | `192.168.1.1` (3232235777) | none | Standard log format extraction |
| **TC-06** | `192.168.01.1` | **Invalid** | — | — | Disallowed leading zero in octet (`01`) |
| **TC-07** | `1.2.3.4:99999` | **Invalid** | — | — | Port number out of range (> 65535); rejects entire address |
| **TC-08** | `12.34.56` | **Invalid** | — | — | Incomplete address: only 3 octets |
| **TC-09** | `no number here` | **Invalid** | — | — | Text containing zero token characters |
| **TC-10** | `Boundary min 0.0.0.0:0 test` | **Valid** | `0.0.0.0` (0) | 0 | Minimum numeric boundary values for IP and port |
| **TC-11** | `Boundary max 255.255.255.255:65535 test` | **Valid** | `255.255.255.255` (4294967295) | 65535 | Maximum numeric boundary values for IP and port |
| **TC-12** | `Check port overflow 10.0.0.1:65536` | **Invalid** | — | — | Port boundary overflow: exactly 65536 |
| **TC-13** | `Check octet overflow 10.0.0.256` | **Invalid** | — | — | Octet boundary overflow: exactly 256 |
| **TC-14** | `.192.168.1.1` | **Invalid** | — | — | Leading dot makes candidate token malformed |
| **TC-15** | `192.168.1.1:` | **Invalid** | — | — | Colon present without port digits |
| **TC-16** | `192.168.1.1::80` | **Invalid** | — | — | Double colon syntax error |
| **TC-17** | `192..168.1.1` | **Invalid** | — | — | Consecutive dots with missing octet |
| **TC-18** | `192.168.1.1:080` | **Invalid** | — | — | Leading zero in port number |
| **TC-19** | `10.0.0.00` | **Invalid** | — | — | Octet containing multiple zeros (`00`) |
| **TC-20** | `address 1.1.1.1 and 2.2.2.2 present` | **Invalid** | — | — | Multiple valid addresses on a single line |

---

## 2. Test Execution Verification

All 20 test cases were compiled against `main.cpp` using:
```bash
g++ -Wall -Wextra -pedantic -std=c++17 main.cpp -o ipparser
./ipparser < test_cases.txt
```
All outputs strictly match the required specification format with zero discrepancies.
