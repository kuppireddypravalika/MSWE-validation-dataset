#include "loan_account.h"

MyMoneyFile* MyMoneyFile::instance() {
    static MyMoneyFile file;
    return &file;
}

void MyMoneyFile::setInstitutions(std::vector<Institution> list) {
    institutions_ = std::move(list);
}

const std::vector<Institution>& MyMoneyFile::institutionList() const {
    return institutions_;
}

MyMoneyAccountLoan::MyMoneyAccountLoan() {
    std::iota(data.begin(), data.end(), 0);
}

MyMoneyAccountLoan account(const std::string& institution, bool variableInterest) {
    auto file = MyMoneyFile::instance();
    MyMoneyAccountLoan acc;

    const auto& list = file->institutionList();
    if (institution != "(No Institution)" && !list.empty()) {
        auto it = std::find_if(list.begin(), list.end(), [&institution](const Institution& inst) {
            return inst.name == institution;
        });
        if (it != list.end()) {
            acc.institutionId = it->id;
        }
    }

    if (variableInterest) {
        acc.variableInterest = true;
        acc.nextChange = 10;
        acc.freqAmount = 1;
        acc.freqUnit = 12;
    }

    return acc;
}