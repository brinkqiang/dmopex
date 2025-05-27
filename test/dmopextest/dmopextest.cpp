
#include "dmopex.h"
#include "gtest.h"

class env_dmopex
{
public:
    void init(){}
    void uninit(){}
};

class frame_dmopex : public testing::Test
{
public:
    virtual void SetUp()
    {
        env.init();
    }
    virtual void TearDown()
    {
        env.uninit();
    }
protected:
    env_dmopex env;
};

TEST_F(frame_dmopex, init)
{
    Idmopex* module = dmopexGetModule();
    if (module)
    {
        module->Test();
        module->Release();
    }
}
