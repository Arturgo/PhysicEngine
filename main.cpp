#include <iostream>
#include <ctime>
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

void drawPoints(
	vector<Point>& points,
	RenderWindow& window
) {
	int radius = 5;
	for(Point p : points) {
		CircleShape point(radius);
		point.setFillColor(Color(0, 0, 255));
		point.setPosition(Vector2f(p.x - radius, p.y - radius));
		window.draw(point);
	}
}

int main() {
	srand(time(NULL));
	vector<Point> points;
	
	for(int iPoint = 0;iPoint < 100;iPoint++) {
		points.push_back(Point(-200 + rand() % 1200, -100 + rand() % 800));
	}
	
	voronoi_diagram<double> vd;
	construct_voronoi(points.begin(), points.end(), &vd);
	
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
			
			drawPoints(points, window);
			
			VertexArray delaunay_segment(Lines, 2);
			
			delaunay_segment[0].position = Vector2f(
				points[it->cell()->source_index()].x,
				points[it->cell()->source_index()].y
			);
			delaunay_segment[1].position = Vector2f(
				points[it->twin()->cell()->source_index()].x,
				points[it->twin()->cell()->source_index()].y
			);
			
			delaunay_segment[0].color = delaunay_segment[1].color = Color(255, 0, 0);
			
			window.draw(delaunay_segment);
		}
		
		window.display();
		
		sleep(milliseconds(50));
	}
	
	return 0;
}
