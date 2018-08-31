# GSAT-Local-Search
The algorithm now can approximately achieve the level of the paper < A New Method for Solving Hard Satisfiability Problems> 
(Selman et al. 1992).
Compared to the paper, the algorithm now can solve problems in the same size with a similar number of tries, and a much 
smaller time consumption (may due to the huge difference of CPU performance, I am using a i7-8750H CPU).
The results are listed here (using the data set provided in the repo):

·50-215-250 data set
 1 3 2 5 1 2 2 2 2 2 12 1 5 2 1 3 4 2 1 3     
        tries            time consumption
Avg: 		2.8 times	       below 0.02s(including file IO and other process, so not that accurate in such a small-size data set)
Paper： 6.4 times		    0.4s

·70-301-350 data set
 51 0.108	  23 0.054		20 0.052		2 0.013		  55 0.124
 2 0.015		13 0.036 	  8 0.027 		4 0.017 		7 0.023
 32 0.075 	14 0.039	 	65 0.137 	  5 0.021 		3 0.016
 25 0.061 	15 0.040  	36 0.083 	  2 0.016 		3 0.016
          tries          time consumption
Avg: 		  19.25 times		 0.0485s
Paper：	 11.4 times 		0.9s

·150-645-1500 data set
 59 0.869	  12 0.186		14 0.218   	31 0.459 	  9 0.145
 29 0.427 	10 0.151	 	14 0.208  	15 0.224  	25 0.367
 19 0.287 	8 0.118 		94 1.385  	14 0.214  	31 0.463
 37 0.557 	1 0.019		  23 0.341  	19 0.290   	24 0.360
          tries         time consumption
Avg:		 	24.4 times	 	0.36575s
Paer：	   100.5 times	 45s

·200-862-2000 data set (with a maximum of 1000 tries) (-------means fail to solve within the 1000-try limit)
 45 1.188 	  477 12.751	  --------	  --------	  --------
 412 10.815 	---------	    --------	  442 11.640	186 4.973
 191 5.056 	  ---------	    11 0.293 	  --------  	762 20.111
 15 0.416 	  736 19.501  	130 3.429 	95 2.502  	739 19.606
          tries         time consumption
Avg:			240 times	  	8.6372s (only considering those success ones)
Paper：   248.5 times	 2.8min
fail rate: 35%
