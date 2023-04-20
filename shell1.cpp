//
// Created by Brennan on 4/9/2023.
//
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <thread>

using namespace std;
class Shell {
private:

    /**
     * trims leading and trailing spaces from an input string
     * @param input
     * @return
     */
    static string trim(string input) {
        // finds first non whitespace and erase all before
        input.erase(input.begin(), find_if(input.begin(), input.end(),
                                           [](int ch) { return !isspace(ch); }));
        // does the same thing but in reverse using a reverse iterative approach
        input.erase(find_if(input.rbegin(), input.rend(), [](int ch) { return !isspace(ch); })
                            .base(),
                    input.end());
        return input;
    }

    /**
     * helper method to print a message prompt to the user on command line
     * @param in
     */
    static void prompt(const string &in) {
        cout << in + ">";
    }

    /**
     * This method takes an input line from command line and
     * tokenizes it
     * @param tokens
     * @param message
     */
    static void takeCommand(vector<string> *tokens, const string& message) {
        tokens->clear();
        prompt(message);
        string str;
        getline(cin, str);
        str = trim(str);
        string ss;
        for (const char i: str) {
            if (i == ' ') {

                if (!ss.empty()) {
                    tokens->push_back(trim(ss));
                }
                ss.clear();
            } else
                ss = ss + i;
        }
        ss = trim(ss);
        if (ss.length() > 0) {
            tokens->push_back(trim(ss));
        }
    }

    string message;

    int wait;

public:
    Shell(const string &message, const int &wait) {
        this->message = message;
        this->wait = wait;
    }

    /**
     * This starts a command line shell that can execute programs
     * input is tokenized and passed to exec after forking
     */
    void start() {

        //tokenize
        auto *tokens = new vector<string>();
        takeCommand(tokens, message);

        //start shell loop
        while (tokens->empty() || tokens->at(0) != "quit") {

            if (!tokens->empty()) {

                //name of prog
                const char *fileName = (tokens->at(0).c_str());

                //options of prog
                char *commands[tokens->size() + 1];
                commands[tokens->size()] = nullptr;
                for (int i = 0; i < tokens->size(); i++) {
                    commands[i] = (char *) tokens->at(i).c_str();
                }

                //fork here
                pid_t c_pid = fork();
                if (c_pid == -1) {

                    perror("fork");
                    exit(EXIT_FAILURE);
                } else if (c_pid <= 0) {

                    execvp(fileName, commands);
                }
            }
            this_thread::sleep_for(chrono::milliseconds(10));
            takeCommand(tokens, message);
        }
    }
};

int main() {
    Shell shell("EnterCommand", 15);
    shell.start();
    return 0;
}