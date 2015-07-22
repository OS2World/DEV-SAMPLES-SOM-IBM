//
// COMPONENT_NAME: somtr::SAMPLES
//
// ORIGINS: 27
//
// Licensed Materials - Property of IBM                                     
//                                                                          
// 25H7-912, 5697-197 (EMEA), 17H5-843 (JAPAN)                              
//                                                                          
// (c) Copyright IBM Corp. 1995                                             
//
// DISCLAIMER OF WARRANTIES.
// The following [enclosed] code is sample code created by IBM
// Corporation. This sample code is not part of any standard or IBM
// product and is provided to you solely for the purpose of assisting
// you in the development of your applications.  The code is provided
// "AS IS". IBM MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE, REGARDING THE FUNCTION OR PERFORMANCE OF
// THIS CODE.  IBM shall not be liable for any damages arising out of
// your use of the sample code, even if they have been advised of the
// possibility of such damages.
//
// DISTRIBUTION.
// This sample code can be freely distributed, copied, altered, and
// incorporated into other software, provided that it bears the above
// Copyright notice and DISCLAIMER intact.
// 

//==============================================================================
// DESCRIPTION: The client program for the Hanoi sample program.  Since this 
//              client coordinates the commitment of transactions, it must also
//              be a DSOM server.
//==============================================================================

#include <stdlib.h>       // Standard library functions
#include <string.h>       // ANSI standard string interface
#include <somd.xh>        // DSOM interface
#include <somosutl.h>     // somOS::Server utilities interface
#include <somthrd.h>      // Threads interface
#include <somtran.xh>     // Transaction Service interface
#include <sompipo.xh>     // Persistence interface
#include <sompposx.xh>    // POSIX PDS interface

#include "hancon.xh"      // HanoiConstants module
#include "hanres.xh"      // HanoiResource interface
#include "phanres.xh"     // PersistentHanoiResource interface
#include "traninv.xh"     // TranInv interface
#include "hanoi.xh"       // Hanoi interface

static void initialise_DSOM(int argc, char* argv[], Environment*const ev);
static Hanoi*const make_tower(Environment*const ev);
static void run(Environment*const ev, Hanoi*const tower);
static void move_disks(Hanoi* tower, Environment* ev, long count, long from_spindle
                 , long to_spindle);
static void usage();
static int getopt(int argc,char* argv[],char* names,int* optind,char* optarg[]);
static somTD_SOMThreadProc process_requests;
static void print_name(Environment*const ev, int impl_active); 
static void show_state(Hanoi* tower, Environment* ev);
static void specific_error(const char* report, Environment*const ev, int impl_active);
static void fatal_error(const char* function, Environment*const ev, int impl_active);
static void fatal_error(const char* function, Environment*const ev, unsigned long rc
                  , int impl_active);

const unsigned long OK = 0;
const unsigned long CLIENT_SERVER_STACK_SIZE = 100000;

//==============================================================================
// Function: error
//                                                                             
// Return whether or not the specified environment indicates an error occured in
// the previous SOM call.                                                       
//==============================================================================
inline int                         // whether or not error occured
error( const Environment*const ev  // SOM environment
     )
{ 
  return ev->_major != NO_EXCEPTION;
}

//==============================================================================
// Function: free_exception
//                                                                             
// If an exception exists, free it.
//==============================================================================
static          void                  // No return value
free_exception( Environment*const ev  // SOM environment
              )
{ 
  if (error(ev))
    somdExceptionFree(ev);
}

//==============================================================================
// Function: main                                                       
//                                                                             
// Client program that uses the Tower of Hanoi server. 
//==============================================================================
      int                                 // return code
main( int                        argc     // number of program arguments
    , char*                      argv[]   // program arguments
    )
{  
   Environment e;                         // SOM environment
   Environment *const ev = &e;             

   initialise_DSOM(argc,argv,ev);

   // Create a current object in order to begin and commit a transaction
   CosTransactions_Current *const current = new CosTransactions_Current;

   if (somIsObj(current)) 
   {
     Hanoi*const tower = make_tower(ev);

     somPrintf("\nBeginning a top-level transaction...\n");

     // Begin a top-level transaction
     current->begin(ev);

     if (!error(ev))
     {
       somPrintf("\nCurrent state of Hanoi instance is:\n");

       // Display the current state
       show_state(tower,ev);

       if (!error(ev))
       {
         somPrintf("\nMoving disks to the first free spindle...\n");

         // Run the Tower of Hanoi simulation
         run(ev,tower);

         if (error(ev))
           specific_error("Hanoi::run failed",ev,1);
       }
       else
         specific_error("Hanoi::Display failed",ev,1);

       somPrintf("\nCommitting the top-level transaction...\n");

       // commit the top-level transaction and opt not to report heuristics
       current->commit(ev,FALSE);

       if (error(ev))
         specific_error("CosTransactions::Current::commit failed",ev,1);

       somPrintf("\nCurrent state of Hanoi instance is:\n");

       // Display the current state
       show_state(tower,ev);

       if (error(ev))
         specific_error("Hanoi::Display failed",ev,1);
     }
     else
       specific_error("Begin of top-level transaction failed",ev,1);

     somPrintf("\nBeginning a second top-level transaction...\n");

     // Begin a top-level transaction
     current->begin(ev);

     if (!error(ev))
     {
       somPrintf("\nCurrent state of Hanoi instance is:\n");

       // Display the current state
       show_state(tower,ev);

       if (!error(ev))
       {
         somPrintf("\nMoving disks to the first free spindle...\n");

         // Run the Tower of Hanoi simulation
         run(ev,tower);

         if (error(ev))
           specific_error("Hanoi::run failed",ev,1);
       }
       else
         specific_error("Hanoi::Display failed",ev,1);

       somPrintf("\nRolling back the second top-level transaction...\n");

       // roll back the top-level transaction
       current->rollback(ev);

       if (error(ev))
         specific_error("CosTransactions::Current::rollback failed",ev,1);

       somPrintf("\nCurrent state of Hanoi instance is now:\n");

       // Display the current state
       show_state(tower,ev);

       if (error(ev))
         specific_error("Hanoi::Display failed",ev,1);
     }
     else
       specific_error("Begin of top-level transaction failed",ev,1);

     // Destroy the Tower of Hanoi object but leave the persistent data alone
     delete tower;

     // Discard the current object 
     delete current;
   } 
   else
     specific_error("Current object creation failed",ev,1);

   somPrintf("\nExecution complete.\n");

   somos_exit(0);

   return 0; // not executed - some compilers error a missing return 
}

//==============================================================================
// Function: make_tower
//                                                                             
// Create a Hanoi instance.  
//
// N.B. See the Persistence Service chapter of the SOMobjects Developer's 
//      Toolkit User's Guide for information about the persistence classes
//      used here.
//==============================================================================
static      Hanoi*const           // Hanoi instance
make_tower( Environment*const ev  // SOM environment
          )
{
   // Set up persistent id. of tower object
   somPersistencePOSIX_PID_POSIX *tower_pid = (somPersistencePOSIX_PID_POSIX*)(void*)(somdCreate(ev,"somPersistencePOSIX::PID_POSIX",TRUE));
   if (tower_pid == 0) 
     fatal_error("somdCreate of somPersistencePOSIX::PID_POSIX",ev,1);

   tower_pid->_set_pathName(ev,"tower.pos");
   if (error(ev))
     fatal_error("somPersistencePOSIX_PID_POSIX::_set_pathName",ev,1);

   // Set up Tower of Hanoi factory
   ExtendedNaming_ExtendedNamingContext* enc = (ExtendedNaming_ExtendedNamingContext*)(SOMD_ORBObject->resolve_initial_references(ev,"FactoryService"));
   if (error(ev))
     fatal_error("ORB::resolve_initial_references",ev,1);

   somPersistence_IRPOFactory* hanoiFactory = (somPersistence_IRPOFactory*)(enc->find_any(ev,"class == 'Hanoi'",0));
   if (error(ev))
     fatal_error("ExtendedNaming::ExtendedNamingContext::find_any",ev,1);

   Hanoi*const tower = (Hanoi*)(void*)(hanoiFactory->createIRPO(ev,tower_pid));                   
   if (error(ev))
     fatal_error("somPersistence::IRPOFactory::createIRPO",ev,1);

   tower->_set_name(ev,"tower");

   if (error(ev)) 
     fatal_error("Hanoi::_set_name",ev,1);

   return tower;
}

//==============================================================================
// Function: run
//                                                                             
// Run the Tower of Hanoi simulation.  We should start from a state in which
// all the disks are on a single spindle.  We select one of the other empty
// spindles and then move all the disks from the full spindle to the chosen
// empty spindle by calling the recursive function move_disks.
//==============================================================================
static void                                 // No return value
run(   Environment*const          ev        // SOM environment
   ,   Hanoi*const                tower     // Hanoi object
   )
{
  long from_spindle = tower->get_full_spindle(ev);
  if (error(ev))
    specific_error("Hanoi::get_full_spindle failed",ev,1);

  long to_spindle = tower->get_empty_spindle(ev);
  if (error(ev))
    specific_error("Hanoi::get_empty_spindle failed",ev,1);

  // Check that both spindles are valid
  if (from_spindle == -1) 
  {
    somPrintf("\nError detected in run(): No spindle holds all the disks");
    return;
  } 

  if (to_spindle == -1) 
  {
    somPrintf("\nError detected in run(): No empty spindle is available");
    return;
  } 

  // Call recursive function to move all the disks.
  move_disks(tower,ev,HanoiConstants_DISKS,from_spindle,to_spindle);
}

//==============================================================================
// Function: move_disks 
//                                                                             
// Move a number of disks from one spindle to another spindle percolating any
// exceptions to the caller.  We implement this as a recursive function, i.e.
// a function which calls itself.  To move n+1 disks from spindle A to
// spindle B, we first of all move the top n disks from A to the remaining 
// spindle C, then move the (n+1)th disk from A to B, then the n disks from C 
// to B.  To move a single disk, we just do it.  Each time we want to move a  
// disk we do it by calling the move_single_disk method on the Hanoi instance
// after which we display the new state of the Hanoi instance.
//==============================================================================
static      void                       // No return value 
move_disks( Hanoi*       tower         // Hanoi object
          , Environment* ev            // SOM environment
          , long         count         // no. of disks to move
          , long         from_spindle  // source of move
          , long         to_spindle    // target of move
          )
{
  // Create a current object in order to begin and commit a transaction
  CosTransactions_Current *const current = new CosTransactions_Current;

  if (somIsObj(current))
  {
    // Start a subtransaction  
    // N.B. The only reason to start a subtransaction here is to show how 
    //      nested transactions are used.
    current->begin(ev);
  
    if (!error(ev))
    {
      // Determine the spare spindle
      const long spare_spindle = 3 - (from_spindle + to_spindle);

      // If there is more than one disk, then move the rest to the spare spindle
      if (count > 1) 
        move_disks(tower,ev,count - 1, from_spindle, spare_spindle);
    
      if (!error(ev))
      {
        // Move the last disk 
        tower->move_single_disk(ev,from_spindle,to_spindle);
      
        if (!error(ev))
        {
          // Having changed the state, display it.
          show_state(tower,ev);
        
          if (!error(ev))
          {
            // If there is more than one disk, then move the rest from the spare spindle
            if (count > 1) 
              move_disks(tower,ev,count - 1, spare_spindle, to_spindle);
          
            if (!error(ev))
            {
              // Commit the subtransaction and if it rolls back, cause the parent
              // transaction also to roll back since we choose to percolate errors
              // to the top-level transaction.
              current->commit(ev,FALSE); 
              if (error(ev))
              {
                specific_error("CosTransactions::Current::commit failed",ev,1);
                current->rollback_only(ev);
              }
            }
          }
        }
        else
          specific_error("Hanoi::move_single_disk failed",ev,1);
      }
    }
    else
      specific_error("Begin of subtransaction failed",ev,1);
  
    // Discard the current object 
    delete current;
  }
  else
    specific_error("Current object creation failed",ev,1);
}

//==============================================================================
// Function: initialise_DSOM                                                        
//                                                                             
// Get a SOM/DSOM environment and start the client's DSOM server.                            
//                                                                             
// This is required so that Coordinators that are created in this process can   
// receive registration and recovery operations, and also allows us to convert 
// between objects and strings.            
//==============================================================================
static           void                      // No return value
initialise_DSOM( int               argc    // number of program arguments
               , char*             argv[]  // program argument values
               , Environment*const ev      // SOM environment
               )
{
  unsigned long rc;                    
  somToken request_thread;             

  string   impl_alias = 0;             
  string   impl_id = 0;                
  boolean  initialise_mode = FALSE;    
  int      optind = 1;                 
  char*    optarg = 0;                 
  int c;

  somPrintf("Initializing DSOM...\n");

  // Get options
  while(-1 != (c = getopt(argc, argv, "a:A:id", &optind, &optarg)))
  {
    switch( c )
    {
      case 'a':
      case 'A':
        impl_alias = optarg;
        break;
      case 'd':
        SOM_TraceLevel = 1;
        break;
      case 'i':
        initialise_mode = TRUE;
        break;
      default:
        usage ();
        break;
    }
  }

  // Cannot pass both implementation alias and id.  If neither is passed,
  // use a default value.
  if( optind != argc )
  {
    if(impl_alias != 0) 
      usage();
    impl_id = argv[optind];
  }
  else
    if (impl_alias == 0)
      impl_alias = "hanoiClient1";

  SOM_InitEnvironment(ev); 

  // Set up SOMOS internals.
  somos_setup();

  SOMD_Init(ev); 
  if (error(ev)) 
  {
    specific_error("SOMD_Init failed",ev,0);
    somos_exit(SOMOS_SOMD_INIT_FAILED);
  }

  // Create a SOM object adapter object
  SOMD_SOMOAObject = new SOMOA; 

  if (SOMD_SOMOAObject == 0) 
  {
    specific_error("SOM object adapter object not allocated",ev,0);
    somos_exit(SOMOS_SOMOA_NEW_FAILED);
  }

  // Find implementation by alias or id.
  if (impl_alias)
    SOMD_ImplDefObject = SOMD_ImplRepObject->find_impldef_by_alias(ev,impl_alias);
  else
    SOMD_ImplDefObject = SOMD_ImplRepObject->find_impldef(ev,impl_id);

  if (error(ev)) 
  {
    specific_error("Implementation definition not found",ev,0);
    somos_exit(SOMOS_FIND_IMPLDEF_FAILED);
  }

  // Initialize any required object services - before impl_is_ready.           
  somos_init_services(initialise_mode);

  // Implementation now ready to process requests so tell the SOM object
  // adapter we are ready
  SOMD_SOMOAObject->impl_is_ready(ev,SOMD_ImplDefObject); 

  if (error(ev))
  {
    specific_error("Implementation could not be activated",ev,0);
    somos_exit(SOMOS_IMPL_IS_READY_FAILED);
  }

  // Initialize any required object services - after impl_is_ready.
  somos_init_services_afterimpl(initialise_mode);

  somPrintf( "DSOM initialized.\nStarting client's server thread...\n" );

  // Start the request processing thread passing no data.
  rc = SOMStartThread( &request_thread
                     , process_requests
                     , NULL
                     , 0
                     , CLIENT_SERVER_STACK_SIZE
                     );

  if (rc != OK)
    fatal_error("SOMStartThread",ev,rc,1);
}

//==============================================================================
// Function: getopt 
//
// Get the value of a command line option.   
//
// The routine locates the next option from the program arguments in the list of 
// names of valid options.  If no more options are found, the routine returns   
// -1.  Otherwise the routine returns the option name, and if an argument is     
// required, points optarg at the option value in argv.  The optind index is     
// updated to point to the next argument.                                        
//==============================================================================
static  int               // First character of the option
getopt( int    argc       // Number of arguments                 
      , char*  argv[]     // Array of arguments                  
      , char*  names      // Valid option names                      
      , int*   optind     // Index in argv of the current option 
      , char** optarg     // Pointer to the option value             
      )
{
  int option_name;

  // If optind points past the end of the argument list, return -1.             
  if (*optind >= argc) 
    return -1;

  // If the first character of the current argument is not a slash or minus,
  // return -1;                                                               
  if (argv[*optind][0] != '/' && argv[*optind][0] != '-') 
    return -1;

  option_name = argv[*optind][1];

  // Check that the rest of the current argument matches one of the parameter   
  // names.                                                                     
  int current_name = 0;

  while (current_name != -1 && current_name < strlen(names))
  {
    // If the option name matches the current one, get its value, if any.         
    if( names[current_name] == option_name )
    {
      // If the next character is a colon, then the option requires a parameter.    
      *optind += 1;

      if( names[current_name+1] == ':' )
      {
        // If there are no more arguments, or the value begins with a slash or minus, 
        // then there is no value.                                                    
        // Otherwise just get the next argument as the value, and move on past it.    
        if (*optind >= argc || *argv[*optind] == '/' || *argv[*optind] == '-')
          *optarg = 0;
        else
        {
          *optarg = argv[*optind];
          *optind += 1;
        }
      }
      else
        *optarg = 0;

      // Indicate that the search is complete.
      current_name = -1;
    }

    // Move on to the next argument name, ignoring a colon if there is one.
    else 
      if( names[++current_name] == ':' )
        current_name++;
  }

  return option_name;
}

//==============================================================================
// Function: process_requests
//                                                                             
// Starts the DSOM server request processing loop.
//==============================================================================
static            void       // No return value
process_requests( void* data // data passed to this function
                )
{
  Environment e;                   
  Environment *const ev = &e;             

  SOM_InitEnvironment(ev);

  print_name(ev,1);
  somPrintf( ": Client's server accepting requests.\n" );

  (void)SOMD_SOMOAObject->execute_request_loop(ev, SOMD_WAIT);

  fatal_error("execute_request_loop",ev,1);
}

//==============================================================================
// Function: usage                                             
// 
// Display usage information.                                   
//==============================================================================
static void   // No return value
usage(
     )
{
  somPrintf("Client [-i] [-d] [-a impl_alias | impl_uuid]\n");

  somos_exit(SOMOS_USAGE_ERROR);
}

//==============================================================================
// Function: print_name
//                                                                             
// Print the alias of the ImplementationDef or a default name if the impl. is
// not ready. 
//==============================================================================
static      void                           // No return value
print_name( Environment*const ev           // SOM environment
          , int               impl_active  // whether or not impl. is ready
          ) 
{
  char* name;

  if (impl_active)
    name = SOMD_ImplDefObject->_get_impl_alias(ev);  
  else
    name = "hanoiClient";

  somPrintf(name);
}

//==============================================================================
// Function: show_state
//                                                                             
// Display the state of a Hanoi instance.
//==============================================================================
static      void                // No return value
show_state( Hanoi*       tower  // Hanoi instance
          , Environment* ev     // SOM environment
          )
{
  string pic = tower->display(ev);
  somPrintf(pic);
  ORBfree(pic);
}

//==============================================================================
// Function: specific_error
//                                                                             
// Notify a specific error to the user and return to the caller having marked
// the current transaction so that it may only roll back.
//==============================================================================
static          void                           // No return value
specific_error( const char*       report       // details of error
              , Environment*const ev           // SOM environment               
              , int               impl_active  // whether or not impl. is ready 
              )
{
  print_name(ev, impl_active);
  somPrintf(" error: %s\n", report);
  free_exception(ev);

  CosTransactions_Current *const current = new CosTransactions_Current;

  if (somIsObj(current))
  {
    current->rollback_only(ev);
    if (error(ev))
      fatal_error("CosTransactions::Current::rollback_only",ev,impl_active);
    delete current;
  }
  else
    fatal_error("CosTransactions::Current creation",ev,impl_active);

  return;
}

//==============================================================================
// Function: fatal_error
//                                                                             
// Notify a fatal error to the user, clean up, and terminate the client.  This  
// function is overloaded as different fatal errors require different diagnostic
// parameters.                                                                  
//==============================================================================
static       void                           // No return value
fatal_error( const char*        function    // name of function that failed          
           , Environment*const  ev          // SOM env. carrying an exception 
           , int                impl_active // whether or not impl. is ready
           )
{
  print_name(ev, impl_active);
  somPrintf( " error: %s failed, exception = %s\n"
           , function
           , somExceptionId(ev)
           );
  free_exception(ev);
  somos_exit(SOMOS_SERVER_FAILED);
}

             void                            // No return value
fatal_error( const char*        function     // name of function that failed          
           , Environment*const  ev           // SOM environment                       
           , unsigned long      rc           // return code                           
           , int                impl_active  // whether or not impl. is ready
           )
{
  print_name(ev, impl_active);
  somPrintf( " error: %s failed, return code = %d\n"
           , function
           , rc
           );
  free_exception(ev);
  somos_exit(SOMOS_SERVER_FAILED);
}
