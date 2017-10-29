#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <boost/polygon/voronoi.hpp>
using namespace std;
using namespace sf;
using namespace boost::polygon;

struct Point {
	int x, y;
	Point(int _x = 0, int _y = 0) {
		x = _x;
		y = _y;
	}
};

Point operator + (const Point &a, const Point &b) {
	return Point(a.x + b.x, a.y + b.y);
}

Point operator - (const Point &a, const Point &b) {
	return Point(a.x - b.x, a.y - b.y);
}

Point operator * (const int &s, const Point &a) {
	return Point(s * a.x, s * a.y);
}

double norme(const Point &a) {
	return sqrt(a.x * a.x + a.y * a.y);
}

double dist(const Point &a, const Point &b) {
	return norme(a - b);
}

int vect(const Point &a, const Point &b) {
	return a.x * b.y - a.y * b.x;
}

int scal(const Point &a, const Point &b) {
	return a.x * b.x - a.y * b.y;
}

namespace boost {
	namespace polygon {
		template <>
		struct geometry_concept<Point> { typedef point_concept type; };

		template <>
		struct point_traits<Point> {
			typedef int coordinate_type;

			static inline coordinate_type get(const Point& point, orientation_2d orient) {
				return (orient == HORIZONTAL) ? point.x : point.y;
			}
		};
	}
}

int main() {
	vector<Point> points;
	
	for(int iPoint = 0;iPoint < 10;iPoint++) {
		points.push_back(Point(rand() % 800, rand() % 600));
	}
	
	voronoi_diagram<double> vd;
	construct_voronoi(points.begin(), points.end(), &vd);
	
	cout << vd.edges().size() << endl;
	
	RenderWindow window(VideoMode(800, 600), "Voronoi");
	
	while(window.isOpen()) {
		Event event;
		while(window.pollEvent(event)) {
			if(event.type == Event::Closed)
				window.close();
		}
		
		window.clear(Color::Black);
		
		for(voronoi_diagram<double>::const_edge_iterator it = vd.edges().begin();it != vd.edges().end();++it) {
			VertexArray segment(Lines, 2);
			if(it->is_finite()) {
				segment[0].position = Vector2f(it->vertex0()->x(), it->vertex0()->y());
				segment[1].position = Vector2f(it->vertex1()->x(), it->vertex1()->y());
				
				window.draw(segment);
			}
			else {
				if (it->vertex0()) {
					Point p1 = points[it->cell()->source_index()];
					Point p2 = points[it->twin()->cell()->source_index()];
					int end_x = (p1.y - p2.y) * 640;
					int end_y = (p1.x - p2.x) * -640;
					
					segment[0].position = Vector2f(it->vertex0()->x(), it->vertex0()->y());
					segment[1].position = Vector2f(end_x, end_y);
					window.draw(segment);
				}
			}
		}
		
		for(Point p : points) {
			VertexArray point(Points, 1);
	
			point[0].position = Vector2f(p.x, p.y);
			
			window.draw(point);
		}
		
		window.display();
		
		sleep(milliseconds(50));
	}
	
	return 0;
}
