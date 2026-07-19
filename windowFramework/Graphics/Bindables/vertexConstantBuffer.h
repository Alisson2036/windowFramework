#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <type_traits>
#include <span>
#include <cassert>   // for assert
#include "bindable.h"


template <typename T>
class ConstantVertexBuffer : public Bindable
{
public:
    // Strictly enforces the HLSL 16-byte alignment rule at compile time
    static_assert(sizeof(T) % 16 == 0, "Constant buffer struct size must be a multiple of 16 bytes. Add padding to your struct.");

    // Create from a single object
    void create(const T& data)
    {
        create_internal(&data, 1);
    }

    // Create from an array
    void create(std::span<const T> data)
    {
        create_internal(data.data(), static_cast<UINT>(data.size()));
    }

    // Update from a single object
    void update(const T& data)
    {
        update_internal(&data, 1);
    }

    // Update from an array
    void update(std::span<const T> data)
    {
        update_internal(data.data(), static_cast<UINT>(data.size()));
    }

    void setSlot(int n)
    {
        bufferSlot = n;
    }

    void bind() override
    {  
        getContext()->VSSetConstantBuffers(bufferSlot, 1, constantBuffer.GetAddressOf());
    }

protected:

    void create_internal(const T* data, UINT count)
    {
        assert(count > 0 && "Cannot create a constant buffer of size 0.");

        arraySize = count;

        D3D11_BUFFER_DESC constantBufferDesc = {};
        constantBufferDesc.ByteWidth = sizeof(T) * arraySize;
        constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

        D3D11_SUBRESOURCE_DATA subresource = {};
        subresource.pSysMem = data;

        // If data is null, just allocate memory
        D3D11_SUBRESOURCE_DATA* pSubresource = data ? &subresource : nullptr;

        _throwHr(getDevice()->CreateBuffer(&constantBufferDesc, pSubresource, &constantBuffer));
        this->initialized = true;
    }

    void update_internal(const T* data, UINT count)
    {
        if (count == 0 || !data) return;

        // Dont copy more than it has been allocated
        UINT elementsToCopy = min(count, arraySize);

        D3D11_MAPPED_SUBRESOURCE msr = {};
        _throwHr(
            getContext()->Map(constantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr)
        );

        // Fixed: replaced objectSize * arraySize with sizeof(T) * elementsToCopy
        memcpy(msr.pData, data, sizeof(T) * elementsToCopy);

        getContext()->Unmap(constantBuffer.Get(), 0u);
    }

    // constant buffer
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

    // maximum elements this buffer can hold
    UINT arraySize = 0;

    // slot do constant buffer
    int bufferSlot = 0;
};