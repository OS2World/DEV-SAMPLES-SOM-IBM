3/8/96 16:54:04

25H7912  (C)  COPYRIGHT International Business Machines Corp. 1996
All Rights Reserved
Licensed Materials - Property of IBM
US Government Users Restricted Rights - Use, duplication or
disclosure restricted by GSA ADP Schedule Contract with IBM Corp.

DISCLAIMER OF WARRANTIES.
The following [enclosed] code is sample code created by IBM
Corporation. This sample code is not part of any standard or IBM
product and is provided to you solely for the purpose of assisting
you in the development of your applications.  The code is provided
"AS IS". IBM MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE, REGARDING THE FUNCTION OR PERFORMANCE OF
THIS CODE.  IBM shall not be liable for any damages arising out of
your use of the sample code, even if they have been advised of the
possibility of such damages.

DISTRIBUTION.
This sample code can be freely distributed, copied, altered, and
incorporated into other software, provided that it bears the above
Copyright notice and DISCLAIMER intact.


The Multi-framework sample is a step-by-step introduction to the SOM 
Development Toolkit and its frameworks. As you step through the sample
you'll build and understand the design of an appointment calendar
program.  Each phase introduces additional application features and SOM
frameworks. By the final phase, you have a simple but realistic application
that uses a significant cross section of the SOM toolkit. You may find 
classes in the sample a useful starting point for building your own 
applications. The features and frameworks for each phase are as follows:

Phase1   - Sample Base Class Definition.
Phase2   - Graphical User Interface(GUI).
Phase3   - Distributed Calendar Using Naming and DSOM.
Phase4   - Persistent Calendar Using POSSOM.
Phas4DB2 - Persistent Calendar Using DB/2 Backing Storage.
Phase5   - Persistent Calendar Using Events and Transaction Rollback.
Phase6   - Distributed or Persistent Calendar Using Security.

How to Build, Configure and Execute the sample:

Build:

   Desktop Compile:
   Open the folder that contains the source files of the phase you wish to
   compile.
   Double click the buildsmp program icon.
   Enter the text selection for your compiler.

   Directory Compile:
   Change your working directory to the directory that contains the source
   files of the phase you wish to compile. For example:
   cd \som\samples\sommfss\phase5
   Use the makefile appropriate for your compiler to compile the sample.

Make sure somdd is running before compiling the sample. If not execute
the command:
start somdd

The executable generated for the graphical interface, in Phase2 and Phase3,
is somsamp.exe. For all phases, the executable generated for the text based
interface is sample.exe.

The makefile will update the Interface Repository with the class 
information. The last interface repository file in your SOMIR environment
variable path will be the interface repository file which contains the 
sample class description. To view the class description for AppointmentBook
enter the following command:
irdump AppointmentBook

Also, for Phase3 to Phase6, the registration of the server implementations
and the classes associated with the servers will be done for you by the 
makefile.  If  you would like to view this information enter the following 
command:
regimpl -l


Configure:

Make sure somdd is running.

Phase1, Phase2:
None required.

Phase3:
Make sure the server is initialized before the first execution as follows:
somossvr.exe -a BookServer -i

Phase4, Phase5:
Make sure the server and the client are initialized before the first 
execution as follows:
somossvr.exe -a BookServer -i
sample -a BookClient -i

Phas4DB2:
Start DB2 and follow the above instructions for Phase4.

Initialize additional clients for Phase4 to Phase6 on the first execution
as follows:
sample -a BookClient2 -i
This assumes that the implementation BookClient2 was already registered
using the command:
regimpl -A -i BookClient2 -p sample.exe -v somOS::Server -m on

Phase6:
To set up the BookServer as a secure server, you need to do the following: 

Set the authentication attribute in the security stanza of "somenv.ini":
DISABLE_AUTHN=FALSE
Update the existing server in the implementation repository, enter: 
regimpl -U -i BookServer -s on
Ensure you have a userid and password defined to LAN Requestor 4.0. 
Logon using one of your valid user ids.
Stop the BookServer and restart it: start somossvr.exe -a BookServer

Now you may run the client application as before, following the instructions for
whatever phase you are using. 


Execute:

Phase1, Phase3:
To run the text executable:
sample 

Phase2, Phase3:
To run the GUI executable:
somsamp

Phase4, Phs4DB2, Phase5:
To run the text executable, after it is initialized:
sample -a BookClient
To run an additional client, after it is initialized:
sample -a BookClient2

Phase6: To run Phase3 to Phase5 using security follow the same instructions
as above.
