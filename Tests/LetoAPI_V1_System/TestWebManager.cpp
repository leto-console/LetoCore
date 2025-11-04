#include <gtest/gtest.h>

#include <cstdint>

#include <Time/TimeUtils.hpp>

#include <System/DeviceID.hpp>
#include <LetoAPI_V1_System/Make.hpp>
#include <LetoAPI_V1_System/Web/WebManager.hpp>

class WebManagerTest : public ::testing::Test {
protected:

    uint8_t TestChannel() const { return 1; }
    uint8_t TestPort() const { return 66; }
    uint32_t TestID() const { return 0xC1A0BABE; }

    void SetUp() override 
    { 
        Make_LetoAPI_V1();
        SetDeviceID(0x777);
    }
    void TearDown() override { }
};

struct CallbackInfo_t
{
    uint8_t channel, port;
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
void Callback(uint8_t channel, uint8_t port, uint32_t id, const void* data, uint32_t size)
{
    CallbackInfo<Tag>(true, {channel, port, id, data, size});
}

/*
    Проверка начальной инициализации
*/
TEST_F(WebManagerTest, CreateConnection) 
{
    // Arrange
    struct Tag;
    WebManager_V1& manager = WebManager_V1::TagInstance<Tag>();

    // Act
    WebConnection_V1 connection;
    bool res = manager.CreateConnection(
        &connection, 
        TestChannel(), 
        TestPort(), 
        TestID(), 
        &Callback<Tag>);

    // Assert
    EXPECT_TRUE(res);
    EXPECT_EQ(connection.channel,   TestChannel());
    EXPECT_EQ(connection.port,      TestPort());
    EXPECT_EQ(connection.id,        TestID());
    EXPECT_EQ(manager.GetConnections(nullptr, 0), 1);
}

/*
    Проверка приема пакета
*/
TEST_F(WebManagerTest, ReceiveData) 
{
    // Arrange
    struct Tag;
    WebManager_V1& manager = WebManager_V1::TagInstance<Tag>();

    WebConnection_V1 connection;
    bool res = manager.CreateConnection(
        &connection, 
        TestChannel(), 
        TestPort(), 
        TestID(), 
        &Callback<Tag>);


    WebPacket_V1 packet{};
    packet.port = TestPort();
    packet.id_from = TestID();

    // Act
    manager.ReceiveData(TestChannel(), packet);

    // Assert
    EXPECT_EQ(CallbackInfo<Tag>().channel, TestChannel());
    EXPECT_EQ(CallbackInfo<Tag>().port, TestPort());
    EXPECT_EQ(CallbackInfo<Tag>().id, TestID());
    EXPECT_EQ(CallbackInfo<Tag>().size, sizeof(packet.data));
    EXPECT_EQ(CallbackInfo<Tag>().calls_count, 1);
}

/*
    Проверка отсутствие приема чужого пакета
*/
TEST_F(WebManagerTest, ReceiveAnotherData) 
{
    // Arrange
    struct Tag;
    WebManager_V1& manager = WebManager_V1::TagInstance<Tag>();

    WebConnection_V1 connection;
    bool res = manager.CreateConnection(
        &connection, 
        TestChannel(), 
        TestPort(), 
        TestID(), 
        &Callback<Tag>);


    WebPacket_V1 packet{};
    packet.port = TestPort();
    packet.id_from = TestID();
    packet.id_to = GetDeviceID() + 1;

    // Act
    manager.ReceiveData(TestChannel(), packet);

    // Assert
    EXPECT_EQ(CallbackInfo<Tag>().calls_count, 0);
}

/*
    Проверка отправки пакета
*/
TEST_F(WebManagerTest, SendData) 
{
    // Arrange
    struct Tag;
    WebManager_V1& manager = WebManager_V1::TagInstance<Tag>();

    WebConnection_V1 connection;
    bool res = manager.CreateConnection(
        &connection, 
        TestChannel(), 
        TestPort(), 
        TestID(), 
        &Callback<Tag>);

    uint8_t data[4]{};

    // Act
    manager.SendData(connection, data, sizeof(data));

    // Assert
    WebPacket_BufferItem item;
    EXPECT_TRUE(manager.CheckBuffer(item));

    EXPECT_EQ(item.channel, TestChannel());
    EXPECT_EQ(item.packet.id_from, GetDeviceID());
    EXPECT_EQ(item.packet.id_to, TestID());
    EXPECT_EQ(item.packet.port, TestPort());

    manager.NextBuffer();
    EXPECT_FALSE(manager.CheckBuffer(item));
}

/*
    Проверка отправки пакета с подтверждением
*/
TEST_F(WebManagerTest, SendSynchronizeData) 
{
    // Arrange
    struct Tag;
    WebManager_V1& manager = WebManager_V1::TagInstance<Tag>();

    WebConnection_V1 connection;
    bool res = manager.CreateConnection(
        &connection, 
        TestChannel(), 
        TestPort(), 
        TestID(), 
        &Callback<Tag>);

    uint8_t data[4]{};

    // Act
    manager.SendSynchronizeData(connection, data, sizeof(data));
    manager.Loop();

    // Assert
    WebPacket_BufferItem item;
    ASSERT_TRUE(manager.CheckBuffer(item));

    EXPECT_EQ(item.channel, TestChannel());
    EXPECT_EQ(item.packet.id_from, GetDeviceID());
    EXPECT_EQ(item.packet.id_to, TestID());
    EXPECT_EQ(item.packet.port, TestPort());
    EXPECT_EQ(item.packet.retries, 1);
    EXPECT_TRUE(item.packet.flags & WP_FLAG_SYN);
    EXPECT_FALSE(item.packet.flags & WP_FLAG_ACK);

    manager.NextBuffer();
    ASSERT_FALSE(manager.CheckBuffer(item));

    /*
        При попытке снова отправить такое же сообщение до истечения таймаута и количества попыток
        ничего не произойдет
    */
    {
        // Act
        manager.SendSynchronizeData(connection, data, sizeof(data));
        manager.Loop();

        // Assert
        WebPacket_BufferItem item;
        ASSERT_FALSE(manager.CheckBuffer(item));
    }

    /*
        Пока не прошел таймаут ожидания, повторные пакеты не отправляются
    */
    {
        // Act
        TimeUtils::SleepMs(100);
        manager.Loop();

        // Assert
        ASSERT_FALSE(manager.CheckBuffer(item));
    }

    /*
        Как только таймаут пройден, отправляется повторное сообщение с retries + 1
    */
    {
        // Act
        TimeUtils::SleepMs(100);
        manager.Loop();

        // Assert
        ASSERT_TRUE(manager.CheckBuffer(item));

        EXPECT_EQ(item.channel, TestChannel());
        EXPECT_EQ(item.packet.id_from, GetDeviceID());
        EXPECT_EQ(item.packet.id_to, TestID());
        EXPECT_EQ(item.packet.port, TestPort());
        EXPECT_EQ(item.packet.retries, 2);
        EXPECT_TRUE(item.packet.flags & WP_FLAG_SYN);
        EXPECT_FALSE(item.packet.flags & WP_FLAG_ACK);

        manager.NextBuffer();
        ASSERT_FALSE(manager.CheckBuffer(item));
    }

    // Arrange
    WebPacket_V1 packet = item.packet;
    
    /*
        Как только пришел ответ о получении, снимается выдача повторных сообщений
    */
    {
        // Act
        packet.flags &=~ WP_FLAG_SYN;
        packet.flags &=~ WP_FLAG_ACK;
        packet.flags |= WP_FLAG_ACK;

        manager.ReceiveData(TestChannel(), packet);
        
        for (int i = 0; i < 10; ++i)
        {
            TimeUtils::SleepMs(100);
            manager.Loop();
        }

        // Assert
        ASSERT_FALSE(manager.CheckBuffer(item));
        EXPECT_EQ(CallbackInfo<Tag>().calls_count, 0);
    }

    /*
        Повторные подтверждения ни на что не влияют
    */
    {
        // Act
        packet.retries = 5;
        manager.ReceiveData(TestChannel(), packet);

        for (int i = 0; i < 10; ++i)
        {
            TimeUtils::SleepMs(100);
            manager.Loop();
        }

        // Assert
        ASSERT_FALSE(manager.CheckBuffer(item));
        EXPECT_EQ(CallbackInfo<Tag>().calls_count, 0);
    }

    /*
        При отправке такого же сообщение после успеха, операция начинается по-новой
    */
    {
        // Act
        manager.SendSynchronizeData(connection, data, sizeof(data));
        manager.Loop();

        // Assert
        WebPacket_BufferItem item;
        ASSERT_TRUE(manager.CheckBuffer(item));

        EXPECT_EQ(item.channel, TestChannel());
        EXPECT_EQ(item.packet.id_from, GetDeviceID());
        EXPECT_EQ(item.packet.id_to, TestID());
        EXPECT_EQ(item.packet.port, TestPort());
        EXPECT_EQ(item.packet.retries, 1);
        EXPECT_TRUE(item.packet.flags & WP_FLAG_SYN);
        EXPECT_FALSE(item.packet.flags & WP_FLAG_ACK);

        manager.NextBuffer();
        ASSERT_FALSE(manager.CheckBuffer(item));
    }

    /*
        По истечение количества попыток, сбрасывается режим синхронизации
        В таком случае снова появляется возможность отправить сообщение
    */
    {
        // Act
        for (int i = 0; i < 20; ++i)
        {
            TimeUtils::SleepMs(100);
            manager.Loop();
        }

        // Assert
        EXPECT_EQ(manager.BufferCount(), 9);

        manager.SendSynchronizeData(connection, data, sizeof(data));
        manager.Loop();

        EXPECT_EQ(manager.BufferCount(), 10);
    }

}

/*
    Проверка получения пакета с подтверждением
*/
TEST_F(WebManagerTest, ReceiveSynchronizeData) 
{
    // Arrange
    struct Tag;
    WebManager_V1& manager = WebManager_V1::TagInstance<Tag>();

    WebConnection_V1 connection;
    bool res = manager.CreateConnection(
        &connection, 
        TestChannel(), 
        TestPort(), 
        TestID(), 
        &Callback<Tag>);

    WebPacket_V1 packet{};
    packet.id_from = TestID();
    packet.port = TestPort();
    packet.flags |= WP_FLAG_SYN;
    packet.retries = 1;
    packet.CalcCRC16();

    // Pre-act assert
    EXPECT_EQ(CallbackInfo<Tag>().calls_count, 0);

    // Act
    manager.ReceiveData(TestChannel(), packet);

    // Assert
    WebPacket_BufferItem item;
    ASSERT_TRUE(manager.CheckBuffer(item));

    EXPECT_EQ(CallbackInfo<Tag>().calls_count, 1);
    EXPECT_EQ(item.channel, TestChannel());
    EXPECT_EQ(item.packet, packet);
    EXPECT_EQ(item.packet.retries, 1);
    EXPECT_FALSE(item.packet.flags & WP_FLAG_SYN);
    EXPECT_TRUE(item.packet.flags & WP_FLAG_ACK);

    manager.NextBuffer();
    ASSERT_FALSE(manager.CheckBuffer(item));

    /*
        При повторном получении SYN-пакета callback не вызывается
        При этом снова отправляется ACK-пакет
    */
    {
        for (int retr = 2; retr < 4; ++retr)
        {
            // Arrange
            packet.retries = retr;

            // Act
            manager.ReceiveData(TestChannel(), packet);

            // Assert
            EXPECT_EQ(CallbackInfo<Tag>().calls_count, 1);

            WebPacket_BufferItem item;
            ASSERT_TRUE(manager.CheckBuffer(item));

            EXPECT_EQ(item.channel, TestChannel());
            EXPECT_EQ(item.packet, packet);
            EXPECT_EQ(item.packet.retries, retr);
            EXPECT_FALSE(item.packet.flags & WP_FLAG_SYN);
            EXPECT_TRUE(item.packet.flags & WP_FLAG_ACK);

            manager.NextBuffer();
            ASSERT_FALSE(manager.CheckBuffer(item));
        }
    }

    /*
        Если будет получен новый пакет с retr < или = предыдущему, он будет проигнорирован
    */
    {
        for (int retr = 3; retr > 0; --retr)
        {
            // Arrange
            packet.retries = retr;

            // Act
            manager.ReceiveData(TestChannel(), packet);

            // Assert
            EXPECT_EQ(CallbackInfo<Tag>().calls_count, 1);

            WebPacket_BufferItem item;
            ASSERT_FALSE(manager.CheckBuffer(item));
        }
    }

    /*
        После истечения 400 мс с последнего сообщения, сбрасывается состояние
        синхронизационного приема 
    */
    {
        // Arrange
        packet.retries = 4;

        // Act
        manager.ReceiveData(TestChannel(), packet);

        // Assert
        EXPECT_EQ(CallbackInfo<Tag>().calls_count, 1);

        WebPacket_BufferItem item;
        ASSERT_TRUE(manager.CheckBuffer(item));

        EXPECT_EQ(item.channel, TestChannel());
        EXPECT_EQ(item.packet, packet);
        EXPECT_EQ(item.packet.retries, 4);
        EXPECT_FALSE(item.packet.flags & WP_FLAG_SYN);
        EXPECT_TRUE(item.packet.flags & WP_FLAG_ACK);

        manager.NextBuffer();
        ASSERT_FALSE(manager.CheckBuffer(item));

        // Act
        for (int i = 0; i < 4; ++i)
        {
            TimeUtils::SleepMs(100);
            manager.Loop();
        }

        manager.ReceiveData(TestChannel(), packet);

        // Assert
        EXPECT_EQ(CallbackInfo<Tag>().calls_count, 2);
    }
}