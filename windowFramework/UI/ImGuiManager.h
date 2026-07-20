#pragma once
#include <windows.h>

// Forward declarations para evitar poluir o header com includes pesados
struct ID3D11Device;
struct ID3D11DeviceContext;

class ImGuiManager {
public:
    // O construtor faz toda a inicialização dos contextos e backends
    ImGuiManager(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context);

    // O destrutor libera automaticamente os recursos (RAII)
    ~ImGuiManager();

    // Impede cópia da classe para evitar destruição dupla dos ponteiros internos
    ImGuiManager(const ImGuiManager&) = delete;
    ImGuiManager& operator=(const ImGuiManager&) = delete;

    // Métodos de ciclo de frame
    void BeginFrame() const;
    void EndFrame() const;

private:
    bool m_initialized = false;
};