#include "perf_precomp.hpp"

namespace opencv_test { namespace {

struct Layer_Padding_Test : public TestBaseWithParam<tuple<Backend, Target>>
{
    void test_padding(const std::vector<int>& input_shape, const std::vector<int>& paddings, const std::string& type = "constant", float value = 0.0)
    {
        int input_dims = input_shape.size();
        Mat input(input_dims, input_shape.data(), CV_32F);
        randu(input, -1.0f, 1.0f);

        LayerParams lp;
        lp.set("type", type);
        lp.set("value", value);
        lp.set("paddings", DictValue::arrayInt(paddings.data(), paddings.size()));

        Net net;
        net.addLayerToPrev("padding", "Padding", lp);
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

PERF_TEST_P_(Layer_Padding_Test, Padding_Constant)
{
    // Batch=1, Channels=256, Height=64, Width=64
    // Pad 1 pixel on all sides
    test_padding({1, 256, 64, 64}, {0,0, 0,0, 1,1, 1,1}, "constant", 0.0);
}

PERF_TEST_P_(Layer_Padding_Test, Padding_Reflection)
{
    // Batch=1, Channels=64, Height=128, Width=128
    test_padding({1, 64, 128, 128}, {0,0, 0,0, 2,2, 2,2}, "reflect");
}

INSTANTIATE_TEST_CASE_P(/**/, Layer_Padding_Test, dnnBackendsAndTargets(false, false));

}} // namespace
