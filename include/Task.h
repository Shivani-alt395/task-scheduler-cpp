#pragma once
#include <string>

// A unit of work to be scheduled.
// Lower `priority` value = runs sooner (1 = most urgent).
struct Task {
    int id;
    std::string name;
    int priority;
    std::string deadline; // simple string like "2026-08-20", not parsed for this project

    bool operator==(const Task& other) const {
        return id == other.id;
    }
};
