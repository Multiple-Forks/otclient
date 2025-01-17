/*
 * Copyright (c) 2010-2020 OTClient <https://github.com/edubart/otclient>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef CONNECTION_H
#define CONNECTION_H

#include <asio/streambuf.hpp>

#include "declarations.h"
#include <framework/luaengine/luaobject.h>

class Connection : public LuaObject
{
    using ErrorCallback = std::function<void(const std::error_code&)>;
    using RecvCallback = std::function<void(uint8*, uint16)>;

    enum
    {
        READ_TIMEOUT = 30,
        WRITE_TIMEOUT = 30,
        SEND_BUFFER_SIZE = 65536,
        RECV_BUFFER_SIZE = 65536
    };

public:
    Connection();
    ~Connection() override;

    static void poll();
    static void terminate();

    void connect(const std::string& host, uint16 port, const std::function<void()>& connectCallback);
    void close();

    void write(uint8* buffer, size_t size);
    void read(uint16 bytes, const RecvCallback& callback);
    void read_until(const std::string& what, const RecvCallback& callback);
    void read_some(const RecvCallback& callback);

    void setErrorCallback(const ErrorCallback& errorCallback) { m_errorCallback = errorCallback; }

    int getIp();
    std::error_code getError() { return m_error; }
    bool isConnecting() { return m_connecting; }
    bool isConnected() { return m_connected; }
    ticks_t getElapsedTicksSinceLastRead() { return m_connected ? m_activityTimer.elapsed_millis() : -1; }

    ConnectionPtr asConnection() { return static_self_cast<Connection>(); }

protected:
    void internal_connect(const asio::ip::basic_resolver<asio::ip::tcp>::iterator& endpointIterator);
    void internal_write();
    void onResolve(const std::error_code& error, const asio::ip::tcp::resolver::iterator& endpointIterator);
    void onConnect(const std::error_code& error);
    void onCanWrite(const std::error_code& error);
    void onWrite(const std::error_code& error, size_t writeSize, const std::shared_ptr<asio::streambuf>&
                 outputStream);
    void onRecv(const std::error_code& error, size_t recvSize);
    void onTimeout(const std::error_code& error);
    void handleError(const std::error_code& error);

    std::function<void()> m_connectCallback;
    ErrorCallback m_errorCallback;
    RecvCallback m_recvCallback;

    asio::basic_waitable_timer<std::chrono::high_resolution_clock> m_readTimer;
    asio::basic_waitable_timer<std::chrono::high_resolution_clock> m_writeTimer;
    asio::basic_waitable_timer<std::chrono::high_resolution_clock> m_delayedWriteTimer;
    asio::ip::tcp::resolver m_resolver;
    asio::ip::tcp::socket m_socket;

    static std::list<std::shared_ptr<asio::streambuf>> m_outputStreams;
    std::shared_ptr<asio::streambuf> m_outputStream;
    asio::streambuf m_inputStream;
    bool m_connected;
    bool m_connecting;
    std::error_code m_error;
    stdext::timer m_activityTimer;

    friend class Server;
};

#endif
