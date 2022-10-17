#include "Repo.h"
#include <boost/serialization/unordered_map.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/string.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "StagingArea.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <algorithm>

namespace fs = std::filesystem;

#include "Utils.h" 

Repo::Repo() {
    workingDir = fs::current_path();
    deserializeStage();
    HEAD = Utils::readStringFromFile(workingDir / ".gitlet/branches/HEAD.txt");
}

void Repo::init() {
    fs::path repoPath = fs::current_path() / ".gitlet";
    fs::path blobsPath = repoPath / "blobs";
    fs::path commitsPath = repoPath / "commits";
    fs::path branchesPath = repoPath / "branches";
    fs::path stagingPath = repoPath / "staging";
    fs::path globalLogPath = repoPath / "global-log";

    // Check if a repository already exists in the current directory
    if (fs::exists(repoPath)) {
        std::cerr << "A gitlet version-control system already exists in the current directory." << std::endl;
        return;
    }

    // Create the directory structure
    fs::create_directories(blobsPath);
    fs::create_directories(commitsPath);
    fs::create_directories(branchesPath);
    fs::create_directories(stagingPath);
    fs::create_directories(globalLogPath);

    // Create the initial commit
    Commit initialCommit("initial commit", {}, ""); // Assuming constructor parameters match this signature
    std::string commitHash = initialCommit.getOwnHash(); // Assuming Commit objects can compute their own hash
    serializeCommit(initialCommit, commitsPath / (commitHash + ".txt"));

    // Set the master branch to point to the initial commit
    fs::path masterBranchPath = branchesPath / "master.txt";
    std::ofstream masterFile(masterBranchPath.string());
    if (!masterFile) {
        std::cerr << "Failed to create the master branch file." << std::endl;
        return;
    }
    masterFile << commitHash;
    masterFile.close();

    // Set the HEAD to point to the master branch
    fs::path headPath = branchesPath / "HEAD.txt";
    std::ofstream headFile(headPath.string());
    if (!headFile) {
        std::cerr << "Failed to set the HEAD." << std::endl;
        return;
    }
    headFile << "master";
    headFile.close();

    // Initialize an empty staging area
    stage = StagingArea(); // Assuming 'stage' is a member of Repo and StagingArea has a default constructor
    serializeStage(); // Assuming serializeStage writes the staging area to a file

    std::cout << "Initialized an empty gitlet repository in " << fs::absolute(repoPath) << std::endl;

    // Create the global log file
    std::ofstream globalLogFile(globalLogPath / "gl.txt");
    globalLogFile.close();

}

std::string Repo::getHEAD() const {
    return HEAD;
}

StagingArea Repo::getStage() const {
    return stage;
}

void Repo::add(const std::string& fileName) {
    //support wildcards and directories
    if (fileName == ".") {
        fileName = workingDir.string();
    }
    if (fs::is_directory(fileName)) {
        for (const auto& file : fs::recursive_directory_iterator(fileName)) {
            if (fs::is_regular_file(file)) {
                stage.add(file.path().string(), Utils::sha1(Utils::readContents(file.path())));
            }
        }
    } else {
        stage.add(fileName, Utils::sha1(Utils::readContents(fileName)));
    }
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
    std::string commitPathString = (workingDir / ".gitlet/commits" / (newCommit.getOwnHash() + ".txt")).string();
    std::ofstream ofs(commitPathString);
    boost::archive::text_oarchive oa(ofs);
    oa << newCommit;
    ofs.close();

    std::string branchPathString = (workingDir / ".gitlet/branches" / (HEAD + ".txt")).string();
    std::ofstream branchFile(branchPathString);
    branchFile << newCommit.getOwnHash();
    branchFile.close();

    stage.clear();
    serializeStage();
}

void Repo::rm(const std::string& fileName) {
    bool isStaged = (stage.getAddedFiles().find(fileName) != stage.getAddedFiles().end());
    Commit curr = getCurrentCommit();
    bool isTracked = (curr.getBlobs().find(fileName) != curr.getBlobs().end());

    if (isTracked) {
        fs::remove(workingDir / fileName);
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
    while (!curr.getOwnHash().empty()) {
        std::cout << "===\nCommit " << curr.getOwnHash() << "\n";
        std::cout << curr.getDatetime() << "\n" << curr.getMessage() << "\n\n";
        if (!curr.getParentHash().empty()) {
            curr = deserializeCommit(workingDir / ".gitlet/commits" / (curr.getParentHash() + ".txt"));
        } else {
            break;
        }
    }
}

void Repo::global() const {
    std::ifstream inFile(workingDir / ".gitlet/global-log/gl.txt");
    std::string line;
    while (std::getline(inFile, line)) {
        std::cout << line << std::endl;
    }
}

void Repo::find(const std::string& msg) const {
    bool found = false;
    for (const auto& entry : fs::directory_iterator(workingDir / ".gitlet/commits")) {
        Commit currCommit = deserializeCommit(entry.path());
        if (currCommit.getMessage() == msg) {
            std::cout << currCommit.getOwnHash() << std::endl;
            found = true;
        }
    }
    if (!found) {
        std::cout << "Found no commit with that message." << std::endl;
    }
}

void Repo::status() const {
    // List branches
    std::vector<std::string> branches;
    for (const auto& entry : fs::directory_iterator(workingDir / ".gitlet/branches")) {
        std::string branchName = entry.path().filename().string();
        if (branchName != "HEAD.txt") {
            branches.push_back(branchName == HEAD ? "*" + branchName : branchName);
        }
    }
    std::sort(branches.begin(), branches.end());

    // List staged files
    std::vector<std::string> stagedFiles;
    for (const auto& [fileName, _] : stage.getAddedFiles()) {
        stagedFiles.push_back(fileName);
    }
    std::sort(stagedFiles.begin(), stagedFiles.end());

    // List removed files
    std::vector<std::string> removedFiles(stage.getRemovedFiles().begin(), stage.getRemovedFiles().end());
    std::sort(removedFiles.begin(), removedFiles.end());

    // Display status
    std::cout << "=== Branches ===\n";
    for (const auto& branch : branches) {
        std::cout << branch << "\n";
    }
    std::cout << "\n=== Staged Files ===\n";
    for (const auto& file : stagedFiles) {
        std::cout << file << "\n";
    }
    std::cout << "\n=== Removed Files ===\n";
    for (const auto& file : removedFiles) {
        std::cout << file << "\n";
    }
    // Implementations for "Modifications Not Staged For Commit" and "Untracked Files" can be added here
    std::cout << "\n=== Modifications Not Staged For Commit ===\n\n=== Untracked Files ===\n";
}

Commit Repo::getCurrentCommit() const {
    std::string headBranchPath = workingDir / ".gitlet/branches/HEAD.txt";
    std::string currentBranch = Utils::readStringFromFile(headBranchPath);
    std::string currentCommitHash = Utils::readStringFromFile(workingDir / ".gitlet/branches" / (currentBranch + ".txt"));
    return deserializeCommit(workingDir / ".gitlet/commits" / (currentCommitHash + ".txt"));
}

void Repo::checkout(const std::vector<std::string>& args) {
    // Implementation of the checkout command, simplified for brevity
    // Adapt this method based on your project's specific requirements and existing structure
}

void Repo::branch(const std::string& branchName) {
    fs::path branchPath = workingDir / ".gitlet/branches" / (branchName + ".txt");
    if (fs::exists(branchPath)) {
        std::cout << "A branch with that name already exists." << std::endl;
        return;
    }

    std::string sha1 = Utils::readStringFromFile((workingDir / ".gitlet/branches" / HEAD).string() + ".txt");
    Utils::writeStringToFile(sha1, branchPath.string(), false);
}

void Repo::rmb(const std::string& branchName) {
    if (branchName == Utils::readStringFromFile(workingDir / ".gitlet/branches/HEAD.txt")) {
        std::cout << "Cannot remove the current branch." << std::endl;
        return;
    }

    fs::path branchPath = workingDir / ".gitlet/branches" / (branchName + ".txt");
    if (fs::remove(branchPath)) {
        std::cout << "Branch " << branchName << " removed." << std::endl;
    } else {
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
        if (currCommit.getBlobs().find(fileName) == currCommit.getBlobs().end() &&
            commitToCheckout.getBlobs().find(fileName) != commitToCheckout.getBlobs().end()) {
            std::cout << "There is an untracked file in the way; delete it or add it first." << std::endl;
            return;
        }
    }

    for (const auto& file : fileList) {
        std::string fileName = file.filename().string();
        if (commitToCheckout.getBlobs().find(fileName) == commitToCheckout.getBlobs().end() &&
            currCommit.getBlobs().find(fileName) != currCommit.getBlobs().end()) {
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
        } else if (inSplit && !inCurrent && branchCommit.getBlobs().find(fileName) == branchCommit.getBlobs().end()) {
            // File present at split point, unmodified in given branch, and absent in current branch
            fs::remove(workingDir / fileName);            
            stage.addToRemovedFiles(fileName);
        }
    }

    if (conflict) {
        std::cout << "Encountered a merge conflict." << std::endl;
    } else {
        std::string message = "Merged " + branchName + " into " + HEAD + ".";
        commitment(message);
    }

    serializeStage();
}

void Repo::handleConflict(const std::string& fileName, const std::string& currentBlobHash, const std::string& branchBlobHash) {
    std::string conflictMarkerHead = "<<<<<<< HEAD\n";
    std::string conflictMarkerMid = "=======\n";
    std::string conflictMarkerEnd = ">>>>>>>\n";

    std::string currentContents = currentBlobHash.empty() ? std::string() :
                                  std::string(Utils::readContents(workingDir / ".gitlet/blobs" / (currentBlobHash + ".txt")).begin(),
                                              Utils::readContents(workingDir / ".gitlet/blobs" / (currentBlobHash + ".txt")).end());
    std::string branchContents = branchBlobHash.empty() ? std::string() :
                                 std::string(Utils::readContents(workingDir / ".gitlet/blobs" / (branchBlobHash + ".txt")).begin(),
                                             Utils::readContents(workingDir / ".gitlet/blobs" / (branchBlobHash + ".txt")).end());
    std::string conflictData = conflictMarkerHead + currentContents + conflictMarkerMid + branchContents + conflictMarkerEnd;
    std::vector<char> conflictDataVec(conflictData.begin(), conflictData.end());
    Utils::writeContents(workingDir / fileName, conflictDataVec);
    stage.add(fileName, Utils::sha1(conflictDataVec));
}

void Repo::checkoutFile(const Commit& commit, const std::string& fileName) {
    std::string blobHash = commit.getBlobs().at(fileName);
    std::vector<char> blobData = Utils::readContents(workingDir / ".gitlet/blobs" / (blobHash + ".txt"));
    Utils::writeContents(workingDir / fileName, blobData);
}
Commit Repo::findSplitPoint(const Commit& currentCommit, const Commit& branchCommit) {
    std::unordered_set<std::string> currentAncestors = getAllAncestors(currentCommit);
    std::unordered_set<std::string> branchAncestors = getAllAncestors(branchCommit);

    for (const auto& ancestorHash : currentAncestors) {
        if (branchAncestors.find(ancestorHash) != branchAncestors.end()) {
            return deserializeCommit(workingDir / ".gitlet/commits" / (ancestorHash + ".txt"));
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

        Commit currentCommit = deserializeCommit(workingDir / ".gitlet/commits" / (currentHash + ".txt"));
        if (!currentCommit.getParentHash().empty()) {
            toVisit.push(currentCommit.getParentHash());
        }
    }

    return ancestors;
}

void Repo::serializeStage() {
    std::ofstream ofs(".gitlet/staging/stage.txt");
    boost::archive::text_oarchive oa(ofs);
    oa << stage; // Assuming 'stage' is an instance of StagingArea
}

void Repo::deserializeStage() {
    std::ifstream ifs(".gitlet/staging/stage.txt");
    if(ifs.good()) { // Check if the file exists and is not empty
        boost::archive::text_iarchive ia(ifs);
        ia >> stage; // Assuming 'stage' is an instance of StagingArea
    }
}


void Repo::serializeCommit(const Commit& commit, const std::string& path) {
    std::ofstream ofs(path);
    boost::archive::text_oarchive oa(ofs);
    oa << commit;
}

Commit Repo::deserializeCommit(const std::string& path) const {
    Commit commit; // Assuming default constructor is available
    std::ifstream ifs(path);
    if(ifs.good()) { // Check if the file exists and is not empty
        boost::archive::text_iarchive ia(ifs);
        ia >> commit;
    }
    return commit;
}