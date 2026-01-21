#pragma once
#include "DXGI.h"
#include <d3d12.h>
#include<wrl/client.h>
#include"util.h"


class Device final {
public:

    static Device& instance() noexcept {
        static Device instance;
        return instance;
    }

    [[nodiscard]] bool create() noexcept;

    [[nodiscard]] ID3D12Device* get() const noexcept;

    [[nodiscard]] const DXGI& dxgi() const noexcept;
private:
    Device() = default;

    ~Device() = default;

    Device(const Device& r) = delete;
    Device& operator=(const Device& r) = delete;
    Device(Device&& r) = delete;
    Device& operator=(Device&& r) = delete;

    DXGI dxgiInstance_{};
    Microsoft::WRL::ComPtr<ID3D12Device>device_{};

/*
    [[nodiscard]] bool create(const DXGI& dxgi) noexcept;


    [[nodiscard]] ID3D12Device* get() const noexcept;


    ID3D12Device* device_;*/

};