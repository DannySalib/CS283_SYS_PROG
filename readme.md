## Course Repository for Systems Programming Class

#### Introduction
For this course I will be using git/github to share sample programs that we will discuss in class, and you will be using an enterprise git solution such as GitHub or GitLab for your own work and submitting programming assignments.

GitHub is an excellent platform for you to showcase your work.  You may use it for this class, but if you do, your repository needs to be publically accessable to make things easier.  If you want to keep your work private please use Drexel's GitLab service.

#### Creating a `git` repo on GitHub

1. Go to GitHub and create a new repository

2. Create a directory for your work files.

3. Make sure your linux platform has the `git` tool installed.  It will be there if you are using `tux`.  If you are using a local virtual machine you can install git using the directions here: https://git-scm.com/book/en/v2/Getting-Started-Installing-Git

4. Follow the directions provided by GitHub or GitLab to associate your local directory with your centralized repository.

#### Using GitHub/GitLab for your own Assignments

There will be a number of assignments in this course.  All assignments **must be part of the same git repo you created for this class**.  Each assignment will be placed in a directory on the **main** branch using a name that I will provide in the assignment directions.  For example `1-C_Warmup`.

For each assigment (in its associated directory):

1. If there is a written component to the assignment that must be placed in a `readme.md` file.  If you need help with writing using markdown format, GitHub has an excellent [Markdown Reference](https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax) to get you started.

2. All of your assignments should be fully automated with respect to building your code.  We will be using simple `makefiles` for this.  All `makefiles` should have a `make clean` option to clean up (delete) files created by the compiler.  The default `make` command should compile and link your program(s).  I will be demonstrating this in class.

3. All assignment should be built and tested using `gcc` on an Ubuntu LTS version (22.04, 24.04) Operating System.  No exceptions. If you create and test programs using Windows or MacOS, and they don't properly compile or run on Ubuntu Linux, they will not be accepted.  So, just do everything using Linux and you will have no issues.

4. Program source code will need to adhere to a general style we will be discussing in class.  While I will be fine with deviations from the style of code that I use, but you should be consistent and not adopt bad style practices that I will also be discussing in class (e.g., super long variable names, comments for comments sake, etc).  I also expect file names to be short and to the point, use proper extensions, and do not include special characters other than dashes or underscores.  Submissions that have file names that include things like spaces will not be graded.  

#### How to Git* Video Guide

See [Submitting Assignments](https://github.com/drexel-systems/sysprog-overview/blob/main/howto/SUBMIT_ASSIGNMENTS.md).