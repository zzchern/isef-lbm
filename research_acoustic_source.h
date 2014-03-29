#include <vector>
#include "field.h"
#include "sound_map.h"
using namespace std;

struct Acoustic_Source
{
		int x;
		int y;
};

struct Acoustic_System
{
		vector<Acoustic_Source> sources;
		float mean_val;
		float max_val;
		float min_val;
		float dev_val;
		int R1;
		int R2;
};

class Research_AS
{
public:
		Research_AS(Field *field);
		void add_acoustic_system(int number_of_sources, Point* positions);
		void add_default_acoustic_system(int grid_size);
		void set_number_of_iterations(int n_of_i);
		void analyze_acoustic_systems(); // Start analyzing
		void save_results(); // Save results to .txt
private:
		void rate();
		bool rated;
		Field* field;
		int number_of_iterations;
		vector<Acoustic_System> systems;
};
