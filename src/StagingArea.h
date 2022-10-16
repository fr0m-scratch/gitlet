#ifndef STAGINGAREA_H
#define STAGINGAREA_H

#include <unordered_map>
#include <vector>
#include <string>

class StagingArea {
public:
    StagingArea();

    void add(const std::string& fileName, const std::string& sha1);
    void addToRemovedFiles(const std::string& fileName);
    void clear();

    std::unordered_map<std::string, std::string> getAddedFiles() const;
    std::vector<std::string> getRemovedFiles() const;

private:
    std::unordered_map<std::string, std::string> addedFiles;
    std::vector<std::string> removedFiles;
};

#endif // STAGINGAREA_H
