#include <SFML/Graphics.hpp>
#include <complex>
#include <iostream>
#include "ComplexPlane.h"

int main() {
    // Set window dimensions
    const int width = 960;
    const int height = 540;

    // Create the SFML RenderWindow
    sf::RenderWindow window(sf::VideoMode(width, height), "Mandelbrot Set");

    // Create the ComplexPlane object
    ComplexPlane complexPlane(width, height);

    // Load the font
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error: Could not load font!" << std::endl;
        return -1;
    }

    // Set up the text display
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setPosition(10, 10);

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                // Close the window
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                // Handle zoom on mouse button click
                if (event.mouseButton.button == sf::Mouse::Left) {
                    complexPlane.setCenter({event.mouseButton.x, event.mouseButton.y});
                    complexPlane.zoomIn();
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    complexPlane.setCenter({event.mouseButton.x, event.mouseButton.y});
                    complexPlane.zoomOut();
                }
            }

            if (event.type == sf::Event::MouseMoved) {
                // Track mouse movement
                complexPlane.setMouseLocation({event.mouseMove.x, event.mouseMove.y});
            }
        }

        // Update rendering and load text
        complexPlane.updateRender();
        complexPlane.loadText(text);

        // Draw everything
        window.clear();
        window.draw(complexPlane);
        window.draw(text);
        window.display();
    }

    return 0;
}
