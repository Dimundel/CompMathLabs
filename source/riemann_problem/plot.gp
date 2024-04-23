# Set output format to animated GIF
set terminal gif animate delay 2
set output 'rarefaction.gif'

# Set up your plot
set xrange [-10:10] # Adjust these values based on your data
set yrange [10:16] # Adjust these values based on your data
set style data lines # Choose your preferred plot style

# Count the number of blocks (datasets) in the file
stats 'data_rarefaction.txt' nooutput
blocks = STATS_blocks

time_from_comment(index) = system(sprintf("awk '/# Time:/{i++}i==%d{print $3; exit}' data_rarefaction.txt", index+1))

# Loop through each block and plot it
do for [i=0:blocks-1] {
  time = time_from_comment(i)
  set label 1 at graph 0.02, 0.95 sprintf("Time: %s", time) front
  plot 'data_rarefaction.txt' index i using 1:2 title sprintf("density")
}