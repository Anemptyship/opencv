#include "perf_precomp.hpp"

namespace opencv_test { namespace {

struct Layer_Permute_Test : public TestBaseWithParam<tuple<Backend, Target>>
{
    void test_permute(const std::vector<int>& input_shape, const std::vector<int>& order)
    {
        int input_dims = input_shape.size();
        Mat input(input_dims, input_shape.data(), CV_32F);
        randu(input, -1.0f, 1.0f);

        LayerParams lp;
        lp.set("order", DictValue::arrayInt(order.data(), order.size()));

        Net net;
        net.addLayerToPrev("permute", "Permute", lp);
        net.setInput(input);

        net.setPreferableBackend(get<0>(GetParam()));
        net.setPreferableTarget(get<1>(GetParam()));

        TEST_CYCLE()
        {
            Mat res = net.forward();
        }
        SANITY_CHECK_NOTHING();
    }
};

PERF_TEST_P_(Layer_Permute_Test, Permute_Fallback)
{
    // Test the fallback path (general permutation)
    // 5D input: N, C, D, H, W -> N, D, H, W, C
    test_permute({1, 32, 16, 32, 32}, {0, 2, 3, 4, 1});
}

PERF_TEST_P_(Layer_Permute_Test, Permute_ND)
{
    // 6D input to force fallback path
    test_permute({1, 8, 8, 8, 8, 8}, {0, 5, 1, 2, 3, 4});
}

INSTANTIATE_TEST_CASE_P(/**/, Layer_Permute_Test, dnnBackendsAndTargets(false, false));

}} // namespace
