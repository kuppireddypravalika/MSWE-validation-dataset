#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstdlib>

enum ExprType { EN_AUTO, EN_OTHER };

struct Expression {
    ExprType type;
    Expression* left;
    Expression* right;
};

struct ListNode {
    Expression* data;
    ListNode* next;
};

void process_list(ListNode* head);

struct ListDeleter {
    void operator()(ListNode* n) const {
        while (n) {
            ListNode* tmp = n->next;
            delete n;
            n = tmp;
        }
    }
};

static std::unique_ptr<ListNode, ListDeleter> build_list(std::vector<Expression>& nodes) {
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    for (auto& e : nodes) {
        auto* n = new ListNode{&e, nullptr};
        if (!head)
            head = tail = n;
        else {
            tail->next = n;
            tail = n;
        }
    }
    return std::unique_ptr<ListNode, ListDeleter>(head);
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 1;
    size_t nodes = 10000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--repeat=",0)==0)
            repeat = std::atoi(arg.substr(9).c_str());
        else if (arg.rfind("--nodes=",0)==0)
            nodes = std::stoul(arg.substr(8));
    }

    std::vector<Expression> vec(nodes);
    for (auto& e : vec) {
        e.type = EN_AUTO;
        e.left = e.right = nullptr;
    }
    auto list_head = build_list(vec);

    if (mode == "correct") {
        process_list(list_head.get());
        std::cout << 0 << '\n';
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    auto t1 = clock::now();
    for (int r=0; r<repeat; ++r)
        process_list(list_head.get());
    auto t2 = clock::now();

    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << " ms\n";
    return 0;
}
