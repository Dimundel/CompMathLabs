# Set output format to animated GIF
set terminal gif animate delay 2
set output 'energy.gif'

# Set up your plot
set xrange [-10:10] # Adjust these values based on your data
# set yrange [GPVAL_Y_MIN:GPVAL_Y_MAX] # Adjust these values based on your data
set style data lines # Choose your preferred plot style

# Count the number of blocks (datasets) in the file
stats 'data.txt' nooutput
blocks = STATS_blocks

time_from_comment(index) = system(sprintf("awk '/# Time:/{i++}i==%d{print $3; exit}' data.txt", index+1))

# Loop through each block and plot it
do for [i=0:blocks-1] {
  time = time_from_comment(i)
  set label 1 at graph 0.02, 0.95 sprintf("Time: %s", time) front
  plot 'data.txt' index i using 1:4 title sprintf("energy")
}