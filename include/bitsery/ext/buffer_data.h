//MIT License
//
//Copyright (c) 2020 Mindaugas Vinkelis
//Copyright (c) 2020 Nick Renieris
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

#ifndef BITSERY_EXT_BUFFER_DATA_H
#define BITSERY_EXT_BUFFER_DATA_H

#include "../details/serialization_common.h"
#include "../details/adapter_common.h"

namespace bitsery {

namespace ext {

constexpr bool store_size = false;

template <typename TSize>
class BufferData {
public:

    explicit BufferData(TSize& size) :_size{ size } {}

    template<typename Ser, typename T, typename Fnc>
    void serialize(Ser& ser, const T& obj, Fnc&& fnc) const {
        auto& writer = ser.adapter();
        if constexpr (store_size) {
            writer.template writeBytes<4>(static_cast<uint32_t>(_size));
        }
        writer.template writeBuffer<1>(static_cast<const uint8_t*>(obj), _size);
    }

    template<typename Des, typename T, typename Fnc>
    void deserialize(Des& des, T& obj, Fnc&& fnc) const {
        auto& reader = des.adapter();
        if constexpr (store_size) {
            uint32_t s = 0u;
            reader.template readBytes<4>(s);
            _size = static_cast<TSize>(s);
        }
        reader.template readBuffer<1>(static_cast<uint8_t*>(obj), _size);
    }

private:
    TSize& _size;
};
}

namespace traits {
template<typename TSize, typename T>
struct ExtensionTraits<ext::BufferData<TSize>, T> {
    using TValue = void;
    static constexpr bool SupportValueOverload = false;
    static constexpr bool SupportObjectOverload = true;
    static constexpr bool SupportLambdaOverload = false;
};
}

}

#endif