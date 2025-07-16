#include <string>
#include <cctype>
#include <cstring>

// Minimal string implementation which reallocates on every append
class SimpleString {
    char* data = nullptr;
    size_t len = 0;
    size_t cap = 0;
public:
    SimpleString() = default;
    explicit SimpleString(const std::string& s) { *this = s; }
    SimpleString(const SimpleString& other) { *this = other; }
    SimpleString(SimpleString&& other) noexcept : data(other.data), len(other.len), cap(other.cap) {
        other.data = nullptr; 
        other.len = other.cap = 0;
    }
    ~SimpleString() { delete[] data; }
    SimpleString& operator=(const SimpleString& other) {
        if (this != &other) { resize(other.len); std::memcpy(data, other.data, len); }
        return *this;
    }
    SimpleString& operator=(const std::string& s) { resize(s.size()); std::memcpy(data, s.data(), len); return *this; }
    void resize(size_t n) {
        if (cap < n) {
            char* newd = new char[n];
            if (data) std::memcpy(newd, data, len);
            delete[] data;
            data = newd;
            cap = n;
        }
        len = n;
    }
    size_t size() const { return len; }
    SimpleString substr(size_t pos, size_t n) const { return SimpleString(std::string(data + pos, n)); }
    SimpleString substr(size_t pos) const { return SimpleString(std::string(data + pos, len - pos)); }
    void append(const SimpleString& s) {
        size_t old = len;
        resize(len + s.len);
        std::memcpy(data + old, s.data, s.len);
    }
    void append(const std::string& s) {
        size_t old = len;
        resize(len + s.size());
        std::memcpy(data + old, s.data(), s.size());
    }
    std::string str() const { return std::string(data, len); }
};

static bool iequals(const std::string &a, const std::string &b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(a[i])) !=
            std::tolower(static_cast<unsigned char>(b[i])))
            return false;
    }
    return true;
}

void search_replace(std::string &text, const std::string &find, const std::string &replace) {
    if (find.empty()) return; // early exit for empty find string
    SimpleString Left;  
    SimpleString Right(text);  
    size_t pos = 0;
    while ((pos = Right.substr(pos, Right.size()).str().find(find)) != std::string::npos) {
        Left.append(Right.substr(0, pos));
        Left.append(replace);
        Right = Right.substr(pos + find.size());
    }
    Left.append(Right.str()); // append any remaining text
    text = Left.str();
}

// explicit instantiation for benchmark usage
void search_replace(std::string&, const std::string&, const std::string&);