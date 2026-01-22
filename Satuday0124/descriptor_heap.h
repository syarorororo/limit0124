#pragma once
#include<d3d12.h>
#include<unordered_map>
#include<optional>
#include<memory>

class DescriptorHeap;
    
 class DescriptorHeapContainer  final {
public:

    static DescriptorHeapContainer& instance() noexcept {
        static DescriptorHeapContainer instance;
        return instance;
    }

    [[nodiscard]] bool create(D3D12_DESCRIPTOR_HEAP_TYPE type,UINT numDescriptors,bool shaderVisible = false) noexcept;

    void applyPendingFree() noexcept;

    [[nodiscard]] ID3D12DescriptorHeap* get(D3D12_DESCRIPTOR_HEAP_TYPE type) const noexcept;
 
    [[nodiscard]] std::optional<UINT> allocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type) noexcept;

    void releaseDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT descriptorIndex) noexcept;
 private:
     DescriptorHeapContainer();

     ~DescriptorHeapContainer();

     DescriptorHeapContainer(const DescriptorHeapContainer& r) = delete;
     DescriptorHeapContainer& operator=(const DescriptorHeapContainer& r) = delete;
     DescriptorHeapContainer(DescriptorHeapContainer&& r) = delete;
     DescriptorHeapContainer& operator=(DescriptorHeapContainer&& r) = delete;

     std::unordered_map<UINT, std::unique_ptr<DescriptorHeap>>map_{};
   
/*
    [[nodiscard]] bool create(const Device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible = false) noexcept;

    [[nodiscard]] ID3D12DescriptorHeap* get() const noexcept;

    [[nodiscard]] D3D12_DESCRIPTOR_HEAP_TYPE getType() const noexcept;
private:
    ID3D12DescriptorHeap* heap_{};
    D3D12_DESCRIPTOR_HEAP_TYPE type_{};*/
};