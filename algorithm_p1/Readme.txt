1. Preliminary

In this programming assignment, I use Maven tool to construct my project, 
So in order to run my program, you need to install Maven tool and java JDK.
For now the information of Software version is followed:

Java HotSpot(TM) 64-Bit Server VM warning: ignoring option PermSize=512m; support was removed in 8.0
Apache Maven 3.3.3 (7994120775791599e205a5524ec3e0dfe41d4a06; 2015-04-22T07:57:37-04:00)
Maven home: /usr/local/src/apache-maven-3.3.3
Java version: 1.8.0_65, vendor: Oracle Corporation
Java home: /usr/local/src/jdk1.8.0_65/jre
Default locale: en_US, platform encoding: UTF-8
OS name: "linux", version: "2.6.32-573.18.1.el6.x86_64", arch: "amd64", family: "unix"


*** In you Operation System, you should set the PATH environment to make mvn and java command can work without any prefix.






2. How to Run the Algorithms

2.1 Use ShellScript to run program
* HeapSort Algorithm
  # sh HeapSort.sh
  
* Dijkstra Algorithm
  # sh Dijkstra.sh
  
* Kruskal  Algorithm
  # sh Kruskal.sh
  
2.2 Use Maven tool to run Junit Test
  #mvn compile
  #mvn test    
  
  