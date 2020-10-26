#!/bin/bash

# We must specify the exe name as an argument, validation around that
if [ $# -lt 1 ]; then
    printf "Usage: %s <exe_name>\n" "$0" >&2
    exit 1
fi

# appliction name from command argument
bin="$1"

function validate_md5sum
{
    printf "file1 = $1\nfile2 = $2\n"
    file1="$1.txt"
    file2="$2.txt"

    # create md5sum for both input and output files
    md5sum < "$1" > $file1
    md5sum < "$2" > $file2

    if ! cmp --silent "$file1" "$file2"; then
        printf "Test FAILED, md5sum did not match for file $1 and $2\n\n\n"
    else
        printf "Test PASSED for input file [$1 and $2]\n\n\n"
        # if file exist then delete these md5sum temp files
        if [ -f $file1 ]; then
            rm $file1
        fi

        if [ -f $file2 ]; then
            rm $file2
        fi
    fi
}

# If we have all the input files in the current directory along with the exe
# we can also consider passing the file name in an array like done for the piped input
for filepath in $PWD/* ; do

    filename=$(basename "$filepath")

    file_in="$filename"
    if [[ -d $file_in ]]; then
        printf "$file_in is a directory\n"
        continue
    fi

    printf "Testing for $file_in.............................\n"
    file_out="$filename.out"
    # Run application, pass the output filename and indirect the input file
    "./$bin" "$file_out" < "$file_in"

    validate_md5sum $file_in $file_out

    if [ -f $file_out ]; then
        rm $file_out
    fi
done

# if piped input data
# declare an array of commands
declare -a pipedData=("ls" "ls -lart" "python script.py" "cat testscript.sh")

## create a temp output director for test output files
mkdir -p "$PWD/testOutput/"

# Loop the array
for file in "${pipedData[@]}"; do

    file_out="$PWD/testOutput/${file// /}.out"          # the output file from application 
    bash_file_out="$PWD/testOutput/${file// /}.bash"    # the output file from bash
    # create the output file by running the application
    $file | ./$bin $file_out

    # create the output file by bash 
    $file > "$bash_file_out"

    validate_md5sum $file_out $bash_file_out

    if [ -f $file_out ]; then
        rm $file_out
    fi

    if [ -f $bash_file_out ]; then
        rm $bash_file_out
    fi

done

exit 0
