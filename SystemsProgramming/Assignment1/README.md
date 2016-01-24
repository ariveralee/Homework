# Fork Example

This program starts an application that loops n times where each loop consists of writing 10 records of 120 character "sequences" to a file. These sequences are stored in an array and then are used for comparision by reading back from the file.

The second componeent to this program is a timer that starts the program and calls fork().
The idea of this is to see how long it takes for a program to start and if this is affected by calling multiple instances of the file. 
