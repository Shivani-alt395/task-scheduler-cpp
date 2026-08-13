#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../include/MinHeap.h"

TEST_CASE("New heap is empty", "[heap]") {
    MinHeap h;
    REQUIRE(h.isEmpty());
    REQUIRE(h.size() == 0);
}

TEST_CASE("Push increases size and peek returns lowest priority", "[heap]") {
    MinHeap h;
    h.push(Task{1, "low urgency", 5, ""});
    h.push(Task{2, "high urgency", 1, ""});
    h.push(Task{3, "medium urgency", 3, ""});

    REQUIRE(h.size() == 3);
    REQUIRE(h.peek().id == 2); // priority 1 is most urgent
}

TEST_CASE("extractMin returns tasks in priority order", "[heap]") {
    MinHeap h;
    h.push(Task{1, "c", 3, ""});
    h.push(Task{2, "a", 1, ""});
    h.push(Task{3, "b", 2, ""});

    REQUIRE(h.extractMin().id == 2); // priority 1
    REQUIRE(h.extractMin().id == 3); // priority 2
    REQUIRE(h.extractMin().id == 1); // priority 3
    REQUIRE(h.isEmpty());
}

TEST_CASE("extractMin on empty heap throws", "[heap]") {
    MinHeap h;
    REQUIRE_THROWS_AS(h.extractMin(), std::out_of_range);
}

TEST_CASE("peek on empty heap throws", "[heap]") {
    MinHeap h;
    REQUIRE_THROWS_AS(h.peek(), std::out_of_range);
}

TEST_CASE("Duplicate task id is rejected", "[heap]") {
    MinHeap h;
    h.push(Task{1, "first", 5, ""});
    REQUIRE_THROWS_AS(h.push(Task{1, "duplicate", 2, ""}), std::invalid_argument);
    REQUIRE(h.size() == 1); // failed push must not corrupt the heap
}

TEST_CASE("remove() cancels a task by id and preserves heap order", "[heap]") {
    MinHeap h;
    h.push(Task{1, "a", 5, ""});
    h.push(Task{2, "b", 1, ""});
    h.push(Task{3, "c", 3, ""});
    h.push(Task{4, "d", 2, ""});

    REQUIRE(h.remove(3) == true);
    REQUIRE(h.size() == 3);
    REQUIRE_FALSE(h.contains(3));

    // Remaining tasks must still come out in priority order.
    REQUIRE(h.extractMin().id == 2); // priority 1
    REQUIRE(h.extractMin().id == 4); // priority 2
    REQUIRE(h.extractMin().id == 1); // priority 5
}

TEST_CASE("remove() on missing id returns false and changes nothing", "[heap]") {
    MinHeap h;
    h.push(Task{1, "a", 5, ""});
    REQUIRE(h.remove(999) == false);
    REQUIRE(h.size() == 1);
}

TEST_CASE("remove() the last remaining task empties the heap", "[heap]") {
    MinHeap h;
    h.push(Task{1, "only", 1, ""});
    REQUIRE(h.remove(1) == true);
    REQUIRE(h.isEmpty());
}

TEST_CASE("Heap handles a larger randomized-ish sequence correctly", "[heap]") {
    MinHeap h;
    std::vector<int> priorities = {9, 3, 7, 1, 5, 2, 8, 4, 6};
    int id = 100;
    for (int p : priorities) {
        h.push(Task{id++, "t", p, ""});
    }

    int last = -1;
    while (!h.isEmpty()) {
        Task t = h.extractMin();
        REQUIRE(t.priority >= last); // must come out non-decreasing
        last = t.priority;
    }
}
