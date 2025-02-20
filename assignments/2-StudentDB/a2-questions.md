## Assignment 2 Questions

#### Directions
Please answer the following questions and submit in your repo for the second assignment.  Please keep the answers as short and concise as possible.

1. In this assignment I asked you provide an implementation for the `get_student(...)` function because I think it improves the overall design of the database application.   After you implemented your solution do you agree that externalizing `get_student(...)` into it's own function is a good design strategy?  Briefly describe why or why not.

I beleive that externalizing `get_student()` into its own function was a good design strategy. By making it into its own function, you can use the preconditions of the function to guarentee an output while also writing it to be as efficient as it can for its purpose. Above all, my code also becomes reusable accross all my other written functions. 

2. Another interesting aspect of the `get_student(...)` function is how its function prototype requires the caller to provide the storage for the `student_t` structure:

    ```c
    int get_student(int fd, int id, student_t *s);
    ```

    Notice that the last parameter is a pointer to storage **provided by the caller** to be used by this function to populate information about the desired student that is queried from the database file. This is a common convention (called pass-by-reference) in the `C` programming language. 

    In other programming languages an approach like the one shown below would be more idiomatic for creating a function like `get_student()` (specifically the storage is provided by the `get_student(...)` function itself):

    ```c
    //Lookup student from the database
    // IF FOUND: return pointer to student data
    // IF NOT FOUND: return NULL
    student_t *get_student(int fd, int id){
        student_t student;
        bool student_found = false;
        
        //code that looks for the student and if
        //found populates the student structure
        //The found_student variable will be set
        //to true if the student is in the database
        //or false otherwise.

        if (student_found)
            return &student;
        else
            return NULL;
    }
    ```
    Can you think of any reason why the above implementation would be a **very bad idea** using the C programming language?  Specifically, address why the above code introduces a subtle bug that could be hard to identify at runtime? 

**Potential Bug**: `student_t student;`
C requires memory allocation directly from the programmer at times and does not always manage, create, or send back memory unless programmed to do so, thus making the code above prone to memory allocation errors. This is due to the functions lack of specificity in instructions regadrding memory. Instead, memory should be handled outside of the function and refrenced (via pointer) in the `get_student()` function.

3. Another way the `get_student(...)` function could be implemented is as follows:

    ```c
    //Lookup student from the database
    // IF FOUND: return pointer to student data
    // IF NOT FOUND or memory allocation error: return NULL
    student_t *get_student(int fd, int id){
        student_t *pstudent;
        bool student_found = false;

        pstudent = malloc(sizeof(student_t));
        if (pstudent == NULL)
            return NULL;
        
        //code that looks for the student and if
        //found populates the student structure
        //The found_student variable will be set
        //to true if the student is in the database
        //or false otherwise.

        if (student_found){
            return pstudent;
        }
        else {
            free(pstudent);
            return NULL;
        }
    }
    ```
    In this implementation the storage for the student record is allocated on the heap using `malloc()` and passed back to the caller when the function returns. What do you think about this alternative implementation of `get_student(...)`?  Address in your answer why it work work, but also think about any potential problems it could cause.  
    
I think this implementation is better. By first allocating memory for the student pointer, checking if it was succesful, and returning your results accordingly, allows for better handling for memory. A potential problem however could be in the case that memory is allocated (thus `pstudent != NULL`), but not enough memory was allocated (`sizeof(pStudent) != sizeof(struct student_t)`). 

4. Lets take a look at how storage is managed for our simple database. Recall that all student records are stored on disk using the layout of the `student_t` structure (which has a size of 64 bytes).  Lets start with a fresh database by deleting the `student.db` file using the command `rm ./student.db`.  Now that we have an empty database lets add a few students and see what is happening under the covers.  Consider the following sequence of commands:

    ```bash
    > ./sdbsc -a 1 john doe 345
    > ls -l ./student.db
        -rw-r----- 1 bsm23 bsm23 128 Jan 17 10:01 ./student.db
    > du -h ./student.db
        4.0K    ./student.db
    > ./sdbsc -a 3 jane doe 390
    > ls -l ./student.db
        -rw-r----- 1 bsm23 bsm23 256 Jan 17 10:02 ./student.db
    > du -h ./student.db
        4.0K    ./student.db
    > ./sdbsc -a 63 jim doe 285 
    > du -h ./student.db
        4.0K    ./student.db
    > ./sdbsc -a 64 janet doe 310
    > du -h ./student.db
        8.0K    ./student.db
    > ls -l ./student.db
        -rw-r----- 1 bsm23 bsm23 4160 Jan 17 10:03 ./student.db
    ```

    For this question I am asking you to perform some online research to investigate why there is a difference between the size of the file reported by the `ls` command and the actual storage used on the disk reported by the `du` command.  Understanding why this happens by design is important since all good systems programmers need to understand things like how linux creates sparse files, and how linux physically stores data on disk using fixed block sizes.  Some good google searches to get you started: _"lseek syscall holes and sparse files"_, and _"linux file system blocks"_.  After you do some research please answer the following:

    - Please explain why the file size reported by the `ls` command was 128 bytes after adding student with ID=1, 256 after adding student with ID=3, and 4160 after adding the student with ID=64? 

**Adding ID 1**
When student 1 is added, it takes up 64 bytes of space. However, storing data at a particular index of a file is 0-based and so there is a 64 byte hole at index 0, thus making the total space an additional 64 bytes bigger (128 total).
**Adding ID 3**
When student 3 s added, 124 bytes of space is written into our current data base, 64 of which is student 3 data, while the other 64 is an empty student struct in place of `ID=2`. The empty student struct is necessary to preserve the database's structure of writing into the file via `index = ID * sizeof(struct student) `
**Adding ID 64**
By following the logic of our first two points:
\* Adding ID 64 $\to$64 bytes
\* An empty student struct for ID 4 to 63 (60 total) $\to 60 * 64=3840$ bytes
\* Prior Data (ID 1, 2 (empty), 3) $\to$192 bytes
\* Zero Based index (ID 0) $\to$ 64 bytes 
\* TOTAL 4160
- Why did the total storage used on the disk remain unchanged when we added the student with ID=1, ID=3, and ID=63, but increased from 4K to 8K when we added the student with ID=64? 

**How linux stores data**
While we understand how our data base, it is not the same as how linux writes data into files into storage. Linux will allocate space in storage in 4K chunks. In order to store data of ID 1-63, Linux only requires 1 chunk (4K total), but requires 2 chunks (8K total) to fit ID 64 since empty/non-empty ID records took up all 4K of space.


- Now lets add one more student with a large student ID number  and see what happens:

    ```bash
    > ./sdbsc -a 99999 big dude 205 
    > ls -l ./student.db
    -rw-r----- 1 bsm23 bsm23 6400000 Jan 17 10:28 ./student.db
    > du -h ./student.db
    12K     ./student.db
    ```
    We see from above adding a student with a very large student ID (ID=99999) increased the file size to 6400000 as shown by `ls` but the raw storage only increased to 12K as reported by `du`.  Can provide some insight into why this happened?

Adding ID 99999 = (ID 99999 + ID 0) * 64 bytes = (99999 + 1) * 64 bytes = 6,400 KiloBytes

Chunks needed to add ID 99999 = $\Big\lceil\text{File Size (kB) / 4,000 (kB)}\Big\rceil= \Big\lceil6,400/4,000\Big\rceil=2$

Inital Chunks for current DB = 1 chunk = 4kb raw space

Number of chunks after adding ID 99999 = 
$\qquad$Initial Chunks + Chunks needed to add ID 99999 = 1 + 2 = 3 chunks = 12kb space$
