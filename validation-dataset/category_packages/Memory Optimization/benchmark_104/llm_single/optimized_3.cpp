#include "common.h"

int select_candidate(const Dataset& ds)
{
    const auto* poRootGroup = ds.GetRootGroup();
    if (poRootGroup && poRootGroup->GetGroupNames().empty())
    {
        std::shared_ptr<MDArray> poCandidateArray;
        const auto& mdArrayNames = poRootGroup->GetMDArrayNames();
        const size_t count = mdArrayNames.size();
        for (size_t i = 0; i < count; ++i)
        {
            auto poArray = poRootGroup->OpenMDArray(mdArrayNames[i]);
            if (!poArray) continue;

            int dimCount = poArray->GetDimensionCount();
            if (dimCount >= 3)
            {
                poCandidateArray.reset();
                break;
            }
            else if (dimCount == 2)
            {
                const auto& dims = poArray->GetDimensions();
                if (dims[0].t == GDAL_DIM_TYPE_HORIZONTAL_Y &&
                    dims[1].t == GDAL_DIM_TYPE_HORIZONTAL_X)
                {
                    if (!poCandidateArray)
                    {
                        poCandidateArray = poArray;
                    }
                    else
                    {
                        poCandidateArray.reset();
                        break;
                    }
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
