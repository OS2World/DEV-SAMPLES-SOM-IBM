/*
 *   COMPONENT_NAME: somx
 *
 *   ORIGINS: 27
 *
 *
 *    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1994,1996 
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */
#ifndef lint
static char *sccsid = "@(#) 2.13 src/somples.os2/somd/c/animal/anitest.c, somx.somd, som2.5, s259604a 12/26/95 17:39:48 [1/24/96 17:31:02]";
#endif

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


/*
 *  "anitest" is a sample program demonstrating the creation, destruction
 *  and invocation of methods on remote objects using DSOM.
 */

#include <stdlib.h>
#include <somd.h>
#include "danimal.h"
#include "dldog.h"
#include "dbdog.h"
#include "dloc.h"

void printEv(Environment *);
void displayAnimal(dAnimal, Environment *);
#define checkEv(ev) ((ev)->_major != NO_EXCEPTION)

int main(int argc, char *argv[])
{
    Environment *ev;
    ExtendedNaming_ExtendedNamingContext enc;
    SOMObject   factory;
    dAnimal Pooh;
    dLittleDog  Snoopie;
    dBigDog     Lassie;
    dLocation   locPooh;
    dLocation   locSnoopie;
    dLocation   locLassie;

    /* local and DSOM initialization */
    ev = SOM_CreateLocalEnvironment();
    SOMD_Init(ev);

    /* get the context where factory objects are stored */
    enc = (ExtendedNaming_ExtendedNamingContext)
          _resolve_initial_references(SOMD_ORBObject, ev,
                                      "FactoryService");
    if (checkEv(ev))
    {
      printEv(ev);
      SOMD_Uninit(ev);
      SOM_DestroyLocalEnvironment(ev);
      exit(1);
    }

    /* find the "dAnimal" factory on "aniServer1" */
    factory = _find_any(enc, ev, "class=='dAnimal' and alias=='aniServer1'", 0);
    if (checkEv(ev))
    {
      printEv(ev);
      _release(enc, ev);
      SOMD_Uninit(ev);
      SOM_DestroyLocalEnvironment(ev);
      exit(1);
    }

    /* ask animal Factory to create remote animal */
    Pooh = _create_animal(factory, ev, "dAnimal");
    if (checkEv(ev))
    {
      printEv(ev);
      _release(factory, ev);
      _release(enc, ev);
      SOMD_Uninit(ev);
      SOM_DestroyLocalEnvironment(ev);
      exit(1);
    }

    /* save pointers to dLocation object */
    locPooh = __get_loc(Pooh, ev);

    if (checkEv(ev))
    {
      printEv(ev);
      _somFree(Pooh);
      _release(factory, ev);
      _release(enc, ev);
      SOMD_Uninit(ev);
      SOM_DestroyLocalEnvironment(ev);
      exit(1);
    }

    Snoopie = _create_animal(factory, ev, "dLittleDog");

    if (checkEv(ev))
    {
      printEv(ev);
      _release(locPooh, ev);
      _somFree(Pooh);
      _release(factory, ev);
      _release(enc, ev);
      SOMD_Uninit(ev);
      SOM_DestroyLocalEnvironment(ev);
      exit(1);
    }

    locSnoopie = __get_loc(Snoopie, ev);

    if (checkEv(ev))
    {
      printEv(ev);
      _somFree(Snoopie);
      _release(locPooh, ev);
      _somFree(Pooh);
      _release(factory, ev);
      _release(enc, ev);
      SOMD_Uninit(ev);
      SOM_DestroyLocalEnvironment(ev);
      exit(1);
    }

    Lassie = _create_animal(factory, ev, "dBigDog");

    if (checkEv(ev))
    {
      printEv(ev);
      _release(locSnoopie, ev);
      _somFree(Snoopie);
      _release(locPooh, ev);
      _somFree(Pooh);
      _release(factory, ev);
      _release(enc, ev);
      SOMD_Uninit(ev);
      SOM_DestroyLocalEnvironment(ev);
      exit(1);
    }

    locLassie = __get_loc(Lassie, ev);

    if (checkEv(ev))
    {
      printEv(ev);
      _somFree(Lassie);
      _release(locSnoopie, ev);
      _somFree(Snoopie);
      _release(locPooh, ev);
      _somFree(Pooh);
      _release(factory, ev);
      _release(enc, ev);
      SOMD_Uninit(ev);
      SOM_DestroyLocalEnvironment(ev);
      exit(1);
    }

    /* invoke methods before instance data is set */
    displayAnimal(Pooh, ev);
    if (checkEv(ev)) goto error;

    displayAnimal(Snoopie, ev);
    if (checkEv(ev)) goto error;

    somPrintf("%s says %s.\n", dAnimal__get_name(Snoopie, ev),
			       __get_noise(Snoopie, ev));
    if (checkEv(ev)) goto error;

    displayAnimal(Lassie, ev);
    if (checkEv(ev)) goto error;

    somPrintf("%s says %s.\n", dAnimal__get_name(Lassie, ev),
			       __get_noise(Lassie, ev));
    if (checkEv(ev)) goto error;


    /* set instance data for animal objects */
    dAnimal__set_name(Pooh, ev, "Winnie the Pooh");
    if (checkEv(ev)) goto error;

    dAnimal__set_type(Pooh, ev, "Storybook Bear");
    if (checkEv(ev)) goto error;

    __set_food(Pooh, ev, "honey");
    if (checkEv(ev)) goto error;

    __set_city(locPooh, ev, "Austin");
    if (checkEv(ev)) goto error;

    __set_state(locPooh, ev, "TX");
    if (checkEv(ev)) goto error;

    dAnimal__set_name(Snoopie, ev, "Snoopie");
    if (checkEv(ev)) goto error;

    dAnimal__set_type(Snoopie, ev, "Cartoon Beagle");
    if (checkEv(ev)) goto error;

    __set_food(Snoopie, ev, "cookies");
    if (checkEv(ev)) goto error;

    __set_city(locSnoopie, ev, "Dublin");
    if (checkEv(ev)) goto error;

    __set_state(locSnoopie, ev, "OH");
    if (checkEv(ev)) goto error;

    __set_trick(Snoopie, ev, "dancing");
    if (checkEv(ev)) goto error;

    __set_LDweight(Snoopie, ev, 320.47);
    if (checkEv(ev)) goto error;

    dAnimal__set_name(Lassie, ev, "Lassie");
    if (checkEv(ev)) goto error;

    dAnimal__set_type(Lassie, ev, "TV Collie");
    if (checkEv(ev)) goto error;

    __set_food(Lassie, ev, "steak");
    if (checkEv(ev)) goto error;

    __set_city(locLassie, ev, "Hollywood");
    if (checkEv(ev)) goto error;

    __set_state(locLassie, ev, "CA");
    if (checkEv(ev)) goto error;

    __set_trick(Lassie, ev, "saving lives");
    if (checkEv(ev)) goto error;

    __set_BDweight(Lassie, ev, 42.38);
    if (checkEv(ev)) goto error;


    /* invoke methods after instance data is set */
    displayAnimal(Pooh, ev);
    if (checkEv(ev)) goto error;

    displayAnimal(Snoopie, ev);
    if (checkEv(ev)) goto error;

    somPrintf("%s says %s.\n", dAnimal__get_name(Snoopie, ev),
			       __get_noise(Snoopie, ev));
    if (checkEv(ev)) goto error;

    displayAnimal(Lassie, ev);
    if (checkEv(ev)) goto error;

    somPrintf("%s says %s.\n", dAnimal__get_name(Lassie, ev),
			       __get_noise(Lassie, ev));
    if (checkEv(ev)) goto error;


    /* modify instance data */
    __set_city(locPooh, ev, "Portland");
    if (checkEv(ev)) goto error;

    __set_state(locPooh, ev, "ME");
    if (checkEv(ev)) goto error;

    __set_trick(Snoopie, ev, "writing novels");
    if (checkEv(ev)) goto error;

    __set_BDweight(Lassie, ev, 45.25);
    if (checkEv(ev)) goto error;


    /* display again after modifications */
    displayAnimal(Pooh, ev);
    if (checkEv(ev)) goto error;

    displayAnimal(Snoopie, ev);
    if (checkEv(ev)) goto error;

    displayAnimal(Lassie, ev);
    if (checkEv(ev)) goto error;


    /* display class name of proxy objects */
    somPrintf("\nPooh's proxy class name is %s.\n",
	       _somdProxyGetClassName(Pooh, ev));
    if (checkEv(ev)) goto error;

    somPrintf("Snoopie's proxy class name is %s.\n",
	       _somdProxyGetClassName(Snoopie, ev));
    if (checkEv(ev)) goto error;

    somPrintf("Lassie's proxy class name is %s.\n",
	       _somdProxyGetClassName(Lassie, ev));
    if (checkEv(ev)) goto error;


    /* display class name of target objects */
    somPrintf("\nPooh's target class name is %s.\n",
	       _somdTargetGetClassName(Pooh, ev));
    if (checkEv(ev)) goto error;

    somPrintf("Snoopie's target class name is %s.\n",
	       _somdTargetGetClassName(Snoopie, ev));
    if (checkEv(ev)) goto error;

    somPrintf("Lassie's target class name is %s.\n",
	       _somdTargetGetClassName(Lassie, ev));
    if (checkEv(ev)) goto error;


    /* destroy local proxy objects and remote animal objects */
    _release(locLassie, ev);
    _somFree(Lassie);
    _release(locSnoopie, ev);
    _somFree(Snoopie);
    _release(locPooh, ev);
    _somFree(Pooh);
    _release(factory, ev);
    _release(enc, ev);

    SOMD_Uninit(ev);
    SOM_DestroyLocalEnvironment(ev);
    exit(0);

error:
    printEv(ev);

    _release(locLassie, ev);
    _somFree(Lassie);
    _release(locSnoopie, ev);
    _somFree(Snoopie);
    _release(locPooh, ev);
    _somFree(Pooh);
    _release(factory, ev);
    _release(enc, ev);

    SOMD_Uninit(ev);
    SOM_DestroyLocalEnvironment(ev);
    exit(1);
}


/*
 *  Prints exception information.
 */

void printEv(Environment *ev)
{
  char *exId;
  StExcep *params;

  exId = somExceptionId(ev);
  params = somExceptionValue(ev);

  somPrintf("Error Occurred\n");
  somPrintf("exception string: %s\n", exId);
  if (ev->_major == SYSTEM_EXCEPTION) {
    somPrintf("minor error code: %u\n", params->minor);
    somPrintf(" completion code: %s\n", (params->completed == YES ? "YES" :
	      params->completed == NO ? "NO": "MAYBE"));
  }
  somdExceptionFree(ev);
  return;
}


/*
 *  displayAnimal prints the buffer returned from "display" method.
 */

void displayAnimal(dAnimal someAnimal, Environment *ev)
{
    string buff;

    buff = _display(someAnimal, ev);

    if (ev->_major == NO_EXCEPTION)
    {
      somPrintf(buff);
    }

    return;
}

