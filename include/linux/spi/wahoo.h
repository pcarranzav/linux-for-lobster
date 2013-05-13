/*
 * tle62x0.h - platform glue to Infineon TLE62x0 driver chips
 *
 * Copyright 2007 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
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


