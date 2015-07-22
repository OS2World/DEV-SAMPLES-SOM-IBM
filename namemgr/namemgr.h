/*
 *
 *    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */

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

 #ifndef _NAMEMGR
 #define _NAMEMGR

 class NameMgr
 {
 protected:
 CosNaming_Name* name;
 int namemax;
 ExtendedNaming_PropertyList* pList;

 public:
 NameMgr(int maximum);

 ExtendedNaming_ExtendedNamingContext* GetRootNC(Environment* ev);

 void NewName(int maximum);

 void SetName(int index, char* id, char* kind);

 ExtendedNaming_ExtendedNamingContext* CreateContext
  (Environment* ev, ExtendedNaming_ExtendedNamingContext* current);

 ExtendedNaming_ExtendedNamingContext* BindContext
  (Environment* ev, ExtendedNaming_ExtendedNamingContext* rcontext,
   ExtendedNaming_ExtendedNamingContext* context);

 void BindWithProperties
  (Environment* ev, SOMObject* obj, ExtendedNaming_ExtendedNamingContext* rcontext);

 void RebindWithProperties
  (Environment* ev, SOMObject* obj, ExtendedNaming_ExtendedNamingContext* rcontext);

 void DumpContext
  (Environment* ev, ExtendedNaming_ExtendedNamingContext* nc, int indent);

 void NewPropertyList(int length, int maximum);

 void SetProperties
   (int propnum, char* propname, boolean sharable, TypeCode type, void* value);

 SOMObject*  NameResolve
   (Environment* ev, ExtendedNaming_ExtendedNamingContext* context);

 void unBind(Environment* ev, ExtendedNaming_ExtendedNamingContext* context);

 ~NameMgr();

 int checkEnv(Environment* ev) ;
 };
 #endif




