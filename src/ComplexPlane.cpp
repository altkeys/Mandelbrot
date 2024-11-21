#include "../include/ComplexPlane.h"

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) {
    m_pixel_size = { pixelWidth, pixelHeight };
    m_aspectRatio = static_cast<float>(pixelHeight) / pixelWidth;
    m_plane_center = { 0, 0 };
    m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
    m_zoomCount = 0;
    m_State = State::CALCULATING;
    m_vArray.setPrimitiveType(sf::Points);
    m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::zoom_in() {

}

void ComplexPlane::zoom_out() {

}

void ComplexPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_vArray);
}

void ComplexPlane::set_center(sf::Vector2i mousePixel) {}

void ComplexPlane::set_mouse_location(sf::Vector2i mousePixel) {}

void ComplexPlane::load_text(sf::Text& text) {}

void ComplexPlane::update_renderer() {}

sf::Vector2f ComplexPlane::get_coords(sf::Vector2i mouse_pixel) { return map_pixels_to_coords(mouse_pixel); }

/* Private Functions */

int ComplexPlane::count_iterations(sf::Vector2f coord) {}

void ComplexPlane::iterations_to_rgb(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) {}

sf::Vector2f ComplexPlane::map_pixels_to_coords(sf::Vector2i mousePixel) {
    float x_offset = (m_plane_center.x - m_plane_size.x) / 2.0f,
          y_offset = (m_plane_center.y - m_plane_size.y) / 2.0f,
          x = static_cast<float>(mousePixel.x) / m_pixel_size.x * m_plane_size.x,
          y = static_cast<float>(mousePixel.y - m_pixel_size.y) / (0 - m_pixel_size.y) * m_plane_size.y;

    sf::Vector2f mapped = { (x + x_offset), (y + y_offset) };
    return mapped;

}

