/**
 * @file BasicApp.hpp
 * @brief The basic sandbox: enough to watch physics run.
 *
 * Written once by devkit, then it is yours. Carve into it.
 */

#pragma once

#include "ICore.hpp"
#include "RayGraphicModule.hpp"

#include <memory>

class BasicApp : public IApp {

    public:
        BasicApp() {
            _window = _graphic->createWindow(900, 600, "physics - basic");
        }

        ~BasicApp() override {
            if (_window)
                _graphic->deleteWindow(_window);
        }

    protected:
        void event() override {
            if (_window->pollEvent())
                _window->eventClose();
        }

        void update() override {
            if (!_window->isOpen())
                stop();
            //TODO: step physics forward
        }

        void display() override {
            _window->beginDraw();
            //TODO: draw
            _window->endDraw();
        }

    private:
        /* The vendor is linked statically: we take the CLASS, not the module
         * protocol. acquire() and release() assume an IModuleManager that does
         * not exist here - do not call them. */
        std::unique_ptr<IGraphic2Module> _graphic = std::make_unique<RayGraphicModule>();
        graphic::IWindow2 *_window = nullptr;
};
