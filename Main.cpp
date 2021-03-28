#include<SFML/Graphics.hpp>
#include<SFML/OpenGL.hpp>
#include"Pendulum.h"
#include<vector>
#include<imgui.h>
#include<imgui-SFML.h>


int main() {
	
	sf::RenderWindow window(sf::VideoMode(1280,800),"test", sf::Style::Titlebar | sf::Style::Close);
	sf::Event event;

	ImGui::SFML::Init(window);

	int scale = 2;

	Pendulum p1(window.getSize().x / 2, window.getSize().y / 2 - 100, 100 ,200, 90 , scale);
	Pendulum p2(100, 20,90, &p1,scale);
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::Clock clock;

	glLineWidth(1);


	bool pressed = false;
	bool drawTrace = false;

	std::vector<sf::Vertex> traceVertexArray;
	std::vector<sf::Vertex> lineVertexArray(100);

	window.setFramerateLimit(60);

	while (window.isOpen()) {
		while (window.pollEvent(event)) {

			ImGui::SFML::ProcessEvent(event);
			switch (event.type)
			{
			case sf::Event::Closed :
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}else if (event.key.code == sf::Keyboard::Space) {
					if (drawTrace) drawTrace  = false;
					else drawTrace = true;
				}
				break;
			case sf::Event::MouseButtonPressed:
				pressed = true;
				break;
			case sf::Event::MouseButtonReleased:
				p1.active = false;
				p2.active = false;
				pressed = false;
			default:
				break;
			}
		}


		ImGui::SFML::Update(window, clock.getElapsedTime());

		ImGui::Begin("Sample window");

		ImGui::SliderFloat("Mass 1 ", &p1.m, 1.f, 500.f);
		ImGui::SliderFloat("Mass 2 ", &p2.m, 1.f, 500.f);
		if (ImGui::SliderInt("r1 ", &p1.r, 5, 400)) {
			p1.init();
			p2.init();
		}
		if (ImGui::SliderInt("r2 ", &p2.r, 5, 400)) {
			p1.init();
			p2.init();
		}

		ImGui::End();

		float dt = clock.getElapsedTime().asSeconds() * 10;
		clock.restart();




		p2.updatePos();

		if (pressed) {
			p1.va = 0;
			p1.v = 0;
			p2.va = 0;
			p2.v = 0;
		}

		p1.update(dt);
		p2.update(dt);
		p1.updateMouse(&window,pressed);
		p2.updateMouse(&window,pressed);
		
		traceVertexArray.insert(traceVertexArray.begin(), sf::Vertex(sf::Vector2f(p2.getPointX(), p2.getPointY()),sf::Color::Green));

		for (int i = 0; i < std::min(lineVertexArray.size(),traceVertexArray.size()); i++) {
			lineVertexArray[i] = sf::Vertex(traceVertexArray[i].position, sf::Color(255, 0, 0, (255* 3)/ (i+1)));
		}
		
		window.clear(sf::Color(255,255,255,255));

		if(drawTrace)
			window.draw(traceVertexArray.data(), traceVertexArray.size(), sf::PrimitiveType::LineStrip);

		window.draw(lineVertexArray.data(), std::min(lineVertexArray.size(), traceVertexArray.size()), sf::PrimitiveType::LineStrip);
		window.draw(p2.arm);
		window.draw(p2.body);
		window.draw(p1.arm);
		window.draw(p1.body);
		ImGui::SFML::Render(window);
		window.display();

	}
}