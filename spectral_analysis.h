// Based on FFT (Fast Fourier Transform)

class Spectrogram
{
public:
		Spectrogram();
		void load_array(const char* filename);
		void save_results();
		void analyze();
		float* get_freq_array();
private:
		void FFT(float* A, float* F, int N);
		bool is_allocated;
		int size;
		float* amp_array;
		float* frq_array;
		float time_step;
		
};
