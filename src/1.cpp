#include "generator.hpp"
#include "punkt.hpp"
#include "queue.hpp"
#include "tsp.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <queue>

int main()
{
	generator gen;
	TSP tsp;
	qu q;

	std::queue<punkt::pkt> Q;
	srand(time(0));
	ifstream in;
	gen.instanceGenerator(25, 1000);
	in.open("dane.txt");

	int i = 0;
	int liczba;
	in >> liczba;
	const int liczba_ = liczba;

	punkt::pkt tab[25];
	int x, y, id;

	while (!in.eof())
	{
		in >> id >> x >> y;
		tab[i].setId(id);
		tab[i].setX(x);
		tab[i].setY(y);
		i++;
	}

	Q.push(tab[0]);
	tsp.komiwojazer(liczba_, tab, Q);
	punkt::pkt t[26];
	q.printQueue(Q);
	q.returnQueue(Q, t);
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Window");
	/*const int temp_size = liczba_*2;
	int tab_pomX[liczba_];
	int tab_pomY[liczba_];
	int i=0;
	while(!Q.empty()){
	 tab_pomX[i] = Q.front().getX();
	 tab_pomY[i] = Q.front().getY();
	 Q.pop();
	}*/
	while (window.isOpen())
	{
		sf::Font font;
		font.loadFromFile("times.ttf");
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::Black);
		float temp2 = 0;
		float temp1 = 0;
		sf::VertexArray lines(sf::LineStrip, liczba_ + 1);
		for (int i = 0; i < liczba_ + 1; i++)
		{
			//float(Q.front().getX());
			//float(Q.front().getY());

			temp1 = t[i].getX();
			temp2 = t[i].getY();
			sf::String str;

			string z="";
			z+= char((t[i].getId() / 10) % 10 + '0');
			z+= char(t[i].getId() % 10 + '0');


			sf::Text entity(z, font, 30);
			entity.setPosition(temp1, temp2);
			window.draw(entity);
			//Q.pop();
			lines[i].position = sf::Vector2f(temp1, temp2);
		}

		window.draw(lines);
		window.display();
	}
}