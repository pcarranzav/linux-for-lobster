/*
*/

struct chan {
	char id;
	char on;
	unsigned char gain;
	char coupling;
	unsigned int offset;
};

struct trig {
	char channel;  // Channel for edge detection (0: A, 1: B)
	char slope; 	// Trigger on rising (1) or falling (0) edge
	char aut;		// Auto trigger on (1) or off (0)
	char roll;		// Roll mode
	char noise;		// Noise reject
	char hf;			// High Frequency reject
	char holdOff; 	// Hold-Off value
	char level; 	// Trigger level value
};


