#include "Repo.h"
#include <boost/serialization/unordered_map.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <unordered_set> 

namespace fs = std::filesystem;

Repo::Repo() : HEAD("master"), workingDir(fs::current_path()) {
    std::string pathToHead = ".gitlet/branches/HEAD.txt";
    if (fs::exists(pathToHead)) {
        std::ifstream inFile(pathToHead);
        std::getline(inFile, HEAD);
        inFile.close();
    }
    std::string pathToStage = ".gitlet/staging/stage.txt";
    if (fs::exists(pathToStage)) {
        std::ifstream ifs(pathToStage);
        boost::archive::text_iarchive ia(ifs);
        ia >> stage;
        ifs.close();
    }
}

void Repo::init() {
    fs::path gitPath = fs::current_path() / ".gitlet";
    if (fs::exists(gitPath)) {
        std::cout << "A gitlet version-control system already exists in the current directory." << std::endl;
        return;
    }

    fs::create_directories(gitPath / "blobs");
    fs::create_directories(gitPath / "branches");
    fs::create_directories(gitPath / "commits");
    fs::create_directories(gitPath / "staging");
    fs::create_directories(gitPath / "global-log");

    Commit initialCommit("initial commit", std::unordered_map<std::string, std::string>(), "");
    std::string commitHash = initialCommit.getOwnHash();

    std::ofstream ofs(".gitlet/commits/" + commitHash + ".txt");
    boost::archive::text_oarchive oa(ofs);
    oa << initialCommit;
    ofs.close();

    std::string pathToMaster = ".gitlet/branches/master.txt";
    std::ofstream masterFile(pathToMaster);
    masterFile << commitHash;
    masterFile.close();

    std::string pathToHead = ".gitlet/branches/HEAD.txt";
    std::ofstream headFile(pathToHead);
    headFile << "master";
    headFile.close();

    stage = StagingArea();
    std::string pathToStage = ".gitlet/staging/stage.txt";
    std::ofstream stageFile(pathToStage);
    boost::archive::text_oarchive stage_oa(stageFile);
    stage_oa << stage;
    stageFile.close();

    std::cout << "Initialized an empty gitlet repository in " << gitPath << std::endl;
}

std::string Repo::getHEAD() const {
    return HEAD;
}

StagingArea Repo::getStage() const {
    return stage;
}

void Repo::add(const std::string& fileName) {
    fs::path filePath = findFile(fileName, workingDir);
    if (!fs::exists(filePath)) {
        std::cout << "File does not exist." << std::endl;
        return;
    }

    std::ifstream ifs(filePath, std::ios::binary);
    std::vector<char> blob((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();

    std::string blobHash = Utils::sha1(blob);
    Commit currentCommit = getCurrentCommit();
    if (currentCommit.getBlobs().find(fileName) != currentCommit.getBlobs().end() &&
        currentCommit.getBlobs().at(fileName) == blobHash) {
        if (stage.getRemovedFiles().find(fileName) != stage.getRemovedFiles().end()) {
            stage.getRemovedFiles().erase(fileName);
            serializeStage();
        }
        return;
    }

    if (stage.getRemovedFiles().find(fileName) != stage.getRemovedFiles().end()) {
        stage.getRemovedFiles().erase(fileName);
    }

    fs::path blobPath = workingDir / ".gitlet/blobs" / (blobHash + ".txt");
    std::ofstream blobFile(blobPath, std::ios::binary);
    blobFile.write(blob.data(), blob.size());
    blobFile.close();

    stage.add(fileName, blobHash);
    serializeStage();
}

void Repo::commitment(const std::string& msg) {
    if (stage.getAddedFiles().empty() && stage.getRemovedFiles().empty()) {
        std::cout << "No changes added to the commit." << std::endl;
        return;
    } else if (msg.empty()) {
        std::cout << "Please enter a commit message." << std::endl;
        return;
    }

    Commit curr = getCurrentCommit();
    std::unordered_map<std::string, std::string> copiedBlobs = curr.getBlobs();
    for (const auto& file : stage.getAddedFiles()) {
        copiedBlobs[file.first] = file.second;
    }
    for (const auto& fileToRemove : stage.getRemovedFiles()) {
        copiedBlobs.erase(fileToRemove);
    }

    Commit newCommit(msg, copiedBlobs, curr.getOwnHash());
    std::string newCommitHash = newCommit.getOwnHash();
    std::ofstream ofs(".gitlet/commits/" + newCommitHash + ".txt");
    boost::archive::text_oarchive oa(ofs);
    oa << newCommit;
    ofs.close();

    fs::path branchPath = ".gitlet/branches/" + HEAD + ".txt";
    std::ofstream branchFile(branchPath);
    branchFile << newCommitHash;
    branchFile.close();

    stage.clear();
    serializeStage();
}

void Repo::rm(const std::string& fileName) {
    bool isStaged = stage.getAddedFiles().find(fileName) != stage.getAddedFiles().end();
    Commit curr = getCurrentCommit();
    bool isTracked = curr.getBlobs().find(fileName) != curr.getBlobs().end();

    if (isTracked) {
        fs::remove(fileName);
        stage.addToRemovedFiles(fileName);
        if (isStaged) {
            stage.getAddedFiles().erase(fileName);
        }
        serializeStage();
    } else if (isStaged) {
                stage.getAddedFiles().erase(fileName);
        serializeStage();
    } else {
        std::cout << "No reason to remove the file." << std::endl;
    }
}

void Repo::log() const {
    Commit curr = getCurrentCommit();
    while (curr.getOwnHash() != "") {
        std::cout << "===" << std::endl;
        std::cout << "Commit " << curr.getOwnHash() << std::endl;
        std::cout << curr.getDatetime() << std::endl;
        std::cout << curr.getMessage() << std::endl;
        std::cout << std::endl;
        if (!curr.getParentHash().empty()) {
            curr = deserializeCommit(".gitlet/commits/" + curr.getParentHash() + ".txt");
        } else {
            break;
        }
    }
}

void Repo::global() const {
    // Assuming the global log is maintained in a single file
    std::ifstream inFile(".gitlet/global-log/gl.txt");
    std::string line;
    while (std::getline(inFile, line)) {
        std::cout << line << std::endl;
    }
}

void Repo::find(const std::string& msg) const {
    fs::path commitFolder = workingDir / ".gitlet/commits";
    int printCount = 0;

    for (const auto& entry : fs::directory_iterator(commitFolder)) {
        Commit currCommit = deserializeCommit(entry.path().string());
        if (currCommit.getMessage() == msg) {
            std::cout << currCommit.getOwnHash() << std::endl;
            ++printCount;
        }
    }

    if (printCount == 0) {
        std::cout << "Found no commit with that message." << std::endl;
    }
}

void Repo::status() const {
    // Listing branches
    std::vector<std::string> branches;
    for (const auto& entry : fs::directory_iterator(".gitlet/branches")) {
        std::string branchName = entry.path().filename().string();
        if (branchName != "HEAD.txt") {
            branchName = branchName.substr(0, branchName.size() - 4); // Remove .txt extension
            branches.push_back(branchName == HEAD ? "*" + branchName : branchName);
        }
    }
    std::sort(branches.begin(), branches.end());

    // Listing staged files
    std::vector<std::string> stagedFiles;
    for (const auto& file : stage.getAddedFiles()) {
        stagedFiles.push_back(file.first);
    }
    std::sort(stagedFiles.begin(), stagedFiles.end());

    // Listing removed files
    std::vector<std::string> removedFiles(stage.getRemovedFiles().begin(), stage.getRemovedFiles().end());
    std::sort(removedFiles.begin(), removedFiles.end());

    // Displaying the status
    std::cout << "=== Branches ===" << std::endl;
    for (const auto& branch : branches) {
        std::cout << branch << std::endl;
    }
    std::cout << std::endl;

    std::cout << "=== Staged Files ===" << std::endl;
    for (const auto& staged : stagedFiles) {
        std::cout << staged << std::endl;
    }
    std::cout << std::endl;

    std::cout << "=== Removed Files ===" << std::endl;
    for (const auto& removed : removedFiles) {
        std::cout << removed << std::endl;
    }
    std::cout << std::endl;

    // Implementations for "Modifications Not Staged For Commit" and "Untracked Files" sections can be added here
    std::cout << "=== Modifications Not Staged For Commit ===" << std::endl;
    std::cout << std::endl;
    std::cout << "=== Untracked Files ===" << std::endl;
}

// Helper methods for serialization and deserialization
void Repo::serializeStage() {
    std::ofstream stageFile(".gitlet/staging/stage.txt");
    boost::archive::text_oarchive stage_oa(stageFile);
    stage_oa << stage;
    stageFile.close();
}

Commit Repo::deserializeCommit(const std::string& path) const {
    std::ifstream ifs(path);
    boost::archive::text_iarchive ia(ifs);
    Commit commit;
    ia >> commit;
    ifs.close();
    return commit;
}

void Repo::checkout(const std::vector<std::string>& args) {
    if (args.size() == 2) {
        std::string branchName = args[1];
        fs::path branchPath = ".gitlet/branches/" + branchName + ".txt";
        if (!fs::exists(branchPath)) {
            std::cout << "No such branch exists." << std::endl;
            return;
        }

        std::string newCommitID = Utils::readStringFromFile(branchPath.string());
        Commit newCommit = deserializeCommit(".gitlet/commits/" + newCommitID + ".txt");
        Commit currCommit = getCurrentCommit();

        for (const auto& file : fs::directory_iterator(workingDir)) {
            std::string fileName = file.path().filename().string();
            if (fileName.ends_with(".txt") && 
                !currCommit.getBlobs().contains(fileName) &&
                newCommit.getBlobs().contains(fileName)) {
                std::cout << "There is an untracked file in the way; delete it or add it first." << std::endl;
                return;
            }
        }

        for (const auto& file : fs::directory_iterator(workingDir)) {
            std::string fileName = file.path().filename().string();
            if (!newCommit.getBlobs().contains(fileName) &&
                currCommit.getBlobs().contains(fileName)) {
                fs::remove(file.path());
            }
        }

        for (const auto& [fileName, blobHash] : newCommit.getBlobs()) {
            fs::path blobPath = workingDir / ".gitlet/blobs" / (blobHash + ".txt");
            std::vector<char> blobBytes = Utils::readContents(blobPath);
            fs::path newFilePath = workingDir / fileName;
            Utils::writeContents(newFilePath, blobBytes);
        }

        stage.clear();
        serializeStage();
        Utils::writeStringToFile(branchName, ".gitlet/branches/HEAD.txt");
    } else if (args.size() == 3) {
        std::string fileName = args[2];
        Commit headCommit = getCurrentCommit();
        auto commitMap = headCommit.getBlobs();

        if (commitMap.find(fileName) == commitMap.end()) {
            std::cout << "File does not exist in that commit." << std::endl;
            return;
        }

        fs::path filePath = workingDir / fileName;
        if (fs::exists(filePath)) {
            fs::remove(filePath);
        }

        fs::path blobPath = workingDir / ".gitlet/blobs" / (commitMap[fileName] + ".txt");
        std::vector<char> blobBytes = Utils::readContents(blobPath);
        Utils::writeContents(filePath, blobBytes);
    } else if (args.size() == 4) {
        std::string commitID = args[1];
        std::string fileName = args[3];

        fs::path commitsPath = workingDir / ".gitlet/commits";
        for (const auto& entry : fs::directory_iterator(commitsPath)) {
            std::string identifier = entry.path().filename().string();
            if (identifier.find(commitID) != std::string::npos) {
                commitID = identifier.substr(0, identifier.size() - 4);
                break;
            }
        }

        fs::path commitFilePath = commitsPath / (commitID + ".txt");
        if (!fs::exists(commitFilePath)) {
            std::cout << "No commit with that id exists." << std::endl;
            return;
        }

        Commit currCommit = deserializeCommit(commitFilePath.string());
        if (currCommit.getBlobs().find(fileName) == currCommit.getBlobs().end()) {
            std::cout << "File does not exist in that commit." << std::endl;
            return;
        }

        fs::path filePath = workingDir / fileName;
        if (fs::exists(filePath)) {
            fs::remove(filePath);
        }

        fs::path blobPath = workingDir / ".gitlet/blobs" / (currCommit.getBlobs().at(fileName) + ".txt");
        std::vector<char> blobBytes = Utils::readContents(blobPath);
        Utils::writeContents(filePath, blobBytes);
    }
}

void Repo::branch(const std::string& branchName) {
    fs::path branchPath = ".gitlet/branches/" + branchName + ".txt";
    if (fs::exists(branchPath)) {
        std::cout << "A branch with that name already exists." << std::endl;
        return;
    }

    std::string sha1 = Utils::readStringFromFile(".gitlet/branches/" + HEAD + ".txt");
    Utils::writeStringToFile(sha1, branchPath.string(), false);
}

void Repo::rmb(const std::string& branchName) {
    if (branchName == Utils::readStringFromFile(".gitlet/branches/HEAD.txt")) {
        std::cout << "Cannot remove the current branch." << std::endl;
        return;
    }

    fs::path branchPath = ".gitlet/branches/" + branchName + ".txt";
    if (!fs::remove(branchPath)) {
        std::cout << "A branch with that name does not exist." << std::endl;
    }
}

void Repo::reset(const std::string& commitID) {
    Commit commitToCheckout = deserializeCommit(".gitlet/commits/" + commitID + ".txt");
    if (commitToCheckout.getOwnHash().empty()) {
        std::cout << "No commit with that id exists." << std::endl;
        return;
    }

    std::vector<fs::path> fileList;
    for (const auto& file : fs::directory_iterator(workingDir)) {
        if (file.path().filename().string().ends_with(".txt")) {
            fileList.push_back(file.path());
        }
    }

    Commit currCommit = getCurrentCommit();
    for (const auto& file : fileList) {
        std::string fileName = file.filename().string();
        if (!currCommit.getBlobs().contains(fileName) &&
            commitToCheckout.getBlobs().contains(fileName)) {
            std::cout << "There is an untracked file in the way; delete it or add it first." << std::endl;
            return;
        }
    }

    for (const auto& file : fileList) {
        std::string fileName = file.filename().string();
        if (!commitToCheckout.getBlobs().contains(fileName) &&
            currCommit.getBlobs().contains(fileName)) {
            fs::remove(file);
        }
    }

    for (const auto& [fileName, blobHash] : commitToCheckout.getBlobs()) {
        fs::path blobPath = workingDir / ".gitlet/blobs" / (blobHash + ".txt");
        std::vector<char> blobBytes = Utils::readContents(blobPath);
        fs::path newFilePath = workingDir / fileName;
        Utils::writeContents(newFilePath, blobBytes);
    }

    stage.clear();
    serializeStage();
    Utils::writeStringToFile(commitID, ".gitlet/branches/" + HEAD + ".txt", false);
}

void Repo::merge(const std::string& branchName) {
    if (branchName == HEAD) {
        std::cout << "Cannot merge a branch with itself." << std::endl;
        return;
    }

    if (!stage.getAddedFiles().empty() || !stage.getRemovedFiles().empty()) {
        std::cout << "You have uncommitted changes." << std::endl;
        return;
    }

    fs::path branchPath = ".gitlet/branches/" + branchName + ".txt";
    if (!fs::exists(branchPath)) {
        std::cout << "A branch with that name does not exist." << std::endl;
        return;
    }

    Commit currentCommit = getCurrentCommit();
    Commit branchCommit = deserializeCommit(".gitlet/commits/" + Utils::readStringFromFile(branchPath.string()));
    Commit splitPoint = findSplitPoint(currentCommit, branchCommit);

    if (splitPoint.getOwnHash() == branchCommit.getOwnHash()) {
        std::cout << "Given branch is an ancestor of the current branch." << std::endl;
        return;
    }
    if (splitPoint.getOwnHash() == currentCommit.getOwnHash()) {
        checkout({branchName});
        std::cout << "Current branch fast-forwarded." << std::endl;
        return;
    }

    bool conflict = false;
    for (const auto& [fileName, branchBlobHash] : branchCommit.getBlobs()) {
        bool inCurrent = currentCommit.getBlobs().find(fileName) != currentCommit.getBlobs().end();
        bool inSplit = splitPoint.getBlobs().find(fileName) != splitPoint.getBlobs().end();
        std::string currentBlobHash = inCurrent ? currentCommit.getBlobs().at(fileName) : "";
        std::string splitBlobHash = inSplit ? splitPoint.getBlobs().at(fileName) : "";

        if ((inCurrent && inSplit && currentBlobHash != splitBlobHash && branchBlobHash != splitBlobHash) ||
            (inCurrent && !inSplit && branchBlobHash != currentBlobHash) ||
            (!inCurrent && inSplit && branchBlobHash != splitBlobHash)) {
            conflict = true;
            handleConflict(fileName, currentBlobHash, branchBlobHash);
        } else if (!inCurrent && !inSplit) {
            checkoutFile(branchCommit, fileName);
            stage.add(fileName, branchBlobHash);
        } else if (!inSplit && inCurrent) {
            // File only in current branch, do nothing
        } else if (inSplit && !inCurrent && !branchCommit.getBlobs().contains(fileName)) {
            // File present at split point, unmodified in given branch, and absent in current branch
            fs::remove(workingDir / fileName);
            stage.removeFile(fileName);
        }
    }

    if (conflict) {
        std::cout << "Encountered a merge conflict." << std::endl;
    } else {
        std::string message = "Merged " + branchName + " into " + HEAD + ".";
        commitment(message);
    }
}

void Repo::handleConflict(const std::string& fileName, const std::string& currentBlobHash, const std::string& branchBlobHash) {
    std::string currentContents = currentBlobHash.empty() ? "" : Utils::readContents(workingDir / ".gitlet/blobs" / (currentBlobHash + ".txt"));
    std::string branchContents = branchBlobHash.empty() ? "" : Utils::readContents(workingDir / ".gitlet/blobs" / (branchBlobHash + ".txt"));

    std::string conflictData = "<<<<<<< HEAD\n" + currentContents + 
                               "=======\n" + branchContents + 
                               ">>>>>>>\n";

    Utils::writeContents(workingDir / fileName, conflictData);
    stage.add(fileName, Utils::sha1(conflictData));
}

void Repo::checkoutFile(const Commit& commit, const std::string& fileName) {
    std::string blobHash = commit.getBlobs().at(fileName);
    std::string fileContent = Utils::readContents(workingDir / ".gitlet/blobs" / (blobHash + ".txt"));
    Utils::writeContents(workingDir / fileName, fileContent);
}

Commit Repo::findSplitPoint(const Commit& currentCommit, const Commit& branchCommit) {
    // Assuming there is a method to get all ancestor commits of a given commit
    std::unordered_set<std::string> currentAncestors = getAllAncestors(currentCommit);
    std::unordered_set<std::string> branchAncestors = getAllAncestors(branchCommit);

    for (const auto& ancestorHash : currentAncestors) {
        if (branchAncestors.find(ancestorHash) != branchAncestors.end()) {
            return deserializeCommit(".gitlet/commits/" + ancestorHash + ".txt");
        }
    }

    return Commit(); // Return an empty commit if no common ancestor is found
}

std::unordered_set<std::string> Repo::getAllAncestors(const Commit& commit) {
    std::unordered_set<std::string> ancestors;
    std::queue<std::string> toVisit;
    toVisit.push(commit.getOwnHash());

    while (!toVisit.empty()) {
        std::string currentHash = toVisit.front();
        toVisit.pop();
        ancestors.insert(currentHash);

        Commit currentCommit = deserializeCommit(".gitlet/commits/" + currentHash + ".txt");
        if (!currentCommit.getParentHash().empty()) {
            toVisit.push(currentCommit.getParentHash());
        }
    }

    return ancestors;
}

