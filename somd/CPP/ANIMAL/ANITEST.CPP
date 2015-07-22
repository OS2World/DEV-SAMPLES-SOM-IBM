//
//   COMPONENT_NAME: somx
//
//   ORIGINS: 27
//
//
//    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996  
//   All Rights Reserved
//   Licensed Materials - Property of IBM
//   US Government Users Restricted Rights - Use, duplication or
//   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
//
#ifndef lint
static char *sccsid = "@(#) 2.17 src/somples.os2/somd/cpp/animal/anitest.cpp, somx.somd, som2.5, s259604a 12/26/95 17:46:00 [1/24/96 17:31:08]";
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
#include <somd.xh>
#include "danimal.xh"
#include "dldog.xh"
#include "dbdog.xh"
#include "dloc.xh"

void printEv(Environment *);
void displayAnimal(dAnimal *, Environment *);
#define checkEv(ev) ((ev)->_major != NO_EXCEPTION)

int main(int argc, char *argv[])
{
    Environment *ev;
    ExtendedNaming_ExtendedNamingContext *enc;
    AnimalFactory   *factory;
    dAnimal     *Pooh = (dAnimal *) NULL;
    dLittleDog  *Snoopie = (dLittleDog *) NULL;
    dBigDog     *Lassie = (dBigDog *) NULL;
    dLocation   *locPooh = (dLocation *) NULL;
    dLocation   *locSnoopie = (dLocation *) NULL;
    dLocation   *locLassie = (dLocation *) NULL;

    /* local and DSOM initialization */
    ev = SOM_CreateLocalEnvironment();
    SOMD_Init(ev);

    /* get the context where factory objects are stored */
    enc = (ExtendedNaming_ExtendedNamingContext *)
          SOMD_ORBObject->resolve_initial_references(ev,
                                                     "FactoryService");
    if (checkEv(ev))
    {
        printEv(ev);
        SOMD_Uninit(ev);
        SOM_DestroyLocalEnvironment(ev);
        exit(1);
    }

    /* find the "AnimalFactory" factory on "aniServer2" */
    factory = (AnimalFactory *) enc->find_any(ev, "class=='dAnimal' and alias=='aniServer2'", 0);
    if (checkEv(ev))
    {
      printEv(ev);
      ((SOMDObject *)enc)->release(ev);
      SOMD_Uninit(ev);
      SOM_DestroyLocalEnvironment(ev);
      exit(1);
    }

    /* ask animal Factory to create remote animal */
    Pooh = (dAnimal *) factory->create_animal(ev, "dAnimal");
    if (checkEv(ev))
    {
      printEv(ev);
      ((SOMDObject *)factory)->release(ev);
      ((SOMDObject *)enc)->release(ev);
      SOMD_Uninit(ev);
      SOM_DestroyLocalEnvironment(ev);
      exit(1);
    }

    /* save pointers to dLocation object */
    locPooh = Pooh->_get_loc(ev);
    if (checkEv(ev))
    {
      printEv(ev);
      Pooh->somFree();
      ((SOMDObject *)factory)->release(ev);
      ((SOMDObject *)enc)->release(ev);
      SOMD_Uninit(ev);
      SOM_DestroyLocalEnvironment(ev);
      exit(1);
    }

    Snoopie =(dLittleDog *) factory->create_animal(ev, "dLittleDog");

    if (checkEv(ev))
    {
      printEv(ev);
      ((SOMDObject *)locPooh)->release(ev);
      Pooh->somFree();
      ((SOMDObject *)factory)->release(ev);
      ((SOMDObject *)enc)->release(ev);
      SOMD_Uninit(ev);
      SOM_DestroyLocalEnvironment(ev);
      exit(1);
    }

    locSnoopie = Snoopie->_get_loc(ev);
    if (checkEv(ev))
    {
      printEv(ev);
      Snoopie->somFree();
      ((SOMDObject *) locPooh)->release(ev);
      Pooh->somFree();
      ((SOMDObject *)factory)->release(ev);
      ((SOMDObject *)enc)->release(ev);
      SOMD_Uninit(ev);
      SOM_DestroyLocalEnvironment(ev);
      exit(1);

    }

    Lassie =(dBigDog *) factory->create_animal(ev, "dBigDog");

    if (checkEv(ev))
    {
      printEv(ev);
      ((SOMDObject *) locSnoopie)->release(ev);
      Snoopie->somFree();
      ((SOMDObject *) locPooh)->release(ev);
      Pooh->somFree();
      ((SOMDObject *)factory)->release(ev);
      ((SOMDObject *)enc)->release(ev);
      SOMD_Uninit(ev);
      SOM_DestroyLocalEnvironment(ev);
      exit(1);
    }

    locLassie = Lassie->_get_loc(ev);
    if (checkEv(ev))
    {
      printEv(ev);
      Lassie->somFree();
      ((SOMDObject *) locSnoopie)->release(ev);
      Snoopie->somFree();
      ((SOMDObject *) locPooh)->release(ev);
      Pooh->somFree();
      ((SOMDObject *)factory)->release(ev);
      ((SOMDObject *)enc)->release(ev);
      SOMD_Uninit(ev);
      SOM_DestroyLocalEnvironment(ev);
      exit(1);
    }


    /* invoke methods before instance data is set */
    displayAnimal(Pooh, ev);
    if (checkEv(ev)) goto error;

    displayAnimal(Snoopie, ev);
    if (checkEv(ev)) goto error;

    somPrintf("%s says %s.\n", Snoopie->_get_name(ev),
                               Snoopie->_get_noise(ev));
    if (checkEv(ev)) goto error;

    displayAnimal(Lassie, ev);
    somPrintf("%s says %s.\n", Lassie->_get_name(ev),
                               Lassie->_get_noise(ev));
    if (checkEv(ev)) goto error;


    /* set instance data for animal objects */
    Pooh->_set_name(ev, "Winnie the Pooh");
    if (checkEv(ev)) goto error;

    Pooh->_set_type(ev, "Storybook Bear");
    if (checkEv(ev)) goto error;

    Pooh->_set_food(ev, "honey");
    if (checkEv(ev)) goto error;

    locPooh->_set_city(ev, "Austin");
    if (checkEv(ev)) goto error;

    locPooh->_set_state(ev, "TX");
    if (checkEv(ev)) goto error;

    Snoopie->_set_name(ev, "Snoopie");
    if (checkEv(ev)) goto error;

    Snoopie->_set_type(ev, "Cartoon Beagle");
    if (checkEv(ev)) goto error;

    Snoopie->_set_food(ev, "cookies");
    if (checkEv(ev)) goto error;

    locSnoopie->_set_city(ev, "Dublin");
    if (checkEv(ev)) goto error;

    locSnoopie->_set_state(ev, "OH");
    if (checkEv(ev)) goto error;

    Snoopie->_set_trick(ev, "dancing");
    if (checkEv(ev)) goto error;

    Snoopie->_set_LDweight(ev, 320.47);
    if (checkEv(ev)) goto error;

    Lassie->_set_name(ev, "Lassie");
    if (checkEv(ev)) goto error;

    Lassie->_set_type(ev, "TV Collie");
    if (checkEv(ev)) goto error;

    Lassie->_set_food(ev, "steak");
    if (checkEv(ev)) goto error;

    locLassie->_set_city(ev, "Hollywood");
    if (checkEv(ev)) goto error;

    locLassie->_set_state(ev, "CA");
    if (checkEv(ev)) goto error;

    Lassie->_set_trick(ev, "saving lives");
    if (checkEv(ev)) goto error;

    Lassie->_set_BDweight(ev, 42.38);
    if (checkEv(ev)) goto error;


    /* invoke methods after instance data is set */
    displayAnimal(Pooh, ev);
    if (checkEv(ev)) goto error;

    displayAnimal(Snoopie, ev);
    if (checkEv(ev)) goto error;

    somPrintf("%s says %s.\n", Snoopie->_get_name(ev),
                               Snoopie->_get_noise(ev));
    if (checkEv(ev)) goto error;

    displayAnimal(Lassie, ev);
    if (checkEv(ev)) goto error;

    somPrintf("%s says %s.\n", Lassie->_get_name(ev),
                               Lassie->_get_noise(ev));
    if (checkEv(ev)) goto error;


    /* modify instance data */
    locPooh->_set_city(ev, "Portland");
    if (checkEv(ev)) goto error;

    locPooh->_set_state(ev, "ME");
    if (checkEv(ev)) goto error;

    Snoopie->_set_trick(ev, "writing novels");
    if (checkEv(ev)) goto error;

    Lassie->_set_BDweight(ev, 45.25);
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
               ((SOMDClientProxy *) Pooh)->somdProxyGetClassName(ev));
    if (checkEv(ev)) goto error;

    somPrintf("Snoopie's proxy class name is %s.\n",
               ((SOMDClientProxy *) Snoopie)->somdProxyGetClassName(ev));
    if (checkEv(ev)) goto error;

    somPrintf("Lassie's proxy class name is %s.\n",
               ((SOMDClientProxy *) Lassie)->somdProxyGetClassName(ev));
    if (checkEv(ev)) goto error;


    /* display class name of target objects */
    somPrintf("\nPooh's target class name is %s.\n",
               ((SOMDClientProxy *) Pooh)->somdTargetGetClassName(ev));
    if (checkEv(ev)) goto error;

    somPrintf("Snoopie's target class name is %s.\n",
               ((SOMDClientProxy *) Snoopie)->somdTargetGetClassName(ev));
    if (checkEv(ev)) goto error;

    somPrintf("Lassie's target class name is %s.\n",
               ((SOMDClientProxy *) Lassie)->somdTargetGetClassName(ev));
    if (checkEv(ev)) goto error;


    /* destroy local proxy objects and remote animal objects */
    ((SOMDObject *) locLassie)->release(ev);
    Lassie->somFree();
    ((SOMDObject *) locSnoopie)->release(ev);
    Snoopie->somFree();
    ((SOMDObject *) locPooh)->release(ev);
    Pooh->somFree();
    ((SOMDObject *)factory)->release(ev);
    ((SOMDObject *)enc)->release(ev);

    SOMD_Uninit(ev);
    SOM_DestroyLocalEnvironment(ev);
    exit(0);
    return 0;

error:
    printEv(ev);

    if (locPooh != (dLocation *) NULL)
        ((SOMDObject *) locPooh)->release(ev);

    if (Pooh != (dAnimal *) NULL)
        Pooh->somFree();

    if (locSnoopie != (dLocation *) NULL)
       ((SOMDObject *) locSnoopie)->release(ev);

    if (Snoopie != (dLittleDog *) NULL)
        Snoopie->somFree();

    if (locLassie != (dLocation *) NULL)
       ((SOMDObject *) locLassie)->release(ev);

    if (Lassie != (dBigDog *) NULL)
        Lassie->somFree();

    if (factory != (SOMObject *) NULL)
        ((SOMDObject *)factory)->release(ev);

    if (enc != (ExtendedNaming_ExtendedNamingContext *) NULL)
       ((SOMDObject *)enc)->release(ev);

    SOMD_Uninit(ev);
    SOM_DestroyLocalEnvironment(ev);
    exit(1);
    return 1;
}


/*
 *  Prints exception information.
 */

void printEv(Environment *ev)
{
  char *exId;
  StExcep *params;

  exId = somExceptionId(ev);
  params = (StExcep *) somExceptionValue(ev);

  somPrintf("Error Occurred\n");
  somPrintf("exception string: %s\n", exId);
  somPrintf("minor error code: %u\n", params->minor);
  somPrintf(" completion code: %s\n", (params->completed == YES ? "YES" :
             params->completed == NO ? "NO": "MAYBE"));
  somdExceptionFree(ev);
  return;
}


/*
 *  displayAnimal prints the buffer returned from "display" method.
 */

void displayAnimal(dAnimal *someAnimal, Environment *ev)
{
    string buff;

    buff = someAnimal->display(ev);

    if (ev->_major == NO_EXCEPTION)
    {
      somPrintf(buff);
    }

    return;
}

