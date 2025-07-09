#pragma once
#include <array>
#include <string>
#include <vector>

struct Institution {
    std::string name;
    std::string id;
};

class MyMoneyFile {
public:
    static MyMoneyFile* instance();
    void setInstitutions(std::vector<Institution> list);
    const std::vector<Institution>& institutionList() const;
private:
    std::vector<Institution> institutions_;
};

constexpr size_t DataSize = 100000;

struct MyMoneyAccountLoan {
    std::array<int, DataSize> data;
    std::string institutionId;
    bool variableInterest = false;
    int nextChange = 0;
    int freqAmount = 0;
    int freqUnit = 0;

    MyMoneyAccountLoan();
};

MyMoneyAccountLoan account(const std::string& institution, bool variableInterest);
