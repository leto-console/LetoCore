/*
 * WinPipeUART.hpp
 *
 *  Created on: Feb 12, 2025
 *      Author: Timur
 */

#ifndef INC_USART_VIRTUAL_WIN_UART_HPP_
#define INC_USART_VIRTUAL_WIN_UART_HPP_

#ifdef _WIN32

#include <windows.h>
#include <string>
#include <vector>
#include <iostream>

class WinPipeUART {
public:
    enum class Mode { Server, Client };

    WinPipeUART(bool server, const std::string& pipeName) { init(server, pipeName); };
    ~WinPipeUART() { close(); }

    // Инициализация: один раз в начале
    bool init(bool server, const std::string& pipeName) {
        close(); // на случай повторного вызова
        m_mode = server ? Mode::Server : Mode::Client;
        m_pipeName = (m_mode == Mode::Server)
            ? R"(\\.\pipe\)" + pipeName
            : R"(\\.\pipe\)" + pipeName;

        // Создаём события для OVERLAPPED
        m_ovConnect.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
        m_ovRead.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
        m_ovWrite.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
        if (!m_ovConnect.hEvent || !m_ovRead.hEvent || !m_ovWrite.hEvent) {
            std::cerr << "[VirtualUart] Failed to create events\n";
            close();
            return false;
        }

        if (m_mode == Mode::Server) {
            return initAsServer();
        }
        else {
            return initAsClient();
        }
    }

    // Вызывать в основном цикле (как HAL_UART_Poll)
    void poll() {
        if (m_state == State::Connecting) {
            pollConnect();
        }
        else if (m_state == State::Connected) {
            pollRead();
            // Запись: можно добавить pollWrite(), если нужна надёжная доставка
        }
        else if (m_state == State::Disconnected && m_mode == Mode::Client) {
            // Автоповтор подключения каждые 500 мс
            static DWORD lastRetry = 0;
            DWORD now = GetTickCount();
            if (now - lastRetry > 500) {
                lastRetry = now;
                initAsClient();
            }
        }
    }

    // Отправка (неблокирующая, без подтверждения)
    bool send(const void* data, size_t len) {
        if (m_state != State::Connected || len == 0) return false;
        DWORD written = 0;
        // Пытаемся синхронно — быстро, если буфер не переполнен
        if (WriteFile(m_hPipe, data, static_cast<DWORD>(len), &written, nullptr)) {
            return written == len;
        }
        DWORD err = GetLastError();
        if (err == ERROR_IO_PENDING) {
            // Асинхронно — просто запустили, не ждём
            return true;
        }
        // Иначе — ошибка (разрыв, etc)
        handleIoError(err);
        return false;
    }

    // Приём: возвращает true, если есть данные
    bool receive(std::vector<uint8_t>& outData) {
        if (!m_rxPending && m_rxBuffer.size() > 0) {
            outData.swap(m_rxBuffer);
            m_rxBuffer.clear();
            return true;
        }
        return false;
    }

    // Состояние подключения
    bool isConnected() const { return m_state == State::Connected; }

private:
    enum class State { Disconnected, Connecting, Connected };

    Mode m_mode = Mode::Client;
    std::string m_pipeName;
    HANDLE m_hPipe = INVALID_HANDLE_VALUE;
    OVERLAPPED m_ovConnect = {};
    OVERLAPPED m_ovRead = {};
    OVERLAPPED m_ovWrite = {};
    State m_state = State::Disconnected;

    // Буфер принятых данных (копируем только по запросу — избегаем копий)
    std::vector<uint8_t> m_rxBuffer;
    char m_readBuf[256] = {};
    bool m_rxPending = false;

    void close() {
        if (m_hPipe != INVALID_HANDLE_VALUE) {
            CancelIo(m_hPipe);
            CloseHandle(m_hPipe);
            m_hPipe = INVALID_HANDLE_VALUE;
        }
        if (m_ovConnect.hEvent) CloseHandle(m_ovConnect.hEvent);
        if (m_ovRead.hEvent)    CloseHandle(m_ovRead.hEvent);
        if (m_ovWrite.hEvent)   CloseHandle(m_ovWrite.hEvent);
        m_ovConnect = {}; m_ovRead = {}; m_ovWrite = {};
        m_state = State::Disconnected;
    }

    bool initAsServer() {
        m_hPipe = CreateNamedPipeA(
            m_pipeName.c_str(),
            PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
            1, 1024, 1024, 0, nullptr
        );
        if (m_hPipe == INVALID_HANDLE_VALUE) {
            std::cerr << "[Server] CreateNamedPipe failed, err=" << GetLastError() << "\n";
            return false;
        }

        m_state = State::Connecting;
        ResetEvent(m_ovConnect.hEvent);
        if (ConnectNamedPipe(m_hPipe, &m_ovConnect)) {
            onConnected();
        }
        else if (GetLastError() == ERROR_IO_PENDING) {
            // Ожидаем в poll()
        }
        else {
            std::cerr << "[Server] ConnectNamedPipe failed, err=" << GetLastError() << "\n";
            close();
            return false;
        }
        return true;
    }

    bool initAsClient() {
        m_hPipe = CreateFileA(
            m_pipeName.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0, nullptr,
            OPEN_EXISTING,
            FILE_FLAG_OVERLAPPED,
            nullptr
        );
        if (m_hPipe == INVALID_HANDLE_VALUE) {
            m_state = State::Disconnected;
            return false;
        }

        m_state = State::Connecting;
        // Для клиента подключение мгновенное — но проверим статус
        DWORD mode = PIPE_READMODE_BYTE;
        SetNamedPipeHandleState(m_hPipe, &mode, nullptr, nullptr);
        onConnected();
        return true;
    }

    void pollConnect() {
        if (m_mode == Mode::Server) {
            DWORD bytes;
            if (GetOverlappedResult(m_hPipe, &m_ovConnect, &bytes, FALSE)) {
                onConnected();
            }
            else if (GetLastError() != ERROR_IO_INCOMPLETE) {
                handleIoError(GetLastError());
            }
        }
        else {
            // Клиент: подключение синхронное, но на всякий — проверим
            onConnected();
        }
    }

    void pollRead() {
        if (!m_rxPending) {
            m_rxPending = true;
            DWORD bytes;
            if (!ReadFile(m_hPipe, m_readBuf, sizeof(m_readBuf), &bytes, &m_ovRead)) {
                DWORD err = GetLastError();
                if (err != ERROR_IO_PENDING) {
                    handleIoError(err);
                    return;
                }
            }
        }

        // Проверяем: завершилось ли чтение?
        DWORD bytes;
        if (GetOverlappedResult(m_hPipe, &m_ovRead, &bytes, FALSE)) {
            if (bytes > 0) {
                m_rxBuffer.insert(m_rxBuffer.end(),
                    reinterpret_cast<uint8_t*>(m_readBuf),
                    reinterpret_cast<uint8_t*>(m_readBuf + bytes));
            }
            m_rxPending = false;
        }
        else if (GetLastError() != ERROR_IO_INCOMPLETE) {
            handleIoError(GetLastError());
        }
    }

    void onConnected() {
        m_state = State::Connected;
        m_rxPending = false;
        m_rxBuffer.clear();
        std::cout << "[VirtualUart] ✅ Connected ("
            << (m_mode == Mode::Server ? "Server" : "Client")
            << ") to " << m_pipeName << "\n";
    }

    void handleIoError(DWORD err) {
        std::cerr << "[VirtualUart] IO error " << err << ", disconnecting.\n";
        close();
        // Сервер — ждёт нового клиента; клиент — будет пытаться переподключиться
        if (m_mode == Mode::Server) {
            initAsServer(); // сразу пересоздаём
        }
    }
};

#endif /* WIN32 */

#endif /* INC_USART_VIRTUAL_WIN_UART_HPP_ */
