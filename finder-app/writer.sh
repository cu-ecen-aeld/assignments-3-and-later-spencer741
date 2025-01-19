
# Arguments,
# 1: First is full path to a file
# 2: Second is a string to be written to the file

# Returns,
# 1: Error code 1 and print outs if the arguments were'nt specified.
# 2: Error code 1 and print outs if file cant be created.

if [ $# -lt 2 ]
then
echo "Requires 2 parameters. USAGE: ./writer.sh file_path write_string"
return 1
fi 

mkdir -p `dirname "$1"`
echo "$2" > "$1"
