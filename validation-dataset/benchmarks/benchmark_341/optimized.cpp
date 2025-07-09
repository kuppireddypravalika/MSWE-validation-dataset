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
    for (size_t i = 0; i < DataSize; ++i)
        data[i] = static_cast<int>(i);
}

MyMoneyAccountLoan account(const std::string& institution, bool variableInterest) {
    auto file = MyMoneyFile::instance();
    MyMoneyAccountLoan acc;

    if (institution != "(No Institution)") {
        const auto& list = file->institutionList();
        for (const auto& inst : list) {
            if (inst.name == institution) {
                acc.institutionId = inst.id;
                break;
            }
        }
    } else {
        acc.institutionId.clear();
    }

    if (variableInterest) {
        acc.variableInterest = true;
        acc.nextChange = 10;
        acc.freqAmount = 1;
        acc.freqUnit = 12;
    }

    return acc;
}
