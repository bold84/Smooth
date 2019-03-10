#pragma once

#include <functional>
#include <smooth/core/Application.h>
#include <smooth/core/network/SecureSocket.h>
#include <smooth/core/ipc/IEventListener.h>
#include <smooth/core/ipc/TaskEventQueue.h>
#include <smooth/core/network/ConnectionStatusEvent.h>
#include <smooth/core/network/Socket.h>
#include <smooth/core/network/IPv4.h>
#include "HTTPPacket.h"

namespace secure_socket_test
{
    class App
            : public smooth::core::Application,
              public smooth::core::ipc::IEventListener<smooth::core::network::TransmitBufferEmptyEvent>,
              public smooth::core::ipc::IEventListener<smooth::core::network::DataAvailableEvent<HTTPPacket>>,
              public smooth::core::ipc::IEventListener<smooth::core::network::ConnectionStatusEvent>


    {
        public:
            App();

            void init() override;
            void tick() override;

            void event(const smooth::core::network::TransmitBufferEmptyEvent&) override;
            void event(const smooth::core::network::DataAvailableEvent<HTTPPacket>&) override;
            void event(const smooth::core::network::ConnectionStatusEvent&) override;

        private:
            smooth::core::ipc::TaskEventQueue<smooth::core::network::TransmitBufferEmptyEvent> tx_empty;
            smooth::core::ipc::TaskEventQueue<smooth::core::network::DataAvailableEvent<HTTPPacket>> data_available;
            smooth::core::ipc::TaskEventQueue<smooth::core::network::ConnectionStatusEvent> connection_status;
            smooth::core::network::PacketSendBuffer<HTTPPacket, 5> tx_buffer{};
            smooth::core::network::PacketReceiveBuffer<HTTPPacket, 5> rx_buffer{};
            std::shared_ptr<smooth::core::network::ISocket> sock{};
    };
}