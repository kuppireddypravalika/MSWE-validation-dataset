#include "common.h"

int select_candidate(const Dataset& ds)
{
    auto poRootGroup = ds.GetRootGroup();
    if (poRootGroup && poRootGroup->GetGroupNames().empty())
    {
        std::shared_ptr<MDArray> poCandidateArray;
        for (const auto& name : poRootGroup->GetMDArrayNames())
        {
            auto poArray = poRootGroup->OpenMDArray(name);
            if (poArray && poArray->GetDimensionCount() >= 3)
            {
                poCandidateArray.reset();
                break;
            }
            else if (poArray &&
                     poArray->GetDimensionCount() == 2 &&
                     poArray->GetDimensions()[0].t == GDAL_DIM_TYPE_HORIZONTAL_Y &&
                     poArray->GetDimensions()[1].t == GDAL_DIM_TYPE_HORIZONTAL_X)
            {
                if (!poCandidateArray)
                {
                    poCandidateArray = std::move(poArray);
                }
                else
                {
                    poCandidateArray.reset();
                    break;
                }
            }
        }
        if (poCandidateArray)
        {
            return poCandidateArray->GetDimensionCount();
        }
    }
    return 0;
}
