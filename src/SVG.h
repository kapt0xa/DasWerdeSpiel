#pragma once

#include "rapidxml/rapidxml.hpp"

namespace spiel
{
    class SVG
    {
    public:
        explicit SVG(const std::string& filePath);
        ~SVG() = default;

        // Add methods to access SVG data as needed

    private:
        rapidxml::xml_document<> doc; // XML document object
        std::vector<char> xmlData; // Buffer to hold the XML data
    };
    //https://developer.mozilla.org/en-US/docs/Web/SVG/Reference/Element
    // <a> hiberlynk, not supported
    // <animate> amimation, not supported
    // <animateMotion> animation, not supported
    // <animateTransform> animation, not supported
    // <circle> to be implemented
    // <clipPath> clipping (cutting), not supported
    // <defs> definitions, not supported
    // <desc> comments, to be implemented
    // <ellipse> to be implemented
    // <feBlend>
    // <feColorMatrix>
    // <feComponentTransfer>
    // <feComposite>
    // <feConvolveMatrix>
    // <feDiffuseLighting>
    // <feDisplacementMap>
    // <feDistantLight>
    // <feDropShadow>
    // <feFlood>
    // <feFuncA>
    // <feFuncB>
    // <feFuncG>
    // <feFuncR>
    // <feGaussianBlur>
    // <feImage>
    // <feMerge>
    // <feMergeNode>
    // <feMorphology>
    // <feOffset>
    // <fePointLight>
    // <feSpecularLighting>
    // <feSpotLight>
    // <feTile>
    // <feTurbulence>
    // <filter>
    // <foreignObject>
    // <g>
    // <image>
    // <line>
    // <linearGradient>
    // <marker>
    // <mask>
    // <metadata>
    // <mpath>
    // <path>
    // <pattern>
    // <polygon>
    // <polyline>
    // <radialGradient>
    // <rect>
    // <script>
    // <set>
    // <stop>
    // <style>
    // <svg>
    // <switch>
    // <symbol>
    // <text>
    // <textPath>
    // <title>
    // <tspan>
    // <use>
    // <view>
}