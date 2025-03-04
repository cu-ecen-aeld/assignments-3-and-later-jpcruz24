filesdir="$1"
searchstr="$2"
# Check if the number of arguments is less than 2
if [ $# -lt 2 ]; then 

  echo "Any of the arguments were not specified $((2-$#))"
  exit 1
# Check if the number of arguments is equal to 2
elif [ $# -eq 2 ]; then 
  # Check if the directory exists
  if [ ! -d "$filesdir" ]; then 
    echo "Error: Directory '$filesdir' does not exist."
	exit 1
  fi
# Check if received more than 2 arguments received
else
  echo "Invalid number of arguments $#"
  exit 1
fi
 
# Change the current directory
cd "$filesdir"
# Count directories and subdirectories recursively
files=$(find "$filesdir" -type f | wc -l)
directories=$(find "$filesdir" -type d | wc -l)
X=$((files ))
Y=$(grep -r "$searchstr" "$filesdir" | wc -l)

# Print the result
echo "The number of files are $X and the number of matching lines are $Y"

exit 0

