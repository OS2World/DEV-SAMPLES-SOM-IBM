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
/* @(#) somx/main.c 2.14 1/9/96 09:26:55 [1/24/96 17:31:11] */

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
   ==========================================================================
   emdemo.c

   This program exercises several facilities of the Event Manager (EMan).
   It shows how to register interest in timer, socket, file descriptor,
   message queue, client and workproc events. It also shows how callback
   routines are to be written.

   This program exercises almost all of the EMan methods.  For exercising
   socket related events, it creates an instance of the SOMSockets class and
   uses it for all TCP/IP calls.  It is important to make EMan applications
   (that use sockets) use the sockets class.  Otherwise, the application could
   make direct TCPIP calls while EMan uses a different socket implementation
   (for example, one based on NETBIOS on OS/2).  EMan picks the appropriate
   implementation of sockets based on the environment variable SOMSOCKETS.

   Note: the last statement in the main program (freeing EMan) is not really
   necessary since the program is ending anyway. It is there to exercise
   "somUninit" method of EMan.

   ==========================================================================
*/


/* Definitions and includes.
 --------------------------- */
#define INCL_DOS
#define INCL_ERRORS                    /* include os2 errors */
#define INCL_DOSPROCESS                /* include thread/process headers */
#define INCL_DOSSEMAPHORES             /* include semaphore headers */

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include <soms.h>                        /* SOMSockets class */

/* EMan includes.
 ---------------- */
#include <eman.h>
#include <emobj.h>

/* Globals.
 ---------- */
static  SOMEEMan  gEManPtr;
static  Sockets   gSockObjPtr = (void *)0;
static  int       ctr = 0;
static  EMObject  target;
static  int       regId0=0, regId1=0, regId2=0, regId3=0, regId4=0,
                  regId5=0, regId6=0, regId7=0, regId8=0, regId9=0;
static  int       sock=0, sockregId=0, msgsockregId=0, PortNumber=0, fd=0,mq=0;

static  SOMEClientEvent clientEvent1, clientEvent2;
static  SOMEEMRegisterData  data;
static  char  portnumbstr[10];

typedef struct foo {
  long mtype;
  char x[100];
} msgstruct;  /* for testing msg queues on AIX */

Environment *mainev;
int ExitMain = 0;

#define MY_MSG_TYPE   75  /* Arbitrarily chosen msg type number */

#pragma linkage(callBack, System)
#pragma linkage(callBack2, System)
#pragma linkage(AcceptConnection, System)
#pragma linkage(ReadSocketAndPrint, System)

void callBack( SOMEEvent  event, void  *targetData );
void ReadMsgAndPrint( SOMEEvent event, void *inputdata);
void ReadInputAndChange( SOMEEvent event, void *inputdata);
void ReadSockAndPrint(int sock);
void port_someSetRegDataEventMask(SOMEEMRegisterData data, Environment *pev,
                                                       long eventtype);

/*=========================================================================*/
/*                          Client Functions                               */
/*=========================================================================*/

/*-------------------------  ReadSocketAndPrint  --------------------------*/

static void ReadSocketAndPrint( SOMEEvent  event, void  *targetData )
{
    Environment *env;
    SOMEEMRegisterData  data;

    data = SOMEEMRegisterDataNew( );
    printf( "Data = %s\n", targetData );
    switch( _somevGetEventType( event, env  )) {
    case  EMSinkEvent:
        printf("ReadSocketAndPrint: Perceived A socket input msg\n" );
        ReadSockAndPrint(_somevGetEventSink(event, env));
        break;
    default: somPrintf("unknown Event type in ReadSocketAndPrint\n");
    }
}

/*-------------------------  AcceptConnection  ----------------------------*/

static void AcceptConnection( SOMEEvent  event, void  *targetData )
{
    SOMEEMRegisterData  data = SOMEEMRegisterDataNew( );
    unsigned int time, state, key;
    int msgsock, callerlen;
    Environment  *env = somGetGlobalEnvironment();

    #ifdef _XS2
        struct sockaddr caller;
    #else
        struct sockaddr_in caller;
    #endif

    printf( "Data = %s\n", targetData );
    switch( _somevGetEventType( event, env )) {

        case  EMSinkEvent:
            printf("Accept Connection: Perceived A socket connection request\n" );
            callerlen = sizeof(caller);
            msgsock = _somsAccept(gSockObjPtr,
                                  mainev,
                                  _somevGetEventSink(event, env),
                                  (struct sockaddr *)&caller,
                                  (long*)&callerlen);
            printf("messagesock=%d\n",msgsock);
            if (msgsock == -1) {
                perror("accept error");
                exit(1);
            }
            /* Socket Ok. Register it with EMan for further communication.
             ------------------------------------------------------------- */
            _someClearRegData( data, env );
            port_someSetRegDataEventMask( data, env, EMSinkEvent);
            _someSetRegDataSink( data, env, msgsock );
            _someSetRegDataSinkMask( data, env, EMInputReadMask );
            msgsockregId = _someRegisterProc( gEManPtr,
                                              env,
                                              data,
                                              ReadSocketAndPrint,
                                              "READMSG" );
            break;
        default:
            somPrintf("unknown Event type in AcceptConnection\n");
    }
}

/*-----------------------  CreateSocketAndRegister  -----------------------*/

static void CreateSocketAndRegister()
{
    Environment *env = somGetGlobalEnvironment();
    int length;

    struct sockaddr_in server;

    /* Create socket object.
     ----------------------- */
    sock = _somsSocket(gSockObjPtr, mainev, AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
            perror("opening stream socket");
            exit(1);
    }

    /* Name socket using wildcards.
     ------------------------------ */
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = 0;
    if (_somsBind(gSockObjPtr,
                  mainev,
                  sock,
                  (struct sockaddr *) &server,
                  sizeof(server)))
    {
        perror("binding stream socket");
        exit(1);
    }

    /* Find out assigned port number and print it out.
     ------------------------------------------------- */
    length = sizeof(server);
    if (_somsGetsockname(gSockObjPtr,
                         mainev,
                         sock,
                         (struct sockaddr *) &server,
                         (long*)&length))
    {
        perror("getting socket name");
        exit(1);
    }

    PortNumber = server.sin_port;
    printf("Socket has port #%d\n",
            _somsNtohs(gSockObjPtr, mainev, server.sin_port));
    sprintf(portnumbstr, " %d\0",
            _somsNtohs(gSockObjPtr, mainev, server.sin_port));

    /* Register this socket with EMan and keep listening for connections.
     -------------------------------------------------------------------- */
    _someClearRegData( data, env );
    port_someSetRegDataEventMask( data, env, EMSinkEvent);
    _someSetRegDataSink( data, env, sock);
    _someSetRegDataSinkMask( data, env, EMInputReadMask );
    sockregId = _someRegisterProc( gEManPtr,
                                   env,
                                   data,
                                   AcceptConnection,
                                   "CONNECTSOCKET" );

    /* Start accepting connections.
     ------------------------------ */
    _somsListen(gSockObjPtr, mainev, sock, 5);

}


/*--------------------------  ConnectAndSendMsg  --------------------------*/

static void ConnectAndSendMsg()
{
    #define DATA "This a wonderful world!"

    int sock;
    struct sockaddr_in server;
    struct hostent *hp;
    char hostname[100];

    /* Create socket object.
     ----------------------- */
    sock = _somsSocket(gSockObjPtr, mainev, AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
            perror("opening stream socket");
            exit(1);
    }

    /* Connect socket using name specified by command line.
     ------------------------------------------------------ */
    server.sin_family = AF_INET;
    /* gethostname(hostname, sizeof(hostname)); */
    strcpy(hostname, getenv("HOSTNAME"));
    printf("Host name is %s\n",&hostname);
    hp = _somsGethostbyname(gSockObjPtr, mainev, hostname);
    if (hp == 0) {
            fprintf(stderr, "%s: unknown host0", hostname);
            exit(2);
    }
    bcopy(hp->h_addr, &server.sin_addr, hp->h_length);
    server.sin_port = _somsHtons(gSockObjPtr, mainev,atoi(portnumbstr));
    printf("connecting to port # %d\n", atoi(portnumbstr));

    if (_somsConnect(gSockObjPtr,
                     mainev,
                     sock,
                     (struct sockaddr *)&server,
                     sizeof(server)),
                     0)
    {
        perror("connecting stream socket");
        exit(1);
    }

    if (_somsSend(gSockObjPtr, mainev, sock, DATA, sizeof(DATA),0 ), 0) {
        perror("writing on stream socket");
    }

    _somsSoclose(gSockObjPtr, mainev, sock);

}

/*------------------------------  unRegister  -----------------------------*/

void  unRegister( int  id )
{
    Environment *env = somGetGlobalEnvironment();

    if (id) _someUnRegister( gEManPtr, env, id );
}

/*----------------------------  ReadSockAndPrint  -------------------------*/

void ReadSockAndPrint(int sock)
{
    char x[100];
    int n;

    if ((n = _somsRecv(gSockObjPtr, mainev, sock, x, sizeof(x), 0)) > 0) {
        printf("String from the socket %d is: %s\n",sock, x);
    } else {
       if (n== 0) {
           printf("EOF encountered. Closing socket and unregistering.\n");
           _somsSoclose(gSockObjPtr, mainev, sock);
           unRegister(msgsockregId);
       } else {
           perror("ReadSockAndPrint");
       }
    }
}

/*--------------------------  ChangeRegistrations  ------------------------*/

void ChangeRegistrations(char x)
{
    msgstruct msg;
    char buf[10];
    FILE *fl;
    int  fdesc;
    char shellcommand[100];
    char filename[100];
    Environment *env = somGetGlobalEnvironment();

    switch (x) {
      case 'c' : /* make a client event occur */
                 _someQueueEvent(gEManPtr, env, clientEvent1);
                 break;
      case 'C' : /* make a client event occur */
                 _someQueueEvent(gEManPtr, env, clientEvent2);
                 break;
      case 'W' : /* register a work proc with eman */
                _someClearRegData( data, env );
                port_someSetRegDataEventMask( data, env, EMWorkProcEvent);
                /* permit up to 2 work procedures to be registered */
                if(!regId2){
                    regId2 = _someRegisterProc(gEManPtr,
                                               env,
                                               data,
                                               callBack,
                                               "WORKPROC1" );
                }
                else if(!regId9){
                    regId9 = _someRegisterProc(gEManPtr,
                                               env,
                                               data,
                                               callBack,
                                               "WORKPROC2" );
                }
                else {
                      printf("WORKPROC1 and WORKPROC2 have already been registered. \n");
                }
                break;
      case 'w' : /* Unregister a workproc from eman */
                 if(regId9) { 
                      unRegister(regId9);
                      regId9=0;
                 }
                 else if(regId2) { 
                      unRegister(regId2);
                      regId2=0;
                 }
                 break;
      case 'P' :  /* create a port (socket) and register to listen on */
	      	  if (!gSockObjPtr)
	            printf("**Socket events not supported without valid socket object**.\n");
                 else 
                   CreateSocketAndRegister();
                 break;
      case 'p' : /* close down the listening port */
  	      	  if (!gSockObjPtr)
	              printf("**Socket events not supported without valid socket object**.\n");
                  else 		   
                     unRegister(sockregId);
                  break;
      case 'S' : /* connect to the previous socket and send a msg */
	      	  if (!gSockObjPtr)
	             printf("**Socket events not supported without valid socket object**.\n");
                 else 		   
                     ConnectAndSendMsg();
                 break;
      case 't' : /* unregister timer events */
                 if(regId1){
                      unRegister(regId1);
                      regId1=0;
                 }
                 break;
      case 'T' : /* reRegister timer events */
                 if(!regId1) {
                     _someClearRegData( data, env );
                     port_someSetRegDataEventMask( data, env, EMTimerEvent);
                     _someSetRegDataTimerInterval( data, env, 100 );
                     regId1 = _someRegisterEv(gEManPtr,
                                              env,
                                              data,
                                              target,
                                              mainev,
                                              "eventMethod",
                                              "Timer Pop" );
                 }
                 break;
      case 'U' : /* change the timer interval value*/
                 _someClearRegData( data, env );
                 port_someSetRegDataEventMask( data, env, EMTimerEvent);
                 _someSetRegDataTimerInterval( data, env, 800 );
                 _someChangeRegData(gEManPtr, env,regId1, data);
                 break;
      case 'H' :
      case 'h' : printf("Enter one of the following single character commands.\n");
                 printf("c or C to create client events\n");
                 printf("W/w to register/unregister a work procedure\n");
                 printf("T/t to register/unregister timer. U to upgrade the timer\n");
                 printf("P/p to register/unregister a listening port (socket)\n");
                 printf("S to connect to the above port and send a message\n");
                 printf("q or Q to quit\n");
                 break;
      case 'q' :
      case 'Q' :
                 _someShutdown(gEManPtr, env);
                 ExitMain = 1;
                 DosExit(EXIT_PROCESS,0);
                 break;
    }

    /* Prompt for next action.
     ------------------------- */
    printf("Enter input to change eman behavior\n");
}


/*-------------------------------  callBack  ------------------------------*/

void callBack( SOMEEvent  event, void  *targetData )
{
    unsigned int  time, state, key;
    int           x, y, w, h, count, interval;
    void          *foo, *bar;
    Environment   *env = somGetGlobalEnvironment();

    printf( "Data = %s\n", targetData );
    switch( _somevGetEventType( event, env )) {

      case  EMTimerEvent:
          printf("callback: Perceived Timer event with data: %s\n", targetData);
          break;

      case  EMSinkEvent:
          printf("callback: Perceived Sink Event event with data: %s\n", targetData);
          break;

      case  EMClientEvent:
          printf("callback: Perceived Client Event event with data: %s\n", targetData );
          break;

      case  EMWorkProcEvent:
          printf("callback: Perceived WorkProc event\n" );
          break;

      default: somPrintf("unknown Event type in callback\n");

    }
}


/*--------------------------  ReadInputAndChange  -------------------------*/

void ReadInputAndChange( SOMEEvent event, void *inputdata)
{
    char x[100];

    scanf("%s", x);
    ChangeRegistrations(x[0]);
}

void os2ReadInputAndChange()
{
    while (1) {
       ReadInputAndChange(NULL, NULL);
    }
}

/*------------------------  someMakeSocketObj  ----------------------------*/

static void *someMakeSocketObj(Environment *ev, string socket_class)
{
    SOMClass classObj;
    somId tempId;
    Sockets sockObj = (Sockets) NULL;

    /* Instantiate the abstract Sockets class.
     ----------------------------------------- */
    SocketsNewClass(0,0);

    /* Instantiate the concrete sockets class.
     ----------------------------------------- */
    tempId = SOM_IdFromString (socket_class);
    classObj = _somFindClass (SOMClassMgrObject, tempId, 0, 0);
    SOMFree (tempId);

    if (classObj != NULL) {
        sockObj = (Sockets) _somNew (classObj);
    } else {
        printf("_somFindClass failed for class %s /n",socket_class);
        exit (-1);
    }
    return (void *) sockObj;
}

/*=========================================================================*/
/*                              Main Program                               */
/*=========================================================================*/

main()
{
    char  *comm_envvar;
    long  rc;
    TID   ThreadId;

    Environment *testEnv = somGetGlobalEnvironment();
    comm_envvar = (char *) somdGetenv( "SOMSOCKETS" ); 

    if (comm_envvar) {
        gSockObjPtr = (void *)someMakeSocketObj(testEnv, comm_envvar);
    }
    else {
	/* Create anyNet sockets object */
	gSockObjPtr = (void *)someMakeSocketObj(testEnv, "AnyNetSockets");
    }  

    gEManPtr = SOMEEManNew();
    data = SOMEEMRegisterDataNew( );   /* Registration data object */
    target = EMObjectNew();   /* object whose method is called back by EMan */

    mainev = somGetGlobalEnvironment();

    /* Create a client event of type "ClientType1".
     ---------------------------------------------- */
    clientEvent1 = SOMEClientEventNew();
    _somevSetEventClientType( clientEvent1, testEnv, "ClientType1" );
    _somevSetEventClientData(
               clientEvent1,
               testEnv,
               "This is a test for EMObject: client event type 1");

    /* Register the event with EMan.
     ------------------------------- */
    _someClearRegData( data, testEnv );
    port_someSetRegDataEventMask( data, testEnv, EMClientEvent);
    _someSetRegDataClientType(data, testEnv, "ClientType1");
    regId0 = _someRegisterEv( gEManPtr,
                              testEnv,
                              data,
                              target,
                              mainev,
                              "eventMethod",
                              "Hello Mom" );

    /* Create and register a different type of client event.
     ------------------------------------------------------- */
    clientEvent2 = SOMEClientEventNew();
    _somevSetEventClientType( clientEvent2, testEnv, "ClientType2" );
    _somevSetEventClientData(
                  clientEvent2,
                  testEnv,
                  "This is a test for callBack proc: client event type 2" );
    _someClearRegData( data, testEnv );  /* Note reuse of registration data object */
    port_someSetRegDataEventMask( data, testEnv, EMClientEvent );
    _someSetRegDataClientType( data, testEnv, "ClientType2" );
    regId4 = _someRegisterProc( gEManPtr, testEnv, data, callBack, "Hello Dad" );

    /*--------------------------------------------------------------------------
      For illustration purposes this program reads input from stdin and makes
      additional registrations or unregistrations with EMan based on the input
      read.  A separate thread is created to handle keyboard input.
     --------------------------------------------------------------------------*/

    if ((rc = DosCreateThread(&ThreadId,
                              (PFNTHREAD)os2ReadInputAndChange,
                              0,
                              0,
                              300000)) != 0)
    {
        printf("Return code from thread create is %d\n", rc);
        DosExit(EXIT_PROCESS, 0);
    }

    /* Prompt for initial input.
     --------------------------- */
    printf("Enter input to change eman behavior. Enter h for help.\n");

    /*------------------------------------------------------------------------

       Two alternative schemes are possible for a program using Eman.
       The client program can provide its own main loop, polling Eman
       periodically to process events registered with EMan.

       Alternatively, the client can turn control over to EMan's main
       loop, with all client processing handled through the callback
       mechanism.

       The latter scheme is employed when the program can be designed
       to be completely event-driven.  Both methods are illustrated below.

     -------------------------------------------------------------------------*/

    #ifdef ILLUSTRATE_LOOP     /* Calling EMan from client main loop */

    while (!ExitMain) {
       _someProcessEvent( gEManPtr, testEnv,
                          EMProcessTimerEvent | EMProcessSinkEvent |
                          EMProcessClientEvent | EMProcessWorkProcEvent );
       /***
            Other main loop work, which is not event-driven, would
            be done here.
       ***/
    }

    #else                        /* Fully Even-Driven scheme */

    /* Let EMan loop for ever processing events as they occur.
     --------------------------------------------------------- */
    _someProcessEvents(gEManPtr, testEnv);

    #endif

    _somFree(gEManPtr); /* Free up EMan and associated storage */

   DosExit(EXIT_PROCESS,0);
}

void port_someSetRegDataEventMask(SOMEEMRegisterData data, Environment *pev,
                                                       long eventtype)
{
#if  defined(__TURBOC__) || defined(_METAWARE)
/*-------------------------------------------------------------------------
   Only for Borland's C and C++ family of compilers, you need the following
   code, since the stack is handled differently for the functions/methods
   involving variable list of parameters. i.e. func(int a, char b,...).

   This applies to the METAWARE compilers for _OS/2 also.
--------------------------------------------------------------------------*/

/* Building two va_lists, one for SOMObject_somDispatch and another for
   someSetRegDataEventMask                                                */

    va_list va_buf  = SOMMalloc(5);
    va_list push_ap = SOMMalloc(2);
    va_list ap      = va_buf;
    va_list nul_ap  = push_ap;

    somId   EventMaskId = somIdFromString("someSetRegDataEventMask");

/* Build the va_list with appropriate parameters */

    va_arg(nul_ap, void *)         = NULL;
    va_arg(ap, SOMEEMRegisterData) = data;

/* Assuming that callstyle = idl . Do not use following
   line if you use callstyle =oidl, because Environment is not supported. */

    va_arg(ap, Environment *)      = pev;
    va_arg(ap, long)               = (long)eventtype;
    va_arg(ap, va_list)            = push_ap;

    SOMObject_somDispatch(data, NULL, EventMaskId,va_buf);

#else
    _someSetRegDataEventMask(data, pev, eventtype, NULL);

#endif
}
