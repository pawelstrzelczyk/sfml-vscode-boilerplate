#include "generator.hpp"
#include "punkt.hpp"
//#include "queue.hpp"
#include "tsp.hpp"
#include <algorithm>
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
	//qu q;

	//std::queue<punkt::pkt> Q;
	srand(time(0));
	ifstream in;
	gen.instanceGenerator(20, 1000, "random10_100.txt");
	in.open("st70.txt");
	//---------------wczytywanie-----------------------
	int liczba;
	in >> liczba;
	punkt::pkt tab[1001];
	const int liczba_ = liczba;
	double x, y, maxX, maxY;
	int id, i = 0;

	while (!in.eof())
	{
		in >> id >> x >> y;
		tab[i].setId(id);
		tab[i].setX(x);
		tab[i].setY(y);
		i++;
		if (maxX < x)
			maxX = x;
		if (maxY < y)
			maxY = y;
	}
	in.close();
	//-------------------------------------------------
	//-------------deklaracja zmiennych------------------
	int colonySize = 40, alpha = 9, beta = 12, *bestOfAll = new int[liczba_ + 1], sim_n = 2000;
	double evaporation = 0.4, Q = 1, MIN = 1000000, *best = new double[liczba_ + 1];
	bool *taken = new bool[liczba_], repeating = true;
	double **distances = new double *[liczba_], **visibility = new double *[liczba_], **pheromone = new double *[liczba_], *oneAntdistance = new double[colonySize];
	int** routes = new int*[colonySize];
	for (i = 0; i < liczba_; i++)
	{
		distances[i] = new double[liczba_];
		visibility[i] = new double[liczba_];
		pheromone[i] = new double[liczba_];
		taken[i] = false;
		if (i < colonySize)
			routes[i] = new int[liczba_ + 1];
	}
	for (int i = 0; i < liczba_; i++)
	{
		for (int j = 0; j < liczba_; j++)
		{
			distances[i][j] = 0;
			visibility[i][j] = 0;

			if (i != j)
				pheromone[i][j] = 0.0001;
			else
				pheromone[i][j] = 0;

			if (i < colonySize)
				routes[i][j] = 0;
		}
	}

	int R, m = colonySize;
	for (int i = 0; i < colonySize; i++)
	{
		oneAntdistance[i] = 0;
		for (int j = 0; j < liczba_ + 1; j++)
		{
			if (j != 0)
				routes[i][j] = 0;
			else
			{
				if (repeating == true)
				{
					for (int z = 0; z < liczba_; z++)
					{
						if (taken[z] == false)
						{
							routes[i][j] = rand() % (liczba_);
							taken[routes[i][j]] = true;
						}
					}
				}
				else if (repeating == false)
				{
					if (m > 0)
					{
						R = rand() % (liczba_);
						if (taken[R] == false)
						{
							routes[i][j] = R;
							taken[routes[i][j]] = true;
							m--;
						}
						else
						{

							do
							{
								R = rand() % (liczba_);
							} while (taken[R] == true);
							routes[i][j] = R;
							taken[routes[i][j]] = true;
							m--;
						}
					}
				}
			}
		}
	}

	int start = clock();
	/*Q.push(tab[0]); //zachlanny
	tsp.komiwojazer(liczba_, tab, Q);
	q.printQueue(Q);
	q.returnQueue(Q, tab);*/
	//-----------------symulacja i zapis najlepszego wyniku-------------------

	for (int i = 0; i < sim_n; i++)
	{
		tsp.ants(liczba_, colonySize, tab, distances, visibility, pheromone, routes, alpha, beta);
		tsp.pherAct(liczba_, evaporation, colonySize, pheromone, routes, distances, Q);
		best = tsp.countCost(liczba_, colonySize, distances, routes);
		for (int i = 0; i < colonySize; i++)
		{
			if (best[i] < MIN)
			{
				MIN = best[i];
				for (int j = 0; j <= liczba_; j++)
				{
					bestOfAll[j] = routes[i][j];
				}
			}
		}
	}
	/*for (int i = 0; i < colonySize; i++)
	{
		for (int j = 0; j < liczba_; j++)
		{
			cout << routes[i][j] << " ";
		}
		cout << endl;
	}*/
	int end = (clock() - start) / 1000;
	//std::cout << end << std::endl;
	std::cout << std::endl
			  << MIN << std::endl;
	ofstream out;
	out.open("route.csv");
	for (int j = 0; j <= liczba_; j++)
	{
		out << bestOfAll[j] + 1 << endl;
	}
	std::cout << std::endl
			  << "time: "
			  << end << std::endl;
	out.close();
	//---------------------------------------------------------------------------
	/*ofstream out;
	out.open("visibility.csv");
	for (int i = 0; i < liczba_; i++)
	{
		for (int j = 0; j < liczba_; j++)
		{
			out << visibility[i][j] << ";";
		}
		out << endl;
	}
	out.close();
	out.open("routes.csv");
	for (int i = 0; i < colonySize; i++)
	{
		for (int j = 0; j < liczba_; j++)
		{
			out << routes[i][j] << ";";
		}
		out << endl;
	}
	out.close();*/
	/*out.open("pheromone.csv");
	for (int i = 0; i < liczba_; i++)
	{
		for (int j = 0; j < liczba_; j++)
		{
			out << pheromone[i][j] << ";";
		}
		out << endl;
	}
	out.close(); */

	//const int temp_size = liczba_ * 2;
	/*int* tab_pomX = new int[liczba_];
	int* tab_pomY = new int[liczba_];
	i = 0;

	while (!Q.empty())
	{
		tab_pomX[i] = Q.front().getX();
		tab_pomY[i] = Q.front().getY();
		Q.pop();
	}*/
	//-----------------------display----------------------------
	std::cout << maxX << " " << maxY;
	if (int(maxX / 1920) < 1)
		maxX = 1920;
	if (int(maxY / 1050) < 1)
		maxY = 1050;
	sf::RenderWindow window(sf::VideoMode(1920, 1050), "Window");
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
		window.clear(sf::Color::White);
		float temp2 = 0;
		float temp1 = 0;
		sf::VertexArray lines(sf::LineStrip, liczba_ + 1);
		for (int i = 0; i <= liczba_; i++)
		{

			temp1 = (tab[bestOfAll[i]].getX()) / (maxX / 1920.0);
			temp2 = 1050 - (tab[bestOfAll[i]].getY()) / (maxY / 1050.0);
			sf::String str;

			string z = "";
			z += char((tab[bestOfAll[i]].getId() / 100) % 10 + '0');
			z += char((tab[bestOfAll[i]].getId() / 10) % 10 + '0');
			z += char(tab[bestOfAll[i]].getId() % 10 + '0');

			sf::Text entity(z, font, 20);
			entity.setFillColor(sf::Color::Black);
			entity.setPosition(temp1, temp2);
			window.draw(entity);
			lines[i].position = sf::Vector2f(temp1, temp2);
			lines[i].color = sf::Color::Black;
			/*for (int i = 0; i < colonySize; i++)
			{
				for (int j = 0; j <= liczba_; j++)
				{
					temp1 = (tab[routes[i][j]].getX()) / (maxX / 1920.0);
					temp2 = 1050 - (tab[routes[i][j]].getY()) / (maxY / 1050.0);
					sf::String str;

					string z = "";
					z += char((tab[routes[i][j]].getId() / 100) % 10 + '0');
					z += char((tab[routes[i][j]].getId() / 10) % 10 + '0');
					z += char((tab[routes[i][j]]).getId() % 10 + '0');

					sf::Text entity(z, font, 20);
					entity.setFillColor(sf::Color::Black);
					entity.setPosition(temp1, temp2);
					window.draw(entity);

					lines[j].position = sf::Vector2f(temp1, temp2);
					lines[j].color = sf::Color::Black;
				}
				window.draw(lines);
			}*/
		}
		window.draw(lines);
		window.display();
	}
	//---------------------------------------------------------------------
	for (int i = 0; i < liczba_; i++)
	{
		if (i < colonySize)
		{
			delete[] routes[i];
		}

		delete[] distances[i];
		delete[] visibility[i];

		delete[] pheromone[i];
	}

	delete[] oneAntdistance;
	delete[] routes[colonySize];
	delete[] routes;
	delete[] distances;
	delete[] pheromone;
	delete[] visibility;
}