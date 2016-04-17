In this assignment, I use Java to implement Gale Shareply algorithm, use Maven tool to build this project. 


**************************************
How to run code:
**************************************
1. Enter in the source code file:

[root@Spark Assignment]# cd algorithm-a1/
[root@Spark algorithm-a1]# ls
pom.xml  Readme.txt  src
[root@Spark algorithm-a1]# 

2. execute maven command to compile and run the source code:

[root@Spark algorithm-a1]# mvn test
Java HotSpot(TM) 64-Bit Server VM warning: ignoring option PermSize=512m; support was removed in 8.0
[INFO] Scanning for projects...
[INFO]                                                                         
[INFO] ------------------------------------------------------------------------
[INFO] Building algorithm-a1 0.0.1-SNAPSHOT
[INFO] ------------------------------------------------------------------------
[INFO] 
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ algorithm-a1 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] skip non existing resourceDirectory /mnt/Java/Assignment/algorithm-a1/src/main/resources
[INFO] 
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ algorithm-a1 ---
[INFO] Changes detected - recompiling the module!
[INFO] Compiling 1 source file to /mnt/Java/Assignment/algorithm-a1/target/classes
[INFO] 
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ algorithm-a1 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] skip non existing resourceDirectory /mnt/Java/Assignment/algorithm-a1/src/test/resources
[INFO] 
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ algorithm-a1 ---
[INFO] Changes detected - recompiling the module!
[INFO] Compiling 1 source file to /mnt/Java/Assignment/algorithm-a1/target/test-classes
[INFO] 
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ algorithm-a1 ---
[INFO] Surefire report directory: /mnt/Java/Assignment/algorithm-a1/target/surefire-reports

-------------------------------------------------------
 T E S T S
-------------------------------------------------------
Running org.ucf.edu.algorithm_a1.AppTest
Tests run: 1, Failures: 0, Errors: 0, Skipped: 0, Time elapsed: 0.011 sec

Results :

Tests run: 1, Failures: 0, Errors: 0, Skipped: 0

[INFO] 
[INFO] >>> exec-maven-plugin:1.1.1:java (default) > validate @ algorithm-a1 >>>
[INFO] 
[INFO] <<< exec-maven-plugin:1.1.1:java (default) < validate @ algorithm-a1 <<<
[INFO] 
[INFO] --- exec-maven-plugin:1.1.1:java (default) @ algorithm-a1 ---
Gale Shapley Marriage Algorithm

Couples are:
M5:W1
M4:W2
M2:W3
M3:W4
M1:W5
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time: 4.667 s
[INFO] Finished at: 2016-01-17T13:24:16-05:00
[INFO] Final Memory: 17M/495M
[INFO] ------------------------------------------------------------------------
[root@Spark algorithm-a1]# 

3. verfiy the result
