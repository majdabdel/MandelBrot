#pragma once
#ifndef COMPLEXPLANE_H
#define COMPLEXPLANE_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>
#include <complex>
#include <vector>
#include <sstream>
#include <complex>
using namespace sf;
using namespace std;
// Constants
const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0f;
const float BASE_HEIGHT = 4.0f;
const float BASE_ZOOM = 0.5f;
const int m_pixelHeight = 540;
const int m_pixelWidth = 960;

class ComplexPlane : public sf::Drawable {
public:
	// Constructor
	ComplexPlane(int pixelWidth, int pixelHeight);

	// Public member functions
	void zoomIn();
	void zoomOut();
	void setCenter(sf::Vector2i mousePixel);
	void setMouseLocation(Vector2i mousePixel);
	void loadText(Text& text);
	size_t countIterations(Vector2f coord);
	void iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b);
	Vector2f mapPixelToCoords(Vector2i pixel);
	void updateRender();

private:
	// Private enum
	enum class State { CALCULATING, DISPLAYING };

	// Private member variables
	VertexArray m_vArray;       // Vertex array for pixels
	State m_state;                  // Current state of the plane
	Vector2f m_mouseLocation;   // Mouse location on the plane
	Vector2i m_pixel_size;      // Pixel dimensions of the screen
	Vector2f m_plane_center;    // Center of the plane
	Vector2f m_plane_size;      // Size of the complex plane
	int m_zoomCount;                // Zoom level
	float m_aspectRatio;            // Aspect ratio of the screen

	// Private helper function
	void draw(RenderTarget& target, RenderStates states) const override;
};

#endif