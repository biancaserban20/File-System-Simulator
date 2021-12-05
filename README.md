# File-System-Simulator
C

Content
---------------


* Introduction
* Code


INTRODUCTION
------------------------


From my point of view, this project had a light - medium level of difficulty. The duration of solving the project was about 6-7 hours.
I enjoyed solving it because I was able to use my knowledge in C programming and use everything I learned in the 1st year related to simple chained lists and pointers, 
at the course Data Structures.

I think this topic has been helpful in understanding the pointers, and I'm glad I gained more information about it.


CODE
--------------------------------

1. Dir * init_dir (Dir * parent, char * name);

Auxiliary function, used for:
- allocate memory to the directory structure and the name of the directory
- to initialize the name (with name) and the pointers of the structure (with NULL, except for the pointer to the parent of the director)

2. File * init_file (Dir * parent, char * name);

Auxiliary function, used for:
- allocate memory to the file type structure and the file name
- initialize the name (with name) and pointers of the structure (with NULL, except for the pointer to the parent of the file)

3. void touch (Dir * parent, char * name);

Required function, used to create a file.
I have treated several cases, including:
- the parent directory contains subdirectories and one of them has the name name sent as a parameter.
Then the message "File already exists" is displayed and exits the function.
- the parent directory does not contain files. The init_file function is called to initialize the new file and the file is added to the file list of
its parent.
- the parent directory contains files. Check if a file with the name name already exists and, if so, after the message is displayed
"File already exists", exiting function. Otherwise, add the file to the end of the file list (using the init_file function first).

4. void mkdir (Dir * parent, char * name);

Required function, used to create a directory.
We have treated several cases, including:
- the parent directory contains files and one of them has the name name sent as a parameter.
Then the message "Directory already exists" is displayed and quits.
- the parent directory does not contain directories. The init_dir function is called to initialize the new directory and the directory is added to the list
parent's director.
- the parent directory contains directories. It is checked if there is already a directory named name and, if so, after the message is displayed
 "Directory already exists", quits. Otherwise, the directory is added to the end of the file list (using first
init_dir function).

5. void ls (Dir * parent);

Mandatory function, used to display parent directories and files. Browse the directory list and display each one
director. Analog for files.

6. void rm (Dir * parent, char * name);

Required function, used to delete the file with the name name from the parent directory.
We have treated several cases, including:
- parent has no files. Then the message "Could not find the file" will be displayed and it will exit.
- parent has files and: if the file to be deleted is the first one in the list, it is deleted; otherwise scroll through the file list until you find the file with
name name and delete.
- file not deleted. In this case I used an ok variable that takes the value 1 if the file is found and deleted, 0 otherwise.
The message "Could not find the file" is displayed if ok = 0.

7. void rm_allfiles (Dir * parent)

Auxiliary function, created for rmdir and stop functions. Delete all files in the parent directory.
If the parent does not have files, it exits the function. Otherwise, scroll through the list of files and delete each file.

8. rm_all (Dir * parent);

Auxiliary function, created for rmdir and stop functions. Delete all directories in the parent directory. This is a recursive function.
If the parent does not contain directories, all files in the parent are deleted (via the rm_allfiles function) and exited. Otherwise, it goes through
directory list and delete each, including its contents (through the functions rm_allfiles for files and rm_all for directories)

9. void rmdir (Dir * parent, char * name);

Required function, used to delete a directory from the parent directory.
We have treated several cases, including:
- parent has no directories. The message "Could not find the dir" is displayed and exits the function.
- parent has directories and: if the delete directory is first in the list, it is deleted, including its contents (via rm_allfiles and rm_all);
otherwise scroll through the directory list until you find the directory named name and delete it, including its contents
(via rm_allfiles and rm_all).
- The wiper diketet was not found. In this case I used an ok variable that takes the value 1 if the directory is found and deleted,
0 otherwise. The message "Could not find the dir" is displayed if ok = 0.

10. void cd (Dir ** target, char * name);

Mandatory function, change the current directory with the name directory.
We have treated several cases, including:
- name = "..", then the current directory is changed to the parent directory.
- there are no directories in the target directory. Then the message "No directories found!" Is displayed.
- there are directories in the target. If the searched directory is the first in the directory list, then it changes to that. Otherwise, scroll through the list
of directories until the searched one is found and changes with that one.
- directory not found. In this case I used an ok variable that takes the value 1 if the directory is found and changed,
0 otherwise. The message "No directories found!" Is displayed. if ok = 0.

11. void tree (Dir * target, int level);

Mandatory function, displays the hierarchy of files and directories in the target directory. It is a recursive function.
-Basic case: if the target does not have directories and files. Then he leaves office
-Otherwise, the directory list is scrolled and, depending on the level, white spaces are displayed. The directory name is displayed and the function is called
 If the directory contains files, scroll through the list and display the files, also with white space depending on the level. Separately, I treated the case
the target directory is even home and I displayed its files.

12. char * strdel (char * d, int n);

Auxiliary function, created for the pwd function. Delete n characters from a string d (from address d).

13. char * pwd (Dir * target);

We calculated the length of the path to know how much memory to allocate to the path variable. I declared a buffer sir
which will help me build the path. I deleted the characters from the path and buffer so there will not to be residues (characters in memory).
I traveled the "road" from the target to the home directory and, in the meantime, I built my path.

14. void stop (Dir * target);

Calling rm_allfiles to delete all files and rm_all to delete all directories in the target.

15. void mv (Dir * parent, char * oldname, char * newname)

Bonus. Renames the name of a file or directory and adds it to the end of the file / directory list.
We have treated several cases, including:
- the parent directory contains neither directories nor files. Then the message "File / Director not found" will be displayed and it will exit
I went through the directory list and the file list to see if there was already a directory / file named newname. In case
if so, the message "File / Director already exists" is displayed. Otherwise, in the same browsing, I look for any directory / file
 with the name oldname. If so, we have declared a type variable that takes the value 0 if there is a directory to replace and 1 if
there is a file. When browsing the directory list, if I found a directory to replace, dir_pointer will be equal to it.
* If type is 0, then rename dir_pointer. Here we will have several cases:
- if dir_pointer is the head of the directory list: if it is the only item in the list, then quit (it was only necessary
renaming it), otherwise (there are several directories in the list) the header of the list will be the second item and dir_pointer will be moved to the end
list.
- otherwise (the directory is not the first in the list), the directory list is scrolled and the directory is retained before dir_pointer (prev). If
dir_pointer is the last one in the list, it leaves the function (only its renaming was necessary). Otherwise, the pointers are restored and
it will move to the end of the list.
* If type is 1, then the file with the name oldname will be deleted and another with the name newname will be created, which will be added to the end of the list.
* If after these browsing, type does not have one of these 2 values, the message "File / Director not found" will be displayed.

16. int main ();

I created the directory structure named "home. I did the reading and based on the read command, I called the functions. In the end,
I released the memory allocated to the structure and strings.

17. Memory management

1. I have dynamically allocated memory for all strings and structures used.
2. I  have freed memory in all delete, pwd and stop functions. I also took care to free the memory for the structure and
the strings used in main.
