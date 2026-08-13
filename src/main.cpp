#include <iostream>
#include <sstream>
#include <string>
#include "../include/MinHeap.h"

// Simple command-line task scheduler.
// Commands:
//   add <id> <priority> <name...>
//   run
//   cancel <id>
//   list
//   quit

void printHelp() {
    std::cout << "Commands:\n"
              << "  add <id> <priority> <name...>   Add a task (lower priority = more urgent)\n"
              << "  run                              Run (pop) the next most urgent task\n"
              << "  cancel <id>                      Cancel a task by id\n"
              << "  list                             List all pending tasks\n"
              << "  help                              Show this message\n"
              << "  quit                              Exit\n";
}

int main() {
    MinHeap scheduler;
    std::string line;

    std::cout << "Task Scheduler (C++) — type 'help' for commands\n";
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd == "quit" || cmd == "exit") {
            break;
        } else if (cmd == "help") {
            printHelp();
        } else if (cmd == "add") {
            int id, priority;
            std::string name, word;
            if (!(iss >> id >> priority)) {
                std::cout << "Usage: add <id> <priority> <name...>\n";
                continue;
            }
            while (iss >> word) { name += (name.empty() ? "" : " ") + word; }
            if (name.empty()) name = "unnamed";
            try {
                scheduler.push(Task{id, name, priority, ""});
                std::cout << "Added task " << id << " (\"" << name << "\", priority " << priority << ")\n";
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        } else if (cmd == "run") {
            if (scheduler.isEmpty()) {
                std::cout << "No tasks to run.\n";
            } else {
                Task t = scheduler.extractMin();
                std::cout << "Running task " << t.id << ": " << t.name << " (priority " << t.priority << ")\n";
            }
        } else if (cmd == "cancel") {
            int id;
            if (!(iss >> id)) {
                std::cout << "Usage: cancel <id>\n";
                continue;
            }
            bool removed = scheduler.remove(id);
            std::cout << (removed ? "Cancelled task " + std::to_string(id) + "\n"
                                   : "No task with id " + std::to_string(id) + "\n");
        } else if (cmd == "list") {
            if (scheduler.isEmpty()) {
                std::cout << "(no pending tasks)\n";
            } else {
                for (const auto& t : scheduler.all()) {
                    std::cout << "  [" << t.id << "] " << t.name << " — priority " << t.priority << "\n";
                }
            }
        } else if (!cmd.empty()) {
            std::cout << "Unknown command: " << cmd << " (type 'help')\n";
        }
    }

    std::cout << "Goodbye.\n";
    return 0;
}
