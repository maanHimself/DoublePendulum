#include "Pendulum.h"

Pendulum::Pendulum(int x, int y, int r,int m, int a ,float scale)
{
	this->scale = scale;
	this->x = x;
	this->y = y;
	this->r = r *scale;
	this->a = (a * PI) / 180;
	this->m = m;
	this->upperPendulum = nullptr;
	init();
}


Pendulum::Pendulum(int r,int m,int a, Pendulum* upperPendulum,float scale)
{
	this->scale = scale;
	this->r = r * scale;
	this->x = upperPendulum->getPointX();
	this->y = upperPendulum->getPointY();
	this->a = (a * PI) / 180;
	this->m = m;
	this->upperPendulum = upperPendulum;
	init();
}

void Pendulum::update(float dt)
{
	this->v += this->va * dt;
	this->a += this->v * dt;
	//std::cout << "dv " << v << std::endl;
	//std::cout << "da"  << a << std::endl;

	this->arm.setRotation(((double)this->a * 180) / PI);
	
	if (upperPendulum != nullptr) {
		this->x = upperPendulum->getPointX();
		this->y = upperPendulum->getPointY();
		this->arm.setPosition(this->x,this->y);
	}

	this->body.setPosition(getPointX(), getPointY());

}

void Pendulum::updatePos()
{

	double x1 = upperPendulum->getPointX();
	double y1 = upperPendulum->getPointY();
	
	double x2 = getPointX();
	double y2 = getPointY();
	
	double m1 = upperPendulum->m;
	double m2 = this->m;
	
	double a1 = upperPendulum->a;
	double a2 = this->a;
	
	double v1 = upperPendulum->v;
	double v2 = this->v;
	
	double r1 = upperPendulum->r;
	double r2 = this->r;

	double num1 = -1 * G * (2 * (double)m1 + (double)m2) * sin(a1);
	double num2 = -1 * m2 * G * sin(a1 - 2 * a2) ;
	double num3 = (double)-1 * (double)2 * (double)sin(a1 - a2) * (double)m2 * (v2 * v2 * r2  +  v1 * v1 * r1* cos(a1 - a2));
	double den = r1 * (2 * m1 + m2 - m2 * cos(2 * a1 - 2 * a2));

	upperPendulum->va = (num1 + num2 + num3) / den;

	num1 = 2 * sin(a1 - a2);
	num2 = v1 * v1 * r1 * (m1 + m2);
	num3 = G * (m1 + m2) * cos(a1) + v2 * v2 * r2 * m2 * cos(a1 - a2);
	den = r2 * (2 * m1 + m2 - m2 * cos(2 * a1 - 2 * a2));

	this->va = (num1 *  ( num2 + num3)) / den;
}

void Pendulum::updateMouse(sf::RenderWindow* window, bool pressed)
{

	sf::Vector2f mouseView = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
		
	if (body.getGlobalBounds().contains(mouseView) && pressed) {
		active = true;
	}
	if (active) {
		sf::Vector2f mousePos(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);
		sf::Vector2f o(this->x, this->y);

		sf::Vector2f dir = o - mousePos;

		sf::Vector2f p(0, -50);
		float dot = dir.x * p.x + dir.y * p.y;
		float rhs = sqrt(dir.x * dir.x + dir.y * dir.y) * sqrt(p.x * p.x + p.y * p.y);
		float theta = acos(dot / rhs);

		if (mousePos.x > this->x) {
			theta = 2 * PI - theta;
		}

		this->a = theta;
	
	}
	
}

float Pendulum::getPointX()
{	
	return this->x - sin(this->a) * this->r;
}

float Pendulum::getPointY()
{
	return this->y + cos(this->a) * this->r;
}

void Pendulum::init()
{
	sf::Vector2f armSize(5 * scale, this->r);
	this->arm = sf::RectangleShape(armSize);
	arm.setOrigin(arm.getSize().x / 2, 0);
	arm.setRotation(((double)a * 180) / PI);
	arm.setPosition(this->x, this->y);
	this->arm.setFillColor(sf::Color::Black);

	this->body = sf::CircleShape(10 * scale);
	this->body.setOrigin(this->body.getRadius(), this->body.getRadius());
	this->body.setPosition(getPointX(), getPointY());
	this->body.setFillColor(sf::Color::Red);

}
