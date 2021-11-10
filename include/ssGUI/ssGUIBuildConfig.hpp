#ifndef SSGUI_CONFIG
#define SSGUI_CONFIG

#define USE_DEBUG 1         //This will print all the state of each update (Pre-Update, Render, Post-Render, etc..)
#define REFRESH_CONSOLE 1   //Normally you have this on when USE_DEBUG is on in order to refresh the console otherwise it will be spammed
#define SLOW_UPDATE 0        //This will slow down the update for easier debugging
#define USE_SFML_TIME 0     //Can use SFML time function instead, but it's limited to int32 which could be not enough though

#endif