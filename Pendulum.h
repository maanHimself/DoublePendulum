#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>

class Pendulum
{
public:
	Pendulum(int x, int y, int r,int m,int a, float scale);
	Pendulum(int r,int m,int a, Pendulum* upperPendulum, float scale);
	void update(float dt);
	void updatePos();
	void updateMouse(sf::RenderWindow* window, bool pressed);
	float getPointX();
	float getPointY();
	sf::RectangleShape arm;
	sf::CircleShape body;
	float m;
	float a;
	float va = 0;
	float v = 0;
	bool active = false;
	int r;
	void init();
	
private:
	int x, y;
	Pendulum* upperPendulum;
	const double PI = 3.14159265359;
	const double G = 9.81;
	float scale = 1;

};

