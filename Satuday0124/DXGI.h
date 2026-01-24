#pragma once
#include <d3d12.h>
#include<wrl/client.h>
#include <dxgi1_4.h>

class DXGI final {
public:


    DXGI() = default;

    ~DXGI() = default;

    [[nodiscard]] bool setDisplayAdapter() noexcept;

    [[nodiscard]] IDXGIFactory4* factory() const noexcept;

    [[nodiscard]] IDXGIAdapter1* displayAdapter() const noexcept;
private:
    
    Microsoft::WRL::ComPtr<IDXGIFactory4> dxgiFactory_{};
    Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter_{};
};