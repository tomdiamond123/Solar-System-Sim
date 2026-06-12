#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

const int WIDTH = 800;
const int HEIGHT = 800;

const double AU {149.6e9};
const double G {6.67438e-11};
const double SCALE {50/AU};
const double TIMESTEP = 3600*24;

class Planet{
	private:
		double x_;
		double y_;
		double radius_;
		sf::Color colour_;
		double mass_;

		sf::CircleShape bodyImage_;

		bool isSun_ {false};

		std::vector<std::array<int, 2>> orbit_ {};
		double distanceToSun_ {0};

		double xVel_ {0};
		double yVel_ {0};

	public:
		Planet(double x, double y, double radius, sf::Color colour, double mass, double yVel = 0, bool isSun = false)
		: x_{x}, y_{y}, radius_{radius}, colour_{colour}, mass_{mass}, yVel_{yVel}, isSun_{isSun}
		{
			bodyImage_.setRadius(radius_);
			bodyImage_.setFillColor(colour_);
		}

		double getYVel() const{return yVel_;}

		void draw(sf::RenderWindow& window){
			double x {x_ * SCALE + WIDTH/2};
			double y {y_ * SCALE + HEIGHT/2};

			bodyImage_.setPosition({static_cast<float>(x),static_cast<float>(y)});
			window.draw(bodyImage_);
		}
};


int main()
{
	sf::RenderWindow window( sf::VideoMode( { WIDTH, HEIGHT } ), "Solar System Simulator" );

	Planet sun = Planet(0, 0, 20, sf::Color::Yellow, 1.98892e30, 0, true);

	Planet earth = Planet(-1*AU, 0, 8, sf::Color::Blue, 5.9742e24);

	std::vector<Planet> planets {sun, earth};

	while ( window.isOpen() )
	{
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() )
				window.close();
		}

		window.clear();
		for(Planet& planet : planets){
			planet.draw(window);
		}
		window.display();
	}
}
