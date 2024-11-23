#include "../include/ComplexPlane.h"
#include <iostream>
#include <vector>
#include <thread>

int main() {
    int pixel_width = sf::VideoMode::getDesktopMode().width,
        pixel_height = sf::VideoMode::getDesktopMode().height;

    sf::VideoMode video(pixel_width, pixel_height);
    sf::RenderWindow window(video, "Mandelbrot", sf::Style::Default);
    ComplexPlane plot(pixel_width, pixel_height);

    sf::Text text;
    sf::Font font;

    if (!font.loadFromFile("./FiraSans-Regular.ttf")) {
        std::cout << "Failed to load font file, aborting program..." << std::endl;
        //return -1;
    }

    text.setFont(font);
    text.setCharacterSize(30);
    text.setPosition(pixel_width/17.0f, pixel_height/11.5f);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);

    std::vector<std::thread> threads;
    int amount_threads = std::thread::hardware_concurrency();
    int chunk_height = pixel_height / amount_threads;

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { window.close(); }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    plot.zoom_in();
                    plot.set_center({ event.mouseButton.x, event.mouseButton.y });
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    plot.zoom_out();
                    plot.set_center({ event.mouseButton.x, event.mouseButton.y });
                }
            } else if (event.type == sf::Event::MouseMoved) {
                plot.set_mouse_location({ event.mouseMove.x, event.mouseMove.y });
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { window.close(); }
        }

        /* 
         * Splits the screen into 4 chunks along the vertical axis to speed up rendering.
         *
         * Does not need a mutex lock since each thread will be accessing a different area of
         * plot.m_vArray and therefore will not overlap.
         *
         */
        if (plot.get_state() == State::CALCULATING) {
            for (int i = 0; i < amount_threads; i++) {
                int start_row = i * chunk_height,
                    end_row   = (i + 1) * chunk_height;
                threads.emplace_back(&ComplexPlane::update_render, &plot, start_row, end_row);
            }

            for (std::thread& thread : threads) {
                if (thread.joinable()) { thread.join(); }
                std::cout << "Dispatched thread." << std::endl;
            }
            std::cout << std::endl;

            threads.clear();
            
            //plot.update_render(0, pixel_height);
            plot.set_state(State::DISPLAYING);
        }

        //plot.update_renderer();
        plot.load_text(text);

        window.clear();
        window.draw(plot);
        window.draw(text);
        window.display();
    }

    return 0;
}
