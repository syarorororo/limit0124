#include "window.h"
#include "device.h"
#include "DXGI.h"
#include "command_allocator.h"
#include "command_list.h"
#include "command_queue.h"
#include "swap_chain.h"
#include"depth_buffer.h"
#include"constant_buffer.h"
#include "descriptor_heap.h"
#include "render_target.h"
#include "fence.h"
#include "root_signature.h"
#include "shader.h"
#include "pipline_state_object.h"
#include "triangle_polygon.h"
#include <cassert>
class Application final {
public:

    Application() = default;
    ~Application() = default;

    [[nodiscard]] bool initialize(HINSTANCE instance) noexcept {
        if (S_OK != Window::instance().create(instance, 1280, 720, "MyApp")) {
            assert(false && "ウィンドウの生成に失敗しました");
            return false;
        }

        if (!Device::instance().create()) {
            assert(false && "デバイスの作成に失敗しました");
            return false;
        }

        if (!commandQueueInstance_.create()) {
            assert(false && "コマンドキューの作成に失敗しました");
            return false;
        }

        if (!swapChainInstance_.create(commandQueueInstance_)) {
            assert(false && "スワップチェインの作成に失敗しました");
            return false;
        }

        if (!DescriptorHeapContainer::instance().create(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, swapChainInstance_.getDesc().BufferCount)) {
            assert(false && "ディスクリプタヒープの作成に失敗しました");
            return false;
        }
        if (!DescriptorHeapContainer::instance().create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 5, true)) {
            assert(false && "定数バッファ用ディスクリプタヒープの作成に失敗");
            return false;
        }
        if (!DescriptorHeapContainer::instance().create(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1)) {
            assert(false && "デプスバッファ用ディスクリプタヒープの作成に失敗");
            return false;
        }
        if (!renderTargetInstance_.createBackBuffer(swapChainInstance_)) {
            assert(false && "レンダーターゲットの作成に失敗しました");
            return false;
        }

        if (!commandAllocatorInstance_[0].create( D3D12_COMMAND_LIST_TYPE_DIRECT)) {
            assert(false && "コマンドアロケータの作成に失敗しました");
            return false;
        }
        if (!commandAllocatorInstance_[1].create(D3D12_COMMAND_LIST_TYPE_DIRECT)) {
            assert(false && "コマンドアロケータの作成に失敗しました");
            return false;
        }

        if (!commandListInstance_.create( commandAllocatorInstance_[0])) {
            assert(false && "コマンドリストの作成に失敗しました");
            return false;
        }

        if (!fenceInstance_.create()) {
            assert(false && "フェンスの作成に失敗しました");
            return false;
        }

        if (!rootSignatureInstance_.create()) {
            assert(false && "ルートシグネチャの作成に失敗しました");
            return false;
        }

        if (!shaderInstance_.create()) {
            assert(false && "シェーダーの作成に失敗しました");
            return false;
        }

        if (!piplineStateObjectInstance_.create( shaderInstance_, rootSignatureInstance_)) {
            assert(false && "パイプラインステートオブジェクトの作成に失敗しました");
            return false;
        }
        return true;
    }

    void loop() noexcept {
        while (Window::instance().messageLoop()) {

            const auto backBufferIndex = swapChainInstance_.get()->GetCurrentBackBufferIndex();

            if (frameFenceValue_[backBufferIndex] != 0) {
                fenceInstance_.wait(frameFenceValue_[backBufferIndex]);
            }

            commandAllocatorInstance_[backBufferIndex].reset();

            commandListInstance_.reset(commandAllocatorInstance_[backBufferIndex]);

            auto pToRT = resourceBarrier(renderTargetInstance_.get(backBufferIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
            commandListInstance_.get()->ResourceBarrier(1, &pToRT);

            D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { renderTargetInstance_.getDescriptorHandle(backBufferIndex) };
            commandListInstance_.get()->OMSetRenderTargets(1, handles, false, nullptr);

            const float clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };  // クリア
            commandListInstance_.get()->ClearRenderTargetView(handles[0], clearColor, 0, nullptr);

            commandListInstance_.get()->SetPipelineState(piplineStateObjectInstance_.get());

            commandListInstance_.get()->SetGraphicsRootSignature(rootSignatureInstance_.get());

            const auto [w, h] =Window::instance().size();
            D3D12_VIEWPORT viewport{};
            viewport.TopLeftX = 0.0f;
            viewport.TopLeftY = 0.0f;
            viewport.Width = static_cast<float>(w);
            viewport.Height = static_cast<float>(h);
            viewport.MinDepth = 0.0f;
            viewport.MaxDepth = 1.0f;
            commandListInstance_.get()->RSSetViewports(1, &viewport);

            D3D12_RECT scissorRect{};
            scissorRect.left = 0;
            scissorRect.top = 0;
            scissorRect.right = w;
            scissorRect.bottom = h;
            commandListInstance_.get()->RSSetScissorRects(1, &scissorRect);

            trianglePolygonInstance_.draw(commandListInstance_);

            auto rtToP = resourceBarrier(renderTargetInstance_.get(backBufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
            commandListInstance_.get()->ResourceBarrier(1, &rtToP);

            commandListInstance_.get()->Close();

            ID3D12CommandList* ppCommandLists[] = { commandListInstance_.get() };
            commandQueueInstance_.get()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

            swapChainInstance_.get()->Present(1, 0);

            commandQueueInstance_.get()->Signal(fenceInstance_.get(), nextFenceValue_);
            frameFenceValue_[backBufferIndex] = nextFenceValue_;
            nextFenceValue_++;
        }

    }

    D3D12_RESOURCE_BARRIER resourceBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to) noexcept {
        D3D12_RESOURCE_BARRIER barrier{};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = resource;
        barrier.Transition.StateBefore = from;
        barrier.Transition.StateAfter = to;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        return barrier;
    }
private:
    CommandQueue     commandQueueInstance_{};
    SwapChain        swapChainInstance_{};
    RenderTarget     renderTargetInstance_{};
    CommandAllocator commandAllocatorInstance_[2]{};
    CommandList      commandListInstance_{};
    Fence  fenceInstance_{};
    UINT64 frameFenceValue_[2]{};
    UINT64 nextFenceValue_ = 1;
    //
    RootSignature      rootSignatureInstance_{};
    Shader             shaderInstance_{};
    PiplineStateObject piplineStateObjectInstance_{};
    TrianglePolygon    trianglePolygonInstance_{};
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    Application app;
    if (!app.initialize(hInstance)) {
        assert(false && "アプリケーションの初期化に失敗しました");
    }

    app.loop();
    return 0;
}