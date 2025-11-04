#include <gtest/gtest.h>

#include <cstdint>

#include <Time/TimeUtils.hpp>

#include <System/DeviceID.hpp>
#include <LetoAPI_V1_System/Make.hpp>
#include <LetoAPI_V1_System/Lobby/LobbyManager.hpp>
#include <LetoAPI_V1_System/Web/WebManager.hpp>

class LobbyManagerTest : public ::testing::Test {
protected:

    uint16_t TestAppID() const { return 0xBABE; }
    uint8_t TestMaxCount() const { return 4; }

    //uint8_t TestChannel() const { return 1; }
    //uint8_t TestPort() const { return 66; }
    //uint32_t TestID() const { return 0xC1A0BABE; }

    void SetUp() override 
    { 
        Make_LetoAPI_V1();
        SetDeviceID(0x777);
    }
    void TearDown() override { }
};

struct CallbackInfo_t
{
    uint32_t id; 
    const void* data; 
    uint32_t size;
    uint32_t calls_count;
};

template<typename Tag>
CallbackInfo_t CallbackInfo(bool add = false, CallbackInfo_t add_info = {})
{
    static CallbackInfo_t info;
    add_info.calls_count = info.calls_count + 1;
    if (add) info = add_info;
    return info;
}

template<typename Tag>
void Callback(uint32_t id, const void* data, uint32_t size)
{
    CallbackInfo<Tag>(true, {id, data, size});
}

/*
    Проверка создания лобби
*/
TEST_F(LobbyManagerTest, CreateConnection) 
{
    // Arrange
    struct Tag;
    LobbyManager_V1& manager = LobbyManager_V1::TagInstance<Tag>();
    
    // Act
    LobbyConnection_V1 connection;

    bool res = manager.CreateLobby(
        &connection, 
        TestAppID(), 
        TestMaxCount(), 
        &Callback<Tag>);
        
    // Assert
    EXPECT_TRUE(res);
    EXPECT_EQ(connection.owner,         GetDeviceID());
    EXPECT_EQ(connection.app_id,        TestAppID());
    EXPECT_EQ(connection.connected,     1);
    EXPECT_EQ(connection.max_count,     TestMaxCount());
    EXPECT_EQ(connection.members[0],    GetDeviceID());
    EXPECT_EQ(connection.state,         LC_STATE_CONNECTED);

    //// Ожидание 700 мс
    //for (uint8_t i = 0; i < 14; ++i)
    //{
    //    manager.Loop();
    //    TimeUtils::SleepMs(50);
    //}
//
    //EXPECT_EQ(WebManager_V1::Instance().BufferCount(), 1);
}
