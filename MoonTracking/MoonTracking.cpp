#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "SpiceUsr.h"
#include <ctime>

using namespace std;

int main()
{
    furnsh_c("C:\\Users\\Jos\351 Martins\\Desktop\\de430.bsp");     // Load planetary ephemeris kernel
    furnsh_c("C:\\Users\\Jos\351 Martins\\Desktop\\naif0012.tls");  // Load leap seconds kernel
   

    // Get current UTC time as a string in the format "YYYY-MM-DDTHH:MM:SS"
    char utc_time[32];
    time_t now = time(NULL);
    strftime(utc_time, sizeof(utc_time), "%Y-%m-%dT%H:%M:%S", gmtime(&now));

    cout << "Current UTC time: " << utc_time << endl << std::endl;

    // Get current Ephemeris Time (ET)
    SpiceDouble et_start;
    utc2et_c(utc_time, &et_start);


    //// compute distance from earth to moon
    //double moon_pos[3];
    //spkezp_c(301, et, "j2000", "none", 399, moon_pos, &et);
    //double distance = vnorm_c(moon_pos);
    //cout << "distance from earth to moon: " << distance << " km" << endl;



    // Set start and end times
    SpiceDouble et_end;
    et_end = et_start + 86400 * 30;


    // Compute the distance to the moon every day for a month
    SpiceDouble et = et_start;
    SpiceDouble moon_pos[3];
    SpiceDouble distance;
    SpiceDouble et_out;
    int day = 3;

    while (et < et_end) {
        // Compute position of Moon relative to Earth at the given time
        spkezp_c(301, et, "J2000", "NONE", 399, moon_pos, &et_out);

        // Compute distance to the Moon
        distance = vnorm_c(moon_pos);

        et2utc_c(et, "ISOC", 6, sizeof(utc_time), utc_time);
        // Print the distance to the Moon
        //cout << "Distance to Moon at " << utc_time << " Distance: " << distance << " km" << endl;

        // Print the distance to the Moon
        cout << "| ";
        for (int i = 0; i < (distance -350000)/ 5000; i++) {
            cout << "-";
        }
        cout << "\t\tDay " << day << ": " << distance << " km " << endl;;

        // Increment time by 1 day (86400 seconds)
        et += 86400;
        day++;
    }

    // Unload the kernel files
    unload_c("de421.bsp");


    return 0;
}
