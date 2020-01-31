# CSC 412 – Operating Systems

## Fall 2019

```
Lectures Pastore Hall, Room 201 Tue. & Thurs., 2:00-3:15 PM
Lab Section 1 Woodward Hall, Room 318 Tue., 3:30-4:30 PM
Lab Section 2 Quinn Hall, Room 104 Thurs., 3:30-4:30 PM
Professor Jean-Yves Herv ́e Tyler Hall, Room 253
also: Tyler Hall, Room 104 (research lab)
Tel: 874 2701
email:jyh@cs.uri.edu
Office hours: Mon.: 2:00 – 4:00 pm
Wed.: 4:00 – 6:00 pm
or by appointment
```
```
Graduate TA: Chris McCooey Tyler Hall, Room 132
email:ccmccooey@uri.edu
Office hours: Wed.: 11:00 am – 1:00 pm
Fri.: 10:30 am – 1:30 pm
```
# The Course at a Glance

## Prerequisites

CSC 212, and students he student must be admitted to a degree-granting college.
If you barely passed CSC 212 or struggled through that course, consider taking it again, or gain
some programming experience, before taking CSC412, because you are going to have a hard time
with all the programming otherwise.

## Topics

General concepts underlying operating systems. Topics include process management, file systems,
concurrency, scheduling, memory management, information management, protection and security,
modeling and performance, networking and communication.

# Equipment and References

## Development environment

To make sure that we all operate with similar configurations for the programming assignments,
you will have to create a VirtualBox virtual machine running the latest version of the Ubuntu
distribution of Linux, even if Ubuntu is your standard working operating system. All subsequent
development in this environment will consist of shell scripting and C programming. I know that


some of you may use macOS or Windows for development. This is fine, but whatever you submit
mustbuild and run on Ubuntu. This is what the graders will use to grade your submissions. There
will benoexceptions to this rule.
Just to make it clear, I am going to repeat: I don’t care if your program/script combo runs fine
on macOS or native Linux. The testing/running environment is the latest version of Ubuntu on
Virtual Box. If we cannot reproduce your results, simply bring to us your virtual box on a thumb
drive^1 for us to look at.

## Course textbook

There is no textbook for this course. I will post online copies of the slides that I will show in class.
If you absolutely want to consult a textbook, used copies of the following textbooks would be fine:

- Abraham Silberschatz, Peter B. Galvin, and Greg GagneOperating System Concepts, Wiley.
    [6th edition or more recent].
- Andrew TanenbaumModern Operating System, Prentice Hall. [3rd edition or more recent]

General Unix/Linux references

There are a number of excellent tutorials out there. I link here a few of those:

- A classic tutorial developed at the University of Surrey (UK) and that many other schools
    have mirrored:
    [http://www.ee.surrey.ac.uk/Teaching/Unix/](http://www.ee.surrey.ac.uk/Teaching/Unix/)
- A short tutorial of basic commands:
    [http://math.mit.edu/services/help/new/unix.php](http://math.mit.edu/services/help/new/unix.php)
- The Berkeley tutorial:
    [http://people.ischool.berkeley.edu/](http://people.ischool.berkeley.edu/) ̃kevin/unix-tutorial
- Despite its humorous title, Unix is a Four Letter Word is actually a serious Unix tutorial
- On the other hand, The UNIX HATER’s Handbook is just a book-length rant against Unix,
    starting from the opening quote (“Two of the most famous products of Berkeley are LSD and
    Unix. I don’t think that this is a coincidence.”). To savor, by small sips, after a frustrating
    coding session.

Linux programming references

If you absolutely want to invest in a reference text, the one to get because it will be useful after this
class is the following:

- Michael Kerrisk,The Linux Programming Interface, No Starch Press, San Francisco, CA,
    2010 (ISBN-10: 1-59327-220-0).

(^1) This means that your development account your VirtualBox Linux install should have a different password than
the one you use for your computer.


Scripting reference

If you are new to shell scripting, you will soon learn that there exist several—slightly—different
competing scripting languages. For this course, we are going to impose theBashshell scripting
language, one of many scripting languages derived from the Bourne shell language. Apologies to
those among you with strong religious feelings on the matter of the “True scripting language,” but
we have a very limited amount of grader time and can’t afford to accommodate all preferences.
There are a number of good tutorials and references for the Bash scripting languages. I list here a
few that I found useful:

- To get started:
    [http://www.aboutlinux.info/2005/10/10-seconds-guide-to-bash-shell.](http://www.aboutlinux.info/2005/10/10-seconds-guide-to-bash-shell.)
    html
- A more complete tutorial:
    https://linuxconfig.org/bash-scripting-tutorial
- I like this tutorial the most:
    https://ryanstutorials.net/bash-scripting-tutorial/
- Two more “advanced” and complete guides:
    [http://tldp.org/LDP/abs/html/](http://tldp.org/LDP/abs/html/)
    [http://www.dartmouth.edu/%7Erc/classes/ksh/](http://www.dartmouth.edu/%7Erc/classes/ksh/)

# Grading

## “Long” examinations

There will be a midterm examination before Thanksgiving, probably on Thursday, November 21st
(unless I am forced to change the date), in our regular classroom, at regular course hours.

## Quizzes

There will be about 6-8 pop quizzes during the semester. They will almost always be unannounced,
not only because I am an evil person, but mostly because I would typically decide to have a quiz
shortly before class.

## Labs

There are lab sessions for this course. These will be hands-on and will go over practical issues of
C/C++ and bash programming. The purpose of these labs is to make sure that you are ready to
tackle the programming assignments by the time they are posted. There will be a pass/fail grade
for each lab, based on attendance and participation.


## Programming assignments

There will be eight scheduled programming assignments this semester. Each assignment will have
a C programming components and a bash script programming component.
The first three assignments will be short and fairly generic, not directly related to operating
systems. The purpose of their C programming component is to ensure that you are reasonably
competent with fundamental aspects of C such as pointer algebra, 1D and nD array manipulation,
bitwise operators, console and file input and output.
After that, we will have one assignment to serve as a C++ refresher (mostly STL stuff such as
vectors and lists) while we dive into the file system.
The remaining four assignments will be more complex and will deal with applied aspects of
operating systems. An assignment schedule is provided at the end of this syllabus.

Recommendations about the programming assignments:

1. All programming assignments are in C/C++ and bash;
2. You have to learn C and bash on your own. I will post sample code and links to tutorials,
    and I will answer specific questions, but the learning is your job.
3. When you program in C++, don’t feel obliged to use classes. I am a big fan of OOP and
    give it an important place in some of my other applied courses (e.g. Computer Graphics and
    Scientific Computing), but unless you are really comfortable with OOP and class design, the
    assignments of this course are not the most natural place to get practice with classes.
4. From observation, many of you (read: over 90%) write super long functions (hundreds of
    lines of code) that are impossible to debug. Force yourselves to break your code into small
    functions that are easy to test.
5. Start assignments early. Maybe you will be able to do the first two or three in a few hours,
    but I can guarantee you that this won’t be the case after that.
6. I check the forums and post answers to specific questions there. So, post questions, and
    check the questions posted by other students and the answers posted there. If you know the
    answer to a classmate’s question, don’t hesitate to post your answer.
7. Throughout the semester, I am going to post many code samples. I spend lots of time
    on these samples. They are heavily commented to attract your attention to different as-
    pects of the code; Sometimes they come in multiple versions, with one version highlighting
    changes/improvements over the previous one. I will not force you to look at my code sam-
    ples (you are free to use better ones you may find online), but I am going to assume that the
    material of any code sample that I post is known and understoodseven days after I post it.
    In particular, it means that this material is fair game for quizzes, exams, and obviously for
    assignments.
8. The last four assignments are meant to be challenging, so don’t complain that “we have
    never been taught how do do that!” I actually take this as a compliment. It’s about high time


```
that you move away from the programming equivalent of baby food in small pots: well-
contained, nicely pureed, and somewhat sweet. There is nothing wrong with baby food and
its programming equivalent: We all started that way. But eventually there comes a time when
you have to start cutting and chewing your food—and your programming—by yourself.
```
9. When working on an assignment, you can be in one of three states:

```
(a) You have no idea what to do/how to get started;
(b) You know and understand what you have to do but you are stuck on one subproblem
that you can’t solve or implement.
(c) You know what you have to do and how to do it, and you may or may not have the time
to do it (which brings us back to Point 2: Start early).
```
```
You don’t want to stay stuck long in State (a) because you are not doing any work (time
spent staring at a computer screen doesn’t count as work). Come to office hours (mine and
the TA’s) and talk with classmates (group sessions are great) if you don’t understand what
you’re supposed to do.
If you hit State (b), then check the forums to see if another student posted a question on this
problem. If not, then post the question and pat yourself on the back for being the first to hit
that point and knowing that your classmates will be grateful to you for posting the question.
```
10. Use online search wisely. I can pretty much guarantee that this semester it won’t help you out
    of State (a) above. Itmayhelp you out of State (b) if you know how to do a good query, with
    the right keywords. Sadly enough many of you don’t. If that is your case, then time learning
    how to select and combine search terms would be time well spent. The most complete
    reference/tutorial site I have found for this is athttp://www.googleguide.com.

## Final project

There will be a final project that will combine most of the things seen during the semester. You
will have 2 weeks to complete this project.

## The Mix

The following coefficients will be used to compute the final grade:

```
Programming Assignments: 50%
Labs 5%
Quizzes 10%
Midterm Exam 20%
Final Project 15%
```
The above mix is based on the assumption that we will have about 8 quizzes. If for some reason
that number was a lot lower, some of the quiz coefficient would get reversed to the assignments
coefficient. For example, if we end up having only 4 quizzes, then half of the quiz coefficient, 4%,
would be transferred to the assignment coefficient(54% instead of 50%).


# General URI Guidelines (annotated)

## Accommodations for special needs

Section 504 of the Rehabilitation act of 1973 and the Americans with Disabilities Act of 1990
require the University of Rhode Island to provide academic adjustments or the accommodations
for students with documented disabilities. The student with a disability shall be responsible for
self-identification to the Disability Services for Students in the Office of Student Life, provid-
ing appropriate documentation of disability, requesting accommodation in a timely manner, and
follow-through regarding accommodations requested. It is the student’s responsibility to make ar-
rangements for any special needs and the instructor’s responsibility to accommodate them with the
assistance of the Office of Disability Services for Students.

Any student with a documented disability is welcome to contact me as early in the semester as
possible so that we may arrange reasonable accommodations. As part of this process, please be in
touch with Disability Services for Students Office at 330 Memorial Union, 401-874-2098.

## Academic honesty

All submitted work must be your own. If you consult other sources (class readings, articles or
books from the library, articles available through internet databases, or websites) these MUST be
properly documented, or you will be charged with plagiarism and will receive an F for the paper.
In some cases, this may result in a failure of the course as well. In addition, the charge of academic
dishonesty will go on your record in the Office of Student Life. If you have any doubt about what
constitutes plagiarism, visit the following website:

```
http://gervaseprograms.georgetown.edu/hc/plagiarism.html,
```
the URI Student Handbook, and University Manual sections on Plagiarism and Cheating at

```
http://www.uri.edu/facsen/8.20-8.27.html
```
Any good writer’s handbook as well as reputable online resources will offer help on matters of
plagiarism and instruct you on how to acknowledge source material. If you need more help un-
derstanding when to cite something or how to indicate your references, PLEASE ASK. academic
enhancement center The work in this course is complex and intensive. To do the best you can,
it is a good idea to visit the Academic Enhancement Center (AEC) in Roosevelt Hall. The AEC
offers a comfortable environment in which to study alone or together, with or without a tutor. AEC
tutors can answer questions, clarify concepts, check understanding, and help you to study. You
can make an appointment or walk during office hours – Monday through Thursday from 9 am.
to 9 pm, Friday from 9 am to 1 pm, and Sunday from 4 pm. to 8 pm. For a complete schedule
For a complete schedule - including when tutors are available specifically for this class - go to
[http://www.uri.edu/aec,](http://www.uri.edu/aec,) call (401) 874-2367, or stop by the fourth floor in Roosevelt Hall.


Unless explicitly stated otherwise, all work should be done individually. Any evidence of
cheating may result in expulsion from the class with a failing grade and will be brought to the
attention of the Dean for disciplinary action.
You are strongly encouraged to discuss the assignments with other students, and try to figure
them out together, but when comes the time of writing a report or developing code, you are ex-
pected to do it by yourself (or with other students from your group when the assignment explicitly
mentions groups of 2 to 3 students).
Copying software from a book or from a web site without identifying the original author(s) of
the work is plagiarism, which is a form of cheating.

## The Writing Center

The Writing Center is for “all writers, all disciplines, at all levels, and all stages of writing.” If an
instructor suggests that you go to the Writing Center, it is not a punishment, and does mean that
you are a terrible writer. It means the instructor wants you to receive more individualized attention
to your writing than s/he is able to provide, given the constraints of the class. It will only improve
your grade. If possible, call ahead for an appointment (874-4690). Drop-in tutorials are often
available. You may make repeat appointments, requesting the same tutor each time if you wish.
See their Web Page:

```
http://www.uri.edu/artsci/writing/center/index.shtml
```
for tips on how to make the best of your Writing Center visit.

## Standards of behavior

Students are responsible for being familiar with and adhering to the published “Community Stan-
dards of Behavior: University Policies and Regulations” which can be accessed in the University
Student Handbook. If you must come in late, please do not disrupt the class. Please turn off all
cell phones, pagers, or any electronic devices.

## Religious holidays

It is the policy of the University of Rhode Island to accord students, on an individual basis, the
opportunity to observe their traditional religious holidays. Students desiring to observe a holiday
of special importance must provide written notification to each instructor.

## Illness Due to Flu

The nation is experiencing widespread influenza-like illness. If any of us develop flu-like symp-
toms, we are being advised to stay home until the fever has subsided for 24 hours. So, if you exhibit
such symptoms, please do not come to class. Notify me at 874-2701 orjyh@cs.uri.eduof
your status, and we will communicate through the medium we have established for the class. We
will work together to ensure that course instruction and work is completed for the semester.


The Centers for Disease Control and Prevention have posted simple methods to avoid transmis-
sion of illness. These include: covering your mouth and nose with tissue when coughing or sneez-
ing; frequent washing or sanitizing your hands; avoiding touching your eyes, nose, and mouth; and
staying home when you are sick. For more information please viewhttp://www.cdc.gov/
fluorhttp://flu.gov. URI Health Services web page,http://www.health.uri.
edu, will carry advice and local updates.

# Additional Rules of Conduct

## Late Submissions

We will not allow any late submissions unless you have a valid, documented excuse for a late sub-
mission.

Typically submissions will be closed at 11:55pm. Note that this is not 11:59pm but 11:55pm
(five five), because Sakai only works by granulations of 5 minutes. Don’t wait until the last minute
to post your assignment.

## Laboratory use

Any abuse of computer or software equipment will be brought to the attention of the appropriate
authority for disciplinary action. Software piracy (the use, appropriation, or storage of illegal
copies of software) is a form of abuse of the equipment and will be treated accordingly.

## Attendance

Lectures
Class attendance is not mandatory, although I strongly suggest that you make efforts to attend
all classes. We will spend a significant part of the time in classes discussing about examples of
programs, possible errors, bugs, etc. Don’t spend too much time taking notes during classes; rather
make efforts to understand on-line what is going on. It will only get easier as we advance in the
semester.
Please note that if you miss an unannounced quiz due to absence in class, you will only be
allowed to take a makeup quiz if you told me in advance that you would be absent or you present
documents justifying your absence.

Labs
Attendance (and participation)ismandatory. You will need to have access to a computer to do
work in labs, but you can work in teams of two, so if you can’t bring a laptop, make sure to
identify a classmate who can, and whom you can team up with. You will receive a pass/fail grade
for each lab based on presence and participation.


## Incomplete grade

In the past I have been much too lenient in giving an Incomplete grade at the end of the semester.
This almost always eventually turns into a headache for me, so starting this semester, I am going
to apply strictly the University guidelines:

A student shall receive a report of Incomplete in any course in which the course work has been
passing up until the time of a documented precipitating incident or condition, but has not been
completed because of illness or another reason which in the opinion of the instructor justifies the
report. (Section 8.53.20 University Manual).

Note that overload from other courses does not fulfill the University policy conditions. Neither
does a hard drive/laptop failure (we have a computer laboratory for your use).

I am going to post regularly grades on the course’s web site, together with indicative levels for
A, B, C, D letter grades. If your grade average is below the D level, then you should drop before
the limit date, or petition for a late drop if that date has passed. You simply cannot dump your
problems on my lap at the end of the semester.

# Tentative Schedule

## Lectures

1. Introduction (Week 1).
2. OS Structures (Week 2).
3. Memory Management (Week 3, Week 4.1).
4. File System (Week 4.2, Week 5.1).
5. Processes (Week 5.2, Week 6, Week 7.1).
6. Interprocess Communication I (Week 7.2, Week 8.1).
7. Threads (Week 8.2, Week 9.1).
8. Synchronization (Week 9.2, Week 10).
9. Deadlock (Week 11).
10. Interprocess Communication II (Week 13.1).
11. Virtual Memory (Week 13.2, Week 14.1)
12. Interprocess Communication III (Week 14.2, Week 15.1).


## Assignments

Again, the dates are not set in stone, as I may have missed a “Wednesday is really a Tuesday,” and
definitely cannot predict snow days, as much as I wish I could.

Assignment 1 [coefficient 1]: posted 09/09, due 09/17. C programming: Elementary C pro-
gramming; Passing of arguments to functions and console input/output
bash scripting: Elementary scripting: variables, loops, passing of arguments to functions, and
console input/output.

Assignment 2 [coefficient 1]: posted 09/16, due 09/24. C programming: array allocation and
pointer algebra. String manipulation.
bash scripting: String manipulation; passing arguments to a C executable.

Assignment 3 [coefficient 1]: posted 09/23, due 10/01. C File input and output; directory
operation.
bash scripting: file and directory management.

Assignment 4 [coefficient 1]: posted 9/30, due 10/08. C++ (lists and vectors) and rocess cre-
ation (forkandwaitpid).

Assignment 5 [coefficient 2]: posted 10/07, due 10/20. Process creation (forkandwaitpid).

Assignment 6 [coefficient 2]: posted 10/21, due 11/03. Interprocess communication 1 (lock
files, pipes, shared memory).

Assignment 7 [coefficient 2]: posted 11/04, due 11/17. Threading.

Assignment 8 [coefficient 2]: posted 11/18, due 12/01. Thread synchronization and interprocess
communication 2.

Final Project: posted 12/02, due 12/17.


