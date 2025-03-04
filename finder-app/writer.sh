# Check if the number of arguments is less than 2
file_path="$1"
writedir=$(dirname "$file_path")
writefile=$(basename "$file_path")
writestr=$2
if [ $# -lt 2 ]; then 
  echo "Any of the arguments were not specified $((2-$#))"
  exit 1
# Check if the number of arguments is equal to 2
elif [ $# -eq 2 ]; then 
  # Check if the directory exists
  if [ ! -d "$writedir" ]; then 
	mkdir -p "$writedir"
	# Check if directory creation was successful
	if [ $? -eq 1 ]; then
	  exit 1
	fi
    touch "$1"
	# Check if file creation was successful
	if [ $? -ne 0 ]; then
	  exit 1
	fi
	echo "$writestr" > "$1"
	exit 0
  else
    if [ ! -f "$1" ]; then #Check if the file exist
	  touch "$1" #create the file
	  if [ $? -ne 0 ]; then # Check if file creation was successful
	    exit 1
	  fi
	fi
	echo "$writestr" > "$1"
	exit 0
  fi
# Check if received more than 2 arguments received
else
  echo "Invalid number of arguments $#"
  exit 1
fi

