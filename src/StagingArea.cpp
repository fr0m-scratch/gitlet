#include "StagingArea.h"

StagingArea::StagingArea() : addedFiles(), removedFiles() {}

void StagingArea::add(const std::string& fileName, const std::string& sha1) {
    addedFiles[fileName] = sha1;
}

void StagingArea::addToRemovedFiles(const std::string& fileName) {
    removedFiles.push_back(fileName);
}

void StagingArea::clear() {
    addedFiles.clear();
    removedFiles.clear();
}

std::unordered_map<std::string, std::string> StagingArea::getAddedFiles() const {
    return addedFiles;
}

std::vector<std::string> StagingArea::getRemovedFiles() const {
    return removedFiles;
}
