// lattice object type
#define LIQUID 0
#define SOLID 1
#define ISOLATED_SOLID 2
#define ABSORBER 3
// compute modes
#define CPU 0
#define CPU_PARALLEL 1
#define GPU 2
// brush figures
#define SQUARE 0
#define CIRCLE 1
// emitter figures
#define SIN_EMIT 0
#define SQR_EMIT 1
#define SAW_EMIT 2
#define TRI_EMIT 3
#define WAV_EMIT 4
// node parameters
#define Nc 9 
#define csSqd 1.0 / 3.0
#define cs 1.0 / sqrt(3.0)
#define start_rho 1.0
#define rho_inc 0.1
//#define maxPressInc 0.01
#define max_rho 1.1
// GPU computing
#define NUM_TREADS 4
#define blockSize 8

enum direction
{
	East = 1, North, West, South, 
	NorthEast, NorthWest, SouthWest, SouthEast
};
