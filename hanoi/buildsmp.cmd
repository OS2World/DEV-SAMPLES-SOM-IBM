/* Command file to start a build for the sample                           */
/* buildsmp.exe accepts the following options:                            */
/* -c start a C compile                                                   */
/* -p start a C++ compile                                                 */
/* Either select -c or -p but not both.                                   */
/* The following options determine the compilers supported by the sample. */
/* Omit appropriate options when the compiler is not supported.           */
/* -i Enables selection of the IBM CSET Compiler from the Interface       */
/* -b Enables selection of the Borland Compiler from the Interface        */
/* -m Enables selection of the Metaware Compiler from the Interface       */
/* -w Enables selection of the Watcom Compiler from the Interface         */
/* -v Enables selection of the VisualAge Compiler from the Interface      */
'@echo off'
sombase = value('SOMBASE',,'OS2ENVIRONMENT')
otp_b   = value('OTP_B',,'OS2ENVIRONMENT')
if otp_b="" then do /* Normal install environment */
   bin = "bin"
   end
else do
   bin = "bin.os2"
   sombase = value('_OTP',,'OS2ENVIRONMENT')
   end
sombin = sombase'\'bin
/* C++ compile             *  */
sombin'\'buildsmp.exe '-p -i -b -m -w -v'/* C compile            *  */
/*sombin'\'buildsmp.exe '-c -i -b -m -w -v'*/
