# A1: IPv4 Address Extractor from Noisy Text

**Course:** EECS 581 Fall 2026  
**Author:** Zema Samuel
**Language:** C++

A robust command-line tool that parses single lines of noisy, unstructured text to detect, extract, and strictly validate embedded IPv4 addresses (with optional ports). 

Implemented entirely using manual base-10 digit accumulation and custom character validation without conversion libraries (`stoi`, `atoi`, `sscanf`), address utilities (`inet_pton`), or regex engines.

---

## Repository Structure

```text
.
├── main.cpp          # Full C++ source code with extractIPv4() and CLI loop
├── AI_DISCLOSURE.md  # Detailed log of AI interactions, prompts, bugs, and fixes
├── test_cases.txt    # Input suite containing normal, edge, and malformed cases
└── README.md         # Project overview and build instructions
