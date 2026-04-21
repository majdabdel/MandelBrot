Mandelbrot Set Visualizer
An interactive Mandelbrot Set renderer built in C++ with SFML. Allows real-time exploration of the complex plane with zoom and pan controls.

Built with: C++, SFML, <complex>

Controls:

Left click — zoom in and recenter
Right click — zoom out and recenter
Mouse movement — displays current complex coordinates on screen

How it works: Iteratively computes whether points on the complex plane escape to infinity, coloring each pixel based on iteration count to produce the classic Mandelbrot fractal. The ComplexPlane class handles coordinate mapping, iteration logic, and pixel rendering.

Dependencies: SFML 2.x

Build: g++ main.cpp ComplexPlane.cpp -o mandelbrot -lsfml-graphics -lsfml-window -lsfml-system
