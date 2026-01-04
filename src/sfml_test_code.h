#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

inline int sfml_test_code()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");
 
    // Load a sprite to display
    const sf::Texture texture("../resrc/cute_image.jpg");
    sf::Sprite sprite(texture);
 
    // Create a graphical text to display
    const sf::Font font("../resrc/UbuntuSansMono[wght].ttf");
    sf::Text text(font, "Hello SFML", 50);

     // Load a music to play
    sf::Music music("../resrc/nice_music.ogg");
 
    // Play the music
    music.play();

    const float circleRadius = 50.f;
    sf::CircleShape circle(circleRadius);
    circle.setFillColor(sf::Color::Green);
    
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::MouseButtonPressed>())
            {
                sf::Vector2i mousePosI = event->getIf<sf::Event::MouseButtonPressed>()->position;
                sf::Vector2f mousePosF = window.mapPixelToCoords(mousePosI);
                circle.setPosition(mousePosF + sf::Vector2f{-circleRadius, -circleRadius});
            }
        }
 
        // Clear screen
        window.clear();
 
        // Draw the sprite
        window.draw(sprite);
        
        window.draw(circle);
 
        // Draw the string
        window.draw(text);
 
        // Update the window
        window.display();
    }
}