#include "conway_algorithm.h"

bool conway_future(bool status, int neighbors) {
	// if cell is live
	if (status) {
		// stay live if their are two to three neighbors
		return neighbors >= 2 && neighbors <= 3;
	} else {
		// be born when there are exactly three neighbors
		return neighbors == 3;
	}
}

