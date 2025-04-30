#include "ComplexPlane.h"
#include <thread>


ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
    m_pixel_size = Vector2i(pixelWidth, pixelHeight);
    m_aspectRatio = static_cast<float>(pixelHeight) / static_cast<float>(pixelWidth);
    m_plane_center = Vector2f(0, 0);
    m_plane_size = Vector2f(BASE_WIDTH, BASE_HEIGHT * m_aspectRatio);
    m_zoomCount = 0;
    m_state = State::CALCULATING;
    m_vArray.setPrimitiveType(Points);
    m_vArray.resize(pixelWidth * pixelHeight);
}
void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(m_vArray);
}

//void ComplexPlane::updateRender()
//{
//    if (m_state == State::CALCULATING) {
//        for (int i = 0; i < m_pixelHeight; ++i) {
//            for (int j = 0; j < m_pixelWidth; ++j) {
//                int index = j + i * m_pixelWidth;
//                m_vArray[index].position = Vector2f(j, i);
//
//                Vector2f coord = mapPixelToCoords({ j, i });
//                size_t iterations = countIterations(coord);
//
//                Uint8 r, g, b;
//                iterationsToRGB(iterations, r, g, b);
//                m_vArray[index].color = Color(r, g, b);
//            }
//        }
//        m_state = State::DISPLAYING;
//    }
//}

void ComplexPlane::updateRender() {
    if (m_state != State::CALCULATING)
        return;

    // Number of threads to use
    unsigned int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4; // Fallback in case the cpu doesnt have a number

    std::vector<std::thread> threads;

    // tell each thread what to do
    auto renderChunk = [&](int startRow, int endRow) 
        {
        for (int j = startRow; j < endRow; ++j) 
        {
            for (int i = 0; i < m_pixelWidth; ++i) 
            {
                int index = i + j * m_pixelWidth;
                m_vArray[index].position = sf::Vector2f(i, j);

                sf::Vector2f coord = mapPixelToCoords({ i, j });
                size_t iter = countIterations(coord);

                sf::Uint8 r, g, b;
                iterationsToRGB(iter, r, g, b);
                m_vArray[index].color = sf::Color(r, g, b);
            }
        }
        };

    int rowsPerThread = m_pixelHeight / numThreads;

    for (unsigned int i = 0; i < numThreads; ++i) 
    {
        int start = i * rowsPerThread;
        int end = (i == numThreads - 1) ? m_pixelHeight : start + rowsPerThread;
        threads.emplace_back(renderChunk, start, end);
    }

    // Join all threads
    for (auto& t : threads) 
    {
        t.join();
    }

    m_state = State::DISPLAYING;
}


void ComplexPlane::zoomIn()
{
    m_zoomCount++;
    float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = { x,y };
    m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut()
{
    m_zoomCount--;
    float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = { x,y };
    m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel)
{
    m_plane_center = mapPixelToCoords(mousePixel);
    m_state = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousPixel)
{
    m_mouseLocation = mapPixelToCoords(mousPixel);
}

void ComplexPlane::loadText(Text& text)
{
    stringstream ss;
    ss << "Mandelbrot Set" << endl;
    ss << "Center: (" << m_plane_center.x << "," << m_plane_center.y << ")" << endl;
    ss << "Center: (" << m_mouseLocation.x << "," << m_mouseLocation.y << ")" << endl;
    ss << "Left-click to Zoom in" << endl << "Right-click to Zoom out";
    text.setString(ss.str());
}

size_t ComplexPlane::countIterations(Vector2f coord)
{
    complex<float> c = { coord.x, coord.y };
    complex<float> z = 0.0;
    int i = 0;
    while (abs(z) < 2 && i < 64)
    {
        z = z * z + c;
        i++;
    }
    return i;

}

void ComplexPlane::iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) {
    if (count == MAX_ITER) {
        r = g = b = 0; // Black for points inside the Mandelbrot set
        return;
    }

    // Normalize count to a value in the range [0, 1)
    float t = static_cast<float>(count) / MAX_ITER;

    // Multi-region gradient
    if (t < 0.2f) { // Dark Blue → Cyan
        r = 0;
        g = static_cast<sf::Uint8>(255 * t);
        b = static_cast<sf::Uint8>(255 * t);
    }
    else if (t < 0.4f) { // Cyan → Green
        r = 0;
        g = static_cast<sf::Uint8>(255);
        b = static_cast<sf::Uint8>(255 * (1 - (t - 0.2f) / 0.2f));
    }
    else if (t < 0.6f) { // Green → Yellow
        r = static_cast<sf::Uint8>(255 * ((t - 0.4f) / 0.2f));
        g = 255;
        b = 0;
    }
    else if (t < 0.8f) { // Yellow → White
        r = 255;
        g = static_cast<sf::Uint8>(255);
        b = static_cast<sf::Uint8>(255 * ((t - 0.6f) / 0.2f));
    }
    else { // White
        r = 0;
        g = static_cast<sf::Uint8>(255 * t);
        b = static_cast<sf::Uint8>(255 * t);
    }
}


sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i mousePixel) {
    float real = ((mousePixel.x / static_cast<float>(m_pixel_size.x)) * m_plane_size.x) +
        (m_plane_center.x - m_plane_size.x / 2.0f);

    float imag = ((mousePixel.y / static_cast<float>(m_pixel_size.y)) * -m_plane_size.y) +
        (m_plane_center.y + m_plane_size.y / 2.0f); // Flip for y-axis

    return { real, imag };
}
