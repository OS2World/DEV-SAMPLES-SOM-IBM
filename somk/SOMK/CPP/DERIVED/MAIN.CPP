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
/* @(#) somx/main.cpp 2.8 12/26/95 17:58:53 [1/24/96 17:31:22] */

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

#include "derived.xh"
#include <stdio.h>
#include <string.h> 

/*
 * The class Test (declared in derived.idl) is derived using multiple
 * interitance from the classes Hello and Mello which have corresponding metaclasses
 * M_Hello and M_Mello. To assure that the class Test will respond to all the
 * methods that its parents respond to, the class of which Test is to be
 * an instance is derived using multiple inheritance from M_Hello and M_Mello. 
 * This is done automatically by the SOM kernel, and the result is named
 * M_Test_Derived. This test verifies this, and checks that that the class methods 
 * introduced by both M_Hello and M_Mello are available on Test.
 */

int main(int argc, char *argv[])
{

    Test *test = new Test;

    SOM_Test(!strcmp(test->somGetClassName(),"Test"));
    SOM_Test(!strcmp(test->somGetClass()->somGetClassName(),"M_Test_Derived"));

    SOM_Test(test->somRespondsTo(somIdFromString("hello_")) == 1);
    SOM_Test(!strcmp(test->hello_(),"Hello World"));
    SOM_Test(test->somRespondsTo(somIdFromString("mello_")) == 1);
    SOM_Test(!strcmp(test->mello_(),"Mello World"));

    SOM_Test(test->somGetClass()->somRespondsTo(somIdFromString("HelloCreate")) == 1);
    Hello *a = ((M_Hello*) (test->somGetClass()))->HelloCreate("Hello from A");
    SOM_Test(!strcmp(a->hello_(),"Hello from A"));

    SOM_Test(test->somGetClass()->somRespondsTo(somIdFromString("MelloCreate")) == 1);
    Mello *b = ((M_Mello*) (test->somGetClass()))->MelloCreate("Mello from B");
    SOM_Test(!strcmp(b->mello_(),"Mello from B"));

    Hello *c = (Hello*) (test->somGetClass()->somNew());
    Mello *d = (Mello*) (test->somGetClass()->somNew());

    SOM_Test(!strcmp(c->hello_(),"Hello World"));
    SOM_Test(!strcmp(d->mello_(),"Mello World"));

    printf("Derived Metaclass Test Successfully Completed.\n");
    return 0;
}

