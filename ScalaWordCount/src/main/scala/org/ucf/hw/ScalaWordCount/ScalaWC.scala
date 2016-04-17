package org.ucf.hw.ScalaWordCount
/**
 * @author ${Bingbing.Rao}
 */
import org.apache.spark.SparkContext
import org.apache.spark.SparkConf
object ScalaWC {
  def main(args : Array[String]) 
  {
    val logFile = "/usr/local/src/spark-1.5.1-bin-hadoop2.6/README.md";
    val conf = new SparkConf().setAppName("Simple Application");
    val sc = new SparkContext(conf);
    val logData = sc.textFile(logFile,2).cache();
    val numAs = logData.filter(line => line.contains("a")).count();
    val numBs = logData.filter(line => line.contains("b")).count();
    println("Lines with a: %s, Lines with b: %s".format(numAs, numBs));
  }
}