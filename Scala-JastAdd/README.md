Scala JastADD Description
========================
2016-04-02
Bingbing Rao
Robin.Rao@knights.ucf.edu

This is an implementation of Scala program language based on JastAdd tools.

Directories and files:

- **AST:** Generated directory with Java code
- **spec:** JastAdd and parser specification files
- **tests:** JUnit tests
- **tools:** The tools used: jastadd, junit,and parsing tools (jflex, beaver, JastAddParser).
- **build.xml:** The ant build-file

To run the examples, you need the following programs installed:

- ant
- javac
- java

The build file supports the following targets:

- **ant gen:** generates the AST directory with Java code
- **ant build:** compiles all Java code in this directory
- **ant test:** runs the JUnit test cases
- **ant doc:** generates javadoc
- **ant genclean:** removes all generated files (the AST directory and all class files).

Conclusion and Results

In this homework, I try to use JastAdd tools to do some static program analysis. I have done the following task:
1. Scan the source code and generate tokens as the input of Parser;
2. User Beaver parse the source code and do some basic variable type check,after that generate AST; 
3. Define a series rule to represent the type of expressions.
4. Transverse the AST and collect the variables information which including the type information;
5. According to the section 2.1.1 of textbook[5], I have define functions to represent initial and finals labels; 
6. Define inFlows and outFlows to represent the flows and reverse-flows information a program; 
7. In my program, I use blockCell interface to mask a basic block info;
8. In my program, I have finished Reaching definition analysis of a Scala program. However there is a small bug about while
   statement. 


Future work:
1. Fix out the bug about while statement in a RD analysis;
2. Check whether or not the type of the formal parameter of a function is val; 
3. Check the return type of a function; 
4. Implement "AE","LV","VB" data flow analysis; 

Some notes about the parser:
- For parsing we use a .jflex file to specify tokens and a .parser file to specify the parsing and AST building.
- The .parser file is run through JastAddParser which is a preprocessor to beaver. The generated jflex and beaver files are placed in the generated AST directory.

Reference 
[1]: http://link.springer.com/chapter/10.1007%2F978-3-642-18023-1_
[2]: http://beaver.sourceforge.net/spec.htm
[3]: http://jastadd.org/web
[4]: http://jflex.de
[5]: Principles of Program Analysis, section 2.1.1. Nielsen, Nielsen, and Hankin
