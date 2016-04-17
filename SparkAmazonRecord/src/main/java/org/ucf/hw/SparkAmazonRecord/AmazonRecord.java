package org.ucf.hw.SparkAmazonRecord;
/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * 
 * review data format
 *+----------+-------+--------------------+-----------+--------------+
 *|      asin|overall|          reviewText| reviewTime|    reviewerID|
 *+----------+-------+--------------------+-----------+--------------+
 *|6301955250|    4.0|The plot and susp...| 08 9, 1997|A1R3P8MRFSN4X3|
 *|B000002KXA|    5.0|This remix and si...|08 10, 1997| AVM91SKZ9M58T|
 * 
 * root
 |-- asin: string (nullable = true)
 |-- overall: double (nullable = true)
 |-- reviewText: string (nullable = true)
 |-- reviewTime: string (nullable = true)
 |-- reviewerID: string (nullable = true)

 

 * limitations under the License.
 */

import scala.Tuple2;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.function.*;
import org.apache.spark.sql.DataFrame;
import org.apache.spark.sql.Row;
import org.apache.spark.sql.SQLContext;


public final class AmazonRecord {
 
  private static JavaSparkContext ctx;

public static void main(String[] args) throws Exception {

    SparkConf sparkConf = new SparkConf().setAppName("AmazonCount");
    ctx = new JavaSparkContext(sparkConf);
    SQLContext sqlContext = new org.apache.spark.sql.SQLContext(ctx);
    /*
     * Read review data from the local file
     * review data format
     *+----------+-------+--------------------+-----------+--------------+
     *|      asin|overall|          reviewText| reviewTime|    reviewerID|
     *+----------+-------+--------------------+-----------+--------------+
     *|6301955250|    4.0|The plot and susp...| 08 9, 1997|A1R3P8MRFSN4X3|
     *|B000002KXA|    5.0|This remix and si...|08 10, 1997| AVM91SKZ9M58T|
     * 
     * root
 		|-- asin: string (nullable = true)
 		|-- overall: double (nullable = true)
 		|-- reviewText: string (nullable = true)
 		|-- reviewTime: string (nullable = true)
 		|-- reviewerID: string (nullable = true)
     * */
    DataFrame review_df = sqlContext.read().json("/home/test/Amazon/review.data");
    /*
     * change the DataFrame format into JavaRDD<Row> and filter the length of reviewText is larger than 100
     * */
    @SuppressWarnings("serial")
	JavaRDD<Row> review_row = review_df.toJavaRDD().filter(new Function<Row,Boolean>(){
    	public Boolean call(Row s) throws Exception
    	{
    		String []data = s.getString(2).toLowerCase().replaceAll("[^a-z]"," ").split(" ");
    		if(data.length>100)
    			return true;
    		else
    			return false;
    	}
    });
    System.out.println("review_row number is:"+review_row.count());
    /*
     *JavaRDD Row(asin,overall,reviewText,reviewTime,reviedID) --> JavaPairRDD(overall,reviewText)
     * */
    @SuppressWarnings("serial")
	JavaPairRDD<Double,String> review_pair = review_row.mapToPair(new PairFunction<Row,Double,String>(){
    	public Tuple2<Double,String> call(Row s) throws Exception {
    		return new Tuple2<Double,String>(s.getDouble(1),s.getString(2));
    	}
    });
    /*Group the resutls by the key "overall"*/
   JavaPairRDD<Double, Iterable<String>> review_group = review_pair.groupByKey();
   
   /*save the result to local file system*/
   review_group.saveAsTextFile("/home/test/Amazon/review_group");
    
    /*
     * **************************************************************************************************
     * Question 2
     * **************************************************************************************************
     * */
   
   
   /*Read meta data from the local file
    * 
    * * meta data format
	*+----------+-----------+-----+--------------------+
	*|      asin| categories|price|               title|
	*+----------+-----------+-----+--------------------+
	*|6301955250|Movies & TV|  0.0|   Rain Killer [VHS]|
	*|B000002KXA|      Music| 9.21|           Rescue Me|
	*|6303935419|Movies & TV|18.69|Reefer Madness [VHS]|
	*+----------+-----------+-----+--------------------+
	*root
		|-- asin: string (nullable = true)
		|-- categories: string (nullable = true)
		|-- price: double (nullable = true)
		|-- title: string (nullable = true)
    * */
   DataFrame meta_df = sqlContext.read().json("/home/test/Amazon/meta.data");
   
   /*
    * change the DataFrame format into JavaRDD<Row>
    * */
    JavaRDD<Row> review_row_all = review_df.toJavaRDD();
    
    /*
     * JavaRDD<Row> (asin,overall,reviewText,reviewTime,reviedID) --> JavaPairRDD<String,String> (asin,reviewText)
     * */
    @SuppressWarnings("serial")
	JavaPairRDD<String,String> review_asin_rdd = review_row_all.mapToPair(new PairFunction<Row,String,String>(){
    	public Tuple2<String,String> call(Row s) throws Exception
    	{
    		return new Tuple2<String,String>(s.getString(0),s.getString(2));
    	}
    });
    
    
    /*
     * change the DataFrame format into JavaRDD<Row>
     * */
    JavaRDD<Row> meta_row_all = meta_df.toJavaRDD();
    /*
     * JavaRDD<Row> (asin,categories,price,title) --> JavaPairRDD<String,String> (asin,categories)
     * */
    @SuppressWarnings("serial")
	JavaPairRDD<String,String> meta_asin_rdd = meta_row_all.mapToPair(new PairFunction<Row,String,String>(){
    	public Tuple2<String,String> call(Row s) throws Exception
    	{
    		return new Tuple2<String,String>(s.getString(0),s.getString(1));
    	}
    });
    
    
    /*
     *(asin,reviewText)+(asin,categories) = (asin,<reviewText,categories>)
     * After Jion operation, select whose category is "Music"
     * */
    @SuppressWarnings("serial")
	JavaPairRDD<String,Tuple2<String,String>> asin_jion = review_asin_rdd.join(meta_asin_rdd).filter(
    		new Function<Tuple2<String,Tuple2<String,String>>,Boolean>(){
				@Override
				public Boolean call(Tuple2<String, Tuple2<String, String>> arg0) throws Exception {
					Tuple2<String, String> data = arg0._2();
					String cate = data._2();
					if(cate.equals("Music"))
						return true;
					else
						return false;
				}
				});
    /*
     * calculate the number of words of each review
     * <asin,<reviewText,Music>>  --> <asin,<the number of words of review,1>>
     * */
    @SuppressWarnings("serial")
	JavaPairRDD<String,Tuple2<Integer,Integer>> asin_join_number = asin_jion.mapToPair(
    		new PairFunction<Tuple2<String,Tuple2<String,String>>,String,Tuple2<Integer,Integer>>(){
    			public Tuple2<String,Tuple2<Integer,Integer>> call(Tuple2<String,Tuple2<String,String>> s){
    				Tuple2<String,String> review_count = s._2();
    				int leng = review_count._1().toLowerCase().replaceAll("[^a-z]"," ").split(" ").length;
    				Tuple2<Integer,Integer> number = new Tuple2<Integer,Integer>(leng,1);
    				Tuple2<String,Tuple2<Integer,Integer>> sd = new Tuple2<String,Tuple2<Integer,Integer>>(s._1(),number);
    				return sd;
    			}
    		});
    
    /*
     * calculate the total number of words for each product, and the number of product
     * */
    @SuppressWarnings("serial")
	JavaPairRDD<String,Tuple2<Integer,Integer>> join_reducebykey = asin_join_number.reduceByKey(
    		new Function2<Tuple2<Integer,Integer>,Tuple2<Integer,Integer>,Tuple2<Integer,Integer>>(){
    			public Tuple2<Integer,Integer> call(Tuple2<Integer,Integer> s1,Tuple2<Integer,Integer> s2){
    				Tuple2<Integer,Integer> sd = new Tuple2<Integer,Integer>(s1._1()+s2._1(),s1._2()+s2._2());
    				return sd;
    			}
    			
    		});
    /*
     * calculate the average words for each product
     * */
    @SuppressWarnings("serial")
	JavaPairRDD<String,Integer> average_words = join_reducebykey.mapToPair(
    		new PairFunction<Tuple2<String,Tuple2<Integer,Integer>>,String,Integer>(){
    			public Tuple2<String,Integer> call(Tuple2<String,Tuple2<Integer,Integer>> s){
    				Tuple2<Integer,Integer> tmp = s._2();
    				return new Tuple2<String,Integer>(s._1(),tmp._1()/tmp._2());
    			}
    			
    		});
    /*save the result to local file system*/
    average_words.saveAsTextFile("/home/test/Amazon/average_word/");
    ctx.stop();
  }
}
