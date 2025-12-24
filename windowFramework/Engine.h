#pragma once
#include "Core\window.h"
#include "Core\graphics.h"
#include "Graphics\pipeline.h"
#include "Input\keyboard.h"
#include "Input\mouse.h"
#include "Input\CursorController.h"
#include "Input\inputProxy.h"
#include "Graphics\Caching\VertexBufferCache.h"
#include "ECS\Registry.h"

// Classe principal do motor do framework, responsável por gerenciar
// a janela, gráficos, pipeline de renderização e entrada do usuário.
class Engine
{
public:
    Engine(UINT screenSizeX, UINT screenSizeY);

    ~Engine();

    // Atualiza o estado do motor (entrada, lógica, etc.).
    // Retorna false se o motor deve ser encerrado.
    bool update();

    // Retorna o tamanho da tela atual.
    vec2 getScreenSize();

    // Acesso ao controlador do cursor do mouse.
    CursorController& mouseController();

    // Acesso ao proxy de entrada (teclado, mouse, etc.).
    InputProxy& input();

    // Retorna ponteiro para o pipeline de renderização.
    Pipeline* getPipeline();

    // Acesso ao registry
    Registry* getRegistry();

    // Acesso ao cache
    VertexBufferCache* getVBCache();

private:
    vec2 screenSize;           // Tamanho da tela (largura, altura)
    window win;                // Gerencia a janela da aplicação
    Graphics gfx;              // Gerencia recursos gráficos (DirectX, etc.)
    Registry registry;         // Registro do sistema ECS
    Pipeline pipeline;         // Pipeline de renderização
    VertexBufferCache vbCache; // Vertex buffering cache

    InputProxy inputProxy;         // Proxy para entrada do usuário
    CursorController mouseControl; // Controlador do cursor do mouse
    Keyboard keyboard;             // Gerencia entrada do teclado
    Mouse mouse;                   // Gerencia entrada do mouse
};