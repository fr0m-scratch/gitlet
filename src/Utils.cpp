#include "Utils.h"

std::string Utils::sha1(const std::vector<std::string>& vals) {
    SHA_CTX context;
    if (!SHA1_Init(&context)) {
        throw std::runtime_error("Failed to initialize SHA");
    }

    for (const std::string& val : vals) {
        if (!SHA1_Update(&context, val.c_str(), val.size())) {
            throw std::runtime_error("Failed to update SHA");
        }
    }

    unsigned char hash[SHA_DIGEST_LENGTH];
    if (!SHA1_Final(hash, &context)) {
        throw std::runtime_error("Failed to finalize SHA");
    }

    return bytesToHexString(hash, SHA_DIGEST_LENGTH);
}

bool Utils::restrictedDelete(const std::string& file) {
    fs::path path(file);
    if (!fs::is_regular_file(path)) {
        return false;
    }
    if (!fs::is_directory(path.parent_path() / ".gitlet")) {
        throw std::invalid_argument("not .gitlet working directory");
    }
    return fs::remove(file);
}

std::vector<char> Utils::readContents(const std::string& file) {
    std::ifstream ifs(file, std::ios::binary | std::ios::ate);
    if (!ifs) {
        throw std::invalid_argument("must be a normal file");
    }

    auto end = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<char> bytes(end - ifs.tellg());

    if (!ifs.read(bytes.data(), bytes.size())) {
        throw std::invalid_argument("could not read file");
    }

    return bytes;
}

void Utils::writeContents(const std::string& file, const std::vector<char>& bytes) {
    std::ofstream ofs(file, std::ios::binary);
    if (!ofs) {
        throw std::invalid_argument("could not open file for writing");
    }

    if (!ofs.write(bytes.data(), bytes.size())) {
        throw std::invalid_argument("could not write to file");
    }
}

fs::path Utils::join(const std::string& first, const std::vector<std::string>& others) {
    fs::path path = first;
    for (const auto& part : others) {
        path /= part;
    }
    return path;
}

