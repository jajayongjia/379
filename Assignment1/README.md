CMPUT 379 assignment1 01/02/2018
By Yongjia Huang and Yuhan Ye

-----------------
Files included:
-----------------
assignment1.c	
Makefile
README.md

---------------------
General usage notes:
---------------------
-This C program is to periodically report the changes of a specified directory. 
-The report of the monitored directory can also be triggered by sending the process a SIGUSR1 Unix Signal.

--------------
How to run?
--------------
command: ./diffdir379 period path
where path is the absolute or relative path to the directory to monitor,and period is the reporting period in seconds.
Open another terminal to send the process a SIGUSR1 Unix Signal.
command: ps -aux | grep diffdir379 
to get the process id of the program and
command: kill -10 process id
to send SIGUSR1 Unix Signal.

-------------------
Division of labour:
-------------------
Yongjia Huang contributes to dealing with files and printing the output.
Yuhan Ye contributes to dealing with time,signal handling,Makefile and README.
We work together to test the program and debug.
The code was tested on VM(XubuntuCS.08.2015-06.2014 CMPUT174)

-------------------------------------------------------------------------------------------
Copyright 2018 Yongjia Huang,Yuhan Ye CMPUT379, University of Alberta, All Rights Reserved.
You may use distribut, or modify this code under terms and conditions of the ode of Student Behavior at University of Alberta
You may find a copy of the license in this project. Otherwise please contact jajayongjia@gmail.com



