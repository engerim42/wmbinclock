/**
* wmBinClock.c by Thomas Kuiper <tkuiper at inxsoft.net> and Sune Molgaard <sune at molgaard.org
*
* Copyright (C) 2003-2004 Thomas Kuiper <tkuiper at inxsoft.net>
* Copyright (C) 2005 - Thomas Kuiper <tkuiper at inxsoft.net> and Sune Molgaard <sune at molgaard.org> (GPLv license2)
* Copyright (C) 2015 - Thomas Kuiper <tkuiper at inxsoft.net> and Sune Molgaard <sune at molgaard.org> (BSD license)
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <X11/X.h>
#include <X11/xpm.h>
#include <math.h>
#include "xutils.h"
#include "wmBinClock_master.xpm"
#include "wmBinClock_mask.xbm"

#define GRAYX 64
#define GRAYY 2
#define NEONREDX 72
#define NEONREDY 2
#define GOLDX 80
#define GOLDY 2
#define CYANX 88
#define CYANY 2
#define PINKX 96
#define PINKY 2
#define BLUEX 104
#define BLUEY 2
#define GREENX 112
#define GREENY 2
#define DARKREDX 120
#define DARKREDY 2
#define NONEX 128
#define NONEY 2
#define MODEV 0
#define MODEH 1
#define SOFF 0
#define SON 1

/* 
 *  Delay between refreshes (in microseconds) 
 */
#define DELAY 10000L
#define DEFAULT_UPDATEDELAY 900L

void ButtonPressEvent(XButtonEvent *);
void KeyPressEvent(XKeyEvent *);
char *StringToUpper(char *);

int UpToDate = 0;
long UpdateDelay;
int GotFirstClick1, GotDoubleClick1;
int GotFirstClick2, GotDoubleClick2;
int GotFirstClick3, GotDoubleClick3;
int DblClkDelay;

void IntToBinary(int x, int *str[]);
void PrintHelp(char err[]);

int main(int argc, char *argv[])
{
    XEvent event;
    struct tm *tmworld;
    int dcoloron_x = GREENX;
    int dcoloron_y = GREENY;
    int coloron_x = NEONREDX;
    int coloron_y = NEONREDY;
    int coloroff_x = GRAYX;
    int coloroff_y = GRAYY;
    int mode = MODEV;
    int superf = SON;
    time_t t;
    char tempstr[20];
    int tmp_str[6];
    int m;
    int clockarray[10][10];
    Window myWindow;
    int i, j, s, tmp_hour, tmp_minute, tmp_second, tmp_day, tmp_month;
    m = 101;
    char strArg[10];
    int iBeginLEDs[2][6] = {{2,0,1,0,1,0},{1,0,0,1,2,0}};
//    iBeginLEDs[0] = ;
//    iBeginLEDs[1] = ;

    if (argc >= 2)
    {
	i = 1;
	while (i < argc)
	{
	    if (strcmp(argv[i],"-h") == 0 || strcmp(argv[i],"--help") == 0)
	    {
		PrintHelp(argv[i]);
		return 0;

	    }
	    else if (strcmp(argv[i],"-i") == 0 )
	    {
		i++;
		if (strcmp(argv[i],"gray") == 0)
		{
		    coloron_x = GRAYX;
		    coloron_y = GRAYY;
		}
		else if (strcmp(argv[i],"neonred") == 0)
		{
		    coloron_x = NEONREDX;
		    coloron_y = NEONREDY;
		}
		else if (strcmp(argv[i],"gold") == 0)
		{
		    coloron_x = GOLDX;
		    coloron_y = GOLDY;
		}
		else if (strcmp(argv[i],"cyan") == 0)
		{
		    coloron_x = CYANX;
		    coloron_y = CYANY;
		}
		else if (strcmp(argv[i],"pink") == 0)
		{
		    coloron_x = PINKX;
		    coloron_y = PINKY;
		}
		else if (strcmp(argv[i],"blue") == 0)
		{
		    coloron_x = BLUEX;
		    coloron_y = BLUEY;
		}
		else if (strcmp(argv[i],"green") == 0)
		{
		    coloron_x = GREENX;
		    coloron_y = GREENY;
		}
		else if (strcmp(argv[i],"darkred") == 0)
		{
		    coloron_x = DARKREDX;
		    coloron_y = DARKREDY;
		}
		else if (strcmp(argv[i],"none") == 0)
		{
		    coloron_x = NONEX;
		    coloron_y = NONEY;
		}
		else
		{
		    PrintHelp(argv[i-1]);
		    return 0;
		}
	    }
	    else if (strncmp(argv[i],"--coloron=",10) == 0)
	    {
		strtok(argv[i],"=");
		strcpy(strArg,strtok(NULL,"\0"));
		
		if (strcmp(strArg,"gray") == 0)
		{
		    coloron_x = GRAYX;
		    coloron_y = GRAYY;
		}
		else if (strcmp(strArg,"neonred") == 0)
		{
		    coloron_x = NEONREDX;
		    coloron_y = NEONREDY;
		}
		else if (strcmp(strArg,"gold") == 0)
		{
		    coloron_x = GOLDX;
		    coloron_y = GOLDY;
		}
		else if (strcmp(strArg,"cyan") == 0)
		{
		    coloron_x = CYANX;
		    coloron_y = CYANY;
		}
		else if (strcmp(strArg,"pink") == 0)
		{
		    coloron_x = PINKX;
		    coloron_y = PINKY;
		}
		else if (strcmp(strArg,"blue") == 0)
		{
		    coloron_x = BLUEX;
		    coloron_y = BLUEY;
		}
		else if (strcmp(strArg,"green") == 0)
		{
		    coloron_x = GREENX;
		    coloron_y = GREENY;
		}
		else if (strcmp(strArg,"darkred") == 0)
		{
		    coloron_x = DARKREDX;
		    coloron_y = DARKREDY;
		}
		else if (strcmp(strArg,"none") == 0)
		{
		    coloron_x = NONEX;
		    coloron_y = NONEY;
		}
		else
		{
		    PrintHelp(argv[i]);
		    return 0;
		}
	    }
	    else if (strcmp(argv[i],"-o") == 0 )
	    {
		i++;
		if (strcmp(argv[i],"gray") == 0)
		{
		    coloroff_x = GRAYX;
		    coloroff_y = GRAYY;
		}
		else if (strcmp(argv[i],"neonred") == 0)
		{
		    coloroff_x = NEONREDX;
		    coloroff_y = NEONREDY;
		}
		else if (strcmp(argv[i],"gold") == 0)
		{
		    coloroff_x = GOLDX;
		    coloroff_y = GOLDY;
		}
		else if (strcmp(argv[i],"cyan") == 0)
		{
		    coloroff_x = CYANX;
		    coloroff_y = CYANY;
		}
		else if (strcmp(argv[i],"pink") == 0)
		{
		    coloroff_x = PINKX;
		    coloroff_y = PINKY;
		}
		else if (strcmp(argv[i],"blue") == 0)
		{
		    coloroff_x = BLUEX;
		    coloroff_y = BLUEY;
		}
		else if (strcmp(argv[i],"green") == 0)
		{
		    coloroff_x = GREENX;
		    coloroff_y = GREENY;
		}
		else if (strcmp(argv[i],"darkred") == 0)
		{
		    coloroff_x = DARKREDX;
		    coloroff_y = DARKREDY;
		}
		else if (strcmp(argv[i],"none") == 0)
		{
		    coloroff_x = NONEX;
		    coloroff_y = NONEY;
		}
		else
		{
		    PrintHelp(argv[i-1]);
		    return 0;
		}
	    }
	    else if (strncmp(argv[i],"--coloroff=",11) == 0)
	    {
		strtok(argv[i],"=");
		strcpy(strArg,strtok(NULL,"\0"));

		if (strcmp(strArg,"gray") == 0)
		{
		    coloroff_x = GRAYX;
		    coloroff_y = GRAYY;
		}
		else if (strcmp(strArg,"neonred") == 0)
		{
		    coloroff_x = NEONREDX;
		    coloroff_y = NEONREDY;
		}
		else if (strcmp(strArg,"gold") == 0)
		{
		    coloroff_x = GOLDX;
		    coloroff_y = GOLDY;
		}
		else if (strcmp(strArg,"cyan") == 0)
		{
		    coloroff_x = CYANX;
		    coloroff_y = CYANY;
		}
		else if (strcmp(strArg,"pink") == 0)
		{
		    coloroff_x = PINKX;
		    coloroff_y = PINKY;
		}
		else if (strcmp(strArg,"blue") == 0)
		{
		    coloroff_x = BLUEX;
		    coloroff_y = BLUEY;
		}
		else if (strcmp(strArg,"green") == 0)
		{
		    coloroff_x = GREENX;
		    coloroff_y = GREENY;
		}
		else if (strcmp(strArg,"darkred") == 0)
		{
		    coloroff_x = DARKREDX;
		    coloroff_y = DARKREDY;
		}
		else if (strcmp(strArg,"none") == 0)
		{
		    coloroff_x = NONEX;
		    coloroff_y = NONEY;
		}
		else
		{
		    PrintHelp(argv[i]);
		    return 0;
		}
	    }
	    else if (strcmp(argv[i],"-d") == 0 )
	    {
		i++;
		if (strcmp(argv[i],"gray") == 0)
		{
		    dcoloron_x = GRAYX;
		    dcoloron_y = GRAYY;
		}
		else if (strcmp(argv[i],"neonred") == 0)
		{
		    dcoloron_x = NEONREDX;
		    dcoloron_y = NEONREDY;
		}
		else if (strcmp(argv[i],"gold") == 0)
		{
		    dcoloron_x = GOLDX;
		    dcoloron_y = GOLDY;
		}
		else if (strcmp(argv[i],"cyan") == 0)
		{
		    dcoloron_x = CYANX;
		    dcoloron_y = CYANY;
		}
		else if (strcmp(argv[i],"pink") == 0)
		{
		    dcoloron_x = PINKX;
		    dcoloron_y = PINKY;
		}
		else if (strcmp(argv[i],"blue") == 0)
		{
		    dcoloron_x = BLUEX;
		    dcoloron_y = BLUEY;
		}
		else if (strcmp(argv[i],"green") == 0)
		{
		    dcoloron_x = GREENX;
		    dcoloron_y = GREENY;
		}
		else if (strcmp(argv[i],"darkred") == 0)
		{
		    dcoloron_x = DARKREDX;
		    dcoloron_y = DARKREDY;
		}
		else if (strcmp(argv[i],"none") == 0)
		{
		    dcoloron_x = NONEX;
		    dcoloron_y = NONEY;
		}
		else
		{
		    PrintHelp(argv[i-1]);
		    return 0;
		}
	    }
	    else if (strncmp(argv[i],"--datecoloron=",14) == 0)
	    {
		strtok(argv[i],"=");
		strcpy(strArg,strtok(NULL,"\0"));

		if (strcmp(strArg,"gray") == 0)
		{
		    dcoloron_x = GRAYX;
		    dcoloron_y = GRAYY;
		}
		else if (strcmp(strArg,"neonred") == 0)
		{
		    dcoloron_x = NEONREDX;
		    dcoloron_y = NEONREDY;
		}
		else if (strcmp(strArg,"gold") == 0)
		{
		    dcoloron_x = GOLDX;
		    dcoloron_y = GOLDY;
		}
		else if (strcmp(strArg,"cyan") == 0)
		{
		    dcoloron_x = CYANX;
		    dcoloron_y = CYANY;
		}
		else if (strcmp(strArg,"pink") == 0)
		{
		    dcoloron_x = PINKX;
		    dcoloron_y = PINKY;
		}
		else if (strcmp(strArg,"blue") == 0)
		{
		    dcoloron_x = BLUEX;
		    dcoloron_y = BLUEY;
		}
		else if (strcmp(strArg,"green") == 0)
		{
		    dcoloron_x = GREENX;
		    dcoloron_y = GREENY;
		}
		else if (strcmp(strArg,"darkred") == 0)
		{
		    dcoloron_x = DARKREDX;
		    dcoloron_y = DARKREDY;
		}
		else if (strcmp(strArg,"none") == 0)
		{
		    dcoloron_x = NONEX;
		    dcoloron_y = NONEY;
		}
		else
		{
		    PrintHelp(argv[i]);
		    return 0;
		}
	    }
	    else if (strcmp(argv[i],"-m") == 0 )
	    {
		i++;

		if (strcmp(argv[i],"vertical") == 0)
		{
		    mode = MODEV;
		}
		else if (strcmp(argv[i],"horizontal") == 0)
		{
		    mode = MODEH;
		}
		else
		{
		    PrintHelp(argv[i-1]);
		    return 0;
		}
	    }
	    else if (strncmp(argv[i],"--mode=",7) == 0)
	    {
		strtok(argv[i],"=");
		strcpy(strArg,strtok(NULL,"\0"));

		if (strcmp(strArg,"vertical") == 0)
		{
		    mode = MODEV;
		}
		else if (strcmp(strArg,"horizontal") == 0)
		{
		    mode = MODEH;
		}
		else
		{
		    PrintHelp(argv[i]);
		    return 0;
		}
	    }
	    else if (strcmp(argv[i],"-s") == 0 )
	    {
		i++;

		if (strcmp(argv[i],"on") == 0)
		{
		    superf = SON;
		}
		else if (strcmp(argv[i],"off") == 0)
		{
		    superf = SOFF;
		}
		else
		{
		    PrintHelp(argv[i-1]);
		    return 0;
		}
	    }
	    else if (strncmp(argv[i],"--superfluous=",7) == 0)
	    {
		strtok(argv[i],"=");
		strcpy(strArg,strtok(NULL,"\0"));

		if (strcmp(strArg,"on") == 0)
		{
		    superf = SON;
		}
		else if (strcmp(strArg,"off") == 0)
		{
		    superf = SOFF;
		}
		else
		{
		    PrintHelp(argv[i]);
		    return 0;
		}
	    }
	    
	    i++;
	}
    }


    initXwindow(argc, argv);
    myWindow =
	openXwindow(argc, argv, wmBinClock_master_xpm,
		    wmBinClock_mask_bits, wmBinClock_mask_width,
		    wmBinClock_mask_height);

    DblClkDelay = 32000;
    UpToDate = 0;
    while (1) {

	if (DblClkDelay > 15)
	{
	    DblClkDelay = 0;
	    GotFirstClick1 = 0;
	    GotDoubleClick1 = 0;
	    GotFirstClick2 = 0;
	    GotDoubleClick2 = 0;
	    GotFirstClick3 = 0;
	    GotDoubleClick3 = 0;

	}
	else
	{
	    ++DblClkDelay;
	}


	/* 
	 *   Process any pending X events.
	 */
	while (XPending(display))
	{
	    XNextEvent(display, &event);
	    switch (event.type)
	    {
	    case Expose:
		RedrawWindow();
		break;
	    case ButtonPress:
		ButtonPressEvent(&event.xbutton);
		break;
	    case KeyPress:
		KeyPressEvent(&event.xkey);
		break;
	    case ButtonRelease:
		break;
	    case EnterNotify:
		XSetInputFocus(display, PointerRoot, RevertToParent,
			       CurrentTime);
		break;
	    case LeaveNotify:
		XSetInputFocus(display, PointerRoot, RevertToParent,
			       CurrentTime);
		break;
	    }
	}

	copyXPMArea(5, 69, 54, 54, 5, 5);

	t = time(0);
	tmworld = localtime(&t);

	if ( mode == MODEV )
	{
	    sprintf(tempstr, "%02i%02i%02i",
		    tmworld->tm_hour, tmworld->tm_min, tmworld->tm_sec);

	    for (i = 0; i < 6; i++)
	    {
		clockarray[i][0] = 0;
		clockarray[i][1] = 0;
		clockarray[i][2] = 0;
		clockarray[i][3] = 0;

		switch (tempstr[i])
		{
		    
		case '1':
		    clockarray[i][3] = 1;
		    break;
		case '2':
		    clockarray[i][2] = 1;
		    break;
		case '3':
		    clockarray[i][3] = 1;
		    clockarray[i][2] = 1;
		    break;
		case '4':
		    clockarray[i][1] = 1;
		    break;
		case '5':
		    clockarray[i][3] = 1;
		    clockarray[i][1] = 1;
		    break;
		case '6':
		    clockarray[i][2] = 1;
		    clockarray[i][1] = 1;
		    break;
		case '7':
		    clockarray[i][3] = 1;
		    clockarray[i][2] = 1;
		    clockarray[i][1] = 1;
		    break;
		case '8':
		    clockarray[i][0] = 1;
		    break;
		case '9':
		    clockarray[i][0] = 1;
		    clockarray[i][3] = 1;
		    break;
		}
	    }
	    for (i = 0; i < 6; i++)
	    {
		if(superf == SON)
		{
		    for (j = 0; j < 4; j++)
		    {
			if (clockarray[i][j] == 1)
			    copyXPMArea(coloron_x, coloron_y, 8, 8, 6 + (i * 9),
					10 + (j * 10));
			else
			    copyXPMArea(coloroff_x, coloroff_y, 8, 8, 6 + (i * 9),
					10 + (j * 10));
		    }
		}
		else
		{
		    for (j = iBeginLEDs[0][i]; j < 4; j++)
		    {
			if (clockarray[i][j] == 1)
			    copyXPMArea(coloron_x, coloron_y, 8, 8, 6 + (i * 9),
					10 + (j * 10));
			else
			    copyXPMArea(coloroff_x, coloroff_y, 8, 8, 6 + (i * 9),
					10 + (j * 10));
		    }
		}
	    }
	}
	else
	{
	    for (i = 0; i < 6; i++)
	    {
		clockarray[0][i] = 0;
		clockarray[1][i] = 0;
		clockarray[2][i] = 0;
		clockarray[3][i] = 0;
		clockarray[4][i] = 0;
	    }

	    tmp_hour = tmworld->tm_hour;
	    IntToBinary(tmp_hour, &tmp_str);

	    for (s = 0; s < 6; s++)
	    {
		if (tmp_str[s] == 1)
		{
		    clockarray[0][5-s] = 1;
		}
	    }

	    tmp_minute = tmworld->tm_min;
	    IntToBinary(tmp_minute, &tmp_str);
	    for (s = 0; s < 6; s++)
	    {
		if (tmp_str[s] == 1)
		{
		    clockarray[1][5-s] = 1;
		}
	    }
	
	    tmp_second = tmworld->tm_sec;
	    IntToBinary(tmp_second, &tmp_str);
	    
	    for (s = 0; s < 6; s++)
	    {
		if (tmp_str[s] == 1)
		{
		    clockarray[2][5-s] = 1;
		}
	    }
	    
	    tmp_day = tmworld->tm_mday;
	    IntToBinary(tmp_day, &tmp_str);
	    
	    for (s = 0; s < 6; s++)
	    {
		if (tmp_str[s] == 1)
		{
		    clockarray[3][5-s] = 1;
		}
	    }

	    tmp_month = tmworld->tm_mon + 1;
	    IntToBinary(tmp_month, &tmp_str);
	    
	    for (s = 0; s < 6; s++)
	    {
		if (tmp_str[s] == 1)
		{
		    clockarray[4][5-s] = 1;
		}
	    }

	    if (superf == SON)
	    {
		for (i = 0; i < 3; i++)
		{
		    for (j = 0; j < 6; j++)
		    {
			if (clockarray[i][j] == 1)
			    copyXPMArea(coloron_x, coloron_y, 8, 8, 6 + (j * 9), 10 + (i * 10));
			else
			    copyXPMArea(coloroff_x, coloroff_y, 8, 8, 6 + (j * 9), 10 + (i * 10));
		    }
		}
	    }
	    else
	    {
		for (i = 0; i < 3; i++)
		{
		    for (j = iBeginLEDs[1][i]; j < 6; j++)
		    {
			if (clockarray[i][j] == 1)
			    copyXPMArea(coloron_x, coloron_y, 8, 8, 6 + (j * 9), 10 + (i * 10));
			else
			    copyXPMArea(coloroff_x, coloroff_y, 8, 8, 6 + (j * 9), 10 + (i * 10));
		    }
		}
	    }

	    if (superf == SON)
	    {
		for (i = 3; i < 5; i++)
		{
		    for (j = 0; j < 6; j++)
		    {
			if (clockarray[i][j] == 1)
			    copyXPMArea(dcoloron_x, dcoloron_y, 8, 8, 6 + (j * 9), 10 + (i * 10));
			else
			    copyXPMArea(coloroff_x, coloroff_y, 8, 8, 6 + (j * 9), 10 + (i * 10));
		    }
		}
	    }
	    else
	    {
		for (i = 3; i < 5; i++)
		{
		    for (j = iBeginLEDs[1][i]; j < 6; j++)
		    {
			if (clockarray[i][j] == 1)
			    copyXPMArea(dcoloron_x, dcoloron_y, 8, 8, 6 + (j * 9), 10 + (i * 10));
			else
			    copyXPMArea(coloroff_x, coloroff_y, 8, 8, 6 + (j * 9), 10 + (i * 10));
		    }
		}
	    }
	}

	RedrawWindow();

	usleep(DELAY);
    }
}
void IntToBinary(int x, int *str[])
{
    int i = 0;
    int counter = 0;
    
    for(i = 0; i < 6 ; i++)
    {
	str[i] = 0;
    }

    while( x > 0 )
    {
	if((x%2) != 0)
	{
	    str[counter] = 1;
	}
	else
	{
	    str[counter] = 0;
	}

	x = x / 2;
	counter++;
    }
}

void ButtonPressEvent(XButtonEvent * xev)
{

    /*
     *  Process single clicks.
     */
    DblClkDelay = 0;
    if ((xev->button == Button1) && (xev->type == ButtonPress))
    {	
	if (GotFirstClick1)
	    GotDoubleClick1 = 1;
	else
	    GotFirstClick1 = 1;
    }
    else if ((xev->button == Button2) && (xev->type == ButtonPress))
    {
	if (GotFirstClick2)
	    GotDoubleClick2 = 1;
	else
	    GotFirstClick2 = 1;
    }
    else if ((xev->button == Button3) && (xev->type == ButtonPress))
    {
	if (GotFirstClick3)
	    GotDoubleClick3 = 1;
	else
	    GotFirstClick3 = 1;
    }




    /*
     *  We got a double click on Mouse Button1 (i.e. the left one)
     */
    if (GotDoubleClick1)
    {
	GotFirstClick1 = 0;
	GotDoubleClick1 = 0;
    }


    /*
     *  We got a double click on Mouse Button2 (i.e. the left one)
     */
    if (GotDoubleClick2)
    {
	GotFirstClick2 = 0;
	GotDoubleClick2 = 0;
    }


    /*
     *  We got a double click on Mouse Button3 (i.e. the left one)
     */
    if (GotDoubleClick3)
    {
	GotFirstClick3 = 0;
	GotDoubleClick3 = 0;
    }

    return;
}

/*
 *  This routine handles key presses.
 *
 */
void KeyPressEvent(XKeyEvent * xev)
{
    return;
}

/*
 * This routine prints the help message.
 *
 */
void PrintHelp(char err[])
{
    char str[16];
    if (strcmp(err,"-h") != 0 && strcmp(err,"--help") != 0)
    {
	strcpy(str,"Error in ");
	strcat(str,err);
	strcat(str,".\n\n");
	printf(str);
    }

    printf("Usage: wmBinClock [OPTIONS]\n\n");
    printf("Colors:\n");
    printf("  -i COLOR, --coloron=COLOR             Select COLOR as LED on color\n");
    printf("  -o COLOR, --coloroff=COLOR            Select COLOR as LED off color\n");
    printf("  -d COLOR, --datecoloron=COLOR         Select COLOR as LED on color\n");
    printf("                                        for date display\n");
    printf("  Where COLOR is one off these colors:\n");
    printf("  gray neonred gold cyan pink blue green darkred none\n\n");
    printf("Display mode\n");
    printf("  -m MODE, --mode=MODE                  Select display mode\n");
    printf("  Where MODE is:\n");
    printf("  \"horizontal\" or \"vertical\"\n");
    printf("  -s {on/off}, --superfluous={on/off}   Display e.g. led for 32 in hours\n");
    printf("  -display (host:screen)                Use an alternative XWindow display\n\n");
    printf("The default is \"wmBinClock -i neonred -o gray -d green -m vertical -s on -display 0:0\"\n");
}
