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
    m_zoomCount++;

    float x = BASE_WIDTH * (std::pow(BASE_ZOOM, m_zoomCount)),
          y = BASE_HEIGHT * m_aspectRatio * (std::pow(BASE_HEIGHT, m_zoomCount));

    m_plane_size = { x, y };
    m_State = State::CALCULATING;
}

void ComplexPlane::zoom_out() {
    m_zoomCount--;

    float x = BASE_WIDTH * (std::pow(BASE_ZOOM, m_zoomCount)),
          y = BASE_HEIGHT * m_aspectRatio * (std::pow(BASE_HEIGHT, m_zoomCount));

    m_plane_size = { x, y };
    m_State = State::CALCULATING;
}

void ComplexPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_vArray);
}

void ComplexPlane::set_center(sf::Vector2i mousePixel) {
    m_plane_center = map_pixels_to_coords(mousePixel);
    m_State = State::CALCULATING;
}

void ComplexPlane::set_mouse_location(sf::Vector2i mousePixel) {
    m_mouseLocation = map_pixels_to_coords(mousePixel);
}

void ComplexPlane::load_text(sf::Text& text) {
    std::ostringstream str;
    str << "Mandelbrot Set" << std::endl
        << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")" << std::endl
        << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")" << std::endl
        << "Left-click to zoom in" << std::endl
        << "Right-click to zoom out" << std::endl;

    text.setString(str.str());
}

void ComplexPlane::update_renderer() {
    int height = m_mousePixel.y, width = m_mousePixel.x;
    if (m_State == State::CALCULATING) {
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                m_vArray[j + i * height ].position = { (float)j, float(i) };
                sf::Vector2f coordinate = map_pixel_to_coords({ j, i });
                sf::Uint8 r, g, b;
                size_t iterations = count_iterations(coordinate);

                iterations_to_rgb(iterations, r, g, b);
                m_vArray[j + i * height].color = { r, g, b };
            }
        }

        m_State = State::DISPLAYING;
    }
}

/* Private Functions */

int ComplexPlane::count_iterations(sf::Vector2f coord) {
    std::complex<float> c(coord.x, coord.y);
    std::complex<float> z(0, 0);

    size_t iterations = 0;
    while (std::abs(z) < 2 && iterations < MAX_ITER) {
        z = z * z + c;
        iterations++;
    }

    // if iterations > 63 then the point escaped
    return iterations;
}

void ComplexPlane::iterations_to_rgb(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) {
    if (count < 63) {
        r = 255, g = 255, b = 255;
    } else {
        r = 0, g = 0; b = 0;
    }
}

sf::Vector2f ComplexPlane::map_pixels_to_coords(sf::Vector2i mousePixel) {
    float x_offset = (m_plane_center.x - m_plane_size.x) / 2.0f,
          y_offset = (m_plane_center.y - m_plane_size.y) / 2.0f,
          x = static_cast<float>(mousePixel.x) / m_pixel_size.x * m_plane_size.x,
          y = static_cast<float>(mousePixel.y - m_pixel_size.y) / (0 - m_pixel_size.y) * m_plane_size.y;

    sf::Vector2f mapped = { (x + x_offset), (y + y_offset) };
    return mapped;

}

