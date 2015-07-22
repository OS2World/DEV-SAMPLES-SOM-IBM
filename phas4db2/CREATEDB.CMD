/* 3/11/96 13:48:08                                                                    */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*   25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996*/
/*  All Rights Reserved                                                       */
/*  Licensed Materials - Property of IBM                                      */
/*  US Government Users Restricted Rights - Use, duplication or               */
/*  disclosure restricted by GSA ADP Schedule Contract with IBM Corp.         */

/*                                                                            */
/*                                                                            */
/*                                                                            */
/* DISCLAIMER OF WARRANTIES.                                                  */
/* The following [enclosed] code is sample code created by IBM                */
/* Corporation. This sample code is not part of any standard or IBM           */
/* product and is provided to you solely for the purpose of assisting         */
/* you in the development of your applications.  The code is provided         */
/* "AS IS". IBM MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT        */
/* NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS       */
/* FOR A PARTICULAR PURPOSE, REGARDING THE FUNCTION OR PERFORMANCE OF         */
/* THIS CODE.  IBM shall not be liable for any damages arising out of         */
/* your use of the sample code, even if they have been advised of the         */
/* possibility of such damages.                                               */
/*                                                                            */
/* DISTRIBUTION.                                                              */
/* This sample code can be freely distributed, copied, altered, and           */
/* incorporated into other software, provided that it bears the above         */
/* Copyright notice and DISCLAIMER intact.                                    */

/* REXX Procedure                                                             */
/*                                                                            */
/* This Procedure creates the Database and Tables for the multi framework     */
/* sample suite.                                                              */
/*                                                                            */
/* Function: - Create DB 'CALENDAR' on Drive D:                               */
/*           - CREATE:                                                        */
/*             TABLE 'BOOKS' .............. Appointment books                 */
/*             TABLE 'APPOINTMENTS'........ Appointments (meetings &          */
/*                                          conference calls)                 */
/*                                                                            */

default_dbname = 'CALENDAR'
default_dbdrive = 'D'

parse upper arg dbname
if dbname='' then dbname=default_dbname

/* ---------------------------------- */
/* Establish SQL <-> REXX Environment */
/* ---------------------------------- */

if rxfuncquery('SQLDBS')<>0 then
  rcy = rxfuncadd('SQLDBS', 'SQLAR', 'SQLDBS');

if rxfuncquery('SQLEXEC')<>0 then
  rcy = rxfuncadd('SQLEXEC', 'SQLAR', 'SQLEXEC');

SAY '--------------------------------------------------------------'
SAY 'REXX Program: CREATEDB.CMD                                    '
SAY '                                                              '
SAY 'This REXX-Program will stop DB2 and update the DB2            '
SAY 'configuration to add the SOMTRXA TP_MON_NAME.  It will then   '
SAY 'CREATE a local Database on drive D: or on the drive that you  '
SAY 'specify and CREATEs the following tables:                     '
SAY '                                                              '
SAY ''BOOKS' .............. Table to contain the appointment books '
SAY ''APPOINTMENTS' ....... Table to contain the appointments      '
SAY '--------------------------------------------------------------'
SAY '                                                              '
SAY '                                                              '
SAY '                                                              '
SAY 'IMPORTANT:  Do you want this program to update the DB2        '
SAY '            configuration?  If you answer "Y", this program   '
SAY '            will issue a DB2STOP command.  Are you sure that  '
SAY '            you want DB2 to be stopped if it is running? Enter'
SAY '            "Y" to update the configuraion, anything else to  '
SAY '            stop.  If you have previously responded "Y" then  '
SAY '            you should respond "N".  If you have not updated  '
SAY '            the DB2 configuration before you run the sample,  '
SAY '            the sample will fail.                             '
SAY '                                                              '


PULL stopdb
IF   stopdb = "Y" then  SAY  'Stopping DB2 to add the SOMTRXA TP_MON_NAME'
IF   stopdb = "Y" then 'DB2STOP'
IF   stopdb = "Y" then 'DB2 UPDATE DATABASE MANAGER CONFIGURATION USING TP_MON_NAME SOMTRXA'


SAY 'Do you want to build the required database, tables, and       '
SAY 'continue to build the sample?  Enter a "Y" if you do.         '

PULL startpg
IF   startpg \= "Y" THEN EXIT

SAY 'Enter drive on which to create the database (default is D:): '

PULL dbdrive
IF   dbdrive = '' then dbdrive=default_dbdrive

/* ------------------------------------------------------- */
/* Start DBM, create database TEST and connect to DB       */
/* ------------------------------------------------------- */

SAY 'creating database 'dbname' on drive 'dbdrive
SAY 'and connecting to the database              '
'DB2START'
'DB2 DROP DATABASE ' || dbname
'DB2 CREATE DATABASE ' dbname ' ON ' dbdrive

CALL SQLEXEC 'CONNECT RESET';
CALL SQLEXEC 'CONNECT TO ' dbname;

if SQLCA.SQLCODE<>0 then do
  SAY 'SQLCODE=' SQLCA.SQLCODE;
  SAY 'SQLMSG=' SQLMSG;
  SAY 'connect to database ' dbname 'failed.'
  return 50;
end;


/* ------------------------------------------------------- */
/* Create table APPOINTMENTS                               */
/* ------------------------------------------------------- */
s1 = 'DROP TABLE APPOINTMENTS' ;
CALL SQLEXEC 'EXECUTE IMMEDIATE :s1';

CALL SQLEXEC 'COMMIT';

s1 = 'CREATE TABLE APPOINTMENTS                          ',
     '      (                                            ',
     '      APPOINTMENTNAME                  CHAR(13),   ',
     '      YEAR                             SMALLINT,   ',
     '      MONTH                            SMALLINT,   ',
     '      DAY                              SMALLINT,   ',
     '      START                             INTEGER,   ',
     '      END                               INTEGER,   ',
     '      SUBJECT                     VARCHAR(1000),   ',
     '      APPTID                            INTEGER,   ',
     '      LOCATION_PHONE               VARCHAR(100),   ',
     '      NUMBEROFUSERS                     INTEGER    ',
     '      )                                            ';

CALL SQLEXEC 'EXECUTE IMMEDIATE :s1';

if SQLCA.SQLCODE<>0 then do
  say 'SQLCODE=' SQLCA.SQLCODE;
  say 'SQLMSG=' SQLMSG;
  return 50;
end;

s1 = 'GRANT ALL ON TABLE APPOINTMENTS TO PUBLIC' ;

CALL SQLEXEC 'EXECUTE IMMEDIATE :s1';

if SQLCA.SQLCODE<>0 then do
  say 'SQLCODE=' SQLCA.SQLCODE;
  say 'SQLMSG=' SQLMSG;
  return 50;
end;

CALL SQLEXEC 'COMMIT';

SAY 'Table APPOINTMENTS successfully created ....'
/* ------------------------------------------------------- */
/* Create table BOOKS                                      */
/* ------------------------------------------------------- */
s1 = 'DROP TABLE BOOKS' ;
CALL SQLEXEC 'EXECUTE IMMEDIATE :s1';

CALL SQLEXEC 'COMMIT';

s1 = 'CREATE TABLE BOOKS                         ',
     '      (                                            ',
     '      BOOKID                           CHAR(25),   ',
     '      MAXIMUM                          INTEGER,    ',
     '      BOOK                             BLOB(25K)   ',
     '      )                                            ';

CALL SQLEXEC 'EXECUTE IMMEDIATE :s1';

if SQLCA.SQLCODE<>0 then do
  say 'SQLCODE=' SQLCA.SQLCODE;
  say 'SQLMSG=' SQLMSG;
  return 50;
end;

s1 = 'GRANT ALL ON TABLE BOOKS TO PUBLIC' ;

CALL SQLEXEC 'EXECUTE IMMEDIATE :s1';

if SQLCA.SQLCODE<>0 then do
  say 'SQLCODE=' SQLCA.SQLCODE;
  say 'SQLMSG=' SQLMSG;
  return 50;
end;

CALL SQLEXEC 'COMMIT';
CALL SQLEXEC 'CONNECT RESET';

SAY 'Table BOOKS successfully created ....'
RETURN 0 ;
