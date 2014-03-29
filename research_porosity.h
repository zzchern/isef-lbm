#include "field.h"

// Критерии:
// * Массовая доля пор
// * Геометрия пор
// * Звукопроницаемость (для различных частот)
// * Звукопоглощаемость/отражаемость
// * Алгоритмы по поиску пути


class Research_Porosity
{
public:
		Research_Porosity(Field *field);
		void set_area(int x_left, int y_top, int x_right, int y_bot);
		float calculate_porosity_of_area();
		int size_of_surface();
		void experiment();
private:
		int x_left;
		int x_right;
		int y_top;
		int y_bot;
		float porosity;
		int *cell;
		Field* field;
		Field* field_exp; // Field for experiments
		// Sound reflection and propagation
		void create_exp_field();
		void delete_exp_field();
		void create_wave();
		void copy_sample_of_investigation();
		void iterations();
		int direction_of_wave_motion;
		int L;	
};
