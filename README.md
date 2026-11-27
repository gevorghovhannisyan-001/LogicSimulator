# Logic Circuit Simulator

A cross-platform desktop digital logic circuit simulator built with **Modern C++ (C++17)**, **CMake**, **Dear ImGui**, **GLFW**, and **OpenGL**. Designed to model and evaluate logic gates interactively using object-oriented design and safe memory management.

## Features

* **Interactive Circuit Simulation:** Add, connect, toggle, and clear logic gates with real-time signal evaluation.
* **Logic Gate Hierarchy:** Object-oriented gate architecture (`Gate` base class with derived gates such as `ConstantGate`, `AND`, `OR`, and `NOT`).
* **Modern C++ Memory Management:** Uses smart pointers (`std::unique_ptr`), move semantics (`std::move`), `explicit` constructors, and variadic templates (`...Args`).
* **ImGui-Based UI:** Real-time control panel and circuit state inspection window rendered over GLFW + OpenGL.
* **Automated Dependency Setup:** Uses CMake (`FetchContent`) for clean build configuration.

## Tech Stack

* **Language:** C++17
* **Build System:** CMake
* **GUI & Graphics:** Dear ImGui, GLFW, OpenGL
* **IDE & Tooling:** Visual Studio 2022, Git, GitHub

## Project Structure

```text
LogicSimulator/
├── src/
│   ├── CMakeLists.txt
│   ├── Gate.h
│   ├── Simulator.h
│   └── main.cpp
├── CMakeLists.txt
├── CMakePresets.json
├── .gitignore
└── README.md
