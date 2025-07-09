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

    if (institution != "(No Institution)") {
        const auto& list = file->institutionList();
        auto it = std::find_if(list.begin(), list.end(), [&institution](const Institution& inst) {
            return inst.name == institution;
        });
        if (it != list.end()) {
            acc.institutionId = it->id;
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
