#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
//#include <SFML/Graphics/VertexArray.hpp>

const int WIDTH = 1600;
const int HEIGHT = 1600;

const double AU {149.6e9};
const double G {6.67438e-11};
const double SCALE {70/AU};
const double TIMESTEP = 3600*24;

class Planet{
	private:
		double x_;
		double y_;
		const double radius_;
		const sf::Color colour_;
		const double mass_;

		sf::CircleShape bodyImage_;

		const bool isSun_ {false};

		std::vector<sf::Vertex> orbit_{};

		double distanceToSun_ {};

		double xVel_ {};
		double yVel_ {};

	public:
		Planet(double x, double y, double radius, sf::Color colour, double mass, double yVel = 0, bool isSun = false)
		: x_{x}, y_{y}, radius_{radius}, colour_{colour}, mass_{mass}, yVel_{yVel}, isSun_{isSun}
		{
			bodyImage_.setRadius(radius_);
			bodyImage_.setFillColor(colour_);
			bodyImage_.setOrigin({static_cast<float>(radius_), static_cast<float>(radius_)});
		}

		double getYVel() const{return yVel_;}
		double getX() const{return x_;}
		double getY() const{return y_;}

		void draw(sf::RenderWindow& window){
			double x {x_ * SCALE + WIDTH/2};
			double y {y_ * SCALE + HEIGHT/2};

			if (orbit_.size()>10000){
				orbit_.erase(orbit_.begin());
			}

			if (orbit_.size()>2){
				window.draw(orbit_.data(), orbit_.size(), sf::PrimitiveType::LineStrip);
			}

			bodyImage_.setPosition({static_cast<float>(x),static_cast<float>(y)});
			window.draw(bodyImage_);
		}

		std::pair<double, double> attraction(const Planet& other){
			double distanceX {other.x_ - x_};
			double distanceY {other.y_ - y_};
			double distance {std::sqrt(pow(distanceX,2) + pow(distanceY,2))};

			if (other.isSun_){
				distanceToSun_ = distance;
			}

			double force {G * mass_ * other.mass_ / pow(distance,2)};
			double theta {std::atan2(distanceY, distanceX)};
			double forceX {std::cos(theta) * force};
			double forceY {std::sin(theta) * force};

			return {forceX, forceY};
		}

		void updatePosition(const std::vector<Planet>& planets){
			double totalFx {};
			double totalFy {};

			for (const auto& planet : planets){
				if (this == &planet){
					continue;
				}

				auto [fx, fy] = attraction(planet);
				totalFx += fx;
				totalFy += fy;
			}

			xVel_ += totalFx / mass_ * TIMESTEP;
			yVel_ += totalFy / mass_ * TIMESTEP;

			x_ += xVel_ * TIMESTEP;
			y_ += yVel_ * TIMESTEP;

			double orbitPointX {x_*SCALE + WIDTH/2};
			double orbitPointY {y_*SCALE + HEIGHT/2};
			sf::Vertex point{{static_cast<float>(orbitPointX),static_cast<float>(orbitPointY)}, colour_};
			orbit_.push_back(point);
		}
};


int main()
{
	sf::RenderWindow window( sf::VideoMode( { WIDTH, HEIGHT } ), "Solar System Simulator" );

	Planet sun = Planet(0, 0, 20, sf::Color::Yellow, 1.98892e30, 0, true);

	Planet earth = Planet(-1*AU, 0, 8, sf::Color::Blue, 5.9742e24, 29.783e3);

	Planet mars = Planet(-1.524*AU, 0, 6, sf::Color::Red, 6.39e23, 24.077e3);

	Planet mercury = Planet(0.387*AU, 0, 4, sf::Color::White, 3.3e23, -47.4e3);

	Planet venus = Planet(0.723*AU, 0, 7, sf::Color(255, 165, 0), 4.8685e24, 35.02e3);

	Planet jupiter = Planet(5.2*AU, 550, 16, sf::Color(255, 165, 0), 1.8982e27, -13.1e3);

	Planet saturn = Planet(9.59*AU, -550, 12, sf::Color::Yellow, 5.6834e26, -9.7e3);

	std::vector<Planet> planets {sun, earth, mars, mercury, venus, jupiter, saturn};

	// auto [forceX, forceY] = earth.attraction(sun);
	// std::cout << forceX << " " << forceY;

	window.setFramerateLimit(60);
	while ( window.isOpen() )
	{
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() )
				window.close();
		}

		window.clear();
		for(Planet& planet : planets){
			planet.updatePosition(planets);
			planet.draw(window);
			// std::cout << planet.getX() << "	" << planet.getY() << '\n';
		}
		window.display();
	}
}
