/*
 *
 *  25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996
 *  All Rights Reserved
 *  Licensed Materials - Property of IBM
 *  US Government Users Restricted Rights - Use, duplication or
 *  disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 *
 *
 *  DISCLAIMER OF WARRANTIES.
 *  The following [enclosed] code is sample code created by IBM
 *  Corporation. This sample code is not part of any standard or IBM
 *  product and is provided to you solely for the purpose of assisting
 *  you in the development of your applications.  The code is provided
 *  "AS IS". IBM MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT
 *  NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE, REGARDING THE FUNCTION OR PERFORMANCE OF
 *  THIS CODE.  IBM shall not be liable for any damages arising out of
 *  your use of the sample code, even if they have been advised of the
 *  possibility of such damages.
 *
 *  DISTRIBUTION.
 *  This sample code can be freely distributed, copied, altered, and
 *  incorporated into other software, provided that it bears the above
 *  Copyright notice and DISCLAIMER intact.
 *
 */

                            Read Me for DB2 Persistence Sample
                            ----------------------------------

This sample stores an object's state data persistently to a DB2/2 database. The DB2 database has a
single database table.

This file establishes a context from which the sample can be executed.

Contents of this Read Me file:

   -	Related Samples
   -	Setup for the Sample
   -	Files Provided
   -	Notes to User
   -	Description of Sample
   -	Flow of Sample
   -	Sample Constraints


Related Samples:
----------------

Other Persistence-related samples provided in this sample package:

   -	Sample showing use of POSSOM with POSIX datastore


Setup/building the Sample:
--------------------------

Steps to setup, build, and run the Persistent PhoneEntry DB2/2 sample follow.

	1.	Configure DSOM, see the main DSOM readme

	2.	Setup the DB2 database as follows (you must have DB2/2 version 2.1 or higher installed on
your OS/2 system before this sample will execute correctly).  This sample expects a database
named TESTDB.	 You may type the following commands interactively from an OS/2 session, or
use the CMD file DB2SETUP.CMD, which has been included to automate this setup.
(commands are shown in capital letters but can be typed in lowercase)

     	1.	DB2START (Starts the database. You must always do this step, the next set of
     DB2-related steps need only be done once to initially create the database table)
     	2.	DB2 DROP DATABASE TESTDB (Drops the test database if it was previously created)
     	3.	DB2 CREATE DATABASE TESTDB (Creates the test database named TESTDB)
     	4.	DB2 CONNECT TO TESTDB (Connects to the test database)
     	5.	DB2 CREATE TABLE USERID.PHONE (ID SMALLINT NOT NULL, NAME CHAR(20),
     PHONE_NUMBER CHAR(20), OFFICE SMALLINT, PRIMARY KEY (ID)) (Initializes
     database with a table named USERID.PHONE)
     	6.	DB2 GRANT ALL ON TABLE USERID.PHONE TO PUBLIC (Gives public access to the
     database table)
     	7.	DB2 COMMIT (Commits the test database)
     	8.	DB2 CONNECT RESET (Resets connection to database, the sample will establish its own
     connection to the test database)

	3.	START SOMDD (Starts DSOM. This is necessary because the makefile will register the
sample's MYSERVER in the implementation repository and start the server)

	4.	For the following compilers:

     	1.	For Cset++, Visual Age, and Metaware compilers: nmake -f makefile_name all (Builds the
     executables. Cleans up any old files from a previous compile), where makefile_name is:
     MAKEFILE for CSet++ compiler, MAKEFILE.AGE for Visual Age compiler, and
     MAKEFILE.MTW for Metaware compiler.
     	2.	For Watcom compiler: wmake -f makefile.wat all (Builds the executables. Cleans up any
     old files from a previous compile).
     	3.	For Borland compiler: make -f makefile.bor all (Builds the executables. Cleans up any
     old files from a previous compile).

	5.	maindb2 (Executes the client program)


Files provided:
---------------

  - BUILDSMP.CMD Command for building the sample
  - DB2SETUP.CMD DB/2 setup file
  -	README.DB2 (Contains directions for setup, build, and running of sample; and sample
description)
  -	MAKEFILE (Makefile for CSet++ compiler to build the sample into executables)
  -	MAKEFILE.AGE (Makefile for VisualAge compiler to build the sample into executables)
  -	MAKEFILE.BOR (Makefile for Borland compiler NOT SUPPORTED at this time)
  -	MAKEFILE.MTW (Makefile for Metaware compiler NOT SUPPORTED at this time)
  -	MAKEFILE.WAT (Makefile for Watcom compiler NOT SUPPORTED at this time)
  -	PHONE.IDL (IDL for PhoneEntry class)
  -	PHONE.CPP (C++ program to define phoneEntry class methods)
  -	MAINDB2.CPP (Client program which builds phonebook entries and stores them
persistently)
  -	SMAP.IDL (IDL for Schema Mapper)
  -	SMAP.SQC (C  program with embedded SQL that implements the Schema Mapper)

Notes to User:
--------------

This sample requires that DB2/2 Version 2.1 or later has been installed on your system.

This sample assumes that DSOM has been correctly configured.

The makefile registers a server named MYSERVER and the required POSSOM classes for the DB2
PDS and PID.

The user should delete any other server that may have the same SOM classes registered with it as are
registered with MYSERVER for this sample.

Exception handling is done minimally.

Description of Sample:
----------------------

Please reference the SOMObjects Object Services Programmer's Guide chapter about the Persistent
Object Service (POSSOM).  This sample is based on the PhoneEntry example which is described in
detail in the "Scenarios for Creating and Using Persistent Objects" section of the POSSOM chapter.

The Persistent Object Service (also known as POSSOM) is used to store objects into a variety of
persistent datastores.  The mechanism that interfaces between the persistence framework and the
datastore is called a Persistent Data Service (PDS).

IBM supplies three PDS's with the SOMObjects Toolkit:	 a POSIX PDS (interfaces to a flat file
datastore), a B-Tree PDS (interfaces to an indexed or keyed file datastore), and a DB2 PDS (interfaces
to the DB2 relational database). A user or database provider can write additional PDS's to interface to
the datastore of their choice.

The strength of the Persistent Object Service is the fact that the client program does not have to write
very much datastore specific code in order to store their objects into a variety of different datastores.
Basically, the specifics for interfacing with a particular datastore are encapsulated in the PDS and
something called a "Persistent Identifier" (or PID).

The goal of this sample is to demonstrate how to write a client program to store persistent object state
data into a DB2/2 relational database.  The application is a phone book where each entry in the phone
book contains a person's name, phone number, and office location.  The objects being made persistent
are of the phoneEntry class type.  The person's name, phone number, and office location are stored in
a DB2/2 relational database table named USERID.PHONE.  A schema mapper is provided and will be
explained in detail later.

In order to better understand the differences imposed on the client program by different datastores,
you should contrast this sample with the PhoneEntry POSIX sample, also provided in this sample
package.  You should note differences in the following between the two samples:

   -	The classes that the makefile registers in the IR.  Note that there are different classes to be
registered depending upon the datastore type (for example, somPersistenceDB2::PDS_DB2
and somPersistenceDB2::PID_DB2 are registered by the MAKEFILE for this sample to work
properly).

   -	The PID is setup differently, depending on datastore type. The PID does encapsulate some of
the characteristics of the datastore, so the PID for DB2 requires setting a database alias
which is the name of the DB2 database alias (in our case TESTDB) as well as a schema
mapper name (the schema mapper is described below).  In this example, we also show how
additional datastore specific information	can be stored in the pid.  We store an id in the pid to
be used as a unique database table entry identifier. In contrast, the POSIX PID requires only
that a pathname be set.

   -	A schema mapper must be created when using the DB2 datastore.

The Schema Mapper:
------------------

The DB2 PDS uses a schema mapper to map object definitions to relational data definitions. Our DB2
schema mapper is defined as a class with methods for storing, restoring, and deleting externalized
objects into and out of the DB2 database.  The DB2 PDS externalizes objects and then invokes these
schema mapper methods on the resulting object stream for storing, restoring, and deleting the object
data in the datastore.

The schema mapper methods must be specialized for each type of object being stored in DB2.  The
implementation of the schema mapper methods is primarily embedded SQL and object method
invocations.  This allows the object implementor to customize the schema mapper methods while still
having the performance advantage of precompiled SQL.  The input for SQL statements (insert and
update) and their results (select) are stored in Programming Language variables, typically one table
column per variable.

When manually creating the schema mapper, you must:

    -	Define the schema mapper IDL (see SMAP.IDL file in sample directory)

    -	Implement the schema mapper store, restore, and Delete methods (see SMAP.SQC file in
sample directory). Both of these files are described in detail below.

SMAP.IDL File

The SMAP.IDL file contains the IDL for the schema mapper. Basically, the IDL is a simple interface
derived from the schema mapper class (somPersistenceDB2::SchemaMapper) and overriding the
store(), restore(), and Delete() methods.

SMAP.SQC File

The SMAP.SQC file was created by running the SOM compiler on the schema mapper IDL (which
generated a SMAP.c file).  The store(), restore(), and Delete() methods were then implemented using
embedded SQL in the SMAP.c file.  The SMAP.c file is renamed to SMAP.sqc and precompiled using
the DB2 precompiler (which will produce a *.c) file. NOTE: The makefile provided with this sample
invokes the DB2 precompiler and produces the appropriate file from the SMAP.SQC file.

Flow of Sample:
---------------

The overall flow of this POSSOM sample (maindb2.cpp) is as follows:

	1.	Instantiate and setup the PID for DB2.  Note that the DB2 PID requires a dbalias (name of
the database alias) and schema mapper name.

	2.	Instantiate and setup the phoneEntry object. We use the init_for_object_creation method
because PhoneEntry is a service-based object (created by os::server).  The os::server creates a
persistent object reference as part of the init_for_object_creation.  Therefore, the object
reference created by somdCreate must be released, because it is no longer valid.

	3.	Initialize the PhoneEntry state data by calling get() and set() methods for name, phone
number, and office location.

	4.	Store the PhoneEntry state data to the DB2 datastore.

	5.	Input new values for name, phone number, and office location by again calling the get() and
set() methods.  We setup new values in the PhoneEntry object so that we can demonstrate
when the restore() from the database is called, the original data is restored to the PhoneEntry
object from the DB2 datastore.

	6.	Restore the original data from the DB2 datastore.

	7.	Delete PhoneEntry data from the DB2 datastore and clean-up the environment.

Sample Constraints:
-------------------

All objects are created on the same server (MYSERVER).
