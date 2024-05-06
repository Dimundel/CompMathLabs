import math
import numpy as np
import matplotlib.pyplot as plt

def func(t, x, y):
    return np.sin(np.pi*x)*np.sin(np.pi*y)*np.exp(-2*np.pi*np.pi*t)

def read_and_process_file(filename):
    with open(filename, 'r') as file:
        content = file.read()
    
    # Split the content by two newlines to get segments
    segments = content.split('\n\n')
    
    # List to hold the last elements of each row
    last_elements = []

    for segment in segments:
        lines = segment.strip().split('\n')
        if not lines:
            continue
        
        # Skip the first line of each segment
        for line in lines[1:]:
            # Assume rows are space-separated, adjust the split as necessary
            parts = line.split()
            if parts and float(parts[1]) == 0.2:  # Check if the line is not empty
                last_elements.append(float(parts[-1]))
            
    return last_elements

filename = 'data.txt'
last_elements = np.array(read_and_process_file(filename))

NX = 71
NY = 71

results = []

i = 0
for t in np.arange(0.000, 0.5005, 0.005):
    for x in np.linspace(0, 1, NX):
        results.append(func(t, x, 0.2))
        
res = np.array(results)

print("NX = " + str(NX) + ", NY = " + str(NY))
print(np.max(np.abs(res - last_elements)))

x = np.log(np.array([1/10, 1/20, 1/30, 1/50, 1/70]))
y = np.log(np.array([0.11707808455942581, 0.07705182890814954, 0.03695850789439503, 0.01979382890814954, 0.01028282890814955]))

plt.scatter(x, y)
plt.grid()
plt.show()

print(np.polyfit(x, y, 1))