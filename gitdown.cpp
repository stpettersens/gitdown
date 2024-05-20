/**
  * gitdown
  * Simple tool to sync git repositories from a plain text list.
  * Copyright 2023-2024 Sam Saint-Pettersen.
  * Released under the MIT License.
*/

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>

using namespace std;

// Forward declarations:
int displayUsage(string, int);
void gitCloneAll(bool, bool, vector<string>, string, string);

int displayVersion() {
    cout << "gitdown v0.2.2 (C++)" << endl;
    return 0;
}

int displayError(string program, string err) {
    cout << "Error: " << err << "." << endl << endl;
    return displayUsage(program, -1);
}

int displayUsage(string program, int exitCode) {
    cout << "gitdown" << endl;
    cout << "Simple tool to sync git repositories from a plain text list." << endl;
    cout << "Copyright 2023-2024 Sam Saint-Pettersen <s.stpettersen@pm.me>" << endl;
    cout << endl << "Released under the MIT License." << endl;
    cout << endl << "Usage: " << program << " [-h | -v]";
    cout << "[(-x)(-q) -l <repos.txt> -r <repo_url_root> -t <target_root_dir>]" << endl;
    cout << endl << "Options are:" << endl << endl;
    cout << "-l | --list: Set the list of repos to clone/pull down." << endl;
    cout << "-r | --root: Set the repository URL root (include trailing '/')." << endl;
    cout << "-t | --target: Set the target directory for cloned repos." << endl;
    cout << "-x | --xgit: Use xgit (multiple server wrapper) rather than git." << endl;
    cout << "-q | --quiet: Don't write to stdout (except for stdout from git itself)." << endl;
    cout << "-h | --help: Display this usage information and exit." << endl;
    cout << "-v | --version: Display version information and exit." << endl << endl;
    return exitCode;
}

int cloneRepos(bool xgit, bool verbose, string list, string root, string target) {
    vector<string> reposList = {};
    fstream f;
    f.open(list, ios::in);
    if (f.is_open()) {
        string repo;
        while (getline(f, repo)) {
           reposList.push_back(repo);
        }
    }

    f.close();

    gitCloneAll(xgit, verbose, reposList, root, target);

    return 0;
}

int main(int argc, char *argv[]) {
    int exitCode = 0;
    const string program = "gitdown";

    bool xgit = false;
    bool verbose = true;
    string list = "";
    string root = "";
    string target = "";

    if (argc > 1) {
        for (auto i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-x") == 0 || strcmp(argv[i], "--xgit") == 0)
                xgit = true;

            if (strcmp(argv[i], "-q") == 0 || strcmp(argv[i], "--quiet") == 0)
                verbose = false;

            if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--list") == 0)
                list = string(argv[(i + 1)]);

            if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--root") == 0)
                root = string(argv[(i + 1)]);

            if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--target") == 0)
                target = string(argv[(i + 1)]);

            else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
                exitCode = displayUsage(program, 0);
                return exitCode;
            }
            else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
                exitCode = displayVersion();
                return exitCode;
            }
        }

        if (list.length() == 0 || root.length() == 0 || target.length() == 0) {
            exitCode = displayError(program, "Please specify options");
            return exitCode;
        }

        exitCode = cloneRepos(xgit, verbose, list, root, target);
    }
    else {
        exitCode = displayError(program, "No options specified");
    }

    return exitCode;
}

