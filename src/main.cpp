#include "../include/ComplexPlane.h"
#include <iostream>

int main() {
    sf::Vector2i pixel_coordinates = { 960, 540 };
    
    ComplexPlane plane(1920, 1080);

    sf::Vector2f mapped_coords = plane.get_coords(pixel_coordinates);

    std::cout << "(" << mapped_coords.x << ", " << mapped_coords.y << ")" << std::endl;

    return 0;
}
