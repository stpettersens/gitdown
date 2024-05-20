/**
  * gitdown
  * Simple tool to sync git repositories from a plain text list.
  * Copyright 2023-2024 Sam Saint-Pettersen.
  * Released under the MIT License.
*/

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <cstdio>
#include <iomanip>
#include <sstream>
#include <filesystem>

using namespace std;

// Forward declarations:
void gitClone(bool, bool, string, string, string);

string getCurrentTime() {
    auto clock = chrono::system_clock::now();
    time_t time = chrono::system_clock::to_time_t(clock);

    auto ms = chrono::duration_cast<chrono::milliseconds>(clock.time_since_epoch()) % 1000;

    tm *local = localtime(&time);
    stringstream timestamp;
    timestamp << '[' << put_time(local, "%Y-%m-%d %H:%M:%S");
    timestamp << '.' << setfill('0') << setw(3) << ms.count() << ']';

    return timestamp.str();
}

void gitCloneAll(bool xgit, bool verbose, vector<string> reposList, string root, string target) {
    for (string repo : reposList) {
        // Thanks, Markus :)
        if (repo.length() > 4 && repo.substr(repo.length() - 4) == ".git")
            repo = repo.substr(0, (repo.length() - 4));

        gitClone(xgit, verbose, repo, root, target);
        if (verbose) cout << endl;
    }
}

void gitClone(bool xgit, bool verbose, string repo, string root, string target) {
    bool cloning = false;
    string dir = target + repo;

    string gitCmd = "git";
    if (xgit && verbose) gitCmd = "xgit";
    else if (xgit && !verbose) gitCmd = "xgit --quiet";

    filesystem::path dp = dir;
    if (filesystem::exists(dp)) {
        if (verbose) {
            cout << getCurrentTime() << endl;
            cout << "Pulling down over existing repo " << repo << "..." << endl;
        }

        char cmd[1024];
        sprintf(cmd, "%s -C %s pull", gitCmd.c_str(), dir.c_str());
        system(cmd);
    }
    else {
        cloning = true;
        if (verbose) {
            cout << getCurrentTime() << endl;
            cout << "Cloning new repo " << repo << "..." << endl;
        }

        char cmd[1024];
        sprintf(cmd, "%s clone %s%s %s",
        gitCmd.c_str(), root.c_str(), repo.c_str(), dir.c_str());
        system(cmd);
    }

    if (cloning) {
        char cmd[1024];
        sprintf(cmd, "%s -C %s config pull.ff only", gitCmd.c_str(), dir.c_str());
        system(cmd);
    }
}
