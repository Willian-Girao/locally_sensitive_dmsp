# Experimental Methods Results
Best results amongst the methods are shown in bold (both path length and # of messages exchanged). The shapes (hull and alpha) are calculated, at each node the mule passes through, using {u} U N(u) (called here **N***): N* <span>&#x2287;</span> **alpha-shape**  <span>&#x2287;</span> **convex-hull**. Thus, it is expected that the method based on the **convex-hull** of **N*** will have **solutions either as good as or worse than the method based on the greedy approach** (and will send less messages), while the method based on the  **<span>&#x3B1;</span>-shape** of **N*** will have **solutions either equal or better than the ones found by the convex-hull, but at most as good as the greedy approach** (with less messages than the greedy one, and a fewer more than the convex-hull one). 

The experimental results listed bellow verifies this expected behavior. The **<span>&#x3B1;</span>** utilized to calculate the <span>&#x3B1;</span>-shape of N* equals the **diameter** of the node's communication range (same for all nodes within the same graph).

## Greedy

**Instance Name**  **Mule Steps**  **Messages Sent**

kroD100  			 6        		   11084

rat195 		   	 4       		    42990

team2_201    	10      		   30522

team3_301    	74      		   21446

lin318            	  8      		    94218

rd400             	14     		    112638

pcb442          	12     		    169902

team6_501          6     		    312228

dsj1000				X     		    X

bonus1000  		X     		    X

## Convex-Hull

**Instance Name**  **Mule Steps**  **Messages Sent**

kroD100  			 6       		   10518

rat195 		   	  4       		    42064

team2_201    	 12      		   29002

team3_301    	 76      		   19240

lin318            	   8      		    91670

rd400             	 14     		   108372

pcb442          	 12     		   164752

team6_501         6     		    307380

dsj1000			  X     		    X

bonus1000  	  X     		    X

## <span>&#x3B1;</span>-Shape

**Instance Name**  **Mule Steps**  **Messages Sent**

kroD100  			 6        		   10634

rat195 		   	  4       		     42206

team2_201    	 10      		     30052

team3_301    	 76      		    19552

lin318            	   8      		     93706

rd400             	 14     		     109382

pcb442          	 12     		     165554

team6_501          6     		      308322

dsj1000				X     		    X

bonus1000  		X     		    X

# Variations Results

Results of experiments to validate possible neighbor selection based variations related to where the points are being selected from (depending on the shape). Best results amongst the methods are shown in bold (both path length and # of messages exchanged).

## <span>&#x3B1;</span>-Shape (<span>&#x3B1;</span> - radius)

**Instance Name**  **Mule Steps**  **Messages Sent**

kroD100  			 6        		   10940

rat195 		   	  4       		     42226

team2_201    	 **10**      		     30438

team3_301    	 76      		    20614

lin318            	   8      		     94106

rd400             	 16     		     111930

pcb442          	 14     		     166630

team6_501          6     		      308644

dsj1000				X     		    X

bonus1000  		X     		    X

## <span>&#x3B1;</span>-Shape (<span>&#x3B1;</span> - auto optimal)

**Instance Name**  **Mule Steps**  **Messages Sent**

kroD100  			 6        		   10546

rat195 		   	  4       		     42120

team2_201    	 12      		     29052

team3_301    	 76      		    19402

lin318            	   8      		     92340

rd400             	 14     		     108816

pcb442          	 12     		     165196

team6_501          X     		    X

dsj1000				X     		    X

bonus1000  		X     		    X

