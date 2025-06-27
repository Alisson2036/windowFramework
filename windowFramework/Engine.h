#pragma once
#include "Core\window.h"
#include "Core\graphics.h"
#include "Graphics\pipeline.h"
#include "Input\keyboard.h"
#include "Input\mouse.h"
#include "Input\CursorController.h"
#include "Input\inputProxy.h"

// Classe principal do motor do framework, respons�vel por gerenciar
// a janela, gr�ficos, pipeline de renderiza��o e entrada do usu�rio.
class Engine
{
public:
    Engine(UINT screenSizeX, UINT screenSizeY);

    ~Engine();

    // Atualiza o estado do motor (entrada, l�gica, etc.).
    // Retorna false se o motor deve ser encerrado.
    bool update();

    // Retorna o tamanho da tela atual.
    vec2 getScreenSize();

    // Acesso ao controlador do cursor do mouse.
    CursorController& mouseController();

    // Acesso ao proxy de entrada (teclado, mouse, etc.).
    InputProxy& input();

    // Retorna ponteiro para o pipeline de renderiza��o.
    Pipeline* getPipeline();

private:
    vec2 screenSize;         // Tamanho da tela (largura, altura)
    window win;              // Gerencia a janela da aplica��o
    Graphics gfx;            // Gerencia recursos gr�ficos (DirectX, etc.)
    Pipeline pipeline;       // Pipeline de renderiza��o

    InputProxy inputProxy;         // Proxy para entrada do usu�rio
    CursorController mouseControl; // Controlador do cursor do mouse
    Keyboard keyboard;             // Gerencia entrada do teclado
    Mouse mouse;                   // Gerencia entrada do mouse
};