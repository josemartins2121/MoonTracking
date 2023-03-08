#define _CRT_SECURE_NO_WARNINGS


/*
	  Local Parameters
	  Location of Meta-Kernel with kernels referencing
		-> in the code example the bsp added only goes to 2020, de420.bsp (large file)
 */
#define METAKR "../kernels/erotat.tm"

#include <iostream>
#include "SpiceUsr.h"
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <cmath>
#include <vector>




#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;




int main()
{
	//Load Meta Kernel 
	furnsh_c(METAKR);

	// Define the observer's latitude and longitude (Rad) FEUP Antenna Location
	const SpiceDouble latitude = rpd_c() * 41.178102;
	const SpiceDouble longitude = rpd_c() * -8.594996;
	const SpiceDouble altitude = 200;

	SpiceDouble          radii[3];
	SpiceInt             n;
	SpiceDouble          rp;
	SpiceDouble          re;
	SpiceDouble          f;
	SpiceDouble          rectan[3];


	/*
		Retrieve the triaxial radii of the Earth
		*/
	bodvrd_c("EARTH", "RADII", 3, &n, radii);

	/*
		Calculate the flattening factor for the Earth.

				 equatorial_radius - polar_radius
		flat =   ________________________________

					   equatorial_radius
		*/
	re = radii[0];
	rp = radii[2];
	f = (re - rp) / re;

	georec_c(longitude, latitude, altitude, radii[0], f, rectan);


	while (1) {

		// Start the timer
		auto start_time = std::chrono::high_resolution_clock::now();

		SpiceDouble             hmoonv[3];
		SpiceDouble             emoonv[3];
		SpiceDouble             ltime;
		SpiceDouble             r;
		SpiceDouble             lat;
		SpiceDouble             lon;
		SpiceDouble				observer_pos[3];
		SpiceDouble				xform[3][3];
		SpiceDouble				dist;
		SpiceDouble				lt;




		// Get current UTC time as a string in the format "YYYY-MM-DDTHH:MM:SS"
		char utc_time[32];
		time_t now = time(NULL);
		strftime(utc_time, sizeof(utc_time), "%Y-%m-%dT%H:%M:%S", gmtime(&now));

		cout << "Current UTC time: " << utc_time << endl << std::endl;

		// Get current Ephemeris Time (ET)
		SpiceDouble et;
		utc2et_c(utc_time, &et);


		/*
			 Look up the apparent position of the Moon relative
			 to the Earth's center in the ITRF93 frame at ET.
			 */
		spkpos_c("moon", et, "itrf93",
			"lt+s", "earth", hmoonv, &ltime);


		/*
			 Express the Moon direction in terms of longitude
			 and latitude in the ITRF93 frame.
			 */
		reclat_c(hmoonv, &r, &lon, &lat);

		printf("Earth-Moon direction using high accuracy\n"
			"PCK and ITRF93 frame:\n"
			"Moon lon (deg):        %15.6f\n"
			"Moon lat (deg):        %15.6f\n"
			"\n",
			lon * dpr_c(),
			lat * dpr_c());




		printf("Geodetic coordinates in deg and km (lon, lat, alt)\n");
		printf(" %13.6f %13.6f %13.6f\n",
			longitude * dpr_c(), latitude * dpr_c(), altitude);
		printf("Rectangular coordinates in km (x, y, z)\n");
		printf(" %13.6f %13.6f %13.6f\n", rectan[0], rectan[1], rectan[2]);
		cout << std::endl;


		// Compute the distance between the Moon and the topocentric position
		vsub_c(hmoonv, rectan, emoonv);
		dist = vnorm_c(emoonv);

		printf("Distance from Moon to topocentric position: %f km\n", dist);
		cout << std::endl;



		// Compute the state (position and velocity) of the observer and target
		SpiceDouble state_observer[6];
		SpiceDouble state_target[6];
		spkezr_c("EARTH", et, "J2000", "lt+s", "SSB", state_observer, &lt);


		// Extract velocity component (km/s)
		SpiceDouble moon_vel = vnorm_c(&state_observer[3]);

		// Print the result
		cout << "The velocity of the Earth relative to the solar system barycenter at J2000 is " << moon_vel << " km/s" << endl;
		cout << std::endl;

		spkezr_c("MOON", et, "J2000", "lt+s", "SSB", state_target, &lt);


		// Extract velocity component (km/s)
		SpiceDouble earth_vel = vnorm_c(&state_observer[3]);

		// Print the result
		cout << "The velocity of the Moon relative to the solar system barycenter at J2000 is " << earth_vel << " km/s" << endl;
		cout << std::endl;


		// Compute the relative velocity between the Earth and Moon
		SpiceDouble relative_velocity[3];
		vsub_c(state_target + 3, state_observer + 3, relative_velocity);

		// Print the relative velocity
		std::cout << "Relative velocity between the Earth and Moon: "
			<< relative_velocity[0] << ", "
			<< relative_velocity[1] << ", "
			<< relative_velocity[2] << std::endl;


		// Extract velocity component (km/s)
		SpiceDouble relavite_velocity_norm = vnorm_c(relative_velocity);
		//
		// Print the result
		cout << "The relative velocity at J2000 is " << relavite_velocity_norm << " km/s" << endl;
		cout << std::endl;


		// Calculate the unit vector pointing from the observer to the transmitter
		SpiceDouble unit_vector[3];
		vhat_c(emoonv, unit_vector);

		// Calculate the component of the relative velocity along the line of sight
		SpiceDouble line_of_sight_velocity = vdot_c(relative_velocity, unit_vector);




		// Calculate the doppler shift
		double f_transmit = 2.4e9; // frequency of the transmitted signal in Hz
		double delta_f = f_transmit * line_of_sight_velocity / clight_c(); // speed of light in m/s



		// Output the result
		std::cout << "Doppler shift of the transmitted signal from " << "Earth" << " to the Moon at frequency " << f_transmit
			<< " is: " << delta_f << " Hz" << std::endl;
		cout << std::endl;

		// Compute the Moon's azimuth and elevation from the observer's position
		double azlsta[6] = { 0.0 };
		SpiceDouble   ltim;

		azlcpo_c("ELLIPSOID", "MOON", et, "lt+s", SPICEFALSE, SPICETRUE, rectan,
			"EARTH", "ITRF93", azlsta, &ltim);

		// Print the results
		std::cout << "Azimuth: " << dpr_c() * azlsta[1] << " degrees" << std::endl;
		std::cout << "Elevation: " << dpr_c() * azlsta[2] << " degrees" << std::endl;


		cout << std::endl;
		// Stop the timer and print the elapsed time
		auto end_time = std::chrono::high_resolution_clock::now();
		auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
		std::cout << "Elapsed time: " << elapsed_time << " microseconds" << std::endl;






#ifdef _WIN32
		Sleep(1000);
#else
		usleep(100000);
#endif


#ifdef _WIN32
		system("cls");
#else
		system("reset");
#endif

	}





	return 0;
}
