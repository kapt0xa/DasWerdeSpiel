#pragma once

#include "VisualCharacter.h"

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

#include <vector>
#include <string>

namespace spiel
{
    class CharacterEditor
    {
    public:
        // main entry point: runs editor loop
        static void run(sf::RenderWindow& window, [[maybe_unused]] tgui::Gui& gui);

    private:
        // ==== Core data ====
        VisualCharacter root;                 // root character
        std::vector<size_t> selectionPath;    // selected node in hierarchy

        // ==== SFML window + GUI references ====
        sf::RenderWindow& windowPtr;
        [[maybe_unused]] tgui::Gui& guiPtr;

        // ==== Loop ====
        void processEvents(bool& running);
        void update();
        void render();

        // ==== Navigation ====
        VisualCharacter* getSelected();
        const VisualCharacter* getSelected() const;

        VisualCharacter* getByPath(std::vector<size_t>& path);
        const VisualCharacter* getByPath(const std::vector<size_t>& path) const;

        void moveUp();
        void moveDown();
        void moveLeft();
        void moveRight();

        // ==== Editing ====
        void addShape();
        void addGroup();
        void deleteSelected();

        // ==== Debug / Console ====
        void printTree() const;
        void printNode(const VisualCharacter& node, int depth, size_t index) const;

        // ==== Rendering ====
        void drawCharacter(const VisualCharacter& character);

        // ==== File ops ====
        void save(const std::string& path);
        void load(const std::string& path);

        // ==== Helpers ====
        std::string indent(int depth) const;
    };
}