#include <iostream>
#include <queue>
#include <string>
#include <limits>

using namespace std;

// A token is (token_number, customer_name)
using Token = pair<int, string>;

// Helper to clear bad input and the rest of the line
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Show a short summary of queue status
void showSummary(const queue<Token>& q, int lastServed, int lastIssued) {
    cout << "-----------------------------\n";
    if (q.empty()) {
        cout << "Queue is empty.\n";
        cout << "Next token to serve: " << lastServed + 1 << "\n";
    } else {
        cout << "Now serving token: " << lastServed + 1 << "\n";
        cout << "Waiting customers: " << q.size() << "\n";
        cout << "Tokens in queue (front -> back): ";
        queue<Token> tmp = q;
        while (!tmp.empty()) {
            cout << tmp.front().first;
            tmp.pop();
            if (!tmp.empty()) cout << " , ";
        }
        cout << "\nLast issued token: " << lastIssued << "\n";
    }
    cout << "-----------------------------\n";
}

// Return position (1-based) of token in queue, or -1 if not present
int positionInQueue(queue<Token> q, int token) {
    int pos = 1;
    while (!q.empty()) {
        if (q.front().first == token) return pos;
        q.pop();
        ++pos;
    }
    return -1;
}

// Remove a specific token from queue. Return true if removed.
bool removeToken(queue<Token>& q, int token) {
    queue<Token> tmp;
    bool removed = false;
    while (!q.empty()) {
        Token t = q.front(); q.pop();
        if (!removed && t.first == token) {
            removed = true; // skip adding this one
            continue;
        }
        tmp.push(t);
    }
    q = move(tmp);
    return removed;
}

int main() {
    queue<Token> bankQueue;
    int lastIssued = 0;  // last token number given
    int lastServed = 0;  // last token number served

    while (true) {
        cout << "\n=== Bank Queue System ===\n";
        cout << "1. Customer\n2. Bank Executive\n3. Exit\nChoose role: ";
        int role;
        if (!(cin >> role)) { cout << "Enter number only.\n"; clearInput(); continue; }
        clearInput();

        if (role == 3) { cout << "Goodbye!\n"; break; }

        if (role == 1) { // Customer
            while (true) {
                cout << "\n-- Customer Menu --\n";
                cout << "1. Queue Me (get token)\n";
                cout << "2. Check Queue / My Position\n";
                cout << "3. Back\nChoose: ";
                int c;
                if (!(cin >> c)) { cout << "Invalid input.\n"; clearInput(); continue; }
                clearInput();

                if (c == 3) break;

                if (c == 1) {
                    string name;
                    cout << "Enter your name: ";
                    getline(cin, name);
                    if (name.empty()) { cout << "Name required.\n"; continue; }
                    ++lastIssued;
                    bankQueue.push({lastIssued, name});
                    cout << "Hello " << name << ", your token is: " << lastIssued << "\n";
                }
                else if (c == 2) {
                    showSummary(bankQueue, lastServed, lastIssued);
                    cout << "Check your token position? (y/n): ";
                    char ch; cin >> ch; clearInput();
                    if (ch == 'y' || ch == 'Y') {
                        cout << "Enter your token number: ";
                        int tok;
                        if (!(cin >> tok)) { cout << "Invalid token.\n"; clearInput(); continue; }
                        clearInput();
                        if (tok <= lastServed) {
                            cout << "Token " << tok << " is already served or skipped.\n";
                        } else {
                            int pos = positionInQueue(bankQueue, tok);
                            if (pos == -1) cout << "Token not in queue (maybe served or never issued).\n";
                            else cout << "Your position: " << pos << " (tokens before you: " << pos-1 << ")\n";
                        }
                    }
                }
                else {
                    cout << "Invalid choice.\n";
                }
            }
        }
        else if (role == 2) { // Bank Executive
            while (true) {
                cout << "\n-- Bank Executive Menu --\n";
                cout << "1. Serve Next\n2. Delete Token\n3. Show Queue Summary\n4. Back\nChoose: ";
                int c;
                if (!(cin >> c)) { cout << "Invalid input.\n"; clearInput(); continue; }
                clearInput();

                if (c == 4) break;

                if (c == 1) {
                    if (bankQueue.empty()) cout << "No customers to serve.\n";
                    else {
                        Token t = bankQueue.front(); bankQueue.pop();
                        ++lastServed;
                        cout << "Serving token " << t.first << " | Name: " << t.second << "\n";
                    }
                }
                else if (c == 2) {
                    cout << "Enter token to delete: ";
                    int tok;
                    if (!(cin >> tok)) { cout << "Invalid input.\n"; clearInput(); continue; }
                    clearInput();
                    if (tok <= lastServed) {
                        cout << "Cannot delete. Token already served or invalid.\n";
                    } else {
                        bool ok = removeToken(bankQueue, tok);
                        if (ok) cout << "Token " << tok << " removed.\n";
                        else cout << "Token " << tok << " not found.\n";
                    }
                }
                else if (c == 3) {
                    showSummary(bankQueue, lastServed, lastIssued);
                }
                else {
                    cout << "Invalid choice.\n";
                }
            }
        }
        else {
            cout << "Choose 1 (Customer), 2 (Bank Executive) or 3 (Exit).\n";
        }
    }

    return 0;
}