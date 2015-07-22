/*
 *   COMPONENT_NAME: somx
 *
 *   ORIGINS: 27
 *
 *
 *    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996  
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */
/* @(#) somx/main.c 2.6 12/26/95 17:50:44 [1/24/96 17:31:13] */

/*
 *
 * DISCLAIMER OF WARRANTIES.
 * The following [enclosed] code is sample code created by IBM
 * Corporation. This sample code is not part of any standard or IBM
 * product and is provided to you solely for the purpose of assisting
 * you in the development of your applications.  The code is provided
 * "AS IS". IBM MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE, REGARDING THE FUNCTION OR PERFORMANCE OF
 * THIS CODE.  IBM shall not be liable for any damages arising out of
 * your use of the sample code, even if they have been advised of the
 * possibility of such damages.
 *
 * DISTRIBUTION.
 * This sample code can be freely distributed, copied, altered, and
 * incorporated into other software, provided that it bears the above
 * Copyright notice and DISCLAIMER intact.
 */

#include "animals.h"
#define BIG_DOG_MAX 4

int main(int argc, char *argv[])
{
    Environment *ev = somGetGlobalEnvironment ();
    string dogNames[] = {
	"Lassie", "Guffy", "Waldo",
	"Pluto", "Elvis", "Spike"
	};
    string dogBreeds[] = {
	"Collie", "Retriever", "German Shepherd",
	"Animated", "Hounddog", "Bulldog"
	};
    string dogColors[] = {
	"tan", "brown", "black"
	};
    sequence(Animals_Animal) allCreatures;
    int i;

    /*
     * Create classes. 
     */
    Animals_AnimalNewClass (Animals_Animal_MajorVersion, 
	Animals_Animal_MinorVersion);
    Animals_DogNewClass (Animals_Dog_MajorVersion, Animals_Dog_MinorVersion);
    Animals_LittleDogNewClass (Animals_LittleDog_MajorVersion, 
	Animals_LittleDog_MinorVersion);
    Animals_BigDogNewClass (Animals_BigDog_MajorVersion,
	Animals_BigDog_MinorVersion);

    SOM_TraceLevel = (argc > 1 ? 1 : 0);

    /*
     * Create objects using constructors 
     */
    Animals_M_Animal_newAnimal (_Animals_Animal, ev, "Scary", "Roar!!!");
    Animals_Kennel_newDog (_Animals_Dog, ev, "Rover", "Grrr", "Retriever",
	"yellow");
    Animals_Kennel_newDog (_Animals_LittleDog, ev, "Pierre", "yap yap",
	"French Poodle", "black");
    for (i=0; i<(sizeof(dogNames)/sizeof(string)); i++)
        Animals_Kennel_newDog (_Animals_BigDog, ev, dogNames[i],
	    (string) NULL, dogBreeds[i], dogColors[i%3]);

    /*
     * Display objects. 
     */
    somPrintf ("=====================\n");
    allCreatures = Animals_M_Animal__get_instances (_Animals_Animal, ev);
   
    for (i=0; i<allCreatures._length; i++) {
        Animals_Animal_display (allCreatures._buffer[i], ev);
    }


    somPrintf ("Total animal population is currently %d.\n", 
	allCreatures._length);

    /*
     * Free objects. 
     */
    for (i=0; i<allCreatures._length; i++)
        SOMObject_somFree (allCreatures._buffer[i]);

    somPrintf ("\n");

    return(0);
}
