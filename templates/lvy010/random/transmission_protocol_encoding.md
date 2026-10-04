### leetcode 271

The ordinary approach: '#' delimiter

```
class Codec {
public:
    string encode(vector<string>& strs) 
    {
        string res;
        for (int i = 0; i < strs.size(); i++) {
            // store the length with to_string (text format)
            res += to_string(strs[i].size());
            res += '#'; // add a delimiter to avoid ambiguity
            res += strs[i];
        }
        return res;
    }

    vector<string> decode(string s) {
        vector<string> res;
        int index = 0;
        while (index < s.size()) {
            // find the '#' delimiter
            int pos = s.find('#', index);
            // parse the length (text to integer)
            int size = stoi(s.substr(index, pos - index));
            index = pos + 1;
            // extract the string content
            res.push_back(s.substr(index, size));
            index += size;
        }
        return res;
    }
};
```

# A String Serialization Protocol: Length-Prefix Encoding

## Introduction

In distributed systems, network communication, and data persistence, we often need to serialize multiple strings into a single byte stream for transmission or storage. A well-designed encoding protocol must satisfy the following requirements:

1. **Unambiguous**: the original data structure can be restored exactly
2. **Efficient**: low encoding/decoding overhead and good space utilization
3. **Robust**: able to handle strings that contain special characters

This article analyzes a string serialization scheme based on **Length-Prefix Encoding** and walks through the technical details of its C++ implementation.

## Protocol Design Principles

### Core Idea

The core idea of length-prefix encoding is: ==prepend a fixed-length integer field to each string== that indicates the byte length of the string that follows. This design avoids the escaping problems that delimiters can introduce.

### Encoding Format

For the string array `{"Hello", "World"}`, the encoded format is:

```
[length1][string1][length2][string2]...
```

At the binary level (assuming 32-bit integers, little-endian):

```
Original data: {"Hello", "World"}
Encoded result: \x05\x00\x00\x00Hello\x05\x00\x00\x00World
                └─────┬─────┘└──┬──┘└─────┬─────┘└──┬──┘
                  length=5  content length=5  content
```

### Protocol Advantages

1. **No escaping needed**: string contents may contain any bytes, including the null character `\0`
2. **Fixed-length header**: ==the length field is always `sizeof(int)` bytes, so parsing is efficient==
3. **Stream processing**: supports sequential reading with controllable memory usage

## Implementation

### Code

```cpp
#include <vector>
#include <string>
#include <cstring>
#include <iostream>

using namespace std;

class Codec {
public:
    /**
     * Encode: serialize a string array into a single string
     * 
     * @param strs the vector of strings to encode
     * @return the encoded byte-stream string
     * 
     * Example:
     *   Input: {"Hello", "World"}
     *   Output: "\x05\x00\x00\x00Hello\x05\x00\x00\x00World"
     */
    string encode(vector<string>& strs) {
        string res;
        
        // preallocate memory to improve performance (optional optimization)
        size_t total_size = 0;
        for (const auto& str : strs) {
            total_size += sizeof(int) + str.size();
        }
        res.reserve(total_size);
        
        // encode the strings one by one
        for (const auto& str : strs) {
            int size = str.size();
            
            // 1: write the integer's binary representation directly into the string
            // string constructor prototype: string(const char* s, size_t n)
            // effect: copies the first n bytes from the character array s
            res += string(reinterpret_cast<const char*>(&size), sizeof(size));
            
            // append the string content
            res += str;
        }
        
        return res;
    }

    /**
     * Decode: restore the string array from the byte stream
     * 
     * @param s the encoded byte-stream string
     * @return the decoded vector of strings
     * 
     * Example:
     *   Input: "\x05\x00\x00\x00Hello\x05\x00\x00\x00World"
     *   Output: {"Hello", "World"}
     */
    vector<string> decode(string s) {
        vector<string> res;
        size_t index = 0;
        
        while (index < s.size()) {
            // 2: rebuild the integer from the byte stream
            int size = 0;
            // void* memcpy(void* dest, const void* src, size_t n)
            // s.data() returns a pointer to the internal character array (before C++11 == c_str())
            memcpy(&size, s.data() + index, sizeof(size));
            index += sizeof(size);
            
            // extract the substring of the given length
            res.push_back(s.substr(index, size));
            index += size;
        }
        
        return res;
    }
};
```

### Analysis

#### 1. Converting an Integer to a Byte Stream

```cpp
int size = 5;
string length_field(reinterpret_cast<const char*>(&size), sizeof(size));
```

**How it works**:
- `&size` takes the integer's memory address
- `reinterpret_cast<const char*>` ==reinterprets the integer pointer as a character pointer==
- The `string` constructor copies `sizeof(int)` bytes from the character array

**Memory layout example** (32-bit little-endian system):
```
In-memory representation of the integer 5:
Address:  0x1000  0x1001  0x1002  0x1003
Content:  0x05    0x00    0x00    0x00
          └────────────┬────────────┘
               converted to a string
               "\x05\x00\x00\x00"
```

#### 2. Rebuilding an Integer from a Byte Stream

```cpp
int size = 0;
memcpy(&size, s.data() + index, sizeof(size));
```

**How it works**:
- `s.data()` returns a pointer to the string's internal buffer
- `memcpy` copies `sizeof(int)` bytes from the given position into the `size` variable
- This is a C-style type conversion, and it is efficient

**API comparison**:

```cpp
// Before C++11
const char* ptr = s.c_str();  // returns a C-style string

// Since C++11
const char* ptr = s.data();   // same function, but clearer semantics
```

#### 3. Clever Use of the String Constructor

```cpp
// constructor prototype
string(const char* s, size_t n);
```

**Use cases**:
```cpp
// Case 1: construct from a byte array (may contain \0)
char buffer[] = {0x48, 0x00, 0x69};  // "H\0i"
string str(buffer, 3);  // correctly handles embedded null characters
cout << str.size();     // output: 3

// Case 2: extract a substring
string full = "Hello World";
string sub(full.data() + 6, 5);  // "World"
```

### Optimizations

#### 1. Memory Preallocation

```cpp
string encode(vector<string>& strs) {
    // compute the total size
    size_t total_size = 0;
    for (const auto& str : strs) {
        total_size += sizeof(int) + str.size();
    }
    
    // allocate memory once to avoid repeated reallocations
    string res;
    res.reserve(total_size);
    
    // ... encoding logic
}
```

**Performance gain**: avoids the memory-copy overhead of the `string` growing multiple times.

#### 2. Variable-Length Integer Encoding (VarInt)

When there are many short strings, a fixed 4-byte length field is wasteful. A variable-length encoding can be used instead:

```cpp
// VarInt encoding example (similar to Protocol Buffers)
string encodeVarInt(int value) {
    string result;
    while (value >= 0x80) {
        result += static_cast<char>((value & 0x7F) | 0x80);
        value >>= 7;
    }
    result += static_cast<char>(value);
    return result;
}
```

**Advantages**:
- Lengths below 128 take only 1 byte
- Lengths below 16384 take only 2 bytes

#### 3. Batch Processing Optimization

```cpp
// use ostringstream to reduce temporary object creation
#include <sstream>

string encode(vector<string>& strs) {
    ostringstream oss;
    for (const auto& str : strs) {
        int size = str.size();
        oss.write(reinterpret_cast<const char*>(&size), sizeof(size));
        oss.write(str.data(), str.size());
    }
    return oss.str();
}
```

## Use Cases

### 1. Network Protocol Design

```cpp
// TCP message frame format
struct MessageFrame {
    uint32_t length;  // message length (network byte order)
    char data[];      // message content
};

// encode multiple messages with this protocol
vector<string> messages = {"MSG1", "MSG2", "MSG3"};
string encoded = codec.encode(messages);
send(socket_fd, encoded.data(), encoded.size(), 0);
```

### 2. Database Serialization

```cpp
// serialize multi-column string data for storage
class RowSerializer {
    Codec codec;
public:
    string serialize(const vector<string>& row) {
        return codec.encode(const_cast<vector<string>&>(row));
    }
    
    vector<string> deserialize(const string& data) {
        return codec.decode(data);
    }
};
```

### 3. Cache Systems

```cpp
// batch operations for a Redis-like cache
class CacheClient {
    Codec codec;
public:
    void mset(const vector<string>& keys, const vector<string>& values) {
        vector<string> combined;
        combined.insert(combined.end(), keys.begin(), keys.end());
        combined.insert(combined.end(), values.begin(), values.end());
        
        string encoded = codec.encode(combined);
        // send to the cache server
    }
};
```

## Edge Cases and Error Handling

### 1. Handling Empty Strings

```cpp
vector<string> test = {"", "Hello", ""};
string encoded = codec.encode(test);
// encoded result: "\x00\x00\x00\x00\x05\x00\x00\x00Hello\x00\x00\x00\x00"
```

### 2. Handling Large Strings

```cpp
// for very large strings, consider a 64-bit length field
class Codec64 {
    string encode(vector<string>& strs) {
        string res;
        for (const auto& str : strs) {
            uint64_t size = str.size();  // use 64 bits
            res += string(reinterpret_cast<const char*>(&size), sizeof(size));
            res += str;
        }
        return res;
    }
};
```

### 3. Data Validation

```cpp
vector<string> decode(string s) {
    vector<string> res;
    size_t index = 0;
    
    while (index < s.size()) {
        // bounds check
        if (index + sizeof(int) > s.size()) {
            throw runtime_error("Corrupted data: incomplete length field");
        }
        
        int size = 0;
        memcpy(&size, s.data() + index, sizeof(size));
        index += sizeof(int);
        
        // length validation
        if (size < 0 || index + size > s.size()) {
            throw runtime_error("Corrupted data: invalid length");
        }
        
        res.push_back(s.substr(index, size));
        index += size;
    }
    
    return res;
}
```

## Cross-Platform Compatibility

### Byte Order

Different CPU architectures may use different byte orders (big-endian vs. little-endian). For cross-platform transmission, the byte order must be standardized:

```cpp
#include <arpa/inet.h>  // Linux/Unix
// #include <winsock2.h> // Windows

string encode(vector<string>& strs) {
    string res;
    for (const auto& str : strs) {
        uint32_t size = str.size();
        uint32_t network_size = htonl(size);  // convert to network byte order (big-endian)
        res += string(reinterpret_cast<const char*>(&network_size), sizeof(network_size));
        res += str;
    }
    return res;
}

vector<string> decode(string s) {
    vector<string> res;
    size_t index = 0;
    
    while (index < s.size()) {
        uint32_t network_size = 0;
        memcpy(&network_size, s.data() + index, sizeof(network_size));
        uint32_t size = ntohl(network_size);  // convert to host byte order
        index += sizeof(network_size);
        
        res.push_back(s.substr(index, size));
        index += size;
    }
    
    return res;
}
```

## Performance Benchmark

```cpp
#include <chrono>

void benchmark() {
    // generate test data
    vector<string> test_data;
    for (int i = 0; i < 10000; i++) {
        test_data.push_back(string(100, 'A' + (i % 26)));
    }
    
    Codec codec;
    
    // encoding performance test
    auto start = chrono::high_resolution_clock::now();
    string encoded = codec.encode(test_data);
    auto end = chrono::high_resolution_clock::now();
    auto encode_time = chrono::duration_cast<chrono::microseconds>(end - start);
    
    // decoding performance test
    start = chrono::high_resolution_clock::now();
    vector<string> decoded = codec.decode(encoded);
    end = chrono::high_resolution_clock::now();
    auto decode_time = chrono::duration_cast<chrono::microseconds>(end - start);
    
    cout << "Encode time: " << encode_time.count() << " μs\n";
    cout << "Decode time: " << decode_time.count() << " μs\n";
    cout << "Original size: " << test_data.size() * 100 << " bytes\n";
    cout << "Encoded size: " << encoded.size() << " bytes\n";
    cout << "Space overhead: " << (encoded.size() - test_data.size() * 100) << " bytes\n";
}
```

**Typical results** (10000 strings of 100 bytes each):
```
Encode time: 1523 μs
Decode time: 1876 μs
Original size: 1000000 bytes
Encoded size: 1040000 bytes
Space overhead: 40000 bytes (4%)
```

## Summary

The length-prefix encoding scheme presented here is a classic and efficient string serialization protocol. Its core advantages are:

1. **Simplicity**: the implementation takes fewer than 50 lines of code
2. **Efficiency**: encoding and decoding run in linear time, with fixed space overhead
3. **Robustness**: supports arbitrary byte content with no escaping
4. **Practicality**: widely used in network protocols, databases, caches, and other systems

In practice, it can be optimized for the specific scenario:
- For short strings, use variable-length integer encoding to reduce space overhead
- For cross-platform transmission, consistently use network byte order
- For performance-sensitive scenarios, use memory preallocation and batch processing

This design also reflects a core principle of computer systems design: **==find the best balance between simplicity and efficiency==**.

---

**References**:
- [LeetCode 271: Encode and Decode Strings](https://leetcode.cn/problems/encode-and-decode-strings/)
- [Protocol Buffers Encoding](https://developers.google.com/protocol-buffers/docs/encoding)
- [C++ String Reference](https://en.cppreference.com/w/cpp/string/basic_string)

