# Python Program to count prime Numbers
import time
total = 0
lower = 2
upper = 131072

start_time = time.time();

for i in range(lower,upper):
	isprime = 1
        for j in range(2,i):
               if (i % j == 0):
                  isprime = 0
           	  break
          
       
	total+=isprime
elapsed_time = time.time() - start_time

print("Number of Prime Numbers = ",total)
print("Execution Time = ", elapsed_time)



