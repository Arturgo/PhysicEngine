#include <iostream>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <boost/polygon/voronoi.hpp>
#include <boost/polygon/polygon.hpp>
using namespace std;
using namespace sf;
using namespace boost::polygon;

typedef polygon_data<int> Polygon;
class Atom;
class Material;

vector<Atom> atoms;
vector<Material> materials;

class Material {
	public:
		Color color;
		
		Material(Color _color) {
			color = _color;
		}
};

class Atom : public Drawable, public Transformable {
	public:
		double x, y;
		int id, materialId;
		Polygon shape;
		
		Atom(double _x = 0, double _y = 0, int _id = 0, int _materialId = 0) {
			x = _x;
			y = _y;
			id = _id;
			materialId = _materialId;
		}
		
		void updateVoronoi(voronoi_diagram<double>& diagram);
		void update(Time elapsed);
};

void Atom::updateVoronoi(voronoi_diagram<double>& diagram) {
	
}

void Atom::update(Time elapsed) {
	
}

namespace boost {
	namespace polygon {
		template <>
		struct geometry_concept<Atom> { typedef point_concept type; };

		template <>
		struct point_traits<Atom> {
			typedef int coordinate_type;

			static inline coordinate_type get(const Atom& point, orientation_2d orient) {
				return (orient == HORIZONTAL) ? point.x : point.y;
			}
		};
	}
}

int main() {
	srand(time(NULL));
	
	materials.push_back(Material(Color(255, 255, 0)));
	materials.push_back(Material(Color(0, 255, 255)));
	
	for(int iPoint = 0;iPoint < 100;iPoint++) {
		atoms.push_back(Atom(-200 + rand() % 1200, -100 + rand() % 800, iPoint, rand() % 2));
	}
	
	RenderWindow window(VideoMode(800, 600), "Voronoi");
	
	Clock clock;
	while(window.isOpen()) {
		Event event;
		while(window.pollEvent(event)) {
			if(event.type == Event::Closed)
				window.close();
		}
		
		voronoi_diagram<double> vd;
		construct_voronoi(atoms.begin(), atoms.end(), &vd);
		
		for(Atom& atom : atoms) {
			atom.updateVoronoi(vd);
		}
		
		window.clear(Color::Black);
		
		for(Atom& atom : atoms) {
			window.draw(atom);
		}
		
		window.display();
		
		
		Time elapsed = clock.restart();
		for(Atom& atom : atoms) {
			atom.update(elapsed);
		}
		
		
		
		
		
		/*for(voronoi_diagram<double>::const_edge_iterator it = vd.edges().begin();it != vd.edges().end();++it) {
			VertexArray segment(Lines, 2);
			if(it->is_finite()) {
				segment[0].position = Vector2f(it->vertex0()->x(), it->vertex0()->y());
				segment[1].position = Vector2f(it->vertex1()->x(), it->vertex1()->y());
				
				window.draw(segment);
			}
			else {
				if (it->vertex0()) {
					Atom p1 = points[it->cell()->source_index()];
					Atom p2 = points[it->twin()->cell()->source_index()];
					int end_x = (p1.y - p2.y) * 640;
					int end_y = (p1.x - p2.x) * -640;
					
					segment[0].position = Vector2f(it->vertex0()->x(), it->vertex0()->y());
					segment[1].position = Vector2f(end_x, end_y);
					window.draw(segment);
				}
			}
			
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
		}*/
		
		sleep(milliseconds(50));
	}
	
	return 0;
}
