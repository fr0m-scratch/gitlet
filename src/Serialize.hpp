// Serialize.hpp
#ifndef SERIALIZE_HPP
#define SERIALIZE_HPP

#include <fstream>
#include <string>

template <typename T>
void serialize(const T& obj, const std::string& filename) {
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs) {
        throw std::runtime_error("Failed to open file for serialization: " + filename);
    }
    ofs.write(reinterpret_cast<const char*>(&obj), sizeof(obj));
}

template <typename T>
T deserialize(const std::string& filename) {
    T obj;
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) {
        throw std::runtime_error("Failed to open file for deserialization: " + filename);
    }
    ifs.read(reinterpret_cast<char*>(&obj), sizeof(obj));
    return obj;
}

#endif // SERIALIZE_HPP
