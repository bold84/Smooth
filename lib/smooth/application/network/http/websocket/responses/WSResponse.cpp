// Smooth - C++ framework for writing applications based on Espressif's ESP-IDF.
// Copyright (C) 2017 Per Malmberg (https://github.com/PerMalmberg)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

// Smooth - C++ framework for writing applications based on Espressif's ESP-IDF.
// Copyright (C) 2017 Per Malmberg (https://github.com/PerMalmberg)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <smooth/application/network/http/websocket/responses/WSResponse.h>
#include <smooth/core/network/util.h>

namespace smooth::application::network::http::websocket::responses
{
    ResponseStatus WSResponse::get_data(std::size_t max_amount, std::vector<uint8_t>& target)
    {
        auto res{ResponseStatus::EndOfData};

        auto remaining = std::distance(data.begin(), data.end());

        if (remaining > 0)
        {
            auto to_send = std::min(static_cast<std::size_t>(remaining), max_amount);
            auto begin = data.begin();
            auto end = data.begin() + static_cast<long>(to_send);

            auto more_to_send = to_send < data.size();

            if (!first_frame)
            {
                op_code = WebsocketProtocol::OpCode::Continuation;
            }

            first_frame = false;

            if (more_to_send)
            {
                auto val = static_cast<uint8_t>(op_code);
                target.emplace_back(val);
            }
            else
            {
                // Set fin-bit
                auto val = 0x80 | static_cast<uint8_t>(op_code);
                target.emplace_back(val);
            }

            set_length(to_send, target);

            std::copy(begin, end, std::back_inserter(target));
            data.erase(begin, end);

            // Anything still left?
            remaining = std::distance(data.begin(), data.end());
            res = remaining > 0 ? ResponseStatus::HasMoreData : ResponseStatus::LastData;
        }

        return res;
    }

    WSResponse::WSResponse(WebsocketProtocol::OpCode code)
            : op_code(code)
    {
    }

    WSResponse::WSResponse(const std::string& text)
            : op_code(WebsocketProtocol::OpCode::Text)
    {
        std::copy(std::make_move_iterator(text.begin()), std::make_move_iterator(text.end()), std::back_inserter(data));
    }

    void WSResponse::set_length(uint64_t len, std::vector<uint8_t>& buff) const
    {
        if (len <= 125)
        {
            buff.emplace_back(len);
        }
        else if (len > std::numeric_limits<uint16_t>::max())
        {
            std::array<uint8_t, 8> size{};
            auto p = reinterpret_cast<uint64_t*>(&size[0]);
            *p = static_cast<uint64_t>(len);
            *p = smooth::core::network::hton(*p);
            buff.emplace_back(127);
            std::copy(size.begin(), size.end(), std::back_inserter(buff));
        }
        else
        {
            std::array<uint8_t, 2> size{};
            auto p = reinterpret_cast<uint16_t*>(&size[0]);
            *p = static_cast<uint16_t>(len);
            *p = smooth::core::network::hton(*p);
            buff.emplace_back(126);
            std::copy(size.begin(), size.end(), std::back_inserter(buff));
        }
    }
}