#pragma once
#define Air 0
#define O2  1
#define CO2 2
#define H2  3
#define H2O 4
#define Oil 5 // castrol oil

#define DirectConversion 0
#define ProportionalConversion 1

//сетматириал аир сеттау 1.0
//гетдт ProportionalConversion

//ввод макс частоты -> умножаем её на два -> сетмакссемплрейт 
//из гетдх умноженного на размер поля в ячейках получаем реальные размеры поля
//выводим гет дт.

//при установке эмиттера пользю вводит частоту в герцах
//проверяем, входит ли в диапазон заданных частот

//получаем из неё период, 1/частоту, делим на дт, и это период в эмиттер.

class UnitConverter
{
public:
	UnitConverter();
	void set_material(int m);
	void set_tau(float Tau);
	float get_dx(int type);
	float get_dt(int type);
	void set_max_sample_rate(int sample_rate);//8000
	int get_max_sample_rate();//8000	
	void display(int nx, int ny);
private:
	float Viscosity[6];
	float SoundSpeedPhys[6];

	int sample_rate;
	float Tau;
	int material;
	float lSoundSpeed;
};
