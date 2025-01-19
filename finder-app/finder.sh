#!/bin/sh

# Accepts two arguments, 
# 1: Path to a directory
# 2: Text string which will be searched within these files

# Returns,
# 1: Exit code 1 and print statements if params weren't specified
# 2: Exit code 1 and print statements if the dir doesn't exist
# 3: Success "The number of files are X and the number of matching lines are Y"

echo $1
echo $2

if [ "$#" -lt 2 ]
then 
echo "Requires two parameters. USAGE: ./finder.sh directory_path search_string"
return 1
fi

if [ ! -d $1 ]
then 
echo "The directory \"$1\" does not exist."
return 1
fi

files=`grep -l -R $2 $1 | wc -l`
lines=`grep -R $2 $1 | wc -l`
echo "The number of files are $files and the number of matching lines are $lines"
