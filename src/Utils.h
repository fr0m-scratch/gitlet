#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>

namespace fs = std::filesystem;

class Utils {
public:

    static std::string sha1(const std::vector<char>& vals);
    
    static bool restrictedDelete(const std::string& file);

    static std::vector<char> readContents(const std::string& file);
    static void writeContents(const std::string& file, const std::vector<char>& bytes);

    static fs::path join(const std::string& first, const std::vector<std::string>& others);

    static std::vector<std::string> plainFilenamesIn(const std::string& dir);

private:
    static std::string bytesToHexString(const unsigned char* bytes, size_t length);
};

#endif // UTILS_H
