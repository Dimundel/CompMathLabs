# Set output format to animated GIF
set terminal gif animate delay 10
set output 'animation_lw.gif'

# Set up your plot
set xrange [0:20] # Adjust these values based on your data
set yrange [-1.5:1.5] # Adjust these values based on your data
set style data lines # Choose your preferred plot style

# Count the number of blocks (datasets) in the file
stats 'data_lw101.txt' nooutput
blocks = STATS_blocks

# Loop through each block and plot it
do for [i=0:blocks-1] {
  plot 'data_lw101.txt' index i using 1:2 title sprintf("Co=1.01")
}
