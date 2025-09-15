#include <iostream>
#include <string>
using namespace std;

class Command {
    string name;
    int duration;
public:
    Command(const string &n, int d) : name(n), duration(d) {}
    string getName() const { return name; }
    int getDuration() const { return duration; }
};

class CommandNode {
public:
    Command* cmd;
    CommandNode* next;
    CommandNode(Command* c) : cmd(c), next(nullptr) {}
};

class CommandExecutor {
public:
    void run(Command& c) {
        cout << "[Executor] Running: " << c.getName()
             << " duration=" << c.getDuration() << "\n";
    }
};

void advanceQueue(CommandNode*& current) {
    if (current) current = current->next;
}

void printQueue(CommandNode* head) {
    cout << "---- Command Queue ----\n";
    CommandNode* cur = head;
    while (cur) {
        cout << "Command: " << cur->cmd->getName()
             << " duration=" << cur->cmd->getDuration() << "\n";
        cur = cur->next;
    }
}

// ----------------- CommandQueue -----------------
class CommandQueue {
    CommandNode* head;
    CommandNode* tail;
public:
    CommandQueue() : head(nullptr), tail(nullptr) {}
    ~CommandQueue() { clear(); }

    void enqueue(Command* c) {
        CommandNode* node = new CommandNode(c);
        if (!head) head = tail = node;
        else { tail->next = node; tail = node; }
    }

    void executeNext(CommandNode*& current) {
        if (current && current->cmd) {
            CommandExecutor exec;
            exec.run(*current->cmd);
            advanceQueue(current);
        }
    }

    void executeAll() {
        CommandNode* current = head;
        while (current) {
            executeNext(current);
        }
    }

    void clear() {
        CommandNode* cur = head;
        while (cur) {
            CommandNode* nxt = cur->next;
            delete cur->cmd;
            delete cur;
            cur = nxt;
        }
        head = tail = nullptr;
    }

    CommandNode* getHead() { return head; }
};

// ----------------- Main -----------------
int main() {
    // Sample Data
    Command* c1 = new Command("Start Motor", 100);
    Command* c2 = new Command("Open Valve", 50);
    Command* c3 = new Command("Stop Motor", 80);

    CommandQueue q;
    q.enqueue(new Command("Init Systems", 30));
    q.enqueue(c1);
    q.enqueue(c2);
    q.enqueue(c3);
    q.enqueue(new Command("Shutdown", 40));

    printQueue(q.getHead());
    q.executeAll();

    q.clear();
    return 0;
}
