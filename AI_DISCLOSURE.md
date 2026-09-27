# Generative AI (GAI) Usage & Disclosure Document

## 1. General Disclosure
* **Tool Used:** Anthropic Claude (Claude 3.5 Sonnet) & OpenAI ChatGPT / Gemini
* **Consultation Date:** September 27, 2026
* **Target Artifact:** IPv4 Parsing & Validation Program (`A1: Extracting IPv4 Addresses from Noisy Text`)

---

## 2. Prompts Used
The following prompts were executed in sequence across generation, testing, and debugging phases:

1. **Initial Architecture Prompt:**
   > *"Act as an expert C++ systems programmer. I need a C++ solution that extracts an IPv4 address (and optional port) embedded within noisy text. Prototype: `bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort)`. Disallow all conversion functions (`atoi`, `stoi`, `sscanf`, etc.) and regex. All digit accumulation must be manual. Grammar rules: octet 0-255, no leading zeros unless single '0', port 0-65535, exactly one match per line, handle non-token delimiters."*

2. **Test Generation Prompt:**
   > *"Generate a comprehensive suite of edge-case test cases in Markdown format for testing boundary limits (0.0.0.0, 255.255.255.255), leading zeros, malformed ports, adjacent token characters, and near-miss garbage text."*

3. **Targeted Bug Fix Prompts:**
   * **Trailing Delimiter Bug:** *"When evaluating `192.168.1.1.`, the parser matches `192.168.1.1` and skips the final dot. The assignment strictly forbids partial matching within candidate token sequences. Fix the token isolator so maximal sequences of `[0-9.:]` are isolated first."*
   * **Malformed Port Invalidation Bug:** *"When testing `1.2.3.4:99999`, the parser discarded the port and emitted the address with port 'none'. The assignment dictates that if a colon is present, a malformed port must reject the entire token. Refactor the parser to strictly fail."*

---

## 3. Code Attribution & Modifications

| Section | Origin | Modifications / Student Contribution |
| :--- | :--- | :--- |
| `isTokenChar()` | AI-Generated | Verified inline efficiency and `unsigned char` safety casting. |
| `validateAndParseToken()` | Mixed (AI + Student) | **Corrected by student:** Added strict token-exhaustion check `if (i != n) return false;` to catch trailing dots (`.`) and extra symbols. Enforced leading-zero rejection. |
| `extractIPv4()` | Mixed (AI + Student) | **Refactored by student:** Swapped greedy sub-scanning for a two-stage maximal-token partition algorithm to guarantee no partial extractions occur on malformed boundaries. |
| `main()` Loop & Formatting | AI-Generated | Integrated bit-shift unpackers `((address >> 24) & 0xFF)` to format standard output as specified in sample runs. |

---

## 4. Specific Bugs Identified & Debugging Log

### Bug 1: Partial Token Matching on Trailing Dot (`192.168.1.1.`)
* **Symptom:** Entering `192.168.1.1.` outputted `Extracted IPv4 address: 192.168.1.1`.
* **Root Cause:** The AI's initial scanning loop stopped reading after 4 octets were parsed, leaving the remaining `.` in the stream for the next iteration rather than invalidating the entire candidate.
* **Resolution:** Implemented maximal token extraction: every contiguous sequence consisting of `[0-9.:]` is carved out first. If that isolated token string contains any trailing character, `validateAndParseToken()` returns `false`.

### Bug 2: Decoupled Port Validation (`1.2.3.4:99999`)
* **Symptom:** Entering `1.2.3.4:99999` yielded valid IP `1.2.3.4` with port `none`.
* **Root Cause:** The initial AI logic treated the port parser as a separate non-fatal step: if the port failed its bounds check, it simply reverted `outPort` to `-1` and returned `true` for the address.
* **Resolution:** Rewrote branch logic so that if `token[i] == ':'`, any failure in port digits or numeric range (`> 65535`) causes `validateAndParseToken()` to return `false` immediately, invalidating the entire candidate token.

### Bug 3: Leading Zero Blind Spot (`192.168.01.1`)
* **Symptom:** Initial code accepted octets with leading zeros (`01`, `007`).
* **Root Cause:** Standard accumulation `val = val * 10 + digit` naturally computes `01` as `1` without recording the digit count.
* **Resolution:** Added `digitCount > 1 && token[digitStart] == '0'` checks to both octet and port parsers to reject leading zeros.

---

## 5. Verification Statement

I confirm that I understand every line of the submitted code. The program has been thoroughly compiled with `-Wall -Wextra -pedantic` and tested against all assignment sample runs and edge boundary conditions. All manual conversion constraints and formatting specifications are fully met.
